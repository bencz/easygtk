/**
 * SPA App - Database Implementation
 */

#include "../include/database.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static sqlite3 *g_db = NULL;

/* Simple password hashing (SHA256 would be better in production) */
static void hash_password(const char *password, char *output, size_t output_size) {
    /* Simple hash - In production, use proper crypto library */
    snprintf(output, output_size, "HASH_%s", password);
}

bool db_init(const char *db_path) {
    int rc = sqlite3_open(db_path, &g_db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(g_db));
        return false;
    }

    /* Create tables */
    const char *users_table =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "email TEXT NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "is_active INTEGER DEFAULT 1,"
        "created_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    const char *products_table =
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "description TEXT,"
        "price REAL NOT NULL,"
        "stock INTEGER NOT NULL,"
        "category TEXT,"
        "created_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    char *err_msg = NULL;
    rc = sqlite3_exec(g_db, users_table, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }

    rc = sqlite3_exec(g_db, products_table, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }

    /* Insert default admin user if not exists */
    const char *check_admin = "SELECT COUNT(*) FROM users WHERE username = 'admin';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(g_db, check_admin, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count == 0) {
                /* Create default admin */
                db_user_create("admin", "admin@example.com", "admin123");
                /* Create demo users */
                db_user_create("john", "john@example.com", "password");
                db_user_create("jane", "jane@example.com", "password");
            }
        }
        sqlite3_finalize(stmt);
    }

    /* Insert demo products if empty */
    const char *check_products = "SELECT COUNT(*) FROM products;";
    rc = sqlite3_prepare_v2(g_db, check_products, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count == 0) {
                db_product_create("Laptop Pro", "High-performance laptop", 1299.99, 15, "Electronics");
                db_product_create("Wireless Mouse", "Ergonomic wireless mouse", 29.99, 50, "Accessories");
                db_product_create("Mechanical Keyboard", "RGB mechanical keyboard", 149.99, 30, "Accessories");
                db_product_create("4K Monitor", "27-inch 4K display", 449.99, 20, "Electronics");
                db_product_create("USB-C Hub", "7-in-1 USB-C hub", 49.99, 100, "Accessories");
                db_product_create("Webcam HD", "1080p webcam", 79.99, 5, "Electronics");
            }
        }
        sqlite3_finalize(stmt);
    }

    return true;
}

void db_close(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

sqlite3 *db_get_connection(void) {
    return g_db;
}

/* ============================================
 * User Operations
 * ============================================ */

bool db_user_create(const char *username, const char *email, const char *password) {
    char password_hash[256];
    hash_password(password, password_hash, sizeof(password_hash));

    const char *sql = "INSERT INTO users (username, email, password_hash) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password_hash, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

User *db_user_authenticate(const char *username, const char *password) {
    char password_hash[256];
    hash_password(password, password_hash, sizeof(password_hash));

    const char *sql = "SELECT id, username, email, password_hash, is_active, created_at "
                      "FROM users WHERE username = ? AND password_hash = ? AND is_active = 1;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return NULL;

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_STATIC);

    User *user = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = (User *)malloc(sizeof(User));
        user->id = sqlite3_column_int(stmt, 0);
        strncpy(user->username, (const char *)sqlite3_column_text(stmt, 1), sizeof(user->username) - 1);
        strncpy(user->email, (const char *)sqlite3_column_text(stmt, 2), sizeof(user->email) - 1);
        strncpy(user->password_hash, (const char *)sqlite3_column_text(stmt, 3), sizeof(user->password_hash) - 1);
        user->is_active = sqlite3_column_int(stmt, 4);
        strncpy(user->created_at, (const char *)sqlite3_column_text(stmt, 5), sizeof(user->created_at) - 1);
    }

    sqlite3_finalize(stmt);
    return user;
}

