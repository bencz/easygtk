/**
 * EasyGTK - Dashboard Example
 *
 * Demonstrates an analytics screen with metric cards, timeline and
 * reactive updates using ViewModel. CSS is loaded optionally
 * for advanced styling, but the example works even without the file.
 */

#include <easygtk/easygtk.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

static EgViewModel *dashboard_vm = NULL;
static EgProgressBar *deployment_progress = NULL;
static EgCssProvider *theme_provider = NULL;
static EgWindow *dashboard_window = NULL;
static EgBox *root_container = NULL;
static EgSwitch *theme_switch = NULL;
static EgLabel *theme_state_label = NULL;
static EgCheckButton *full_bleed_toggle = NULL;
static bool is_dark_theme = false;
static bool surface_full_bleed = false;

static const int APP_SURFACE_MARGIN = 24;

/* ------------------------------------------------------------- */
/* Utilities                                                     */
/* ------------------------------------------------------------- */

static bool css_file_exists(const char *path) {
    return path != NULL && g_file_test(path, G_FILE_TEST_EXISTS);
}

static void set_theme(bool dark, bool sync_switch);
static void apply_surface_mode(void);
static void style_window_button(EgButton *button);

static void load_theme_css(void) {
    if (theme_provider != NULL) {
        return;
    }

    const char *paths[] = {
        "./examples/assets/easygtk_theme.css",
        "../examples/assets/easygtk_theme.css",
        "../../examples/assets/easygtk_theme.css",
        "../../../examples/assets/easygtk_theme.css",
        "examples/assets/easygtk_theme.css",
        "./assets/easygtk_theme.css",
        "../assets/easygtk_theme.css"
    };

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        if (!css_file_exists(paths[i])) {
            continue;
        }

        theme_provider = eg_css_load_from_file(paths[i]);
        if (theme_provider != NULL) {
            return;
        }
    }

    fprintf(stderr,
        "[dashboard_example] Optional CSS not found. Running without custom styles.\n");
}

static void update_dashboard_metrics(void) {
    if (dashboard_vm == NULL) {
        return;
    }

    int active_users = 1250 + rand() % 450;
    int new_signups = 80 + rand() % 70;
    int open_tickets = 12 + rand() % 25;

    double revenue = 42.0 + (rand() % 1200) / 10.0; /* in thousands */
    double conversion = 2.5 + (rand() % 250) / 10.0;
    double progress = (rand() % 96) / 100.0; /* 0.00 - 0.95 */

    char revenue_str[32];
    snprintf(revenue_str, sizeof(revenue_str), "$ %.1fM", revenue / 10.0);

    char conversion_str[32];
    snprintf(conversion_str, sizeof(conversion_str), "%.1f%%", conversion);

    char last_sync_str[64];
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    if (tm_now != NULL) {
        strftime(last_sync_str, sizeof(last_sync_str), "%d/%m %H:%M", tm_now);
    } else {
        snprintf(last_sync_str, sizeof(last_sync_str), "—");
    }

    char deploy_str[64];
    snprintf(deploy_str, sizeof(deploy_str), "Deploy in progress (%d%%)", (int)(progress * 100));

    eg_view_model_set_int(dashboard_vm, "active_users", active_users);
    eg_view_model_set_int(dashboard_vm, "new_signups", new_signups);
    eg_view_model_set_int(dashboard_vm, "open_tickets", open_tickets);
    eg_view_model_set_string(dashboard_vm, "revenue_label", revenue_str);
    eg_view_model_set_string(dashboard_vm, "conversion_label", conversion_str);
    eg_view_model_set_string(dashboard_vm, "last_sync", last_sync_str);
    eg_view_model_set_string(dashboard_vm, "deployment_status", deploy_str);

    if (deployment_progress != NULL) {
        eg_progress_bar_set_fraction(deployment_progress, progress);
        char progress_text[32];
        snprintf(progress_text, sizeof(progress_text), "%d%%", (int)(progress * 100));
        eg_progress_bar_set_text(deployment_progress, progress_text);
        eg_progress_bar_set_show_text(deployment_progress, true);
    }
}

static void on_refresh_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    update_dashboard_metrics();
}

