/**
 * EasyGTK - New Widgets Example
 * 
 * Demonstrates: Image, Picture, Separator, Spinner, LevelBar, HeaderBar,
 *               FileChooser, ColorChooser, AboutDialog
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgWindow *main_window = NULL;
static EgSpinner *spinner = NULL;
static EgLevelBar *level_bar = NULL;
static EgLabel *status_label = NULL;
static EgLabel *color_label = NULL;

/* Callback for spinner toggle */
static void on_toggle_spinner(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    if (eg_spinner_get_spinning(spinner)) {
        eg_spinner_stop(spinner);
        eg_label_set_text(status_label, "Spinner: Stopped");
    } else {
        eg_spinner_start(spinner);
        eg_label_set_text(status_label, "Spinner: Spinning");
    }
}

/* Callback to increase level bar */
static void on_increase_level(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    double value = eg_level_bar_get_value(level_bar);
    if (value < 1.0) {
        eg_level_bar_set_value(level_bar, value + 0.1);
    }
}

/* Callback to decrease level bar */
static void on_decrease_level(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    double value = eg_level_bar_get_value(level_bar);
    if (value > 0.0) {
        eg_level_bar_set_value(level_bar, value - 0.1);
    }
}

/* Callback for file open */
static void on_file_selected(const char *path, void *user_data) {
    (void)user_data;
    
    if (path != NULL) {
        char buf[512];
        snprintf(buf, sizeof(buf), "File: %s", path);
        eg_label_set_text(status_label, buf);
    } else {
        eg_label_set_text(status_label, "Selection cancelled");
    }
}

/* Callback for open file button */
static void on_open_file(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    EgFileFilter *filter_images = eg_file_filter_new("Images");
    eg_file_filter_add_mime_type(filter_images, "image/png");
    eg_file_filter_add_mime_type(filter_images, "image/jpeg");
    eg_file_filter_add_pattern(filter_images, "*.png");
    eg_file_filter_add_pattern(filter_images, "*.jpg");
    
    EgFileFilter *filter_all = eg_file_filter_new("All files");
    eg_file_filter_add_pattern(filter_all, "*");
    
    EgFileFilter *filters[] = { filter_images, filter_all, NULL };
    
    eg_file_chooser_open_with_filters(main_window, "Open File", filters, on_file_selected, NULL);
}

/* Callback for save file */
static void on_save_file(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    eg_file_chooser_save(main_window, "Save File", "document.txt", on_file_selected, NULL);
}

/* Callback for folder select */
static void on_select_folder(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    eg_file_chooser_select_folder(main_window, "Select Folder", on_file_selected, NULL);
}

/* Callback for color selected */
static void on_color_selected(const EgColor *color, void *user_data) {
    (void)user_data;
    
    if (color != NULL) {
        char hex[16];
        eg_color_to_string(color, hex, sizeof(hex));
        char buf[64];
        snprintf(buf, sizeof(buf), "Color: %s", hex);
        eg_label_set_text(color_label, buf);
    } else {
        eg_label_set_text(color_label, "Color: (cancelled)");
    }
}

/* Callback for choose color */
static void on_choose_color(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    EgColor initial = eg_color_from_rgb(52, 152, 219);
    eg_color_chooser_dialog_with_alpha(main_window, "Choose Color", &initial, true, on_color_selected, NULL);
}

