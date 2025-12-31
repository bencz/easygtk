/**
 * EasyGTK - Counter Example
 * 
 * Demonstrates interactivity and state updates.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static int counter = 0;
static EgLabel *label_counter = NULL;

static void update_counter_label(void) {
    char buffer[64];
    snprintf(buffer, sizeof(buffer), 
        "<span size='xx-large' weight='bold'>%d</span>", counter);
    eg_label_set_markup(label_counter, buffer);
}

static void on_increment(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    counter++;
    update_counter_label();
}

static void on_decrement(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    counter--;
    update_counter_label();
}

static void on_reset(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    counter = 0;
    update_counter_label();
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Create the window */
    EgWindow *window = eg_window_new(app, "Counter", 300, 200);
    eg_window_set_resizable(window, false);
    
    /* Main box */
    EgBox *main_box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(main_box), 30);
    eg_widget_set_halign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    
    /* Title */
    EgLabel *title = eg_label_new("Simple Counter");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Counter label */
    label_counter = eg_label_new(NULL);
    eg_widget_set_halign(eg_label_as_widget(label_counter), EG_ALIGN_CENTER);
    update_counter_label();
    
    /* Button box */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);
    
    /* Buttons */
    EgButton *btn_dec = eg_button_new("-");
    eg_widget_set_size_request(eg_button_as_widget(btn_dec), 50, -1);
    eg_button_on_click(btn_dec, on_decrement, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_dec), "destructive-action");
    
    EgButton *btn_reset = eg_button_new("Reset");
    eg_button_on_click(btn_reset, on_reset, NULL);
    
    EgButton *btn_inc = eg_button_new("+");
    eg_widget_set_size_request(eg_button_as_widget(btn_inc), 50, -1);
    eg_button_on_click(btn_inc, on_increment, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_inc), "suggested-action");
    
    eg_box_append(button_box, eg_button_as_widget(btn_dec));
    eg_box_append(button_box, eg_button_as_widget(btn_reset));
    eg_box_append(button_box, eg_button_as_widget(btn_inc));
    
    /* Build the interface */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(label_counter));
    eg_box_append(main_box, eg_box_as_widget(button_box));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    EgApp *app = eg_app_new("com.example.counter");
    if (app == NULL) {
        return 1;
    }
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
