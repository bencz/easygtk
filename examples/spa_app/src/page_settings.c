#include "../include/pages.h"
#include "../include/app_state.h"

void page_settings_update(void) {}

static void on_logout_click(EgWidget *w, void *ud) {
    (void)w; (void)ud;
    app_state_logout();
}

EgWidget *page_settings_create(void) {
    EgBox *page = eg_box_new_vertical(24);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    eg_widget_set_valign(eg_box_as_widget(page), EG_ALIGN_CENTER);
    eg_widget_set_halign(eg_box_as_widget(page), EG_ALIGN_CENTER);
    
    EgLabel *username = eg_label_new("");
    eg_bind_label_text(username, g_app_state->view_model, "username");
    eg_widget_add_css_class(eg_label_as_widget(username), "page-title");
    
    EgButton *btn_logout = eg_button_new("Logout");
    eg_widget_add_css_class(eg_button_as_widget(btn_logout), "btn-danger");
    eg_button_on_click(btn_logout, on_logout_click, NULL);
    
    eg_box_append(page, eg_label_as_widget(username));
    eg_box_append(page, eg_button_as_widget(btn_logout));
    
    return eg_box_as_widget(page);
}
