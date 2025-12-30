/**
 * EasyGTK - ListView e ColumnView Example
 *
 * Demonstra o uso de ListView (lista simples) e ColumnView (tabela).
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;
static EgListView *list_view = NULL;
static EgColumnView *column_view = NULL;

/* ================== ListView Callbacks ================== */

static void on_list_selection_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgListView *lv = (EgListView *)widget;

    int selected = eg_list_view_get_selected(lv);
    if (selected >= 0) {
        const char *text = eg_list_view_get_selected_text(lv);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "ListView: Selecionado item %d - \"%s\"", selected, text ? text : "");
        eg_label_set_text(status_label, buffer);
    }
}

static void on_list_activate(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgListView *lv = (EgListView *)widget;

    int selected = eg_list_view_get_selected(lv);
    const char *text = eg_list_view_get_selected_text(lv);
    printf("ListView: Item %d ativado (duplo clique): %s\n", selected, text ? text : "");
}

/* ================== ColumnView Callbacks ================== */

static void on_column_selection_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgColumnView *cv = (EgColumnView *)widget;

    int selected = eg_column_view_get_selected_row(cv);
    if (selected >= 0) {
        const char *col0 = eg_column_view_get_cell(cv, (unsigned int)selected, 0);
        const char *col1 = eg_column_view_get_cell(cv, (unsigned int)selected, 1);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "ColumnView: Linha %d - %s, %s", selected,
                 col0 ? col0 : "", col1 ? col1 : "");
        eg_label_set_text(status_label, buffer);
    }
}

static void on_column_activate(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgColumnView *cv = (EgColumnView *)widget;

    int selected = eg_column_view_get_selected_row(cv);
    if (selected >= 0) {
        const char *name = eg_column_view_get_cell(cv, (unsigned int)selected, 0);
        printf("ColumnView: Linha %d ativada: %s\n", selected, name ? name : "");
    }
}

/* ================== Botões de Ação ================== */

static void on_add_list_item(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    static int counter = 1;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Novo Item %d", counter++);
    eg_list_view_append(list_view, buffer);

    eg_label_set_text(status_label, "Item adicionado à ListView");
}

static void on_remove_list_item(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    int selected = eg_list_view_get_selected(list_view);
    if (selected >= 0) {
        eg_list_view_remove(list_view, (unsigned int)selected);
        eg_label_set_text(status_label, "Item removido da ListView");
    } else {
        eg_label_set_text(status_label, "Selecione um item para remover");
    }
}

static void on_add_table_row(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    static int id = 100;
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id++);

    eg_column_view_append_rowv(column_view,
        id_str,
        "Novo Usuario",
        "novo@email.com",
        "Ativo",
        NULL);

    eg_label_set_text(status_label, "Linha adicionada à ColumnView");
}

static void on_remove_table_row(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    int selected = eg_column_view_get_selected_row(column_view);
    if (selected >= 0) {
        eg_column_view_remove_row(column_view, (unsigned int)selected);
        eg_label_set_text(status_label, "Linha removida da ColumnView");
    } else {
        eg_label_set_text(status_label, "Selecione uma linha para remover");
    }
}

static void on_clear_all(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    eg_list_view_clear(list_view);
    eg_column_view_clear(column_view);
    eg_label_set_text(status_label, "Todas as listas foram limpas");
}

/* ================== Ordenação ================== */

static void on_sort_list_asc(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    eg_list_view_sort_ascending(list_view);
    eg_label_set_text(status_label, "ListView ordenada A-Z");
}

static void on_sort_list_desc(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    eg_list_view_sort_descending(list_view);
    eg_label_set_text(status_label, "ListView ordenada Z-A");
}

static void on_sort_table_by_name(EgWidget *widget, void *user_data) {
    (void)widget;
    static bool ascending = true;
    eg_column_view_sort_by_column(column_view, 1, ascending);
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "ColumnView ordenada por Nome (%s)", ascending ? "A-Z" : "Z-A");
    eg_label_set_text(status_label, buffer);
    ascending = !ascending;
    (void)user_data;
}

static void on_sort_table_by_email(EgWidget *widget, void *user_data) {
    (void)widget;
    static bool ascending = true;
    eg_column_view_sort_by_column(column_view, 2, ascending);
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "ColumnView ordenada por Email (%s)", ascending ? "A-Z" : "Z-A");
    eg_label_set_text(status_label, buffer);
    ascending = !ascending;
    (void)user_data;
}

