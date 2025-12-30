/**
 * SPA App - Main Application
 * Modern Single Page Application with sidebar navigation
 */

#include <easygtk/easygtk.h>
#include "../include/database.h"
#include "../include/app_state.h"
#include "../include/pages.h"
#include <stdio.h>

/* Navigation button callbacks (EgCallback signature) */
static void on_dashboard_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    app_state_navigate_to(PAGE_DASHBOARD);
}

static void on_users_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    app_state_navigate_to(PAGE_USERS);
}

static void on_products_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    app_state_navigate_to(PAGE_PRODUCTS);
}

static void on_settings_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    app_state_navigate_to(PAGE_SETTINGS);
}

static EgWidget *create_sidebar(void) {
    EgBox *sidebar = eg_box_new_vertical(0);
    eg_widget_add_css_class(eg_box_as_widget(sidebar), "sidebar");

    /* Header */
    EgBox *header = eg_box_new_vertical(4);
    eg_widget_add_css_class(eg_box_as_widget(header), "sidebar-header");

    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='large' weight='bold'>SPA Dashboard</span>");
    eg_widget_add_css_class(eg_label_as_widget(title), "sidebar-title");

    EgLabel *subtitle = eg_label_new("Management System");
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "sidebar-subtitle");

    eg_box_append(header, eg_label_as_widget(title));
    eg_box_append(header, eg_label_as_widget(subtitle));

    /* Navigation */
    EgBox *nav = eg_box_new_vertical(4);
    eg_widget_set_vexpand(eg_box_as_widget(nav), true);

    const char *nav_labels[] = {"", "🏠 Dashboard", "👥 Users", "📦 Products", "⚙️ Settings"};

    for (int i = 1; i < 5; i++) {
        EgButton *btn = eg_button_new(nav_labels[i]);
        eg_widget_add_css_class(eg_button_as_widget(btn), "nav-button");
        g_app_state->nav_buttons[i] = btn;

        /* Connect to navigation callbacks */
        switch(i) {
            case 1: eg_button_on_click(btn, on_dashboard_click, NULL); break;
            case 2: eg_button_on_click(btn, on_users_click, NULL); break;
            case 3: eg_button_on_click(btn, on_products_click, NULL); break;
            case 4: eg_button_on_click(btn, on_settings_click, NULL); break;
        }

        eg_box_append(nav, eg_button_as_widget(btn));
    }

    eg_box_append(sidebar, eg_box_as_widget(header));
    eg_box_append(sidebar, eg_box_as_widget(nav));

    return eg_box_as_widget(sidebar);
}

static EgWidget *create_main_content(void) {
    EgBox *main = eg_box_new_vertical(0);
    eg_widget_add_css_class(eg_box_as_widget(main), "main-content");
    eg_widget_set_hexpand(eg_box_as_widget(main), true);

    /* Header */
    EgBox *header = eg_box_new_vertical(8);
    eg_widget_add_css_class(eg_box_as_widget(header), "page-header");

    EgLabel *page_title = eg_label_new("");
    eg_bind_label_text(page_title, g_app_state->view_model, "current_page_title");
    eg_widget_add_css_class(eg_label_as_widget(page_title), "page-title");

    EgLabel *username = eg_label_new("");
    eg_bind_label_text(username, g_app_state->view_model, "username");
    eg_widget_add_css_class(eg_label_as_widget(username), "page-subtitle");

    eg_box_append(header, eg_label_as_widget(page_title));
    eg_box_append(header, eg_label_as_widget(username));

    /* Content stack */
    g_app_state->content_stack = eg_stack_new();
    eg_widget_set_vexpand(eg_stack_as_widget(g_app_state->content_stack), true);

    eg_stack_add_named(g_app_state->content_stack, page_dashboard_create(), "dashboard");
    eg_stack_add_named(g_app_state->content_stack, page_users_create(), "users");
    eg_stack_add_named(g_app_state->content_stack, page_products_create(), "products");
    eg_stack_add_named(g_app_state->content_stack, page_settings_create(), "settings");

    eg_box_append(main, eg_box_as_widget(header));
    eg_box_append(main, eg_stack_as_widget(g_app_state->content_stack));

    return eg_box_as_widget(main);
}

static EgWidget *create_app_layout(void) {
    EgBox *layout = eg_box_new_horizontal(0);

    /* Sidebar */
    eg_box_append(layout, create_sidebar());

    /* Main content */
    eg_box_append(layout, create_main_content());

    return eg_box_as_widget(layout);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Initialize database */
    if (!db_init("./app.db")) {
        fprintf(stderr, "Failed to initialize database\n");
        return;
    }

    /* Initialize app state */
    app_state_new();

    /* Store app reference */
    g_app_state->app = app;

    /* Create window */
    EgWindow *window = eg_window_new(app, "SPA Dashboard", 1200, 700);
    g_app_state->main_window = window;

    /* Main stack (login vs app) */
    g_app_state->main_stack = eg_stack_new();

    eg_stack_add_named(g_app_state->main_stack, page_login_create(), "login");
    eg_stack_add_named(g_app_state->main_stack, create_app_layout(), "app");

    eg_window_set_child(window, eg_stack_as_widget(g_app_state->main_stack));

    /* Load CSS */
    eg_css_load_from_file("./assets/style.css");

    /* Show login page */
    app_state_navigate_to(PAGE_LOGIN);

    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    EgApp *app = eg_app_new("com.example.spa.dashboard");
    if (!app) return 1;

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);

    /* Cleanup */
    app_state_free(g_app_state);
    db_close();
    eg_app_free(app);

    return status;
}
