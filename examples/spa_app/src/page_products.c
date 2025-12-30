/**
 * SPA App - Products Management Page
 * Full CRUD operations for products
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Page widgets */
static EgBox *products_list_box = NULL;
static EgEntry *search_entry = NULL;

/* Edit dialog widgets (reused) */
static EgWindow *edit_dialog = NULL;
static EgEntry *edit_name_entry = NULL;
static EgEntry *edit_description_entry = NULL;
static EgEntry *edit_price_entry = NULL;
static EgSpinButton *edit_stock_spin = NULL;
static EgEntry *edit_category_entry = NULL;
static int editing_product_id = -1; /* -1 = creating new, >0 = editing existing */

/* Forward declarations */
static void refresh_products_list(void);
static void show_product_dialog(Product *product);
static void on_add_product_clicked(EgWidget *widget, void *user_data);
static void on_edit_product_clicked(EgWidget *widget, void *user_data);
static void on_delete_product_clicked(EgWidget *widget, void *user_data);
static void on_save_product_clicked(EgWidget *widget, void *user_data);
static void on_cancel_dialog_clicked(EgWidget *widget, void *user_data);
static void on_search_changed(EgWidget *widget, void *user_data);

/* Create a product list item row */
static EgWidget *create_product_row(Product *product) {
    EgBox *row = eg_box_new_horizontal(12);
    eg_widget_add_css_class(eg_box_as_widget(row), "list-item");
    eg_widget_set_hexpand(eg_box_as_widget(row), true);

    /* Product info */
    EgBox *info_box = eg_box_new_vertical(4);
    eg_widget_set_hexpand(eg_box_as_widget(info_box), true);

    char name_markup[256];
    snprintf(name_markup, sizeof(name_markup), "<b>%s</b>", product->name);
    EgLabel *name_label = eg_label_new("");
    eg_label_set_markup(name_label, name_markup);
    eg_label_set_xalign(name_label, 0.0f);
    eg_box_append(info_box, eg_label_as_widget(name_label));

    char desc_text[512];
    snprintf(desc_text, sizeof(desc_text), "%.200s | %.60s", product->description, product->category);
    EgLabel *desc_label = eg_label_new(desc_text);
    eg_widget_add_css_class(eg_label_as_widget(desc_label), "text-muted");
    eg_label_set_xalign(desc_label, 0.0f);
    eg_box_append(info_box, eg_label_as_widget(desc_label));

    eg_box_append(row, eg_box_as_widget(info_box));

    /* Price */
    char price_text[64];
    snprintf(price_text, sizeof(price_text), "$%.2f", product->price);
    EgLabel *price_label = eg_label_new(price_text);
    eg_widget_add_css_class(eg_label_as_widget(price_label), "badge-info");
    eg_widget_set_valign(eg_label_as_widget(price_label), EG_ALIGN_CENTER);
    eg_box_append(row, eg_label_as_widget(price_label));

    /* Stock badge */
    char stock_text[32];
    snprintf(stock_text, sizeof(stock_text), "Stock: %d", product->stock);
    EgLabel *stock_label = eg_label_new(stock_text);
    eg_widget_add_css_class(eg_label_as_widget(stock_label),
        product->stock < 10 ? "badge-warning" : "badge-success");
    eg_widget_set_valign(eg_label_as_widget(stock_label), EG_ALIGN_CENTER);
    eg_box_append(row, eg_label_as_widget(stock_label));

    /* Action buttons */
    EgBox *actions_box = eg_box_new_horizontal(8);
    eg_widget_set_valign(eg_box_as_widget(actions_box), EG_ALIGN_CENTER);

    EgButton *edit_btn = eg_button_new("Edit");
    eg_widget_add_css_class(eg_button_as_widget(edit_btn), "btn-small");
    /* Store product_id in user_data - allocate memory for it */
    int *product_id = malloc(sizeof(int));
    *product_id = product->id;
    eg_button_on_click(edit_btn, on_edit_product_clicked, product_id);
    eg_box_append(actions_box, eg_button_as_widget(edit_btn));

    EgButton *delete_btn = eg_button_new("Delete");
    eg_widget_add_css_class(eg_button_as_widget(delete_btn), "btn-small");
    eg_widget_add_css_class(eg_button_as_widget(delete_btn), "btn-danger");
    int *del_product_id = malloc(sizeof(int));
    *del_product_id = product->id;
    eg_button_on_click(delete_btn, on_delete_product_clicked, del_product_id);
    eg_box_append(actions_box, eg_button_as_widget(delete_btn));

    eg_box_append(row, eg_box_as_widget(actions_box));

    return eg_box_as_widget(row);
}