static void set_theme(bool dark, bool sync_switch) {
    is_dark_theme = dark;

    if (dashboard_window != NULL) {
        EgWidget *window_widget = eg_window_as_widget(dashboard_window);
        eg_widget_remove_css_class(window_widget, "theme-light");
        eg_widget_remove_css_class(window_widget, "theme-dark");
        eg_widget_add_css_class(window_widget, dark ? "theme-dark" : "theme-light");
    }

    if (root_container != NULL) {
        EgWidget *root_widget = eg_box_as_widget(root_container);
        eg_widget_remove_css_class(root_widget, "theme-light");
        eg_widget_remove_css_class(root_widget, "theme-dark");
        eg_widget_add_css_class(root_widget, dark ? "theme-dark" : "theme-light");
    }

    if (theme_state_label != NULL) {
        eg_label_set_text(theme_state_label, dark ? "Dark" : "Light");
    }

    if (sync_switch && theme_switch != NULL) {
        if (eg_switch_get_active(theme_switch) != dark) {
            eg_switch_set_active(theme_switch, dark);
        }
    }
}

static void on_theme_switch_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgSwitch *sw = (EgSwitch *)widget;
    set_theme(eg_switch_get_active(sw), false);
}

static void apply_surface_mode(void) {
    if (root_container == NULL) return;
    EgWidget *root_widget = eg_box_as_widget(root_container);
    eg_widget_remove_css_class(root_widget, "full-bleed");
    int margin = APP_SURFACE_MARGIN;
    if (surface_full_bleed) {
        eg_widget_add_css_class(root_widget, "full-bleed");
        margin = 0;
    }
    eg_widget_set_margin(root_widget, margin);
}

static void on_full_bleed_toggled(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgCheckButton *toggle = (EgCheckButton *)widget;
    surface_full_bleed = eg_check_button_get_active(toggle);
    apply_surface_mode();
}

static void style_window_button(EgButton *button) {
    if (button == NULL) return;
    EgWidget *w = eg_button_as_widget(button);
    eg_widget_set_size_request(w, 28, 28);
    eg_widget_set_hexpand(w, false);
    eg_widget_set_vexpand(w, false);
    eg_widget_set_halign(w, EG_ALIGN_CENTER);
    eg_widget_set_valign(w, EG_ALIGN_CENTER);
}

/* ------------------------------------------------------------- */
/* Widget Construction                                           */
/* ------------------------------------------------------------- */

static EgWidget *create_theme_toggle_control(void) {
    EgBox *row = eg_box_new_horizontal(12);

    EgBox *theme_group = eg_box_new_horizontal(4);
    eg_widget_add_css_class(eg_box_as_widget(theme_group), "theme-toggle");

    EgLabel *caption = eg_label_new("Theme");
    eg_widget_add_css_class(eg_label_as_widget(caption), "theme-toggle-caption");

    theme_state_label = eg_label_new("Light");
    eg_widget_add_css_class(eg_label_as_widget(theme_state_label), "theme-toggle-state");

    theme_switch = eg_switch_new();
    eg_widget_add_css_class(eg_switch_as_widget(theme_switch), "theme-switch");
    eg_switch_on_state_changed(theme_switch, on_theme_switch_changed, NULL);

    eg_box_append(theme_group, eg_label_as_widget(caption));
    eg_box_append(theme_group, eg_label_as_widget(theme_state_label));
    eg_box_append(theme_group, eg_switch_as_widget(theme_switch));

    EgBox *layout_group = eg_box_new_horizontal(4);
    EgLabel *layout_caption = eg_label_new("Layout");
    eg_widget_add_css_class(eg_label_as_widget(layout_caption), "layout-toggle-caption");

    full_bleed_toggle = eg_check_button_new("Full bleed");
    eg_widget_add_css_class(eg_check_button_as_widget(full_bleed_toggle), "layout-toggle-check");
    eg_check_button_on_toggled(full_bleed_toggle, on_full_bleed_toggled, NULL);

    eg_box_append(layout_group, eg_label_as_widget(layout_caption));
    eg_box_append(layout_group, eg_check_button_as_widget(full_bleed_toggle));

    eg_box_append(row, eg_box_as_widget(theme_group));
    eg_box_append(row, eg_box_as_widget(layout_group));

    return eg_box_as_widget(row);
}

