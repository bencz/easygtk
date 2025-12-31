/**
 * EasyGTK - Widgets Example
 * 
 * Demonstrates CheckButton, Switch and ProgressBar.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgProgressBar *progress_bar = NULL;
static EgLabel *status_label = NULL;
static EgTimerId timer_id = 0;
static double progress = 0.0;

/* Timer callback to animate the progress bar */
static bool on_timer_tick(void *user_data) {
    (void)user_data;
    
    progress += 0.02;
    if (progress > 1.0) {
        progress = 0.0;
    }
    
    eg_progress_bar_set_fraction(progress_bar, progress);
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%.0f%%", progress * 100);
    eg_progress_bar_set_text(progress_bar, buf);
    
    return true; /* Continue the timer */
}

/* CheckButton callback */
static void on_check_toggled(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgCheckButton *cb = (EgCheckButton *)widget;
    bool active = eg_check_button_get_active(cb);
    
    const char *label = eg_check_button_get_label(cb);
    printf("CheckButton '%s': %s\n", label, active ? "checked" : "unchecked");
}

/* Switch callback - controls the timer */
static void on_switch_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgSwitch *sw = (EgSwitch *)widget;
    bool active = eg_switch_get_active(sw);
    
    if (active) {
        if (timer_id == 0) {
            timer_id = eg_timeout_add(50, on_timer_tick, NULL);
            eg_label_set_text(status_label, "Progress: Running");
        }
    } else {
        if (timer_id != 0) {
            eg_source_remove(timer_id);
            timer_id = 0;
            eg_label_set_text(status_label, "Progress: Paused");
        }
    }
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Main window */
    EgWindow *window = eg_window_new(app, "Widgets Demo", 400, 350);
    eg_window_set_resizable(window, false);
    
    /* Main box */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>New Widgets</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === CheckButtons Section === */
    EgLabel *cb_title = eg_label_new("CheckButtons:");
    eg_widget_set_halign(eg_label_as_widget(cb_title), EG_ALIGN_START);
    
    EgBox *cb_box = eg_box_new_vertical(5);
    eg_widget_set_margin(eg_box_as_widget(cb_box), 10);
    
    EgCheckButton *cb1 = eg_check_button_new("Option 1");
    eg_check_button_on_toggled(cb1, on_check_toggled, NULL);
    eg_widget_set_tooltip(eg_check_button_as_widget(cb1), "First option");
    
    EgCheckButton *cb2 = eg_check_button_new("Option 2");
    eg_check_button_on_toggled(cb2, on_check_toggled, NULL);
    eg_widget_set_tooltip(eg_check_button_as_widget(cb2), "Second option");
    
    EgCheckButton *cb3 = eg_check_button_new("Option 3 (disabled)");
    eg_widget_set_sensitive(eg_check_button_as_widget(cb3), false);
    
    eg_box_append(cb_box, eg_check_button_as_widget(cb1));
    eg_box_append(cb_box, eg_check_button_as_widget(cb2));
    eg_box_append(cb_box, eg_check_button_as_widget(cb3));
    
    /* === Switch Section === */
    EgBox *switch_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(switch_box), EG_ALIGN_START);
    
    EgLabel *switch_label = eg_label_new("Animate Progress Bar:");
    EgSwitch *sw = eg_switch_new();
    eg_switch_on_state_changed(sw, on_switch_changed, NULL);
    eg_widget_set_tooltip(eg_switch_as_widget(sw), "Toggle animation on/off");
    
    eg_box_append(switch_box, eg_label_as_widget(switch_label));
    eg_box_append(switch_box, eg_switch_as_widget(sw));
    
    /* === ProgressBar Section === */
    EgLabel *pb_title = eg_label_new("ProgressBar:");
    eg_widget_set_halign(eg_label_as_widget(pb_title), EG_ALIGN_START);
    
    progress_bar = eg_progress_bar_new();
    eg_progress_bar_set_show_text(progress_bar, true);
    eg_progress_bar_set_text(progress_bar, "0%");
    eg_widget_set_hexpand(eg_progress_bar_as_widget(progress_bar), true);
    
    status_label = eg_label_new("Progress: Paused");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* === Radio Buttons Section === */
    EgLabel *radio_title = eg_label_new("Radio Buttons (group):");
    eg_widget_set_halign(eg_label_as_widget(radio_title), EG_ALIGN_START);
    
    EgBox *radio_box = eg_box_new_horizontal(15);
    
    EgCheckButton *radio1 = eg_check_button_new("Small");
    EgCheckButton *radio2 = eg_check_button_new("Medium");
    EgCheckButton *radio3 = eg_check_button_new("Large");
    
    /* Group as radio buttons */
    eg_check_button_set_group(radio2, radio1);
    eg_check_button_set_group(radio3, radio1);
    eg_check_button_set_active(radio2, true); /* Select the middle one */
    
    eg_box_append(radio_box, eg_check_button_as_widget(radio1));
    eg_box_append(radio_box, eg_check_button_as_widget(radio2));
    eg_box_append(radio_box, eg_check_button_as_widget(radio3));
    
    /* Build the UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(cb_title));
    eg_box_append(main_box, eg_box_as_widget(cb_box));
    eg_box_append(main_box, eg_box_as_widget(switch_box));
    eg_box_append(main_box, eg_label_as_widget(pb_title));
    eg_box_append(main_box, eg_progress_bar_as_widget(progress_bar));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    eg_box_append(main_box, eg_label_as_widget(radio_title));
    eg_box_append(main_box, eg_box_as_widget(radio_box));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.widgets");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    
    if (timer_id != 0) {
        eg_source_remove(timer_id);
    }
    
    eg_app_free(app);
    
    return status;
}
