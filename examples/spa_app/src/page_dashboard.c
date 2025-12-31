#include "../include/pages.h"
#include "../include/app_state.h"

void page_dashboard_update(void) {
    eg_view_model_set_int(g_app_state->view_model, "total_users", db_stats_total_users());
    eg_view_model_set_int(g_app_state->view_model, "total_products", db_stats_total_products());
    eg_view_model_set_double(g_app_state->view_model, "inventory_value", db_stats_total_inventory_value());
    eg_view_model_set_int(g_app_state->view_model, "low_stock_count", db_stats_low_stock_products());
}

EgWidget *page_dashboard_create(void) {
    EgBox *page = eg_box_new_vertical(0);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");

    /* Stats grid */
    EgBox *stats = eg_box_new_horizontal(16);
    eg_widget_set_halign(eg_box_as_widget(stats), EG_ALIGN_CENTER);

    /* Stat cards */
    const char *stat_names[] = {"Total Users", "Total Products", "Inventory Value", "Low Stock"};
    const char *prop_names[] = {"total_users", "total_products", "inventory_value", "low_stock_count"};

    for (int i = 0; i < 4; i++) {
        EgFrame *card = eg_frame_new(NULL);
        eg_widget_add_css_class(eg_frame_as_widget(card), "stat-card");

        EgBox *card_box = eg_box_new_vertical(8);
        EgLabel *label = eg_label_new(stat_names[i]);
        eg_widget_add_css_class(eg_label_as_widget(label), "stat-label");

        EgLabel *value = eg_label_new("0");
        eg_widget_add_css_class(eg_label_as_widget(value), "stat-value");
        eg_bind(eg_label_as_widget(value), g_app_state->view_model, prop_names[i]);

        eg_box_append(card_box, eg_label_as_widget(value));
        eg_box_append(card_box, eg_label_as_widget(label));
        eg_frame_set_child(card, eg_box_as_widget(card_box));
        eg_box_append(stats, eg_frame_as_widget(card));
    }

    eg_box_append(page, eg_box_as_widget(stats));
    return eg_box_as_widget(page);
}