static void on_minimize_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    if (dashboard_window != NULL) {
        eg_window_minimize(dashboard_window);
    }
}

static void on_maximize_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    if (dashboard_window != NULL) {
        eg_window_toggle_maximize(dashboard_window);
    }
}

static void on_close_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    if (dashboard_window != NULL) {
        eg_window_close(dashboard_window);
    }
}

static EgWidget *create_window_controls(void) {
    EgBox *controls = eg_box_new_horizontal(6);
    eg_widget_add_css_class(eg_box_as_widget(controls), "window-controls");

    EgButton *btn_min = eg_button_new("—");
    eg_widget_add_css_class(eg_button_as_widget(btn_min), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_min), "window-button-minimize");
    style_window_button(btn_min);
    eg_button_on_click(btn_min, on_minimize_click, NULL);

    EgButton *btn_max = eg_button_new("□");
    eg_widget_add_css_class(eg_button_as_widget(btn_max), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_max), "window-button-maximize");
    style_window_button(btn_max);
    eg_button_on_click(btn_max, on_maximize_click, NULL);

    EgButton *btn_close = eg_button_new("✕");
    eg_widget_add_css_class(eg_button_as_widget(btn_close), "window-button");
    eg_widget_add_css_class(eg_button_as_widget(btn_close), "window-button-close");
    style_window_button(btn_close);
    eg_button_on_click(btn_close, on_close_click, NULL);

    eg_box_append(controls, eg_button_as_widget(btn_min));
    eg_box_append(controls, eg_button_as_widget(btn_max));
    eg_box_append(controls, eg_button_as_widget(btn_close));

    return eg_box_as_widget(controls);
}

static EgWidget *create_app_chrome_bar(void) {
    EgBox *chrome = eg_box_new_horizontal(20);
    eg_widget_add_css_class(eg_box_as_widget(chrome), "app-chrome");
    eg_widget_set_hexpand(eg_box_as_widget(chrome), true);
    eg_widget_set_margin(eg_box_as_widget(chrome), 12);

    EgBox *title_box = eg_box_new_vertical(2);
    EgLabel *title = eg_label_new("EasyGTK Dashboard");
    eg_widget_add_css_class(eg_label_as_widget(title), "titlebar-title");
    eg_box_append(title_box, eg_label_as_widget(title));
    eg_widget_add_css_class(eg_box_as_widget(title_box), "app-chrome-title");

    EgWidget *theme_toggle = create_theme_toggle_control();
    eg_widget_set_halign(theme_toggle, EG_ALIGN_END);

    EgBox *spacer = eg_box_new_horizontal(0);
    eg_widget_set_hexpand(eg_box_as_widget(spacer), true);

    EgBox *right_stack = eg_box_new_horizontal(12);
    EgWidget *controls = create_window_controls();
    eg_box_append(right_stack, theme_toggle);
    eg_box_append(right_stack, controls);

    eg_box_append(chrome, eg_box_as_widget(title_box));
    eg_box_append(chrome, eg_box_as_widget(spacer));
    eg_box_append(chrome, eg_box_as_widget(right_stack));

    return eg_box_as_widget(chrome);
}

static EgWidget *create_stat_card(const char *title, const char *prop_name, bool accent) {
    EgFrame *frame = eg_frame_new(NULL);
    eg_widget_add_css_class(eg_frame_as_widget(frame), "stat-card");
    if (accent) {
        eg_widget_add_css_class(eg_frame_as_widget(frame), "stat-card-accent");
    }

    EgBox *box = eg_box_new_vertical(6);

    EgLabel *label = eg_label_new(title);
    eg_widget_add_css_class(eg_label_as_widget(label), "stat-title");

    EgLabel *value = eg_label_new("—");
    eg_widget_add_css_class(eg_label_as_widget(value), "stat-value");
    eg_bind(eg_label_as_widget(value), dashboard_vm, prop_name);

    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_label_as_widget(value));
    eg_frame_set_child(frame, eg_box_as_widget(box));

    return eg_frame_as_widget(frame);
}

