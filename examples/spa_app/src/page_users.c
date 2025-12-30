#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>

static EgBox *users_list_box = NULL;

void page_users_update(void) {
    if (!users_list_box) return;
    
    /* Clear existing */
    /* Note: In a real app, we'd need to properly manage children */
    
    /* Get users from DB */
    int count = 0;
    User **users = db_user_list_all(&count);
    
    for (int i = 0; i < count; i++) {
        char markup[512];
        snprintf(markup, sizeof(markup),
            "<b>%s</b> - %s %s",
            users[i]->username,
            users[i]->email,
            users[i]->is_active ? "✓" : "✗"
        );
        
        EgLabel *label = eg_label_new("");
        eg_label_set_markup(label, markup);
        eg_widget_add_css_class(eg_label_as_widget(label), "list-item");
        eg_box_append(users_list_box, eg_label_as_widget(label));
    }
    
    db_user_list_free(users, count);
}

EgWidget *page_users_create(void) {
    EgBox *page = eg_box_new_vertical(16);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    
    users_list_box = eg_box_new_vertical(8);
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(users_list_box));
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    
    eg_box_append(page, eg_scrolled_window_as_widget(scroll));
    return eg_box_as_widget(page);
}
