/**
 * EasyGTK - Exemplo de Containers
 * 
 * Demonstra Stack, Notebook, Paned, Frame e Expander.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;

/* Callback do Notebook */
static void on_page_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgNotebook *notebook = (EgNotebook *)widget;
    char buf[64];
    snprintf(buf, sizeof(buf), "Notebook: página %d selecionada", 
             eg_notebook_get_current_page(notebook) + 1);
    eg_label_set_text(status_label, buf);
}

/* Cria conteúdo para uma página */
static EgWidget *create_page_content(const char *text, const char *color_class) {
    EgBox *box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box), 20);
    
    EgLabel *label = eg_label_new(text);
    eg_widget_set_halign(eg_label_as_widget(label), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_label_as_widget(label), EG_ALIGN_CENTER);
    eg_widget_set_vexpand(eg_label_as_widget(label), true);
    
    if (color_class != NULL) {
        eg_widget_add_css_class(eg_label_as_widget(label), color_class);
    }
    
    eg_box_append(box, eg_label_as_widget(label));
    return eg_box_as_widget(box);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Containers Demo", 700, 550);
    
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Título */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Containers</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === Paned com Stack e Notebook === */
    EgPaned *paned = eg_paned_new_horizontal();
    eg_paned_set_wide_handle(paned, true);
    eg_widget_set_vexpand(eg_paned_as_widget(paned), true);
    
    /* --- Lado esquerdo: Stack com StackSwitcher --- */
    EgBox *stack_container = eg_box_new_vertical(5);
    
    EgLabel *stack_title = eg_label_new(NULL);
    eg_label_set_markup(stack_title, "<b>Stack</b>");
    
    EgStack *stack = eg_stack_new();
    eg_stack_set_transition_type(stack, EG_STACK_TRANSITION_SLIDE_LEFT_RIGHT);
    eg_stack_set_transition_duration(stack, 300);
    
    /* Páginas do Stack */
    EgLabel *page1 = eg_label_new("Página 1 do Stack\n\nUse o switcher acima");
    eg_widget_set_margin(eg_label_as_widget(page1), 20);
    eg_stack_add_titled(stack, eg_label_as_widget(page1), "page1", "Página 1");
    
    EgLabel *page2 = eg_label_new("Página 2 do Stack\n\nTransição animada!");
    eg_widget_set_margin(eg_label_as_widget(page2), 20);
    eg_stack_add_titled(stack, eg_label_as_widget(page2), "page2", "Página 2");
    
    EgLabel *page3 = eg_label_new("Página 3 do Stack\n\n🎉");
    eg_widget_set_margin(eg_label_as_widget(page3), 20);
    eg_stack_add_titled(stack, eg_label_as_widget(page3), "page3", "Página 3");
    
    EgStackSwitcher *switcher = eg_stack_switcher_new();
    eg_stack_switcher_set_stack(switcher, stack);
    eg_widget_set_halign(eg_stack_switcher_as_widget(switcher), EG_ALIGN_CENTER);
    
    eg_box_append(stack_container, eg_label_as_widget(stack_title));
    eg_box_append(stack_container, eg_stack_switcher_as_widget(switcher));
    eg_box_append(stack_container, eg_stack_as_widget(stack));
    eg_widget_set_vexpand(eg_stack_as_widget(stack), true);
    
    /* --- Lado direito: Notebook --- */
    EgBox *notebook_container = eg_box_new_vertical(5);
    
    EgLabel *notebook_title = eg_label_new(NULL);
    eg_label_set_markup(notebook_title, "<b>Notebook (Tabs)</b>");
    
    EgNotebook *notebook = eg_notebook_new();
    eg_notebook_set_scrollable(notebook, true);
    eg_notebook_on_page_changed(notebook, on_page_changed, NULL);
    
    /* Páginas do Notebook */
    eg_notebook_append_page(notebook, create_page_content("Aba 1\nConteúdo da primeira aba", NULL), "Aba 1");
    eg_notebook_append_page(notebook, create_page_content("Aba 2\nConteúdo da segunda aba", NULL), "Aba 2");
    eg_notebook_append_page(notebook, create_page_content("Aba 3\nConteúdo da terceira aba", NULL), "Aba 3");
    
    eg_box_append(notebook_container, eg_label_as_widget(notebook_title));
    eg_box_append(notebook_container, eg_notebook_as_widget(notebook));
    eg_widget_set_vexpand(eg_notebook_as_widget(notebook), true);
    
    /* Configura Paned */
    eg_paned_set_start_child(paned, eg_box_as_widget(stack_container));
    eg_paned_set_end_child(paned, eg_box_as_widget(notebook_container));
    eg_paned_set_position(paned, 300);
    
    /* === Frame e Expander === */
    EgBox *bottom_box = eg_box_new_horizontal(15);
    
    /* Frame */
    EgFrame *frame = eg_frame_new("Frame com Borda");
    EgLabel *frame_content = eg_label_new("Conteúdo dentro do Frame");
    eg_widget_set_margin(eg_label_as_widget(frame_content), 15);
    eg_frame_set_child(frame, eg_label_as_widget(frame_content));
    eg_widget_set_hexpand(eg_frame_as_widget(frame), true);
    
    /* Expander */
    EgExpander *expander = eg_expander_new("Clique para expandir");
    EgBox *expander_content = eg_box_new_vertical(5);
    eg_widget_set_margin(eg_box_as_widget(expander_content), 10);
    
    EgLabel *exp_label1 = eg_label_new("Conteúdo oculto 1");
    EgLabel *exp_label2 = eg_label_new("Conteúdo oculto 2");
    EgLabel *exp_label3 = eg_label_new("Conteúdo oculto 3");
    
    eg_box_append(expander_content, eg_label_as_widget(exp_label1));
    eg_box_append(expander_content, eg_label_as_widget(exp_label2));
    eg_box_append(expander_content, eg_label_as_widget(exp_label3));
    
    eg_expander_set_child(expander, eg_box_as_widget(expander_content));
    eg_widget_set_hexpand(eg_expander_as_widget(expander), true);
    
    eg_box_append(bottom_box, eg_frame_as_widget(frame));
    eg_box_append(bottom_box, eg_expander_as_widget(expander));
    
    /* === Status === */
    status_label = eg_label_new("Explore os containers acima");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* Monta UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_paned_as_widget(paned));
    eg_box_append(main_box, eg_box_as_widget(bottom_box));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.containers");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