/* Callback for show About */
static void on_show_about(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    EgAboutDialog *about = eg_about_dialog_new();
    eg_about_dialog_set_program_name(about, "EasyGTK Demo");
    eg_about_dialog_set_version(about, "1.0.0");
    eg_about_dialog_set_copyright(about, "© 2024 EasyGTK Project");
    eg_about_dialog_set_comments(about, "Demonstration of new widgets from EasyGTK library.");
    eg_about_dialog_set_website(about, "https://github.com/example/easygtk");
    eg_about_dialog_set_website_label(about, "Project Page");
    eg_about_dialog_set_license(about, "MIT License\n\nPermission is hereby granted...");
    eg_about_dialog_set_wrap_license(about, true);
    
    const char *authors[] = { "Developer 1", "Developer 2", NULL };
    eg_about_dialog_set_authors(about, authors);
    
    eg_about_dialog_set_logo_icon_name(about, "applications-development");
    
    eg_about_dialog_show(about, main_window);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    main_window = eg_window_new(app, "New Widgets - EasyGTK", 700, 600);
    
    /* Custom HeaderBar */
    EgHeaderBar *header = eg_header_bar_new();
    eg_header_bar_set_show_title_buttons(header, true);
    
    /* Title in HeaderBar */
    EgLabel *title = eg_label_new("New Widgets Demo");
    eg_widget_add_css_class(eg_label_as_widget(title), "title");
    eg_header_bar_set_title_widget(header, eg_label_as_widget(title));
    
    /* About button in HeaderBar */
    EgButton *btn_about = eg_button_new_with_icon("help-about");
    eg_button_on_click(btn_about, on_show_about, NULL);
    eg_header_bar_pack_end(header, eg_button_as_widget(btn_about));
    
    /* Set HeaderBar as window titlebar */
    eg_window_set_titlebar(main_window, eg_header_bar_as_widget(header));
    
    /* Main container */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* === Section: Image and Picture === */
    EgFrame *frame_images = eg_frame_new("Image & Picture");
    EgBox *box_images = eg_box_new_horizontal(20);
    eg_widget_set_margin(eg_box_as_widget(box_images), 10);
    
    /* Image with icon */
    EgBox *box_icon = eg_box_new_vertical(5);
    EgImage *image = eg_image_new_from_icon("dialog-information");
    eg_image_set_pixel_size(image, 64);
    EgLabel *lbl_icon = eg_label_new("EgImage (icon)");
    eg_box_append(box_icon, eg_image_as_widget(image));
    eg_box_append(box_icon, eg_label_as_widget(lbl_icon));
    eg_widget_set_halign(eg_box_as_widget(box_icon), EG_ALIGN_CENTER);
    
    /* Another Image */
    EgBox *box_icon2 = eg_box_new_vertical(5);
    EgImage *image2 = eg_image_new_from_icon("folder");
    eg_image_set_pixel_size(image2, 64);
    EgLabel *lbl_icon2 = eg_label_new("EgImage (folder)");
    eg_box_append(box_icon2, eg_image_as_widget(image2));
    eg_box_append(box_icon2, eg_label_as_widget(lbl_icon2));
    eg_widget_set_halign(eg_box_as_widget(box_icon2), EG_ALIGN_CENTER);
    
    eg_box_append(box_images, eg_box_as_widget(box_icon));
    eg_box_append(box_images, eg_box_as_widget(box_icon2));
    eg_frame_set_child(frame_images, eg_box_as_widget(box_images));
    
    /* === Section: Spinner === */
    EgFrame *frame_spinner = eg_frame_new("Spinner");
    EgBox *box_spinner = eg_box_new_horizontal(15);
    eg_widget_set_margin(eg_box_as_widget(box_spinner), 10);
    eg_widget_set_halign(eg_box_as_widget(box_spinner), EG_ALIGN_CENTER);
    
    spinner = eg_spinner_new();
    eg_widget_set_size_request(eg_spinner_as_widget(spinner), 32, 32);
    
    EgButton *btn_toggle = eg_button_new("Toggle Spinner");
    eg_button_on_click(btn_toggle, on_toggle_spinner, NULL);
    
    eg_box_append(box_spinner, eg_spinner_as_widget(spinner));
    eg_box_append(box_spinner, eg_button_as_widget(btn_toggle));
    eg_frame_set_child(frame_spinner, eg_box_as_widget(box_spinner));
    
    /* === Section: LevelBar === */
    EgFrame *frame_level = eg_frame_new("LevelBar");
    EgBox *box_level = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box_level), 10);
    
    level_bar = eg_level_bar_new_for_interval(0.0, 1.0);
    eg_level_bar_set_value(level_bar, 0.5);
    eg_widget_set_hexpand(eg_level_bar_as_widget(level_bar), true);
    
    EgBox *box_level_btns = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(box_level_btns), EG_ALIGN_CENTER);
    
    EgButton *btn_dec = eg_button_new("-");
    EgButton *btn_inc = eg_button_new("+");
    eg_button_on_click(btn_dec, on_decrease_level, NULL);
    eg_button_on_click(btn_inc, on_increase_level, NULL);
    
    eg_box_append(box_level_btns, eg_button_as_widget(btn_dec));
    eg_box_append(box_level_btns, eg_button_as_widget(btn_inc));
    
    eg_box_append(box_level, eg_level_bar_as_widget(level_bar));
    eg_box_append(box_level, eg_box_as_widget(box_level_btns));
    eg_frame_set_child(frame_level, eg_box_as_widget(box_level));
    
    /* === Separator === */
    EgSeparator *sep1 = eg_separator_new_horizontal();
    
    /* === Section: FileChooser === */
    EgFrame *frame_file = eg_frame_new("FileChooser");
    EgBox *box_file = eg_box_new_horizontal(10);
    eg_widget_set_margin(eg_box_as_widget(box_file), 10);
    eg_widget_set_halign(eg_box_as_widget(box_file), EG_ALIGN_CENTER);
    
    EgButton *btn_open = eg_button_new("Open...");
    EgButton *btn_save = eg_button_new("Save...");
    EgButton *btn_folder = eg_button_new("Folder...");
    
    eg_button_on_click(btn_open, on_open_file, NULL);
    eg_button_on_click(btn_save, on_save_file, NULL);
    eg_button_on_click(btn_folder, on_select_folder, NULL);
    
    eg_box_append(box_file, eg_button_as_widget(btn_open));
    eg_box_append(box_file, eg_button_as_widget(btn_save));
    eg_box_append(box_file, eg_button_as_widget(btn_folder));
    eg_frame_set_child(frame_file, eg_box_as_widget(box_file));
    
    /* === Section: ColorChooser === */
    EgFrame *frame_color = eg_frame_new("ColorChooser");
    EgBox *box_color = eg_box_new_horizontal(15);
    eg_widget_set_margin(eg_box_as_widget(box_color), 10);
    eg_widget_set_halign(eg_box_as_widget(box_color), EG_ALIGN_CENTER);
    
    EgButton *btn_color = eg_button_new("Choose Color...");
    eg_button_on_click(btn_color, on_choose_color, NULL);
    
    color_label = eg_label_new("Color: (none)");
    
    eg_box_append(box_color, eg_button_as_widget(btn_color));
    eg_box_append(box_color, eg_label_as_widget(color_label));
    eg_frame_set_child(frame_color, eg_box_as_widget(box_color));
    
    /* === Status Label === */
    status_label = eg_label_new("Status: Ready");
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    
    /* Build layout */
    eg_box_append(main_box, eg_frame_as_widget(frame_images));
    eg_box_append(main_box, eg_frame_as_widget(frame_spinner));
    eg_box_append(main_box, eg_frame_as_widget(frame_level));
    eg_box_append(main_box, eg_separator_as_widget(sep1));
    eg_box_append(main_box, eg_frame_as_widget(frame_file));
    eg_box_append(main_box, eg_frame_as_widget(frame_color));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    
    eg_window_set_child(main_window, eg_box_as_widget(main_box));
    eg_window_show(main_window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    EgApp *app = eg_app_new("com.example.newwidgets");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_app_free(app);
    return status;
}
