/**
 * EasyGTK - Generic Container API Example
 *
 * Demonstrates the new eg_container_* functions that work
 * with any container type via vtable.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *info_label = NULL;

/* ============================================
 * Generic function that operates on any container
 * ============================================ */

static void print_container_info(EgWidget *container, const char *name) {
    printf("\n=== %s ===\n", name);

    /* Check if it's a container */
    if (!eg_widget_is_container(container)) {
        printf("  Not a container!\n");
        return;
    }

    /* Show capabilities */
    printf("  Supports multiple children: %s\n",
           eg_container_supports_multiple(container) ? "yes" : "no");
    printf("  Supports named children: %s\n",
           eg_container_supports_named(container) ? "yes" : "no");

    size_t max = eg_container_max_children(container);
    if (max == 0) {
        printf("  Max children: unlimited\n");
    } else {
        printf("  Max children: %zu\n", max);
    }

    /* Count children */
    size_t count = eg_container_get_child_count(container);
    printf("  Current children: %zu\n", count);

    /* List children */
    for (size_t i = 0; i < count; i++) {
        EgWidget *child = eg_container_get_child_at(container, i);
        if (child != NULL) {
            printf("    [%zu] type=%d\n", i, eg_widget_get_type(child));
        }
    }
}

/* ============================================
 * Callbacks
 * ============================================ */

static void on_add_to_box(EgWidget *widget, void *user_data) {
    (void)widget;
    EgBox *box = (EgBox *)user_data;

    /* Create new label */
    static int counter = 0;
    char text[32];
    snprintf(text, sizeof(text), "Item %d", ++counter);

    EgLabel *label = eg_label_new(text);
    eg_widget_add_css_class(eg_label_as_widget(label), "card");
    eg_widget_set_margin(eg_label_as_widget(label), 5);

    /* Use generic API to add */
    eg_container_add(eg_box_as_widget(box), eg_label_as_widget(label));

    /* Update info */
    size_t count = eg_container_get_child_count(eg_box_as_widget(box));
    char info[64];
    snprintf(info, sizeof(info), "Box has %zu children", count);
    eg_label_set_text(info_label, info);
}

static void on_clear_box(EgWidget *widget, void *user_data) {
    (void)widget;
    EgBox *box = (EgBox *)user_data;

    /* Use generic API to clear */
    eg_container_clear(eg_box_as_widget(box));

    eg_label_set_text(info_label, "Box cleared");
}

static void on_show_info(EgWidget *widget, void *user_data) {
    (void)widget;
    EgWidget *container = (EgWidget *)user_data;

    /* Show info in console */
    print_container_info(container, "Selected Container");
}

