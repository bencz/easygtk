#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>

static EgBox *products_list_box = NULL;

void page_products_update(void) {
    if (!products_list_box) return;
    
    int count = 0;
    Product **products = db_product_list_all(&count);
    
    for (int i = 0; i < count; i++) {
        char markup[512];
        snprintf(markup, sizeof(markup),
            "<b>%s</b> - $%.2f | Stock: %d | %s",
            products[i]->name,
            products[i]->price,
            products[i]->stock,
            products[i]->category
        );
        
        EgLabel *label = eg_label_new("");
        eg_label_set_markup(label, markup);
        eg_widget_add_css_class(eg_label_as_widget(label), "list-item");
        eg_box_append(products_list_box, eg_label_as_widget(label));
    }
    
    db_product_list_free(products, count);
}

EgWidget *page_products_create(void) {
    EgBox *page = eg_box_new_vertical(16);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    
    products_list_box = eg_box_new_vertical(8);
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(products_list_box));
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    
    eg_box_append(page, eg_scrolled_window_as_widget(scroll));
    return eg_box_as_widget(page);
}
