/**
 * SPA App - Application State Implementation
 */

#include "../include/app_state.h"
#include "../include/pages.h"
#include <stdlib.h>
#include <string.h>

AppState *g_app_state = NULL;

AppState *app_state_new(void) {
    AppState *state = (AppState *)calloc(1, sizeof(AppState));
    if (!state) return NULL;

    /* Create ViewModel */
    state->view_model = eg_view_model_new();

    /* Initialize properties */
    eg_view_model_add_property(state->view_model, eg_property_new_string("username", ""));
    eg_view_model_add_property(state->view_model, eg_property_new_string("current_page_title", ""));
    eg_view_model_add_property(state->view_model, eg_property_new_bool("is_authenticated", false));

    /* Statistics properties */
    eg_view_model_add_property(state->view_model, eg_property_new_int("total_users", 0));
    eg_view_model_add_property(state->view_model, eg_property_new_int("total_products", 0));
    eg_view_model_add_property(state->view_model, eg_property_new_double("inventory_value", 0.0));
    eg_view_model_add_property(state->view_model, eg_property_new_int("low_stock_count", 0));

    state->logged_user = NULL;
    state->current_page = PAGE_LOGIN;
    state->is_authenticated = false;

    /* Store global reference */
    g_app_state = state;

    return state;
}

void app_state_free(AppState *state) {
    if (!state) return;

    if (state->view_model) {
        eg_view_model_free(state->view_model);
    }

    if (state->current_user_model) {
        eg_model_free(state->current_user_model);
    }

    if (state->logged_user) {
        db_user_free(state->logged_user);
    }

    free(state);
    g_app_state = NULL;
}

bool app_state_login(const char *username, const char *password) {
    User *user = db_user_authenticate(username, password);
    if (!user) return false;

    /* Store logged user */
    g_app_state->logged_user = user;
    g_app_state->is_authenticated = true;

    /* Update ViewModel */
    eg_view_model_set_string(g_app_state->view_model, "username", user->username);
    eg_view_model_set_bool(g_app_state->view_model, "is_authenticated", true);

    /* Create user model */
    if (g_app_state->current_user_model) {
        eg_model_free(g_app_state->current_user_model);
    }
    g_app_state->current_user_model = eg_model_new();
    eg_model_add_property(g_app_state->current_user_model, eg_property_new_int("id", user->id));
    eg_model_add_property(g_app_state->current_user_model, eg_property_new_string("username", user->username));
    eg_model_add_property(g_app_state->current_user_model, eg_property_new_string("email", user->email));

    /* Update statistics */
    eg_view_model_set_int(g_app_state->view_model, "total_users", db_stats_total_users());
    eg_view_model_set_int(g_app_state->view_model, "total_products", db_stats_total_products());
    eg_view_model_set_double(g_app_state->view_model, "inventory_value", db_stats_total_inventory_value());
    eg_view_model_set_int(g_app_state->view_model, "low_stock_count", db_stats_low_stock_products());

    return true;
}

void app_state_logout(void) {
    if (g_app_state->logged_user) {
        db_user_free(g_app_state->logged_user);
        g_app_state->logged_user = NULL;
    }

    if (g_app_state->current_user_model) {
        eg_model_free(g_app_state->current_user_model);
        g_app_state->current_user_model = NULL;
    }

    g_app_state->is_authenticated = false;
    eg_view_model_set_string(g_app_state->view_model, "username", "");
    eg_view_model_set_bool(g_app_state->view_model, "is_authenticated", false);

    /* Navigate to login */
    app_state_navigate_to(PAGE_LOGIN);
}

bool app_state_is_authenticated(void) {
    return g_app_state && g_app_state->is_authenticated;
}

void app_state_navigate_to(AppPage page) {
    if (!g_app_state) return;

    /* Update current page */
    g_app_state->current_page = page;

    /* Update page title */
    const char *titles[] = {
        "Login",
        "Dashboard",
        "Users Management",
        "Products Management",
        "Settings"
    };
    eg_view_model_set_string(g_app_state->view_model, "current_page_title", titles[page]);

    /* Switch stack to appropriate page */
    if (page == PAGE_LOGIN) {
        if (g_app_state->main_stack) {
            eg_stack_set_visible_child_name(g_app_state->main_stack, "login");
        }
    } else {
        if (g_app_state->main_stack) {
            eg_stack_set_visible_child_name(g_app_state->main_stack, "app");
        }

        if (g_app_state->content_stack) {
            const char *page_names[] = {"", "dashboard", "users", "products", "settings"};
            eg_stack_set_visible_child_name(g_app_state->content_stack, page_names[page]);
        }
    }

    /* Update navigation buttons active state */
    for (int i = 0; i < (int)PAGE_COUNT; i++) {
        if (g_app_state->nav_buttons[i]) {
            if (i == (int)page && page != PAGE_LOGIN) {
                eg_widget_add_css_class(eg_button_as_widget(g_app_state->nav_buttons[i]), "active");
            } else {
                eg_widget_remove_css_class(eg_button_as_widget(g_app_state->nav_buttons[i]), "active");
            }
        }
    }

    /* Call page update function */
    switch (page) {
        case PAGE_DASHBOARD:
            page_dashboard_update();
            break;
        case PAGE_USERS:
            page_users_update();
            break;
        case PAGE_PRODUCTS:
            page_products_update();
            break;
        case PAGE_SETTINGS:
            page_settings_update();
            break;
        default:
            break;
    }
}

AppPage app_state_get_current_page(void) {
    return g_app_state ? g_app_state->current_page : PAGE_LOGIN;
}

User *app_state_get_current_user(void) {
    return g_app_state ? g_app_state->logged_user : NULL;
}

const char *app_state_get_username(void) {
    if (!g_app_state || !g_app_state->logged_user) return "";
    return g_app_state->logged_user->username;
}
