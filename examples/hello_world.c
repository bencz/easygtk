/**
 * EasyGTK - Hello World Example
 * 
 * Demonstrates basic usage of the library.
 */

#include <easygtk/easygtk.h>

static EgWindow *window = NULL;
static EgLabel *label = NULL;

static void on_button_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    eg_label_set_text(label, "Button was clicked!");
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Create the window */
    window = eg_window_new(app, "Hello EasyGTK", 400, 300);
    
    /* Create a vertical box */
    EgBox *box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(box), 20);
    eg_widget_set_halign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    
    /* Create the label */
    label = eg_label_new("Hello, World!");
    eg_label_set_markup(label, "<span size='x-large' weight='bold'>Hello, World!</span>");
    
    /* Create the button */
    EgButton *button = eg_button_new("Click here");
    eg_button_on_click(button, on_button_click, NULL);
    
    /* Add widgets to the box */
    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_button_as_widget(button));
    
    /* Set the box as the window's child */
    eg_window_set_child(window, eg_box_as_widget(box));
    
    /* Show the window */
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    /* Create the application */
    EgApp *app = eg_app_new("com.example.helloworld");
    if (app == NULL) {
        return 1;
    }
    
    /* Set the activation callback */
    eg_app_on_activate(app, on_activate, app);
    
    /* Run the application */
    int status = eg_app_run(app);
    
    /* Clean up resources */
    eg_app_free(app);
    
    return status;
}
