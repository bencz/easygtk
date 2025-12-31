/**
 * EasyGTK - Drag and Drop Example
 * 
 * Demonstrates the drag and drop system with:
 * - Drag source
 * - Drop target
 * - Drag begin and end callbacks
 */

#include <easygtk/easygtk.h>
#include <stdio.h>
#include <string.h>

static EgLabel *status_label = NULL;
static EgLabel *drop_label = NULL;
static int drag_count = 0;

/* Callback to prepare drag data */
static const char *on_drag_prepare(EgWidget *widget, void *user_data) {
    (void)widget;
    const char *text = (const char *)user_data;
    return text;
}

/* Callback when drag begins */
static void on_drag_begin(EgWidget *widget, void *user_data) {
    (void)widget;
    const char *name = (const char *)user_data;
    
    char buf[128];
    snprintf(buf, sizeof(buf), "Dragging: %s", name);
    eg_label_set_text(status_label, buf);
    
    printf("Drag started: %s\n", name);
}

/* Callback when drag ends */
static void on_drag_end(EgWidget *widget, bool success, void *user_data) {
    (void)widget;
    const char *name = (const char *)user_data;
    
    char buf[128];
    snprintf(buf, sizeof(buf), "Drag finished: %s (%s)", 
             name, success ? "success" : "cancelled");
    eg_label_set_text(status_label, buf);
    
    printf("Drag finished: %s (success=%d)\n", name, success);
}

/* Callback when data is dropped */
static void on_drop(EgWidget *widget, const char *data, double x, double y, void *user_data) {
    (void)widget; (void)user_data;
    
    drag_count++;
    
    char buf[256];
    snprintf(buf, sizeof(buf), "Received: \"%s\"\nat (%.0f, %.0f)\nTotal: %d drops", 
             data, x, y, drag_count);
    eg_label_set_text(drop_label, buf);
    
    snprintf(buf, sizeof(buf), "Drop received! Text: %s", data);
    eg_label_set_text(status_label, buf);
    
    printf("Drop at (%.0f, %.0f): %s\n", x, y, data);
}

/* Creates a draggable button */
static EgButton *create_draggable_button(const char *label, const char *drag_data) {
    EgButton *btn = eg_button_new(label);
    
    /* Configure as drag source */
    eg_widget_set_drag_source(
        eg_button_as_widget(btn),
        EG_DRAG_ACTION_COPY,
        on_drag_prepare,
        (void *)drag_data
    );
    
    /* Configure begin and end callbacks */
    eg_widget_on_drag_begin(eg_button_as_widget(btn), on_drag_begin, (void *)label);
    eg_widget_on_drag_end(eg_button_as_widget(btn), on_drag_end, (void *)label);
    
    /* Explanatory tooltip */
    eg_widget_set_tooltip(eg_button_as_widget(btn), "Drag this button to the drop area");
    
    /* Drag cursor */
    eg_widget_set_cursor(eg_button_as_widget(btn), "grab");
    
    return btn;
}

/* Callback to clear drop area */
static void on_clear_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    drag_count = 0;
    eg_label_set_text(drop_label, "Drop items here!\n\nDrag the buttons\nfrom the left");
    eg_label_set_text(status_label, "Area cleared");
}