/* ================== Criação da UI ================== */

static EgWidget *create_list_view_section(void) {
    EgFrame *frame = eg_frame_new("ListView - Lista Simples");

    EgBox *box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box), 10);

    /* Lista */
    const char *items[] = {
        "Maça",
        "Banana",
        "Laranja",
        "Uva",
        "Morango",
        "Abacaxi",
        "Manga",
        "Melancia",
        NULL
    };

    list_view = eg_list_view_new_with_items(items, EG_SELECTION_SINGLE);
    eg_list_view_set_show_separators(list_view, true);
    eg_list_view_on_selection_changed(list_view, on_list_selection_changed, NULL);
    eg_list_view_on_activate(list_view, on_list_activate, NULL);

    /* Coloca em ScrolledWindow */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_widget_set_size_request(eg_scrolled_window_as_widget(scroll), -1, 200);
    eg_scrolled_window_set_child(scroll, eg_list_view_as_widget(list_view));

    eg_box_append(box, eg_scrolled_window_as_widget(scroll));

    /* Botões de ação */
    EgBox *btn_box = eg_box_new_horizontal(5);

    EgButton *btn_add = eg_button_new("Adicionar");
    eg_button_on_click(btn_add, on_add_list_item, NULL);
    eg_box_append(btn_box, eg_button_as_widget(btn_add));

    EgButton *btn_remove = eg_button_new("Remover");
    eg_button_on_click(btn_remove, on_remove_list_item, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_remove), "destructive-action");
    eg_box_append(btn_box, eg_button_as_widget(btn_remove));

    eg_box_append(box, eg_box_as_widget(btn_box));

    /* Botões de ordenação */
    EgBox *sort_box = eg_box_new_horizontal(5);

    EgLabel *sort_label = eg_label_new("Ordenar:");
    eg_box_append(sort_box, eg_label_as_widget(sort_label));

    EgButton *btn_sort_asc = eg_button_new("A-Z");
    eg_button_on_click(btn_sort_asc, on_sort_list_asc, NULL);
    eg_box_append(sort_box, eg_button_as_widget(btn_sort_asc));

    EgButton *btn_sort_desc = eg_button_new("Z-A");
    eg_button_on_click(btn_sort_desc, on_sort_list_desc, NULL);
    eg_box_append(sort_box, eg_button_as_widget(btn_sort_desc));

    eg_box_append(box, eg_box_as_widget(sort_box));

    eg_frame_set_child(frame, eg_box_as_widget(box));

    return eg_frame_as_widget(frame);
}

