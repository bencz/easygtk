/**
 * SPA App - Main Application
 * Modern Single Page Application with sidebar navigation
 * Features custom titlebar with window controls
 */

#include <easygtk/easygtk.h>
#include "../include/database.h"
#include "../include/app_state.h"
#include "../include/pages.h"
#include <stdio.h>

/* Window control callbacks */
static void on_minimize_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    eg_window_minimize(g_app_state->main_window);
}

static void on_maximize_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    eg_window_toggle_maximize(g_app_state->main_window);
}

static void on_close_click(EgWidget *widget, void *data) {
    (void)widget; (void)data;
    eg_window_close(g_app_state->main_window);
}

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

static EgWidget *create_custom_titlebar(void) {
    EgHeaderBar *header = eg_header_bar_new();

    /* Hide default window buttons - we'll add our own */
    eg_header_bar_set_show_title_buttons(header, false);

    /* Title widget */
    EgBox *title_box = eg_box_new_vertical(2);
    eg_widget_set_valign(eg_box_as_widget(title_box), EG_ALIGN_CENTER);

    EgLabel *title = eg_label_new("SPA Dashboard");
    eg_widget_add_css_class(eg_label_as_widget(title), "titlebar-title");
    eg_box_append(title_box, eg_label_as_widget(title));

    EgLabel *subtitle = eg_label_new("Management System");
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "titlebar-subtitle");
    eg_box_append(title_box, eg_label_as_widget(subtitle));

    eg_header_bar_set_title_widget(header, eg_box_as_widget(title_box));

    /* Window control buttons (right side) */
    EgBox *controls = eg_box_new_horizontal(4);
    eg_widget_set_valign(eg_box_as_widget(controls), EG_ALIGN_CENTER);

    /* Minimize button */
    EgButton *btn_min = eg_button_new("—");
    eg_widget_add_css_class(eg_button_as_widget(btn_min), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_min), "window-button-minimize");
    eg_button_on_click(btn_min, on_minimize_click, NULL);
    eg_box_append(controls, eg_button_as_widget(btn_min));

    /* Maximize button */
    EgButton *btn_max = eg_button_new("□");
    eg_widget_add_css_class(eg_button_as_widget(btn_max), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_max), "window-button-maximize");
    eg_button_on_click(btn_max, on_maximize_click, NULL);
    eg_box_append(controls, eg_button_as_widget(btn_max));

    /* Close button */
    EgButton *btn_close = eg_button_new("✕");
    eg_widget_add_css_class(eg_button_as_widget(btn_close), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_close), "window-button-close");
    eg_button_on_click(btn_close, on_close_click, NULL);
    eg_box_append(controls, eg_button_as_widget(btn_close));

    eg_header_bar_pack_end(header, eg_box_as_widget(controls));

    return eg_header_bar_as_widget(header);
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
    eg_bind(eg_label_as_widget(page_title), g_app_state->view_model, "current_page_title");
    eg_widget_add_css_class(eg_label_as_widget(page_title), "page-title");

    EgLabel *username = eg_label_new("");
    eg_bind(eg_label_as_widget(username), g_app_state->view_model, "username");
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

    /* Set custom titlebar */
    eg_window_set_titlebar(window, create_custom_titlebar());

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
