/**
 * EasyGTK - Dialogs and ComboBox Example
 * 
 * Demonstrates MessageDialog, ComboBox and ScrolledWindow.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgWindow *main_window = NULL;
static EgLabel *result_label = NULL;

/* Question dialog response callback */
static void on_question_response(EgDialogResponse response, void *user_data) {
    (void)user_data;
    
    const char *result;
    switch (response) {
        case EG_RESPONSE_YES: result = "You answered: YES"; break;
        case EG_RESPONSE_NO:  result = "You answered: NO"; break;
        default:              result = "Dialog cancelled"; break;
    }
    
    eg_label_set_text(result_label, result);
    printf("Response: %d\n", response);
}

/* Confirm dialog response callback */
static void on_confirm_response(EgDialogResponse response, void *user_data) {
    (void)user_data;
    
    const char *result;
    switch (response) {
        case EG_RESPONSE_YES: result = "Action confirmed (OK)"; break;
        default:              result = "Action cancelled"; break;
    }
    
    eg_label_set_text(result_label, result);
}

/* Info Button */
static void on_info_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_dialog_info(main_window, "Information", "This is an informational message.");
}

/* Warning Button */
static void on_warning_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_dialog_warning(main_window, "Warning", "This is a warning message!");
}

/* Error Button */
static void on_error_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_dialog_error(main_window, "Error", "A simulated error occurred.");
}

/* Question Button */
static void on_question_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_dialog_question(main_window, "Question", 
                       "Do you want to continue with this operation?",
                       on_question_response, NULL);
}

/* Confirm Button */
static void on_confirm_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_dialog_confirm(main_window, "Confirm", 
                      "Are you sure you want to perform this action?",
                      on_confirm_response, NULL);
}

/* ComboBox callback */
static void on_combo_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgComboBox *combo = (EgComboBox *)widget;
    
    const char *text = eg_combo_box_get_active_text(combo);
    int index = eg_combo_box_get_active(combo);
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Selected: %s (index %d)", 
             text ? text : "none", index);
    eg_label_set_text(result_label, buffer);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Main window */
    main_window = eg_window_new(app, "Dialogs and ComboBox", 450, 400);
    
    /* ScrolledWindow to contain everything */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_policy(scroll, EG_SCROLL_NEVER, EG_SCROLL_AUTOMATIC);
    
    /* Main box */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Dialogs and Widgets</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === Dialogs Section === */
    EgLabel *dialog_title = eg_label_new("Message Dialogs:");
    eg_widget_set_halign(eg_label_as_widget(dialog_title), EG_ALIGN_START);
    
    EgBox *dialog_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(dialog_box), EG_ALIGN_CENTER);
    
    EgButton *btn_info = eg_button_new("Info");
    eg_button_on_click(btn_info, on_info_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_info), "Show information dialog");
    
    EgButton *btn_warning = eg_button_new("Warning");
    eg_button_on_click(btn_warning, on_warning_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_warning), "Show warning dialog");
    
    EgButton *btn_error = eg_button_new("Error");
    eg_button_on_click(btn_error, on_error_click, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_error), "destructive-action");
    
    EgButton *btn_question = eg_button_new("Question");
    eg_button_on_click(btn_question, on_question_click, NULL);
    
    EgButton *btn_confirm = eg_button_new("Confirm");
    eg_button_on_click(btn_confirm, on_confirm_click, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_confirm), "suggested-action");
    
    eg_box_append(dialog_box, eg_button_as_widget(btn_info));
    eg_box_append(dialog_box, eg_button_as_widget(btn_warning));
    eg_box_append(dialog_box, eg_button_as_widget(btn_error));
    eg_box_append(dialog_box, eg_button_as_widget(btn_question));
    eg_box_append(dialog_box, eg_button_as_widget(btn_confirm));
    
    /* === ComboBox Section === */
    EgLabel *combo_title = eg_label_new("ComboBox (Dropdown):");
    eg_widget_set_halign(eg_label_as_widget(combo_title), EG_ALIGN_START);
    
    const char *items[] = { "Option 1", "Option 2", "Option 3", "Option 4", "Option 5", NULL };
    EgComboBox *combo = eg_combo_box_new_with_items(items);
    eg_combo_box_set_active(combo, 0);
    eg_combo_box_on_changed(combo, on_combo_changed, NULL);
    eg_widget_set_tooltip(eg_combo_box_as_widget(combo), "Select an option");
    
    /* === Result label === */
    result_label = eg_label_new("Result will appear here");
    eg_widget_set_halign(eg_label_as_widget(result_label), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(result_label), "dim-label");
    
    /* === Section demonstrating ScrolledWindow === */
    EgLabel *scroll_title = eg_label_new("ScrolledWindow with many items:");
    eg_widget_set_halign(eg_label_as_widget(scroll_title), EG_ALIGN_START);
    
    EgScrolledWindow *inner_scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_min_content_size(inner_scroll, -1, 100);
    eg_scrolled_window_set_max_content_size(inner_scroll, -1, 100);
    
    EgBox *items_box = eg_box_new_vertical(5);
    for (int i = 1; i <= 20; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Item %d in the list", i);
        EgLabel *item = eg_label_new(buf);
        eg_widget_set_halign(eg_label_as_widget(item), EG_ALIGN_START);
        eg_box_append(items_box, eg_label_as_widget(item));
    }
    
    eg_scrolled_window_set_child(inner_scroll, eg_box_as_widget(items_box));
    
    /* Build UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(dialog_title));
    eg_box_append(main_box, eg_box_as_widget(dialog_box));
    eg_box_append(main_box, eg_label_as_widget(combo_title));
    eg_box_append(main_box, eg_combo_box_as_widget(combo));
    eg_box_append(main_box, eg_label_as_widget(result_label));
    eg_box_append(main_box, eg_label_as_widget(scroll_title));
    eg_box_append(main_box, eg_scrolled_window_as_widget(inner_scroll));
    
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(main_box));
    eg_window_set_child(main_window, eg_scrolled_window_as_widget(scroll));
    eg_window_show(main_window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.dialogs");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