/* ============================================
 * Interface
 * ============================================ */

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    EgWindow *window = eg_window_new(app, "Generic Container API", 700, 500);

    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);

    /* Title */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Generic Container API</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);

    EgLabel *subtitle = eg_label_new("eg_container_* functions work with any container");
    eg_widget_set_halign(eg_label_as_widget(subtitle), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "dim-label");

    /* Paned for demonstration */
    EgPaned *paned = eg_paned_new_horizontal();
    eg_widget_set_vexpand(eg_paned_as_widget(paned), true);

    /* === Left side: Dynamic Box === */
    EgFrame *frame_box = eg_frame_new("Box (multiple children)");
    EgBox *content_box = eg_box_new_vertical(5);
    eg_widget_set_margin(eg_box_as_widget(content_box), 10);

    /* Box that will be manipulated */
    EgBox *dynamic_box = eg_box_new_vertical(5);
    eg_widget_set_vexpand(eg_box_as_widget(dynamic_box), true);
    eg_widget_add_css_class(eg_box_as_widget(dynamic_box), "view");

    /* Control buttons */
    EgBox *btn_box = eg_box_new_horizontal(5);
    eg_widget_set_halign(eg_box_as_widget(btn_box), EG_ALIGN_CENTER);

    EgButton *btn_add = eg_button_new("Add");
    eg_button_on_click(btn_add, on_add_to_box, dynamic_box);
    eg_widget_add_css_class(eg_button_as_widget(btn_add), "suggested-action");

    EgButton *btn_clear = eg_button_new("Clear");
    eg_button_on_click(btn_clear, on_clear_box, dynamic_box);
    eg_widget_add_css_class(eg_button_as_widget(btn_clear), "destructive-action");

    EgButton *btn_info = eg_button_new("Info");
    eg_button_on_click(btn_info, on_show_info, eg_box_as_widget(dynamic_box));

    eg_box_append(btn_box, eg_button_as_widget(btn_add));
    eg_box_append(btn_box, eg_button_as_widget(btn_clear));
    eg_box_append(btn_box, eg_button_as_widget(btn_info));

    /* ScrolledWindow para o box dinamico */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_policy(scroll, EG_SCROLL_AUTOMATIC, EG_SCROLL_AUTOMATIC);
    eg_scrolled_window_set_min_content_size(scroll, 200, 200);
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(dynamic_box));

    eg_box_append(content_box, eg_scrolled_window_as_widget(scroll));
    eg_box_append(content_box, eg_box_as_widget(btn_box));
    eg_frame_set_child(frame_box, eg_box_as_widget(content_box));

    /* === Right side: Demonstration of different containers === */
    EgFrame *frame_demo = eg_frame_new("Other Containers");
    EgBox *demo_box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(demo_box), 10);

    /* Stack */
    EgLabel *stack_label = eg_label_new("Stack (named, multiple):");
    eg_widget_set_halign(eg_label_as_widget(stack_label), EG_ALIGN_START);

    EgStack *stack = eg_stack_new();
    EgLabel *s1 = eg_label_new("Page A");
    EgLabel *s2 = eg_label_new("Page B");
    eg_container_add_named(eg_stack_as_widget(stack), eg_label_as_widget(s1), "page-a");
    eg_container_add_named(eg_stack_as_widget(stack), eg_label_as_widget(s2), "page-b");

    EgButton *btn_stack_info = eg_button_new("Info Stack");
    eg_button_on_click(btn_stack_info, on_show_info, eg_stack_as_widget(stack));

    /* Frame inner (single child) */
    EgLabel *frame_label = eg_label_new("Frame (1 child only):");
    eg_widget_set_halign(eg_label_as_widget(frame_label), EG_ALIGN_START);

    EgFrame *inner_frame = eg_frame_new("Inner Frame");
    EgLabel *inner_content = eg_label_new("Single content");
    eg_widget_set_margin(eg_label_as_widget(inner_content), 10);
    eg_container_add(eg_frame_as_widget(inner_frame), eg_label_as_widget(inner_content));

    EgButton *btn_frame_info = eg_button_new("Info Frame");
    eg_button_on_click(btn_frame_info, on_show_info, eg_frame_as_widget(inner_frame));

    /* Paned inner (2 children) */
    EgLabel *paned_label = eg_label_new("Paned (2 children):");
    eg_widget_set_halign(eg_label_as_widget(paned_label), EG_ALIGN_START);

    EgPaned *inner_paned = eg_paned_new_horizontal();
    EgLabel *p1 = eg_label_new("Left");
    EgLabel *p2 = eg_label_new("Right");
    eg_widget_set_margin(eg_label_as_widget(p1), 10);
    eg_widget_set_margin(eg_label_as_widget(p2), 10);
    eg_paned_set_start_child(inner_paned, eg_label_as_widget(p1));
    eg_paned_set_end_child(inner_paned, eg_label_as_widget(p2));
    eg_widget_set_size_request(eg_paned_as_widget(inner_paned), -1, 60);

    EgButton *btn_paned_info = eg_button_new("Info Paned");
    eg_button_on_click(btn_paned_info, on_show_info, eg_paned_as_widget(inner_paned));

    /* Build demo */
    eg_box_append(demo_box, eg_label_as_widget(stack_label));
    eg_box_append(demo_box, eg_button_as_widget(btn_stack_info));
    eg_box_append(demo_box, eg_label_as_widget(frame_label));
    eg_box_append(demo_box, eg_frame_as_widget(inner_frame));
    eg_box_append(demo_box, eg_button_as_widget(btn_frame_info));
    eg_box_append(demo_box, eg_label_as_widget(paned_label));
    eg_box_append(demo_box, eg_paned_as_widget(inner_paned));
    eg_box_append(demo_box, eg_button_as_widget(btn_paned_info));

    eg_frame_set_child(frame_demo, eg_box_as_widget(demo_box));

    /* Configure main paned */
    eg_paned_set_start_child(paned, eg_frame_as_widget(frame_box));
    eg_paned_set_end_child(paned, eg_frame_as_widget(frame_demo));
    eg_paned_set_position(paned, 350);

    /* Info label */
    info_label = eg_label_new("Click the buttons to interact");
    eg_widget_set_halign(eg_label_as_widget(info_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(info_label), "dim-label");

    /* Build UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(subtitle));
    eg_box_append(main_box, eg_paned_as_widget(paned));
    eg_box_append(main_box, eg_label_as_widget(info_label));

    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);

    /* Print initial info to console */
    printf("\n=== Container Information ===\n");
    print_container_info(eg_box_as_widget(dynamic_box), "Dynamic Box");
    print_container_info(eg_stack_as_widget(stack), "Stack");
    print_container_info(eg_frame_as_widget(inner_frame), "Frame");
    print_container_info(eg_paned_as_widget(inner_paned), "Paned");
    print_container_info(eg_scrolled_window_as_widget(scroll), "ScrolledWindow");
}

/* ============================================
 * Main
 * ============================================ */

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    const char *css =
        ".card {\n"
        "    padding: 10px;\n"
        "    background: alpha(currentColor, 0.1);\n"
        "    border-radius: 6px;\n"
        "}\n"
        ".dim-label {\n"
        "    opacity: 0.7;\n"
        "}\n";

    EgApp *app = eg_app_new("com.example.container_api");
    if (app == NULL) return 1;

    eg_css_load_from_string(css);
    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);
    eg_app_free(app);

    return status;
}