static EgWidget *create_column_view_section(void) {
    EgFrame *frame = eg_frame_new("ColumnView - Tabela");

    EgBox *box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box), 10);

    /* Tabela */
    column_view = eg_column_view_new(EG_SELECTION_SINGLE);

    /* Adiciona colunas - todas com expand para alinhamento correto */
    int id_col = eg_column_view_add_column(column_view, "ID", 0);
    int name_col = eg_column_view_add_column(column_view, "Nome", 1);
    int email_col = eg_column_view_add_column(column_view, "Email", 2);
    int status_col = eg_column_view_add_column(column_view, "Status", 3);

    /* Todas as colunas expandem proporcionalmente */
    eg_column_view_set_column_expand(column_view, id_col, true);
    eg_column_view_set_column_expand(column_view, name_col, true);
    eg_column_view_set_column_expand(column_view, email_col, true);
    eg_column_view_set_column_expand(column_view, status_col, true);

    /* Habilita ordenação por clique no header */
    eg_column_view_set_column_sortable(column_view, id_col, true);
    eg_column_view_set_column_sortable(column_view, name_col, true);
    eg_column_view_set_column_sortable(column_view, email_col, true);
    eg_column_view_set_column_sortable(column_view, status_col, true);

    /* Adiciona dados */
    eg_column_view_append_rowv(column_view, "1", "Ana Silva", "ana@email.com", "Ativo", NULL);
    eg_column_view_append_rowv(column_view, "2", "Bruno Costa", "bruno@email.com", "Ativo", NULL);
    eg_column_view_append_rowv(column_view, "3", "Carla Lima", "carla@email.com", "Inativo", NULL);
    eg_column_view_append_rowv(column_view, "4", "Daniel Souza", "daniel@email.com", "Ativo", NULL);
    eg_column_view_append_rowv(column_view, "5", "Elena Santos", "elena@email.com", "Pendente", NULL);

    eg_column_view_set_show_row_separators(column_view, true);
    eg_column_view_set_show_column_separators(column_view, true);
    eg_column_view_on_selection_changed(column_view, on_column_selection_changed, NULL);
    eg_column_view_on_activate(column_view, on_column_activate, NULL);

    /* Coloca em ScrolledWindow */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_widget_set_size_request(eg_scrolled_window_as_widget(scroll), -1, 200);
    eg_widget_set_hexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_scrolled_window_set_child(scroll, eg_column_view_as_widget(column_view));

    eg_box_append(box, eg_scrolled_window_as_widget(scroll));

    /* Botões de ação */
    EgBox *btn_box = eg_box_new_horizontal(5);

    EgButton *btn_add = eg_button_new("Adicionar Linha");
    eg_button_on_click(btn_add, on_add_table_row, NULL);
    eg_box_append(btn_box, eg_button_as_widget(btn_add));

    EgButton *btn_remove = eg_button_new("Remover Linha");
    eg_button_on_click(btn_remove, on_remove_table_row, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_remove), "destructive-action");
    eg_box_append(btn_box, eg_button_as_widget(btn_remove));

    eg_box_append(box, eg_box_as_widget(btn_box));

    /* Botões de ordenação */
    EgBox *sort_box = eg_box_new_horizontal(5);

    EgLabel *sort_label = eg_label_new("Ordenar por:");
    eg_box_append(sort_box, eg_label_as_widget(sort_label));

    EgButton *btn_sort_name = eg_button_new("Nome");
    eg_button_on_click(btn_sort_name, on_sort_table_by_name, NULL);
    eg_box_append(sort_box, eg_button_as_widget(btn_sort_name));

    EgButton *btn_sort_email = eg_button_new("Email");
    eg_button_on_click(btn_sort_email, on_sort_table_by_email, NULL);
    eg_box_append(sort_box, eg_button_as_widget(btn_sort_email));

    eg_box_append(box, eg_box_as_widget(sort_box));

    eg_frame_set_child(frame, eg_box_as_widget(box));

    return eg_frame_as_widget(frame);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Janela principal */
    EgWindow *window = eg_window_new(app, "ListView e ColumnView - EasyGTK", 900, 600);

    /* Layout principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 15);

    /* Título */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>ListView e ColumnView Demo</span>");
    eg_box_append(main_box, eg_label_as_widget(title));

    /* Descrição */
    EgLabel *desc = eg_label_new(
        "ListView para listas simples (strings) e ColumnView para tabelas com multiplas colunas.\n"
        "Clique em um item para selecionar, duplo clique para ativar. Use os botões de ordenação!"
    );
    eg_label_set_wrap(desc, true);
    eg_widget_add_css_class(eg_label_as_widget(desc), "dim-label");
    eg_box_append(main_box, eg_label_as_widget(desc));

    /* Container horizontal para as duas listas */
    EgBox *lists_box = eg_box_new_horizontal(15);
    eg_widget_set_vexpand(eg_box_as_widget(lists_box), true);

    /* ListView à esquerda */
    EgWidget *list_section = create_list_view_section();
    eg_box_append(lists_box, list_section);

    /* ColumnView à direita */
    EgWidget *column_section = create_column_view_section();
    eg_widget_set_hexpand(column_section, true);
    eg_box_append(lists_box, column_section);

    eg_box_append(main_box, eg_box_as_widget(lists_box));

    /* Separador */
    EgSeparator *sep = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep));

    /* Rodapé com status e botão limpar */
    EgBox *footer = eg_box_new_horizontal(10);

    status_label = eg_label_new("Selecione um item para ver detalhes");
    eg_label_set_xalign(status_label, 0.0f);
    eg_widget_set_hexpand(eg_label_as_widget(status_label), true);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    eg_box_append(footer, eg_label_as_widget(status_label));

    EgButton *btn_clear = eg_button_new("Limpar Tudo");
    eg_button_on_click(btn_clear, on_clear_all, NULL);
    eg_box_append(footer, eg_button_as_widget(btn_clear));

    eg_box_append(main_box, eg_box_as_widget(footer));

    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    EgApp *app = eg_app_new("com.example.listview");
    if (!app) return 1;

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);
    eg_app_free(app);

    return status;
}