/* Refresh products list from database */
static void refresh_products_list(void) {
    if (!products_list_box) return;

    eg_box_clear(products_list_box);

    int count = 0;
    Product **products = NULL;

    /* Filter by search if text entered */
    const char *search_text = search_entry ? eg_entry_get_text(search_entry) : "";

    if (search_text && search_text[0] != '\0') {
        products = db_product_search(search_text, &count);
    } else {
        products = db_product_list_all(&count);
    }

    for (int i = 0; i < count; i++) {
        EgWidget *row = create_product_row(products[i]);
        eg_box_append(products_list_box, row);
    }

    db_product_list_free(products, count);

    /* Update stats in dashboard */
    eg_view_model_set_int(g_app_state->view_model, "total_products", db_stats_total_products());
    eg_view_model_set_double(g_app_state->view_model, "inventory_value", db_stats_total_inventory_value());
    eg_view_model_set_int(g_app_state->view_model, "low_stock_count", db_stats_low_stock_products());
}

/* Show add/edit product dialog */
static void show_product_dialog(Product *product) {
    /* Create dialog window using app reference */
    edit_dialog = eg_window_new(g_app_state->app, product ? "Edit Product" : "Add Product", 450, 380);
    if (!edit_dialog) {
        fprintf(stderr, "Failed to create dialog window\n");
        return;
    }
    eg_window_set_resizable(edit_dialog, false);

    EgBox *content = eg_box_new_vertical(16);
    eg_widget_set_margin(eg_box_as_widget(content), 24);

    /* Title */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, product ? "<b>Edit Product</b>" : "<b>Add New Product</b>");
    eg_widget_add_css_class(eg_label_as_widget(title), "title-3");
    eg_box_append(content, eg_label_as_widget(title));

    /* Form grid */
    EgGrid *form = eg_grid_new();
    eg_grid_set_row_spacing(form, 12);
    eg_grid_set_column_spacing(form, 12);

    /* Name */
    EgLabel *name_label = eg_label_new("Name:");
    eg_label_set_xalign(name_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(name_label), 0, 0, 1, 1);

    edit_name_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(edit_name_entry), true);
    if (product) {
        eg_entry_set_text(edit_name_entry, product->name);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_name_entry), 1, 0, 1, 1);

    /* Description */
    EgLabel *desc_label = eg_label_new("Description:");
    eg_label_set_xalign(desc_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(desc_label), 0, 1, 1, 1);

    edit_description_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(edit_description_entry), true);
    if (product) {
        eg_entry_set_text(edit_description_entry, product->description);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_description_entry), 1, 1, 1, 1);

    /* Price */
    EgLabel *price_label = eg_label_new("Price ($):");
    eg_label_set_xalign(price_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(price_label), 0, 2, 1, 1);

    edit_price_entry = eg_entry_new_with_placeholder("0.00");
    eg_widget_set_hexpand(eg_entry_as_widget(edit_price_entry), true);
    if (product) {
        char price_str[32];
        snprintf(price_str, sizeof(price_str), "%.2f", product->price);
        eg_entry_set_text(edit_price_entry, price_str);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_price_entry), 1, 2, 1, 1);

    /* Stock */
    EgLabel *stock_label = eg_label_new("Stock:");
    eg_label_set_xalign(stock_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(stock_label), 0, 3, 1, 1);

    edit_stock_spin = eg_spin_button_new(0, 10000, 1);
    eg_widget_set_hexpand(eg_spin_button_as_widget(edit_stock_spin), true);
    if (product) {
        eg_spin_button_set_value(edit_stock_spin, product->stock);
    }
    eg_grid_attach(form, eg_spin_button_as_widget(edit_stock_spin), 1, 3, 1, 1);

    /* Category */
    EgLabel *category_label = eg_label_new("Category:");
    eg_label_set_xalign(category_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(category_label), 0, 4, 1, 1);

    edit_category_entry = eg_entry_new_with_placeholder("e.g., Electronics, Accessories");
    eg_widget_set_hexpand(eg_entry_as_widget(edit_category_entry), true);
    if (product) {
        eg_entry_set_text(edit_category_entry, product->category);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_category_entry), 1, 4, 1, 1);

    eg_box_append(content, eg_grid_as_widget(form));

    /* Buttons */
    EgBox *buttons = eg_box_new_horizontal(12);
    eg_widget_set_halign(eg_box_as_widget(buttons), EG_ALIGN_END);
    eg_widget_set_margin(eg_box_as_widget(buttons), 8);

    EgButton *cancel_btn = eg_button_new("Cancel");
    eg_button_on_click(cancel_btn, on_cancel_dialog_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(cancel_btn));

    EgButton *save_btn = eg_button_new(product ? "Save Changes" : "Create Product");
    eg_widget_add_css_class(eg_button_as_widget(save_btn), "btn-primary");
    eg_button_on_click(save_btn, on_save_product_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(save_btn));

    eg_box_append(content, eg_box_as_widget(buttons));

    /* Store editing product id */
    editing_product_id = product ? product->id : -1;

    eg_window_set_child(edit_dialog, eg_box_as_widget(content));
    eg_window_show(edit_dialog);
}

