/**
 * EasyGTK - MenuBar Example
 *
 * Demonstrates a traditional menu bar (File, Edit, View, Help).
 * Classic desktop application layout.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;
static EgWindow *main_window = NULL;

/* Callback for menu actions */
static void on_menu_action(const char *action_name, void *user_data) {
    (void)user_data;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Action: %s", action_name);
    eg_label_set_text(status_label, buffer);

    printf("Menu action: %s\n", action_name);
}

/* Special actions */
static void on_new_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "New document created");
}

static void on_open_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "Opening file...");
}

static void on_save_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "File saved!");
}

static void on_quit_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    if (main_window) {
        eg_window_close(main_window);
    }
}

static void on_fullscreen_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    if (main_window) {
        if (eg_window_is_fullscreen(main_window)) {
            eg_window_unfullscreen(main_window);
            eg_label_set_text(status_label, "Windowed mode");
        } else {
            eg_window_fullscreen(main_window);
            eg_label_set_text(status_label, "Fullscreen mode (F11 to exit)");
        }
    }
}

static void on_about_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_dialog_info(main_window, "About",
        "MenuBar Example\n\n"
        "Traditional menu bar demonstration\n"
        "using EasyGTK.\n\n"
        "Version 1.0");
}

/* Creates the menu bar */
static EgWidget *create_menubar(void) {
    EgBox *menubar = eg_box_new_horizontal(0);
    eg_widget_add_css_class(eg_box_as_widget(menubar), "menubar");

    /* === File Menu === */
    EgPopoverMenu *file_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(file_menu, "New", "document-new-symbolic", "new", on_new_action, NULL);
    eg_popover_menu_add_item_with_icon(file_menu, "Open...", "document-open-symbolic", "open", on_open_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item_with_icon(file_menu, "Save", "document-save-symbolic", "save", on_save_action, NULL);
    eg_popover_menu_add_item(file_menu, "Save as...", "save_as", on_menu_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item(file_menu, "Export...", "export", on_menu_action, NULL);
    eg_popover_menu_add_item(file_menu, "Print...", "print", on_menu_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item_with_icon(file_menu, "Quit", "application-exit-symbolic", "quit", on_quit_action, NULL);

    EgMenuButton *file_btn = eg_menu_button_new_with_label("File");
    eg_menu_button_set_popover(file_btn, file_menu);
    eg_menu_button_set_has_frame(file_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(file_btn));

    /* === Edit Menu === */
    EgPopoverMenu *edit_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(edit_menu, "Undo", "edit-undo-symbolic", "undo", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Redo", "edit-redo-symbolic", "redo", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Cut", "edit-cut-symbolic", "cut", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Copy", "edit-copy-symbolic", "copy", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Paste", "edit-paste-symbolic", "paste", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item(edit_menu, "Select All", "select_all", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Preferences", "preferences-system-symbolic", "preferences", on_menu_action, NULL);

    EgMenuButton *edit_btn = eg_menu_button_new_with_label("Edit");
    eg_menu_button_set_popover(edit_btn, edit_menu);
    eg_menu_button_set_has_frame(edit_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(edit_btn));

    /* === View Menu === */
    EgPopoverMenu *view_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(view_menu, "Zoom In", "zoom-in-symbolic", "zoom_in", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(view_menu, "Zoom Out", "zoom-out-symbolic", "zoom_out", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Original Size", "zoom_100", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item(view_menu, "Show Sidebar", "show_sidebar", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Show Statusbar", "show_statusbar", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item_with_icon(view_menu, "Fullscreen", "view-fullscreen-symbolic", "fullscreen", on_fullscreen_action, NULL);

    EgMenuButton *view_btn = eg_menu_button_new_with_label("View");
    eg_menu_button_set_popover(view_btn, view_menu);
    eg_menu_button_set_has_frame(view_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(view_btn));

    /* === Tools Menu === */
    EgPopoverMenu *tools_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(tools_menu, "Spell Check", "spell_check", on_menu_action, NULL);
    eg_popover_menu_add_item(tools_menu, "Word Count", "word_count", on_menu_action, NULL);
    eg_popover_menu_add_separator(tools_menu);
    eg_popover_menu_add_item(tools_menu, "Macros...", "macros", on_menu_action, NULL);
    eg_popover_menu_add_item(tools_menu, "Scripts...", "scripts", on_menu_action, NULL);

    EgMenuButton *tools_btn = eg_menu_button_new_with_label("Tools");
    eg_menu_button_set_popover(tools_btn, tools_menu);
    eg_menu_button_set_has_frame(tools_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(tools_btn));

    /* === Help Menu === */
    EgPopoverMenu *help_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(help_menu, "Documentation", "help-browser-symbolic", "docs", on_menu_action, NULL);
    eg_popover_menu_add_item(help_menu, "Keyboard Shortcuts", "shortcuts", on_menu_action, NULL);
    eg_popover_menu_add_separator(help_menu);
    eg_popover_menu_add_item(help_menu, "Check for Updates", "check_updates", on_menu_action, NULL);
    eg_popover_menu_add_separator(help_menu);
    eg_popover_menu_add_item_with_icon(help_menu, "About", "help-about-symbolic", "about", on_about_action, NULL);

    EgMenuButton *help_btn = eg_menu_button_new_with_label("Help");
    eg_menu_button_set_popover(help_btn, help_menu);
    eg_menu_button_set_has_frame(help_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(help_btn));

    return eg_box_as_widget(menubar);
}

/* Creates the toolbar */
static EgWidget *create_toolbar(void) {
    EgBox *toolbar = eg_box_new_horizontal(4);
    eg_widget_add_css_class(eg_box_as_widget(toolbar), "toolbar");
    eg_widget_set_margin(eg_box_as_widget(toolbar), 4);

    EgButton *btn_new = eg_button_new_with_icon("document-new-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_new), "New");
    eg_box_append(toolbar, eg_button_as_widget(btn_new));

    EgButton *btn_open = eg_button_new_with_icon("document-open-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_open), "Open");
    eg_box_append(toolbar, eg_button_as_widget(btn_open));

    EgButton *btn_save = eg_button_new_with_icon("document-save-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_save), "Save");
    eg_box_append(toolbar, eg_button_as_widget(btn_save));

    EgSeparator *sep1 = eg_separator_new_vertical();
    eg_box_append(toolbar, eg_separator_as_widget(sep1));

    EgButton *btn_undo = eg_button_new_with_icon("edit-undo-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_undo), "Undo");
    eg_box_append(toolbar, eg_button_as_widget(btn_undo));

    EgButton *btn_redo = eg_button_new_with_icon("edit-redo-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_redo), "Redo");
    eg_box_append(toolbar, eg_button_as_widget(btn_redo));

    EgSeparator *sep2 = eg_separator_new_vertical();
    eg_box_append(toolbar, eg_separator_as_widget(sep2));

    EgButton *btn_cut = eg_button_new_with_icon("edit-cut-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_cut), "Cut");
    eg_box_append(toolbar, eg_button_as_widget(btn_cut));

    EgButton *btn_copy = eg_button_new_with_icon("edit-copy-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_copy), "Copy");
    eg_box_append(toolbar, eg_button_as_widget(btn_copy));

    EgButton *btn_paste = eg_button_new_with_icon("edit-paste-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_paste), "Paste");
    eg_box_append(toolbar, eg_button_as_widget(btn_paste));

    return eg_box_as_widget(toolbar);
}

/* Creates the status bar */
static EgWidget *create_statusbar(void) {
    EgBox *statusbar = eg_box_new_horizontal(10);
    eg_widget_add_css_class(eg_box_as_widget(statusbar), "statusbar");
    eg_widget_set_margin(eg_box_as_widget(statusbar), 6);

    status_label = eg_label_new("Ready");
    eg_label_set_xalign(status_label, 0.0f);
    eg_widget_set_hexpand(eg_label_as_widget(status_label), true);
    eg_box_append(statusbar, eg_label_as_widget(status_label));

    EgSeparator *sep = eg_separator_new_vertical();
    eg_box_append(statusbar, eg_separator_as_widget(sep));

    EgLabel *pos_label = eg_label_new("Line 1, Column 1");
    eg_box_append(statusbar, eg_label_as_widget(pos_label));

    return eg_box_as_widget(statusbar);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Create window */
    main_window = eg_window_new(app, "Text Editor - EasyGTK", 800, 600);

    /* Main layout */
    EgBox *main_box = eg_box_new_vertical(0);

    /* Menu bar */
    eg_box_append(main_box, create_menubar());

    /* Separator after menu */
    EgSeparator *sep1 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep1));

    /* Toolbar */
    eg_box_append(main_box, create_toolbar());

    /* Separator after toolbar */
    EgSeparator *sep2 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep2));

    /* Content area (simulates a text editor) */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_widget_set_hexpand(eg_scrolled_window_as_widget(scroll), true);

    EgTextView *text_view = eg_text_view_new();
    eg_text_view_set_text(text_view,
        "Welcome to the MenuBar example!\n\n"
        "This example demonstrates a traditional desktop application interface\n"
        "with a menu bar (File, Edit, View, Help), toolbar\n"
        "and status bar.\n\n"
        "Try clicking on the menus to see the available actions.\n\n"
        "Features demonstrated:\n"
        "- Menu bar with multiple menus\n"
        "- Menu items with icons\n"
        "- Separators between item groups\n"
        "- Toolbar with icon buttons\n"
        "- Status bar\n"
        "- Editable text area\n"
    );
    eg_text_view_set_wrap_mode(text_view, 2); /* WORD wrap */
    eg_widget_set_margin(eg_text_view_as_widget(text_view), 10);

    eg_scrolled_window_set_child(scroll, eg_text_view_as_widget(text_view));
    eg_box_append(main_box, eg_scrolled_window_as_widget(scroll));

    /* Separator before statusbar */
    EgSeparator *sep3 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep3));

    /* Status bar */
    eg_box_append(main_box, create_statusbar());

    eg_window_set_child(main_window, eg_box_as_widget(main_box));

    /* CSS to style the interface */
    const char *css =
        ".menubar { padding: 2px 6px; }"
        ".menubar menubutton button { padding: 4px 8px; border-radius: 4px; }"
        ".menubar menubutton button:hover { background-color: alpha(currentColor, 0.1); }"
        ".toolbar button { padding: 6px; }"
        ".statusbar { background-color: alpha(currentColor, 0.05); }";
    eg_css_load_from_string(css);

    eg_window_show(main_window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    EgApp *app = eg_app_new("com.example.menubar");
    if (!app) return 1;

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);
    eg_app_free(app);

    return status;
}
