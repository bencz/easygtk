/**
 * SPA App - Login Page
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>
#include <string.h>

static EgEntry *entry_username = NULL;
static EgEntry *entry_password = NULL;
static EgLabel *label_error = NULL;

static void on_login_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;

    const char *username = eg_entry_get_text(entry_username);
    const char *password = eg_entry_get_text(entry_password);

    if (!username || strlen(username) == 0) {
        eg_label_set_markup(label_error, "<span color='#f38ba8'>Please enter username</span>");
        return;
    }

    if (!password || strlen(password) == 0) {
        eg_label_set_markup(label_error, "<span color='#f38ba8'>Please enter password</span>");
        return;
    }

    /* Attempt login */
    if (app_state_login(username, password)) {
        eg_label_set_text(label_error, "");
        eg_entry_set_text(entry_username, "");
        eg_entry_set_text(entry_password, "");
        app_state_navigate_to(PAGE_DASHBOARD);
    } else {
        eg_label_set_markup(label_error, "<span color='#f38ba8'>Invalid username or password</span>");
    }
}

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
