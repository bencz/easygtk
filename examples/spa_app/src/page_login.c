/**
 * SPA App - Login Page
 *
 * Demonstrates use of the validation system in a real page.
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>
#include <string.h>

static EgEntry *entry_username = NULL;
static EgEntry *entry_password = NULL;
static EgLabel *label_error = NULL;

/* ============================================
 * Validation and Login
 * ============================================ */

static void on_login_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;

    /* Validate fields using the validation system */
    EgWidget *fields[] = {
        eg_entry_as_widget(entry_username),
        eg_entry_as_widget(entry_password)
    };

    /* Validate all fields */
    int first_invalid = eg_validate_first_invalid(fields, 2);
    if (first_invalid >= 0) {
        eg_widget_grab_focus(fields[first_invalid]);
        return;
    }

    /* Fields valid - try login */
    const char *username = eg_entry_get_text(entry_username);
    const char *password = eg_entry_get_text(entry_password);

    if (app_state_login(username, password)) {
        eg_label_set_text(label_error, "");
        eg_entry_set_text(entry_username, "");
        eg_entry_set_text(entry_password, "");

        /* Clear visual error state */
        eg_widget_clear_validation_error(eg_entry_as_widget(entry_username));
        eg_widget_clear_validation_error(eg_entry_as_widget(entry_password));

        app_state_navigate_to(PAGE_DASHBOARD);
    } else {
        eg_label_set_markup(label_error, "<span color='#f38ba8'>Invalid username or password</span>");
    }
}

/* ============================================
 * Validator Setup
 * ============================================ */

static void setup_validators(void) {
    /* Username: required, minimum 3 characters */
    EgValidatorChain *username_chain = eg_validator_chain_new();
    eg_validator_add_required(username_chain, "Username is required");
    eg_validator_add_min_length(username_chain, 3, "Username must be at least 3 characters");
    eg_widget_set_validators(eg_entry_as_widget(entry_username), username_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_username), true);

    /* Password: required, minimum 4 characters */
    EgValidatorChain *password_chain = eg_validator_chain_new();
    eg_validator_add_required(password_chain, "Password is required");
    eg_validator_add_min_length(password_chain, 4, "Password must be at least 4 characters");
    eg_widget_set_validators(eg_entry_as_widget(entry_password), password_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_password), true);
}

/* ============================================
 * Page Creation
 * ============================================ */

EgWidget *page_login_create(void) {
    /* Main container */
    EgBox *main_box = eg_box_new_vertical(0);
    eg_widget_set_vexpand(eg_box_as_widget(main_box), true);
    eg_widget_set_hexpand(eg_box_as_widget(main_box), true);
    eg_widget_set_valign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    eg_widget_set_halign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_box_as_widget(main_box), "login-container");

    /* Login box */
    EgBox *login_box = eg_box_new_vertical(20);
    eg_widget_set_size_request(eg_box_as_widget(login_box), 400, -1);
    eg_widget_add_css_class(eg_box_as_widget(login_box), "login-box");

    /* Title */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='xx-large' weight='bold' foreground='#cdd6f4'>Welcome Back</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(title), "login-title");

    /* Subtitle */
    EgLabel *subtitle = eg_label_new("Sign in to continue to your dashboard");
    eg_widget_set_halign(eg_label_as_widget(subtitle), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "login-subtitle");

    /* Username */
    EgLabel *label_username = eg_label_new("Username");
    eg_widget_set_halign(eg_label_as_widget(label_username), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(label_username), "form-label");

    entry_username = eg_entry_new();
    eg_entry_set_placeholder(entry_username, "Enter your username");

    /* Password */
    EgLabel *label_password = eg_label_new("Password");
    eg_widget_set_halign(eg_label_as_widget(label_password), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(label_password), "form-label");

    entry_password = eg_entry_new();
    eg_entry_set_placeholder(entry_password, "Enter your password");
    eg_entry_set_visibility(entry_password, false);

    /* Setup validators AFTER creating the widgets */
    setup_validators();

    /* Error label */
    label_error = eg_label_new("");
    eg_widget_set_halign(eg_label_as_widget(label_error), EG_ALIGN_CENTER);

    /* Login button */
    EgButton *btn_login = eg_button_new("Sign In");
    eg_widget_add_css_class(eg_button_as_widget(btn_login), "btn-primary");
    eg_button_on_click(btn_login, on_login_click, NULL);

    /* Info */
    EgLabel *info = eg_label_new("");
    eg_label_set_markup(info, "<span size='small' foreground='#7f849c'>Default credentials:\n"
                               "Username: <b>admin</b> | Password: <b>admin123</b></span>");
    eg_widget_set_halign(eg_label_as_widget(info), EG_ALIGN_CENTER);

    /* Assembly */
    eg_box_append(login_box, eg_label_as_widget(title));
    eg_box_append(login_box, eg_label_as_widget(subtitle));
    eg_box_append(login_box, eg_label_as_widget(label_username));
    eg_box_append(login_box, eg_entry_as_widget(entry_username));
    eg_box_append(login_box, eg_label_as_widget(label_password));
    eg_box_append(login_box, eg_entry_as_widget(entry_password));
    eg_box_append(login_box, eg_label_as_widget(label_error));
    eg_box_append(login_box, eg_button_as_widget(btn_login));
    eg_box_append(login_box, eg_label_as_widget(info));

    eg_box_append(main_box, eg_box_as_widget(login_box));

    return eg_box_as_widget(main_box);
}