/* Callbacks */
static void on_add_product_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    show_product_dialog(NULL);
}

static void on_edit_product_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    int product_id = *(int *)user_data;
    Product *product = db_product_get_by_id(product_id);
    if (product) {
        show_product_dialog(product);
        db_product_free(product);
    }
}

static void on_delete_confirm(EgDialogResponse response, void *user_data) {
    if (response == EG_RESPONSE_YES) {
        int product_id = *(int *)user_data;
        if (db_product_delete(product_id)) {
            refresh_products_list();
        }
    }
    free(user_data);
}

static void on_delete_product_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    int *product_id = (int *)user_data;

    /* Need to pass product_id to callback, allocate new copy */
    int *id_copy = malloc(sizeof(int));
    *id_copy = *product_id;

    eg_dialog_question(NULL, "Confirm Delete",
        "Are you sure you want to delete this product?",
        on_delete_confirm, id_copy);
}

static void on_save_product_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    const char *name = eg_entry_get_text(edit_name_entry);
    const char *description = eg_entry_get_text(edit_description_entry);
    const char *price_str = eg_entry_get_text(edit_price_entry);
    int stock = (int)eg_spin_button_get_value(edit_stock_spin);
    const char *category = eg_entry_get_text(edit_category_entry);

    double price = 0.0;
    if (price_str && price_str[0] != '\0') {
        price = atof(price_str);
    }

    bool success = false;

    if (editing_product_id < 0) {
        /* Creating new product */
        if (name[0] != '\0') {
            success = db_product_create(name, description, price, stock, category);
        }
    } else {
        /* Updating existing product */
        if (name[0] != '\0') {
            success = db_product_update(editing_product_id, name, description, price, stock, category);
        }
    }

    if (success) {
        eg_window_close(edit_dialog);
        edit_dialog = NULL;
        refresh_products_list();

        /* Show success message */
        eg_dialog_info(g_app_state->main_window,
            editing_product_id < 0 ? "Product Created" : "Product Updated",
            editing_product_id < 0 ? "New product was added successfully." : "Product information was updated.");
    } else {
        /* Show error message */
        eg_dialog_error(g_app_state->main_window, "Error",
            editing_product_id < 0 ? "Failed to create product." : "Failed to update product.");
    }
}

static void on_cancel_dialog_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    if (edit_dialog) {
        eg_window_close(edit_dialog);
        edit_dialog = NULL;
    }
}

static void on_search_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    refresh_products_list();
}

/* Public functions */
void page_products_update(void) {
    refresh_products_list();
}

EgWidget *page_products_create(void) {
    EgBox *page = eg_box_new_vertical(16);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    eg_widget_set_margin(eg_box_as_widget(page), 16);

    /* Header with title and add button */
    EgBox *header = eg_box_new_horizontal(12);
    eg_widget_set_hexpand(eg_box_as_widget(header), true);

    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='large' weight='bold'>Products Management</span>");
    eg_widget_set_hexpand(eg_label_as_widget(title), true);
    eg_label_set_xalign(title, 0.0f);
    eg_box_append(header, eg_label_as_widget(title));

    EgButton *add_btn = eg_button_new("+ Add Product");
    eg_widget_add_css_class(eg_button_as_widget(add_btn), "btn-primary");
    eg_button_on_click(add_btn, on_add_product_clicked, NULL);
    eg_box_append(header, eg_button_as_widget(add_btn));

    eg_box_append(page, eg_box_as_widget(header));

    /* Search bar */
    EgBox *search_box = eg_box_new_horizontal(8);

    EgLabel *search_label = eg_label_new("Search:");
    eg_box_append(search_box, eg_label_as_widget(search_label));

    search_entry = eg_entry_new_with_placeholder("Search products...");
    eg_widget_set_hexpand(eg_entry_as_widget(search_entry), true);
    eg_entry_on_changed(search_entry, on_search_changed, NULL);
    eg_box_append(search_box, eg_entry_as_widget(search_entry));

    eg_box_append(page, eg_box_as_widget(search_box));

    /* Products list in scrolled window */
    products_list_box = eg_box_new_vertical(8);

    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(products_list_box));
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_scrolled_window_set_min_content_size(scroll, -1, 300);

    eg_box_append(page, eg_scrolled_window_as_widget(scroll));

    return eg_box_as_widget(page);
}
