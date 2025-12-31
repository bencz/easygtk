/**
 * EasyGTK - Input Widgets Example
 * 
 * Demonstrates SpinButton, Scale and TextView.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;

/* SpinButton callback */
static void on_spin_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgSpinButton *spin = (EgSpinButton *)widget;
    char buf[64];
    snprintf(buf, sizeof(buf), "SpinButton: %d", eg_spin_button_get_value_as_int(spin));
    eg_label_set_text(status_label, buf);
}

/* Scale callback */
static void on_scale_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgScale *scale = (EgScale *)widget;
    char buf[64];
    snprintf(buf, sizeof(buf), "Scale: %.1f", eg_scale_get_value(scale));
    eg_label_set_text(status_label, buf);
}

/* TextView callback */
static void on_text_changed(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_label_set_text(status_label, "TextView: text changed");
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Input Widgets Demo", 500, 450);
    
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Input Widgets</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === SpinButton === */
    EgFrame *spin_frame = eg_frame_new("SpinButton");
    EgBox *spin_box = eg_box_new_horizontal(10);
    eg_widget_set_margin(eg_box_as_widget(spin_box), 10);
    
    EgLabel *spin_label = eg_label_new("Quantity:");
    EgSpinButton *spin = eg_spin_button_new_int(0, 100, 1);
    eg_spin_button_set_value(spin, 50);
    eg_spin_button_on_value_changed(spin, on_spin_changed, NULL);
    
    eg_box_append(spin_box, eg_label_as_widget(spin_label));
    eg_box_append(spin_box, eg_spin_button_as_widget(spin));
    eg_frame_set_child(spin_frame, eg_box_as_widget(spin_box));
    
    /* === Scale === */
    EgFrame *scale_frame = eg_frame_new("Scale (Slider)");
    EgBox *scale_box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(scale_box), 10);
    
    EgLabel *scale_label = eg_label_new("Volume:");
    EgScale *scale = eg_scale_new_horizontal(0.0, 100.0, 1.0);
    eg_scale_set_value(scale, 75.0);
    eg_scale_set_draw_value(scale, true);
    eg_scale_add_mark(scale, 0, 0, "0");
    eg_scale_add_mark(scale, 50, 0, "50");
    eg_scale_add_mark(scale, 100, 0, "100");
    eg_scale_on_value_changed(scale, on_scale_changed, NULL);
    eg_widget_set_hexpand(eg_scale_as_widget(scale), true);
    
    eg_box_append(scale_box, eg_label_as_widget(scale_label));
    eg_box_append(scale_box, eg_scale_as_widget(scale));
    eg_frame_set_child(scale_frame, eg_box_as_widget(scale_box));
    
    /* === TextView === */
    EgFrame *text_frame = eg_frame_new("TextView (Multi-line)");
    EgBox *text_box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(text_box), 10);
    
    EgTextView *text_view = eg_text_view_new();
    eg_text_view_set_text(text_view, "This is a multi-line\ntext field.\n\nYou can edit freely!");
    eg_text_view_set_wrap_mode(text_view, 2); /* word wrap */
    eg_text_view_on_changed(text_view, on_text_changed, NULL);
    eg_widget_set_size_request(eg_text_view_as_widget(text_view), -1, 100);
    
    /* Put TextView inside ScrolledWindow */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_text_view_as_widget(text_view));
    eg_scrolled_window_set_min_content_size(scroll, -1, 100);
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    
    eg_box_append(text_box, eg_scrolled_window_as_widget(scroll));
    eg_frame_set_child(text_frame, eg_box_as_widget(text_box));
    
    /* === Status === */
    status_label = eg_label_new("Interact with the widgets above");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* Build UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_frame_as_widget(spin_frame));
    eg_box_append(main_box, eg_frame_as_widget(scale_frame));
    eg_box_append(main_box, eg_frame_as_widget(text_frame));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.input_widgets");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