User *db_user_get_by_id(int user_id) {
    const char *sql = "SELECT id, username, email, password_hash, is_active, created_at FROM users WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return NULL;

    sqlite3_bind_int(stmt, 1, user_id);

    User *user = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = (User *)malloc(sizeof(User));
        user->id = sqlite3_column_int(stmt, 0);
        strncpy(user->username, (const char *)sqlite3_column_text(stmt, 1), sizeof(user->username) - 1);
        strncpy(user->email, (const char *)sqlite3_column_text(stmt, 2), sizeof(user->email) - 1);
        strncpy(user->password_hash, (const char *)sqlite3_column_text(stmt, 3), sizeof(user->password_hash) - 1);
        user->is_active = sqlite3_column_int(stmt, 4);
        strncpy(user->created_at, (const char *)sqlite3_column_text(stmt, 5), sizeof(user->created_at) - 1);
    }

    sqlite3_finalize(stmt);
    return user;
}

User **db_user_list_all(int *count) {
    const char *sql = "SELECT id, username, email, password_hash, is_active, created_at FROM users ORDER BY id;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        *count = 0;
        return NULL;
    }

    /* Count rows first */
    User **users = NULL;
    int capacity = 10;
    *count = 0;
    users = (User **)malloc(sizeof(User *) * capacity);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (*count >= capacity) {
            capacity *= 2;
            users = (User **)realloc(users, sizeof(User *) * capacity);
        }

        User *user = (User *)malloc(sizeof(User));
        user->id = sqlite3_column_int(stmt, 0);
        strncpy(user->username, (const char *)sqlite3_column_text(stmt, 1), sizeof(user->username) - 1);
        strncpy(user->email, (const char *)sqlite3_column_text(stmt, 2), sizeof(user->email) - 1);
        strncpy(user->password_hash, (const char *)sqlite3_column_text(stmt, 3), sizeof(user->password_hash) - 1);
        user->is_active = sqlite3_column_int(stmt, 4);
        strncpy(user->created_at, (const char *)sqlite3_column_text(stmt, 5), sizeof(user->created_at) - 1);

        users[*count] = user;
        (*count)++;
    }

    sqlite3_finalize(stmt);
    return users;
}

