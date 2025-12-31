/**
 * EasyGTK - Menu Example
 *
 * Demonstrates the use of PopoverMenu and MenuButton.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;

/* Callback for menu actions */
static void on_menu_action(const char *action_name, void *user_data) {
    (void)user_data;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Action executed: %s", action_name);
    eg_label_set_text(status_label, buffer);

    printf("Menu action: %s\n", action_name);
}

/* Callback for quit */
static void on_quit_action(const char *action_name, void *user_data) {
    (void)action_name;
    EgApp *app = (EgApp *)user_data;
    printf("Exiting...\n");

    /* The app will close when the window is destroyed */
    (void)app;
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Create window */
    EgWindow *window = eg_window_new(app, "Menu Example - EasyGTK", 600, 400);

    /* Create HeaderBar with menu */
    EgHeaderBar *header = eg_header_bar_new();
    eg_header_bar_set_show_title_buttons(header, true);

    /* Title */
    EgLabel *title = eg_label_new("Menu Example");
    eg_header_bar_set_title_widget(header, eg_label_as_widget(title));

    /* === Main Menu (hamburger) === */
    EgPopoverMenu *main_menu = eg_popover_menu_new();

    eg_popover_menu_add_item(main_menu, "New", "new", on_menu_action, NULL);
    eg_popover_menu_add_item(main_menu, "Open", "open", on_menu_action, NULL);
    eg_popover_menu_add_item(main_menu, "Save", "save", on_menu_action, NULL);
    eg_popover_menu_add_separator(main_menu);
    eg_popover_menu_add_item(main_menu, "Preferences", "preferences", on_menu_action, NULL);
    eg_popover_menu_add_separator(main_menu);
    eg_popover_menu_add_item(main_menu, "Quit", "quit", on_quit_action, app);

    EgMenuButton *menu_btn = eg_menu_button_new_with_icon("open-menu-symbolic");
    eg_menu_button_set_popover(menu_btn, main_menu);
    eg_header_bar_pack_end(header, eg_menu_button_as_widget(menu_btn));

    /* === Edit Menu === */
    EgPopoverMenu *edit_menu = eg_popover_menu_new();

    eg_popover_menu_add_item_with_icon(edit_menu, "Cut", "edit-cut-symbolic", "cut", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Copy", "edit-copy-symbolic", "copy", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Paste", "edit-paste-symbolic", "paste", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Undo", "edit-undo-symbolic", "undo", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Redo", "edit-redo-symbolic", "redo", on_menu_action, NULL);

    EgMenuButton *edit_btn = eg_menu_button_new_with_label("Edit");
    eg_menu_button_set_popover(edit_btn, edit_menu);
    eg_header_bar_pack_start(header, eg_menu_button_as_widget(edit_btn));

    /* === View Menu === */
    EgPopoverMenu *view_menu = eg_popover_menu_new();

    eg_popover_menu_add_item(view_menu, "Zoom In", "zoom_in", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Zoom Out", "zoom_out", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Actual Size", "zoom_reset", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item(view_menu, "Fullscreen", "fullscreen", on_menu_action, NULL);

    EgMenuButton *view_btn = eg_menu_button_new_with_label("View");
    eg_menu_button_set_popover(view_btn, view_menu);
    eg_header_bar_pack_start(header, eg_menu_button_as_widget(view_btn));

    eg_window_set_titlebar(window, eg_header_bar_as_widget(header));

    /* === Window content === */
    EgBox *main_box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(main_box), 30);

    /* Title */
    EgLabel *main_title = eg_label_new("");
    eg_label_set_markup(main_title, "<span size='xx-large' weight='bold'>PopoverMenu and MenuButton</span>");
    eg_box_append(main_box, eg_label_as_widget(main_title));

    /* Description */
    EgLabel *desc = eg_label_new(
        "This example demonstrates the use of popup menus (PopoverMenu)\n"
        "integrated with menu buttons (MenuButton).\n\n"
        "Click on the 'Edit' and 'View' buttons in the title bar,\n"
        "or on the menu icon in the right corner."
    );
    eg_label_set_wrap(desc, true);
    eg_box_append(main_box, eg_label_as_widget(desc));

    /* Separator */
    EgSeparator *sep = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep));

    /* Inline buttons section */
    EgLabel *inline_title = eg_label_new("");
    eg_label_set_markup(inline_title, "<span weight='bold'>Inline Menus</span>");
    eg_label_set_xalign(inline_title, 0.0f);
    eg_box_append(main_box, eg_label_as_widget(inline_title));

    EgBox *btn_box = eg_box_new_horizontal(10);

    /* Color menu */
    EgPopoverMenu *color_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(color_menu, "Red", "color_red", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Green", "color_green", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Blue", "color_blue", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Yellow", "color_yellow", on_menu_action, NULL);

    EgMenuButton *color_btn = eg_menu_button_new_with_label("Colors");
    eg_menu_button_set_popover(color_btn, color_menu);
    eg_box_append(btn_box, eg_menu_button_as_widget(color_btn));

    /* Size menu */
    EgPopoverMenu *size_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(size_menu, "Small", "size_small", on_menu_action, NULL);
    eg_popover_menu_add_item(size_menu, "Medium", "size_medium", on_menu_action, NULL);
    eg_popover_menu_add_item(size_menu, "Large", "size_large", on_menu_action, NULL);

    EgMenuButton *size_btn = eg_menu_button_new_with_label("Size");
    eg_menu_button_set_popover(size_btn, size_menu);
    eg_box_append(btn_box, eg_menu_button_as_widget(size_btn));

    /* Frameless menu (flat) */
    EgPopoverMenu *flat_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(flat_menu, "Option 1", "opt1", on_menu_action, NULL);
    eg_popover_menu_add_item(flat_menu, "Option 2", "opt2", on_menu_action, NULL);
    eg_popover_menu_add_item(flat_menu, "Option 3", "opt3", on_menu_action, NULL);

    EgMenuButton *flat_btn = eg_menu_button_new_with_icon("view-more-symbolic");
    eg_menu_button_set_popover(flat_btn, flat_menu);
    eg_menu_button_set_has_frame(flat_btn, false);
    eg_widget_set_tooltip(eg_menu_button_as_widget(flat_btn), "Borderless menu");
    eg_box_append(btn_box, eg_menu_button_as_widget(flat_btn));

    eg_box_append(main_box, eg_box_as_widget(btn_box));

    /* Separator */
    EgSeparator *sep2 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep2));

    /* Status */
    EgLabel *status_title = eg_label_new("");
    eg_label_set_markup(status_title, "<span weight='bold'>Status</span>");
    eg_label_set_xalign(status_title, 0.0f);
    eg_box_append(main_box, eg_label_as_widget(status_title));

    status_label = eg_label_new("Click on a menu item to see the action here.");
    eg_label_set_xalign(status_label, 0.0f);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    eg_box_append(main_box, eg_label_as_widget(status_label));

    /* Spacer */
    EgBox *spacer = eg_box_new_vertical(0);
    eg_widget_set_vexpand(eg_box_as_widget(spacer), true);
    eg_box_append(main_box, eg_box_as_widget(spacer));

    /* Footer */
    EgLabel *footer = eg_label_new("EasyGTK - PopoverMenu and MenuButton Demo");
    eg_widget_add_css_class(eg_label_as_widget(footer), "dim-label");
    eg_box_append(main_box, eg_label_as_widget(footer));

    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    EgApp *app = eg_app_new("com.example.menu");
    if (!app) return 1;

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);
    eg_app_free(app);

    return status;
}
