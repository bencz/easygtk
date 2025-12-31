/**
 * SPA App - Settings Page
 * User settings and theme toggle
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>

static EgSwitch *theme_switch = NULL;

void page_settings_update(void) {
    /* Update theme switch to reflect current state */
    if (theme_switch) {
        eg_switch_set_active(theme_switch, !app_state_is_dark_theme());
    }
}

static void on_logout_click(EgWidget *w, void *ud) {
    (void)w; (void)ud;
    app_state_logout();
}

static void on_theme_switch_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    app_state_toggle_theme();
}

EgWidget *page_settings_create(void) {
    EgBox *page = eg_box_new_vertical(24);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    eg_widget_set_margin(eg_box_as_widget(page), 24);

    /* Title */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='large' weight='bold'>Settings</span>");
    eg_label_set_xalign(title, 0.0f);
    eg_box_append(page, eg_label_as_widget(title));

    /* User Profile Section */
    EgBox *profile_section = eg_box_new_vertical(12);
    eg_widget_add_css_class(eg_box_as_widget(profile_section), "settings-section");

    EgLabel *profile_title = eg_label_new("User Profile");
    eg_widget_add_css_class(eg_label_as_widget(profile_title), "settings-title");
    eg_label_set_xalign(profile_title, 0.0f);
    eg_box_append(profile_section, eg_label_as_widget(profile_title));

    /* Username display */
    EgBox *user_row = eg_box_new_horizontal(12);
    EgLabel *user_label = eg_label_new("Logged in as:");
    eg_widget_add_css_class(eg_label_as_widget(user_label), "settings-label");
    eg_box_append(user_row, eg_label_as_widget(user_label));

    EgLabel *username = eg_label_new("");
    eg_bind(eg_label_as_widget(username), g_app_state->view_model, "username");
    eg_widget_add_css_class(eg_label_as_widget(username), "text-primary");
    eg_box_append(user_row, eg_label_as_widget(username));

    eg_box_append(profile_section, eg_box_as_widget(user_row));
    eg_box_append(page, eg_box_as_widget(profile_section));

    /* Appearance Section */
    EgBox *appearance_section = eg_box_new_vertical(12);
    eg_widget_add_css_class(eg_box_as_widget(appearance_section), "settings-section");

    EgLabel *appearance_title = eg_label_new("Appearance");
    eg_widget_add_css_class(eg_label_as_widget(appearance_title), "settings-title");
    eg_label_set_xalign(appearance_title, 0.0f);
    eg_box_append(appearance_section, eg_label_as_widget(appearance_title));

    /* Theme toggle */
    EgBox *theme_row = eg_box_new_horizontal(12);
    eg_widget_set_hexpand(eg_box_as_widget(theme_row), true);

    EgBox *theme_info = eg_box_new_vertical(4);
    eg_widget_set_hexpand(eg_box_as_widget(theme_info), true);

    EgLabel *theme_label = eg_label_new("Light Theme");
    eg_widget_add_css_class(eg_label_as_widget(theme_label), "settings-label");
    eg_label_set_xalign(theme_label, 0.0f);
    eg_box_append(theme_info, eg_label_as_widget(theme_label));

    EgLabel *theme_desc = eg_label_new("Switch between dark and light color schemes");
    eg_widget_add_css_class(eg_label_as_widget(theme_desc), "settings-description");
    eg_label_set_xalign(theme_desc, 0.0f);
    eg_box_append(theme_info, eg_label_as_widget(theme_desc));

    eg_box_append(theme_row, eg_box_as_widget(theme_info));

    theme_switch = eg_switch_new();
    eg_switch_set_active(theme_switch, !app_state_is_dark_theme());
    eg_widget_set_valign(eg_switch_as_widget(theme_switch), EG_ALIGN_CENTER);
    eg_switch_on_state_changed(theme_switch, on_theme_switch_changed, NULL);
    eg_box_append(theme_row, eg_switch_as_widget(theme_switch));

    eg_box_append(appearance_section, eg_box_as_widget(theme_row));
    eg_box_append(page, eg_box_as_widget(appearance_section));

    /* Actions Section */
    EgBox *actions_section = eg_box_new_vertical(12);
    eg_widget_add_css_class(eg_box_as_widget(actions_section), "settings-section");

    EgLabel *actions_title = eg_label_new("Account");
    eg_widget_add_css_class(eg_label_as_widget(actions_title), "settings-title");
    eg_label_set_xalign(actions_title, 0.0f);
    eg_box_append(actions_section, eg_label_as_widget(actions_title));

    EgButton *btn_logout = eg_button_new("Logout");
    eg_widget_add_css_class(eg_button_as_widget(btn_logout), "btn-danger");
    eg_widget_set_halign(eg_button_as_widget(btn_logout), EG_ALIGN_START);
    eg_button_on_click(btn_logout, on_logout_click, NULL);
    eg_box_append(actions_section, eg_button_as_widget(btn_logout));

    eg_box_append(page, eg_box_as_widget(actions_section));

    /* Spacer */
    EgBox *spacer = eg_box_new_vertical(0);
    eg_widget_set_vexpand(eg_box_as_widget(spacer), true);
    eg_box_append(page, eg_box_as_widget(spacer));

    /* App info */
    EgLabel *info = eg_label_new("");
    eg_label_set_markup(info, "<span size='small' foreground='#7f849c'>SPA Dashboard v1.0\nBuilt with EasyGTK</span>");
    eg_label_set_xalign(info, 0.0f);
    eg_box_append(page, eg_label_as_widget(info));

    return eg_box_as_widget(page);
}
