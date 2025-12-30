/**
 * SPA App - Database Manager
 * SQLite integration for user authentication and data persistence
 */

#ifndef SPA_DATABASE_H
#define SPA_DATABASE_H

#include <stdbool.h>
#include <sqlite3.h>

/* User structure */
typedef struct {
    int id;
    char username[64];
    char email[128];
    char password_hash[256];
    bool is_active;
    char created_at[32];
} User;

/* Product structure */
typedef struct {
    int id;
    char name[128];
    char description[256];
    double price;
    int stock;
    char category[64];
    char created_at[32];
} Product;

/* Database functions */
bool db_init(const char *db_path);
void db_close(void);
sqlite3 *db_get_connection(void);

/* User operations */
bool db_user_create(const char *username, const char *email, const char *password);
User *db_user_authenticate(const char *username, const char *password);
User *db_user_get_by_id(int user_id);
User **db_user_list_all(int *count);
bool db_user_update(int user_id, const char *email, bool is_active);
bool db_user_change_password(int user_id, const char *new_password);
bool db_user_delete(int user_id);
void db_user_free(User *user);
void db_user_list_free(User **users, int count);

/* Product operations */
bool db_product_create(const char *name, const char *description, double price, int stock, const char *category);
Product *db_product_get_by_id(int product_id);
Product **db_product_list_all(int *count);
Product **db_product_search(const char *query, int *count);
bool db_product_update(int product_id, const char *name, const char *description, double price, int stock, const char *category);
bool db_product_delete(int product_id);
void db_product_free(Product *product);
void db_product_list_free(Product **products, int count);

/* Statistics */
int db_stats_total_users(void);
int db_stats_total_products(void);
double db_stats_total_inventory_value(void);
int db_stats_low_stock_products(void);

#endif /* SPA_DATABASE_H */