/* Callback to remove drag source */
static void on_toggle_drag(EgWidget *widget, void *user_data) {
    (void)widget;
    static bool drag_enabled = true;
    EgButton *btn = (EgButton *)user_data;
    
    if (drag_enabled) {
        eg_widget_remove_drag_source(eg_button_as_widget(btn));
        eg_label_set_text(status_label, "Drag disabled on Button 1");
        eg_widget_set_cursor(eg_button_as_widget(btn), "default");
    } else {
        eg_widget_set_drag_source(
            eg_button_as_widget(btn),
            EG_DRAG_ACTION_COPY,
            on_drag_prepare,
            (void *)"Text from Button 1"
        );
        eg_widget_on_drag_begin(eg_button_as_widget(btn), on_drag_begin, (void *)"Button 1");
        eg_widget_on_drag_end(eg_button_as_widget(btn), on_drag_end, (void *)"Button 1");
        eg_label_set_text(status_label, "Drag enabled on Button 1");
        eg_widget_set_cursor(eg_button_as_widget(btn), "grab");
    }
    
    drag_enabled = !drag_enabled;
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Main window */
    EgWindow *window = eg_window_new(app, "Drag and Drop Demo", 600, 400);
    
    /* Main box */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Drag and Drop</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Description */
    EgLabel *desc = eg_label_new("Drag the buttons from the left to the drop area on the right");
    eg_widget_set_halign(eg_label_as_widget(desc), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(desc), "dim-label");
    
/* Horizontal container for drag sources and drop target */
    EgBox *content_box = eg_box_new_horizontal(20);
    eg_widget_set_vexpand(eg_box_as_widget(content_box), true);
    
    /* === Left column: Drag Sources === */
    EgBox *sources_box = eg_box_new_vertical(10);
    
    EgLabel *sources_title = eg_label_new(NULL);
    eg_label_set_markup(sources_title, "<b>Drag Sources</b>");
    eg_widget_set_halign(eg_label_as_widget(sources_title), EG_ALIGN_START);
    
    /* Draggable buttons */
    EgButton *btn1 = create_draggable_button("Button 1", "Text from Button 1");
    EgButton *btn2 = create_draggable_button("Button 2", "Content from Button 2");
    EgButton *btn3 = create_draggable_button("Emoji \xf0\x9f\x8e\x89", "Congratulations! \xf0\x9f\x8e\x89\xf0\x9f\x8e\x8a");
    EgButton *btn4 = create_draggable_button("Code", "printf(\"Hello, World!\");");
    
    eg_box_append(sources_box, eg_label_as_widget(sources_title));
    eg_box_append(sources_box, eg_button_as_widget(btn1));
    eg_box_append(sources_box, eg_button_as_widget(btn2));
    eg_box_append(sources_box, eg_button_as_widget(btn3));
    eg_box_append(sources_box, eg_button_as_widget(btn4));
    
    /* === Right column: Drop Target === */
    EgBox *target_box = eg_box_new_vertical(10);
    eg_widget_set_hexpand(eg_box_as_widget(target_box), true);
    
    EgLabel *target_title = eg_label_new(NULL);
    eg_label_set_markup(target_title, "<b>Drop Area</b>");
    eg_widget_set_halign(eg_label_as_widget(target_title), EG_ALIGN_START);
    
    /* Label that receives drops */
    drop_label = eg_label_new("Drop items here!\n\nDrag the buttons\nfrom the left");
    eg_widget_set_halign(eg_label_as_widget(drop_label), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_label_as_widget(drop_label), EG_ALIGN_CENTER);
    eg_widget_set_hexpand(eg_label_as_widget(drop_label), true);
    eg_widget_set_vexpand(eg_label_as_widget(drop_label), true);
    eg_widget_set_name(eg_label_as_widget(drop_label), "drop-area");
    eg_widget_add_css_class(eg_label_as_widget(drop_label), "card");
    eg_widget_set_margin(eg_label_as_widget(drop_label), 10);
    
    /* Configure as drop target */
    eg_widget_set_drop_target(
        eg_label_as_widget(drop_label),
        EG_DRAG_ACTION_COPY,
        on_drop,
        NULL
    );
    
    eg_box_append(target_box, eg_label_as_widget(target_title));
    eg_box_append(target_box, eg_label_as_widget(drop_label));
    
    /* Build content box */
    eg_box_append(content_box, eg_box_as_widget(sources_box));
    eg_box_append(content_box, eg_box_as_widget(target_box));
    
    /* === Controls === */
    EgBox *controls_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(controls_box), EG_ALIGN_CENTER);
    
    EgButton *btn_clear = eg_button_new("Clear Area");
    eg_button_on_click(btn_clear, on_clear_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_clear), "Clears the drop area");
    
    EgButton *btn_toggle = eg_button_new("Toggle Drag (Button 1)");
    eg_button_on_click(btn_toggle, on_toggle_drag, btn1);
    eg_widget_set_tooltip(eg_button_as_widget(btn_toggle), 
                          "Enables/disables drag on Button 1");
    
    eg_box_append(controls_box, eg_button_as_widget(btn_clear));
    eg_box_append(controls_box, eg_button_as_widget(btn_toggle));
    
    /* === Status === */
    status_label = eg_label_new("Ready - drag a button to start");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* Build main UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(desc));
    eg_box_append(main_box, eg_box_as_widget(content_box));
    eg_box_append(main_box, eg_box_as_widget(controls_box));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.dnd");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
