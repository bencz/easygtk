/**
 * EasyGTK - MenuBar Example
 *
 * Demonstra uma barra de menu tradicional (File, Edit, View, Help).
 * Layout classico de aplicacao desktop.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;
static EgWindow *main_window = NULL;

/* Callback para ações do menu */
static void on_menu_action(const char *action_name, void *user_data) {
    (void)user_data;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Acao: %s", action_name);
    eg_label_set_text(status_label, buffer);

    printf("Menu action: %s\n", action_name);
}

/* Ações especiais */
static void on_new_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "Novo documento criado");
}

static void on_open_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "Abrindo arquivo...");
}

static void on_save_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_label_set_text(status_label, "Arquivo salvo!");
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
            eg_label_set_text(status_label, "Modo janela");
        } else {
            eg_window_fullscreen(main_window);
            eg_label_set_text(status_label, "Modo tela cheia (F11 para sair)");
        }
    }
}

static void on_about_action(const char *action_name, void *user_data) {
    (void)action_name;
    (void)user_data;
    eg_dialog_info(main_window, "Sobre",
        "MenuBar Example\n\n"
        "Demonstracao de barra de menu tradicional\n"
        "usando EasyGTK.\n\n"
        "Versao 1.0");
}

/* Cria a barra de menu */
static EgWidget *create_menubar(void) {
    EgBox *menubar = eg_box_new_horizontal(0);
    eg_widget_add_css_class(eg_box_as_widget(menubar), "menubar");

    /* === Menu File === */
    EgPopoverMenu *file_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(file_menu, "Novo", "document-new-symbolic", "new", on_new_action, NULL);
    eg_popover_menu_add_item_with_icon(file_menu, "Abrir...", "document-open-symbolic", "open", on_open_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item_with_icon(file_menu, "Salvar", "document-save-symbolic", "save", on_save_action, NULL);
    eg_popover_menu_add_item(file_menu, "Salvar como...", "save_as", on_menu_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item(file_menu, "Exportar...", "export", on_menu_action, NULL);
    eg_popover_menu_add_item(file_menu, "Imprimir...", "print", on_menu_action, NULL);
    eg_popover_menu_add_separator(file_menu);
    eg_popover_menu_add_item_with_icon(file_menu, "Sair", "application-exit-symbolic", "quit", on_quit_action, NULL);

    EgMenuButton *file_btn = eg_menu_button_new_with_label("Arquivo");
    eg_menu_button_set_popover(file_btn, file_menu);
    eg_menu_button_set_has_frame(file_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(file_btn));

    /* === Menu Edit === */
    EgPopoverMenu *edit_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(edit_menu, "Desfazer", "edit-undo-symbolic", "undo", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Refazer", "edit-redo-symbolic", "redo", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Recortar", "edit-cut-symbolic", "cut", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Copiar", "edit-copy-symbolic", "copy", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Colar", "edit-paste-symbolic", "paste", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item(edit_menu, "Selecionar tudo", "select_all", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Preferencias", "preferences-system-symbolic", "preferences", on_menu_action, NULL);

    EgMenuButton *edit_btn = eg_menu_button_new_with_label("Editar");
    eg_menu_button_set_popover(edit_btn, edit_menu);
    eg_menu_button_set_has_frame(edit_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(edit_btn));

    /* === Menu View === */
    EgPopoverMenu *view_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(view_menu, "Zoom In", "zoom-in-symbolic", "zoom_in", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(view_menu, "Zoom Out", "zoom-out-symbolic", "zoom_out", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Tamanho Original", "zoom_100", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item(view_menu, "Mostrar Barra Lateral", "show_sidebar", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Mostrar Barra de Status", "show_statusbar", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item_with_icon(view_menu, "Tela Cheia", "view-fullscreen-symbolic", "fullscreen", on_fullscreen_action, NULL);

    EgMenuButton *view_btn = eg_menu_button_new_with_label("Exibir");
    eg_menu_button_set_popover(view_btn, view_menu);
    eg_menu_button_set_has_frame(view_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(view_btn));

    /* === Menu Tools === */
    EgPopoverMenu *tools_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(tools_menu, "Verificar Ortografia", "spell_check", on_menu_action, NULL);
    eg_popover_menu_add_item(tools_menu, "Contador de Palavras", "word_count", on_menu_action, NULL);
    eg_popover_menu_add_separator(tools_menu);
    eg_popover_menu_add_item(tools_menu, "Macros...", "macros", on_menu_action, NULL);
    eg_popover_menu_add_item(tools_menu, "Scripts...", "scripts", on_menu_action, NULL);

    EgMenuButton *tools_btn = eg_menu_button_new_with_label("Ferramentas");
    eg_menu_button_set_popover(tools_btn, tools_menu);
    eg_menu_button_set_has_frame(tools_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(tools_btn));

    /* === Menu Help === */
    EgPopoverMenu *help_menu = eg_popover_menu_new();
    eg_popover_menu_add_item_with_icon(help_menu, "Documentacao", "help-browser-symbolic", "docs", on_menu_action, NULL);
    eg_popover_menu_add_item(help_menu, "Atalhos de Teclado", "shortcuts", on_menu_action, NULL);
    eg_popover_menu_add_separator(help_menu);
    eg_popover_menu_add_item(help_menu, "Verificar Atualizacoes", "check_updates", on_menu_action, NULL);
    eg_popover_menu_add_separator(help_menu);
    eg_popover_menu_add_item_with_icon(help_menu, "Sobre", "help-about-symbolic", "about", on_about_action, NULL);

    EgMenuButton *help_btn = eg_menu_button_new_with_label("Ajuda");
    eg_menu_button_set_popover(help_btn, help_menu);
    eg_menu_button_set_has_frame(help_btn, false);
    eg_box_append(menubar, eg_menu_button_as_widget(help_btn));

    return eg_box_as_widget(menubar);
}

/* Cria a barra de ferramentas */
static EgWidget *create_toolbar(void) {
    EgBox *toolbar = eg_box_new_horizontal(4);
    eg_widget_add_css_class(eg_box_as_widget(toolbar), "toolbar");
    eg_widget_set_margin(eg_box_as_widget(toolbar), 4);

    EgButton *btn_new = eg_button_new_with_icon("document-new-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_new), "Novo");
    eg_box_append(toolbar, eg_button_as_widget(btn_new));

    EgButton *btn_open = eg_button_new_with_icon("document-open-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_open), "Abrir");
    eg_box_append(toolbar, eg_button_as_widget(btn_open));

    EgButton *btn_save = eg_button_new_with_icon("document-save-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_save), "Salvar");
    eg_box_append(toolbar, eg_button_as_widget(btn_save));

    EgSeparator *sep1 = eg_separator_new_vertical();
    eg_box_append(toolbar, eg_separator_as_widget(sep1));

    EgButton *btn_undo = eg_button_new_with_icon("edit-undo-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_undo), "Desfazer");
    eg_box_append(toolbar, eg_button_as_widget(btn_undo));

    EgButton *btn_redo = eg_button_new_with_icon("edit-redo-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_redo), "Refazer");
    eg_box_append(toolbar, eg_button_as_widget(btn_redo));

    EgSeparator *sep2 = eg_separator_new_vertical();
    eg_box_append(toolbar, eg_separator_as_widget(sep2));

    EgButton *btn_cut = eg_button_new_with_icon("edit-cut-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_cut), "Recortar");
    eg_box_append(toolbar, eg_button_as_widget(btn_cut));

    EgButton *btn_copy = eg_button_new_with_icon("edit-copy-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_copy), "Copiar");
    eg_box_append(toolbar, eg_button_as_widget(btn_copy));

    EgButton *btn_paste = eg_button_new_with_icon("edit-paste-symbolic");
    eg_widget_set_tooltip(eg_button_as_widget(btn_paste), "Colar");
    eg_box_append(toolbar, eg_button_as_widget(btn_paste));

    return eg_box_as_widget(toolbar);
}

/* Cria a barra de status */
static EgWidget *create_statusbar(void) {
    EgBox *statusbar = eg_box_new_horizontal(10);
    eg_widget_add_css_class(eg_box_as_widget(statusbar), "statusbar");
    eg_widget_set_margin(eg_box_as_widget(statusbar), 6);

    status_label = eg_label_new("Pronto");
    eg_label_set_xalign(status_label, 0.0f);
    eg_widget_set_hexpand(eg_label_as_widget(status_label), true);
    eg_box_append(statusbar, eg_label_as_widget(status_label));

    EgSeparator *sep = eg_separator_new_vertical();
    eg_box_append(statusbar, eg_separator_as_widget(sep));

    EgLabel *pos_label = eg_label_new("Linha 1, Coluna 1");
    eg_box_append(statusbar, eg_label_as_widget(pos_label));

    return eg_box_as_widget(statusbar);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Criar janela */
    main_window = eg_window_new(app, "Editor de Texto - EasyGTK", 800, 600);

    /* Layout principal */
    EgBox *main_box = eg_box_new_vertical(0);

    /* Barra de menu */
    eg_box_append(main_box, create_menubar());

    /* Separador após menu */
    EgSeparator *sep1 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep1));

    /* Barra de ferramentas */
    eg_box_append(main_box, create_toolbar());

    /* Separador após toolbar */
    EgSeparator *sep2 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep2));

    /* Area de conteudo (simula um editor de texto) */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_widget_set_hexpand(eg_scrolled_window_as_widget(scroll), true);

    EgTextView *text_view = eg_text_view_new();
    eg_text_view_set_text(text_view,
        "Bem-vindo ao exemplo de MenuBar!\n\n"
        "Este exemplo demonstra uma interface tradicional de aplicacao desktop\n"
        "com barra de menu (File, Edit, View, Help), barra de ferramentas\n"
        "e barra de status.\n\n"
        "Experimente clicar nos menus para ver as acoes disponiveis.\n\n"
        "Recursos demonstrados:\n"
        "- Barra de menu com multiplos menus\n"
        "- Itens de menu com icones\n"
        "- Separadores entre grupos de itens\n"
        "- Barra de ferramentas com botoes de icone\n"
        "- Barra de status\n"
        "- Area de texto editavel\n"
    );
    eg_text_view_set_wrap_mode(text_view, 2); /* WORD wrap */
    eg_widget_set_margin(eg_text_view_as_widget(text_view), 10);

    eg_scrolled_window_set_child(scroll, eg_text_view_as_widget(text_view));
    eg_box_append(main_box, eg_scrolled_window_as_widget(scroll));

    /* Separador antes da statusbar */
    EgSeparator *sep3 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep3));

    /* Barra de status */
    eg_box_append(main_box, create_statusbar());

    eg_window_set_child(main_window, eg_box_as_widget(main_box));

    /* CSS para estilizar a interface */
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
