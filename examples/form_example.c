/**
 * EasyGTK - Form Example
 * 
 * Demonstrates the use of grid, entry and labels to create a form.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgEntry *entry_name = NULL;
static EgEntry *entry_email = NULL;
static EgEntry *entry_password = NULL;
static EgLabel *label_status = NULL;

static void on_submit(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    const char *name = eg_entry_get_text(entry_name);
    const char *email = eg_entry_get_text(entry_email);
    
    printf("Name: %s\n", name);
    printf("Email: %s\n", email);
    
    eg_label_set_markup(label_status, 
        "<span color='green'>Form submitted successfully!</span>");
}

static void on_clear(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    eg_entry_set_text(entry_name, "");
    eg_entry_set_text(entry_email, "");
    eg_entry_set_text(entry_password, "");
    eg_label_set_text(label_status, "");
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Create the window */
    EgWindow *window = eg_window_new(app, "Registration Form", 450, 350);
    eg_window_set_resizable(window, false);
    
    /* Main box */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 25);
    
    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='xx-large' weight='bold'>Registration</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Form grid */
    EgGrid *grid = eg_grid_new();
    eg_grid_set_row_spacing(grid, 12);
    eg_grid_set_column_spacing(grid, 15);
    
    /* Name field */
    EgLabel *lbl_name = eg_label_new("Name:");
    eg_widget_set_halign(eg_label_as_widget(lbl_name), EG_ALIGN_END);
    entry_name = eg_entry_new_with_placeholder("Enter your name");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_name), true);
    
    /* Email field */
    EgLabel *lbl_email = eg_label_new("Email:");
    eg_widget_set_halign(eg_label_as_widget(lbl_email), EG_ALIGN_END);
    entry_email = eg_entry_new_with_placeholder("example@email.com");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_email), true);
    
    /* Password field */
    EgLabel *lbl_password = eg_label_new("Password:");
    eg_widget_set_halign(eg_label_as_widget(lbl_password), EG_ALIGN_END);
    entry_password = eg_entry_new_with_placeholder("Enter your password");
    eg_entry_set_visibility(entry_password, false);
    eg_widget_set_hexpand(eg_entry_as_widget(entry_password), true);
    
    /* Add fields to grid */
    eg_grid_attach(grid, eg_label_as_widget(lbl_name), 0, 0, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_name), 1, 0, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_email), 0, 1, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_email), 1, 1, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_password), 0, 2, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_password), 1, 2, 1, 1);
    
    /* Button box */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);
    
    EgButton *btn_submit = eg_button_new("Submit");
    eg_button_on_click(btn_submit, on_submit, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_submit), "suggested-action");
    
    EgButton *btn_clear = eg_button_new("Clear");
    eg_button_on_click(btn_clear, on_clear, NULL);
    
    eg_box_append(button_box, eg_button_as_widget(btn_submit));
    eg_box_append(button_box, eg_button_as_widget(btn_clear));
    
    /* Status label */
    label_status = eg_label_new("");
    eg_widget_set_halign(eg_label_as_widget(label_status), EG_ALIGN_CENTER);
    
    /* Build the interface */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_grid_as_widget(grid));
    eg_box_append(main_box, eg_box_as_widget(button_box));
    eg_box_append(main_box, eg_label_as_widget(label_status));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    EgApp *app = eg_app_new("com.example.form");
    if (app == NULL) {
        return 1;
    }
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