static EgWidget *create_stats_section(void) {
    EgBox *stats = eg_box_new_horizontal(16);
    eg_widget_add_css_class(eg_box_as_widget(stats), "stat-grid");

    EgWidget *cards[] = {
        create_stat_card("Active users", "active_users", true),
        create_stat_card("New signups", "new_signups", false),
        create_stat_card("Monthly revenue", "revenue_label", false),
        create_stat_card("Conversion rate", "conversion_label", false)
    };

    for (size_t i = 0; i < sizeof(cards) / sizeof(cards[0]); i++) {
        eg_box_append(stats, cards[i]);
    }

    return eg_box_as_widget(stats);
}

static EgWidget *create_timeline_section(void) {
    EgBox *timeline = eg_box_new_vertical(12);

    const char *events[][2] = {
        {"09:42", "New marketing campaign published"},
        {"10:05", "Sales team closed Enterprise contract"},
        {"11:20", "Mobile app update sent to stores"}
    };

    for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        EgBox *item = eg_box_new_vertical(2);
        eg_widget_add_css_class(eg_box_as_widget(item), "timeline-item");

        EgLabel *time = eg_label_new(events[i][0]);
        eg_widget_add_css_class(eg_label_as_widget(time), "badge");

        EgLabel *desc = eg_label_new(events[i][1]);
        eg_widget_add_css_class(eg_label_as_widget(desc), "page-subtitle");

        eg_box_append(item, eg_label_as_widget(time));
        eg_box_append(item, eg_label_as_widget(desc));
        eg_box_append(timeline, eg_box_as_widget(item));
    }

    return eg_box_as_widget(timeline);
}

static EgWidget *create_deployment_card(void) {
    EgFrame *card = eg_frame_new(NULL);
    eg_widget_add_css_class(eg_frame_as_widget(card), "progress-card");

    EgBox *box = eg_box_new_vertical(8);

    EgLabel *title = eg_label_new("Deploy pipeline");
    eg_widget_add_css_class(eg_label_as_widget(title), "stat-title");

    EgLabel *status = eg_label_new("—");
    eg_widget_add_css_class(eg_label_as_widget(status), "page-subtitle");
    eg_bind(eg_label_as_widget(status), dashboard_vm, "deployment_status");

    deployment_progress = eg_progress_bar_new();
    eg_progress_bar_set_fraction(deployment_progress, 0.15);

    eg_box_append(box, eg_label_as_widget(title));
    eg_box_append(box, eg_label_as_widget(status));
    eg_box_append(box, eg_progress_bar_as_widget(deployment_progress));

    eg_frame_set_child(card, eg_box_as_widget(box));
    return eg_frame_as_widget(card);
}

static EgWidget *create_actions_row(void) {
    EgBox *row = eg_box_new_horizontal(12);

    EgButton *refresh = eg_button_new("Refresh metrics");
    eg_widget_add_css_class(eg_button_as_widget(refresh), "accent-button");
    eg_button_on_click(refresh, on_refresh_click, NULL);

    EgButton *secondary = eg_button_new("View reports");
    eg_widget_add_css_class(eg_button_as_widget(secondary), "pill-button");

    eg_box_append(row, eg_button_as_widget(refresh));
    eg_box_append(row, eg_button_as_widget(secondary));
    return eg_box_as_widget(row);
}

static EgWidget *create_status_row(void) {
    EgBox *row = eg_box_new_horizontal(24);

    EgLabel *last_sync_title = eg_label_new("Last sync");
    eg_widget_add_css_class(eg_label_as_widget(last_sync_title), "page-subtitle");

    EgLabel *last_sync_value = eg_label_new("—");
    eg_widget_add_css_class(eg_label_as_widget(last_sync_value), "badge");
    eg_bind(eg_label_as_widget(last_sync_value), dashboard_vm, "last_sync");

    EgLabel *tickets_title = eg_label_new("Open tickets");
    eg_widget_add_css_class(eg_label_as_widget(tickets_title), "page-subtitle");

    EgLabel *tickets_value = eg_label_new("0");
    eg_widget_add_css_class(eg_label_as_widget(tickets_value), "badge");
    eg_bind(eg_label_as_widget(tickets_value), dashboard_vm, "open_tickets");

    eg_box_append(row, eg_label_as_widget(last_sync_title));
    eg_box_append(row, eg_label_as_widget(last_sync_value));
    eg_box_append(row, eg_label_as_widget(tickets_title));
    eg_box_append(row, eg_label_as_widget(tickets_value));
    return eg_box_as_widget(row);
}