bool db_user_update(int user_id, const char *email, bool is_active) {
    const char *sql = "UPDATE users SET email = ?, is_active = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, is_active ? 1 : 0);
    sqlite3_bind_int(stmt, 3, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool db_user_change_password(int user_id, const char *new_password) {
    char password_hash[256];
    hash_password(new_password, password_hash, sizeof(password_hash));

    const char *sql = "UPDATE users SET password_hash = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool db_user_delete(int user_id) {
    const char *sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

void db_user_free(User *user) {
    if (user) free(user);
}

void db_user_list_free(User **users, int count) {
    if (users) {
        for (int i = 0; i < count; i++) {
            free(users[i]);
        }
        free(users);
    }
}

/* ============================================
 * Product Operations
 * ============================================ */

bool db_product_create(const char *name, const char *description, double price, int stock, const char *category) {
    const char *sql = "INSERT INTO products (name, description, price, stock, category) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_int(stmt, 4, stock);
    sqlite3_bind_text(stmt, 5, category, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

Product *db_product_get_by_id(int product_id) {
    const char *sql = "SELECT id, name, description, price, stock, category, created_at FROM products WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return NULL;

    sqlite3_bind_int(stmt, 1, product_id);

    Product *product = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        product = (Product *)malloc(sizeof(Product));
        product->id = sqlite3_column_int(stmt, 0);
        strncpy(product->name, (const char *)sqlite3_column_text(stmt, 1), sizeof(product->name) - 1);
        strncpy(product->description, (const char *)sqlite3_column_text(stmt, 2), sizeof(product->description) - 1);
        product->price = sqlite3_column_double(stmt, 3);
        product->stock = sqlite3_column_int(stmt, 4);
        strncpy(product->category, (const char *)sqlite3_column_text(stmt, 5), sizeof(product->category) - 1);
        strncpy(product->created_at, (const char *)sqlite3_column_text(stmt, 6), sizeof(product->created_at) - 1);
    }

    sqlite3_finalize(stmt);
    return product;
}

Product **db_product_list_all(int *count) {
    const char *sql = "SELECT id, name, description, price, stock, category, created_at FROM products ORDER BY id;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        *count = 0;
        return NULL;
    }

    Product **products = NULL;
    int capacity = 10;
    *count = 0;
    products = (Product **)malloc(sizeof(Product *) * capacity);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (*count >= capacity) {
            capacity *= 2;
            products = (Product **)realloc(products, sizeof(Product *) * capacity);
        }

        Product *product = (Product *)malloc(sizeof(Product));
        product->id = sqlite3_column_int(stmt, 0);
        strncpy(product->name, (const char *)sqlite3_column_text(stmt, 1), sizeof(product->name) - 1);
        strncpy(product->description, (const char *)sqlite3_column_text(stmt, 2), sizeof(product->description) - 1);
        product->price = sqlite3_column_double(stmt, 3);
        product->stock = sqlite3_column_int(stmt, 4);
        strncpy(product->category, (const char *)sqlite3_column_text(stmt, 5), sizeof(product->category) - 1);
        strncpy(product->created_at, (const char *)sqlite3_column_text(stmt, 6), sizeof(product->created_at) - 1);

        products[*count] = product;
        (*count)++;
    }

    sqlite3_finalize(stmt);
    return products;
}

Product **db_product_search(const char *query, int *count) {
    const char *sql = "SELECT id, name, description, price, stock, category, created_at FROM products "
                      "WHERE name LIKE ? OR description LIKE ? OR category LIKE ? ORDER BY name;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        *count = 0;
        return NULL;
    }

    char search_pattern[256];
    snprintf(search_pattern, sizeof(search_pattern), "%%%s%%", query);
    sqlite3_bind_text(stmt, 1, search_pattern, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, search_pattern, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, search_pattern, -1, SQLITE_TRANSIENT);

    Product **products = NULL;
    int capacity = 10;
    *count = 0;
    products = (Product **)malloc(sizeof(Product *) * capacity);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (*count >= capacity) {
            capacity *= 2;
            products = (Product **)realloc(products, sizeof(Product *) * capacity);
        }

        Product *product = (Product *)malloc(sizeof(Product));
        product->id = sqlite3_column_int(stmt, 0);
        strncpy(product->name, (const char *)sqlite3_column_text(stmt, 1), sizeof(product->name) - 1);
        strncpy(product->description, (const char *)sqlite3_column_text(stmt, 2), sizeof(product->description) - 1);
        product->price = sqlite3_column_double(stmt, 3);
        product->stock = sqlite3_column_int(stmt, 4);
        strncpy(product->category, (const char *)sqlite3_column_text(stmt, 5), sizeof(product->category) - 1);
        strncpy(product->created_at, (const char *)sqlite3_column_text(stmt, 6), sizeof(product->created_at) - 1);

        products[*count] = product;
        (*count)++;
    }

    sqlite3_finalize(stmt);
    return products;
}

bool db_product_update(int product_id, const char *name, const char *description, double price, int stock, const char *category) {
    const char *sql = "UPDATE products SET name = ?, description = ?, price = ?, stock = ?, category = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_int(stmt, 4, stock);
    sqlite3_bind_text(stmt, 5, category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, product_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

bool db_product_delete(int product_id) {
    const char *sql = "DELETE FROM products WHERE id = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, product_id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc == SQLITE_DONE;
}

void db_product_free(Product *product) {
    if (product) free(product);
}

void db_product_list_free(Product **products, int count) {
    if (products) {
        for (int i = 0; i < count; i++) {
            free(products[i]);
        }
        free(products);
    }
}

/* ============================================
 * Statistics
 * ============================================ */

int db_stats_total_users(void) {
    const char *sql = "SELECT COUNT(*) FROM users;";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return count;
}

int db_stats_total_products(void) {
    const char *sql = "SELECT COUNT(*) FROM products;";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return count;
}

double db_stats_total_inventory_value(void) {
    const char *sql = "SELECT SUM(price * stock) FROM products;";
    sqlite3_stmt *stmt;
    double total = 0.0;

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return total;
}

int db_stats_low_stock_products(void) {
    const char *sql = "SELECT COUNT(*) FROM products WHERE stock < 10;";
    sqlite3_stmt *stmt;
    int count = 0;

    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    return count;
}
