/**
 * EasyGTK - Menu Example
 *
 * Demonstra o uso de PopoverMenu e MenuButton.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;

/* Callback para ações do menu */
static void on_menu_action(const char *action_name, void *user_data) {
    (void)user_data;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Acao executada: %s", action_name);
    eg_label_set_text(status_label, buffer);

    printf("Menu action: %s\n", action_name);
}

/* Callback para sair */
static void on_quit_action(const char *action_name, void *user_data) {
    (void)action_name;
    EgApp *app = (EgApp *)user_data;
    printf("Saindo...\n");

    /* O aplicativo vai fechar quando a janela for destruída */
    (void)app;
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Criar janela */
    EgWindow *window = eg_window_new(app, "Menu Example - EasyGTK", 600, 400);

    /* Criar HeaderBar com menu */
    EgHeaderBar *header = eg_header_bar_new();
    eg_header_bar_set_show_title_buttons(header, true);

    /* Título */
    EgLabel *title = eg_label_new("Menu Example");
    eg_header_bar_set_title_widget(header, eg_label_as_widget(title));

    /* === Menu Principal (hamburger) === */
    EgPopoverMenu *main_menu = eg_popover_menu_new();

    eg_popover_menu_add_item(main_menu, "Novo", "new", on_menu_action, NULL);
    eg_popover_menu_add_item(main_menu, "Abrir", "open", on_menu_action, NULL);
    eg_popover_menu_add_item(main_menu, "Salvar", "save", on_menu_action, NULL);
    eg_popover_menu_add_separator(main_menu);
    eg_popover_menu_add_item(main_menu, "Preferencias", "preferences", on_menu_action, NULL);
    eg_popover_menu_add_separator(main_menu);
    eg_popover_menu_add_item(main_menu, "Sair", "quit", on_quit_action, app);

    EgMenuButton *menu_btn = eg_menu_button_new_with_icon("open-menu-symbolic");
    eg_menu_button_set_popover(menu_btn, main_menu);
    eg_header_bar_pack_end(header, eg_menu_button_as_widget(menu_btn));

    /* === Menu de Edição === */
    EgPopoverMenu *edit_menu = eg_popover_menu_new();

    eg_popover_menu_add_item_with_icon(edit_menu, "Recortar", "edit-cut-symbolic", "cut", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Copiar", "edit-copy-symbolic", "copy", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Colar", "edit-paste-symbolic", "paste", on_menu_action, NULL);
    eg_popover_menu_add_separator(edit_menu);
    eg_popover_menu_add_item_with_icon(edit_menu, "Desfazer", "edit-undo-symbolic", "undo", on_menu_action, NULL);
    eg_popover_menu_add_item_with_icon(edit_menu, "Refazer", "edit-redo-symbolic", "redo", on_menu_action, NULL);

    EgMenuButton *edit_btn = eg_menu_button_new_with_label("Editar");
    eg_menu_button_set_popover(edit_btn, edit_menu);
    eg_header_bar_pack_start(header, eg_menu_button_as_widget(edit_btn));

    /* === Menu de Visualização === */
    EgPopoverMenu *view_menu = eg_popover_menu_new();

    eg_popover_menu_add_item(view_menu, "Zoom In", "zoom_in", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Zoom Out", "zoom_out", on_menu_action, NULL);
    eg_popover_menu_add_item(view_menu, "Tamanho Real", "zoom_reset", on_menu_action, NULL);
    eg_popover_menu_add_separator(view_menu);
    eg_popover_menu_add_item(view_menu, "Tela Cheia", "fullscreen", on_menu_action, NULL);

    EgMenuButton *view_btn = eg_menu_button_new_with_label("Visualizar");
    eg_menu_button_set_popover(view_btn, view_menu);
    eg_header_bar_pack_start(header, eg_menu_button_as_widget(view_btn));

    eg_window_set_titlebar(window, eg_header_bar_as_widget(header));

    /* === Conteúdo da janela === */
    EgBox *main_box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(main_box), 30);

    /* Título */
    EgLabel *main_title = eg_label_new("");
    eg_label_set_markup(main_title, "<span size='xx-large' weight='bold'>PopoverMenu e MenuButton</span>");
    eg_box_append(main_box, eg_label_as_widget(main_title));

    /* Descrição */
    EgLabel *desc = eg_label_new(
        "Este exemplo demonstra o uso de menus popup (PopoverMenu)\n"
        "integrados com botoes de menu (MenuButton).\n\n"
        "Clique nos botoes 'Editar' e 'Visualizar' na barra de titulo,\n"
        "ou no icone de menu no canto direito."
    );
    eg_label_set_wrap(desc, true);
    eg_box_append(main_box, eg_label_as_widget(desc));

    /* Separador */
    EgSeparator *sep = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep));

    /* Seção de botões inline */
    EgLabel *inline_title = eg_label_new("");
    eg_label_set_markup(inline_title, "<span weight='bold'>Menus Inline</span>");
    eg_label_set_xalign(inline_title, 0.0f);
    eg_box_append(main_box, eg_label_as_widget(inline_title));

    EgBox *btn_box = eg_box_new_horizontal(10);

    /* Menu de cores */
    EgPopoverMenu *color_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(color_menu, "Vermelho", "color_red", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Verde", "color_green", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Azul", "color_blue", on_menu_action, NULL);
    eg_popover_menu_add_item(color_menu, "Amarelo", "color_yellow", on_menu_action, NULL);

    EgMenuButton *color_btn = eg_menu_button_new_with_label("Cores");
    eg_menu_button_set_popover(color_btn, color_menu);
    eg_box_append(btn_box, eg_menu_button_as_widget(color_btn));

    /* Menu de tamanhos */
    EgPopoverMenu *size_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(size_menu, "Pequeno", "size_small", on_menu_action, NULL);
    eg_popover_menu_add_item(size_menu, "Medio", "size_medium", on_menu_action, NULL);
    eg_popover_menu_add_item(size_menu, "Grande", "size_large", on_menu_action, NULL);

    EgMenuButton *size_btn = eg_menu_button_new_with_label("Tamanho");
    eg_menu_button_set_popover(size_btn, size_menu);
    eg_box_append(btn_box, eg_menu_button_as_widget(size_btn));

    /* Menu sem frame (flat) */
    EgPopoverMenu *flat_menu = eg_popover_menu_new();
    eg_popover_menu_add_item(flat_menu, "Opcao 1", "opt1", on_menu_action, NULL);
    eg_popover_menu_add_item(flat_menu, "Opcao 2", "opt2", on_menu_action, NULL);
    eg_popover_menu_add_item(flat_menu, "Opcao 3", "opt3", on_menu_action, NULL);

    EgMenuButton *flat_btn = eg_menu_button_new_with_icon("view-more-symbolic");
    eg_menu_button_set_popover(flat_btn, flat_menu);
    eg_menu_button_set_has_frame(flat_btn, false);
    eg_widget_set_tooltip(eg_menu_button_as_widget(flat_btn), "Menu sem borda");
    eg_box_append(btn_box, eg_menu_button_as_widget(flat_btn));

    eg_box_append(main_box, eg_box_as_widget(btn_box));

    /* Separador */
    EgSeparator *sep2 = eg_separator_new_horizontal();
    eg_box_append(main_box, eg_separator_as_widget(sep2));

    /* Status */
    EgLabel *status_title = eg_label_new("");
    eg_label_set_markup(status_title, "<span weight='bold'>Status</span>");
    eg_label_set_xalign(status_title, 0.0f);
    eg_box_append(main_box, eg_label_as_widget(status_title));

    status_label = eg_label_new("Clique em um item do menu para ver a acao aqui.");
    eg_label_set_xalign(status_label, 0.0f);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    eg_box_append(main_box, eg_label_as_widget(status_label));

    /* Spacer */
    EgBox *spacer = eg_box_new_vertical(0);
    eg_widget_set_vexpand(eg_box_as_widget(spacer), true);
    eg_box_append(main_box, eg_box_as_widget(spacer));

    /* Rodapé */
    EgLabel *footer = eg_label_new("EasyGTK - PopoverMenu e MenuButton Demo");
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