/* ------------------------------------------------------------- */
/* Application Lifecycle                                         */
/* ------------------------------------------------------------- */

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    load_theme_css();

    dashboard_vm = eg_view_model_new();
    eg_view_model_add_property(dashboard_vm, eg_property_new_int("active_users", 0));
    eg_view_model_add_property(dashboard_vm, eg_property_new_int("new_signups", 0));
    eg_view_model_add_property(dashboard_vm, eg_property_new_int("open_tickets", 0));
    eg_view_model_add_property(dashboard_vm, eg_property_new_string("revenue_label", "$ 0"));
    eg_view_model_add_property(dashboard_vm, eg_property_new_string("conversion_label", "0%"));
    eg_view_model_add_property(dashboard_vm, eg_property_new_string("last_sync", "—"));
    eg_view_model_add_property(dashboard_vm, eg_property_new_string("deployment_status", "Ready"));

    EgWindow *window = eg_window_new(app, "EasyGTK Dashboard", 1000, 640);
    dashboard_window = window;

    EgBox *shell = eg_box_new_vertical(0);
    eg_widget_set_hexpand(eg_box_as_widget(shell), true);
    eg_widget_set_vexpand(eg_box_as_widget(shell), true);

    EgWidget *chrome_bar = create_app_chrome_bar();
    eg_widget_set_hexpand(chrome_bar, true);
    eg_widget_set_halign(chrome_bar, EG_ALIGN_FILL);
    eg_box_append(shell, chrome_bar);

    EgBox *root = eg_box_new_vertical(18);
    eg_widget_set_margin(eg_box_as_widget(root), APP_SURFACE_MARGIN);
    eg_widget_add_css_class(eg_box_as_widget(root), "app-surface");
    eg_widget_set_hexpand(eg_box_as_widget(root), true);
    eg_widget_set_vexpand(eg_box_as_widget(root), true);
    eg_widget_set_halign(eg_box_as_widget(root), EG_ALIGN_FILL);
    eg_widget_set_valign(eg_box_as_widget(root), EG_ALIGN_FILL);
    root_container = root;

    EgLabel *title = eg_label_new("Product Overview");
    eg_widget_add_css_class(eg_label_as_widget(title), "page-title");

    EgBox *content = eg_box_new_horizontal(18);

    EgBox *main_column = eg_box_new_vertical(18);
    EgBox *side_column = eg_box_new_vertical(18);
    eg_widget_set_hexpand(eg_box_as_widget(main_column), true);

    eg_box_append(main_column, create_stats_section());
    eg_box_append(main_column, create_actions_row());
    eg_box_append(main_column, create_status_row());

    eg_box_append(side_column, create_deployment_card());
    eg_box_append(side_column, create_timeline_section());

    eg_box_append(content, eg_box_as_widget(main_column));
    eg_box_append(content, eg_box_as_widget(side_column));

    eg_box_append(root, eg_label_as_widget(title));
    eg_box_append(root, eg_box_as_widget(content));

    eg_box_append(shell, eg_box_as_widget(root));

    eg_window_set_child(window, eg_box_as_widget(shell));
    eg_window_set_decorated(window, false);

    eg_window_show(window);

    set_theme(is_dark_theme, true);
    apply_surface_mode();
    update_dashboard_metrics();
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    srand((unsigned int)time(NULL));

    EgApp *app = eg_app_new("com.example.easygtk.dashboard");
    if (app == NULL) {
        return 1;
    }

    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);

    if (dashboard_vm != NULL) {
        eg_view_model_free(dashboard_vm);
    }
    if (theme_provider != NULL) {
        eg_css_provider_free(theme_provider);
    }

    eg_app_free(app);
    return status;
}
