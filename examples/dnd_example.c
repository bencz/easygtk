/**
 * EasyGTK - Exemplo de Drag and Drop
 * 
 * Demonstra o sistema de arrastar e soltar com:
 * - Drag source (fonte de arrasto)
 * - Drop target (destino de soltar)
 * - Callbacks de início e fim do drag
 */

#include <easygtk/easygtk.h>
#include <stdio.h>
#include <string.h>

static EgLabel *status_label = NULL;
static EgLabel *drop_label = NULL;
static int drag_count = 0;

/* Callback para preparar dados do drag */
static const char *on_drag_prepare(EgWidget *widget, void *user_data) {
    (void)widget;
    const char *text = (const char *)user_data;
    return text;
}

/* Callback quando o drag começa */
static void on_drag_begin(EgWidget *widget, void *user_data) {
    (void)widget;
    const char *name = (const char *)user_data;
    
    char buf[128];
    snprintf(buf, sizeof(buf), "Arrastando: %s", name);
    eg_label_set_text(status_label, buf);
    
    printf("Drag começou: %s\n", name);
}

/* Callback quando o drag termina */
static void on_drag_end(EgWidget *widget, bool success, void *user_data) {
    (void)widget;
    const char *name = (const char *)user_data;
    
    char buf[128];
    snprintf(buf, sizeof(buf), "Drag finalizado: %s (%s)", 
             name, success ? "sucesso" : "cancelado");
    eg_label_set_text(status_label, buf);
    
    printf("Drag terminou: %s (sucesso=%d)\n", name, success);
}

/* Callback quando dados são soltos */
static void on_drop(EgWidget *widget, const char *data, double x, double y, void *user_data) {
    (void)widget; (void)user_data;
    
    drag_count++;
    
    char buf[256];
    snprintf(buf, sizeof(buf), "Recebido: \"%s\"\nem (%.0f, %.0f)\nTotal: %d drops", 
             data, x, y, drag_count);
    eg_label_set_text(drop_label, buf);
    
    snprintf(buf, sizeof(buf), "Drop recebido! Texto: %s", data);
    eg_label_set_text(status_label, buf);
    
    printf("Drop em (%.0f, %.0f): %s\n", x, y, data);
}

/* Cria um botão arrastável */
static EgButton *create_draggable_button(const char *label, const char *drag_data) {
    EgButton *btn = eg_button_new(label);
    
    /* Configura como fonte de drag */
    eg_widget_set_drag_source(
        eg_button_as_widget(btn),
        EG_DRAG_ACTION_COPY,
        on_drag_prepare,
        (void *)drag_data
    );
    
    /* Configura callbacks de início e fim */
    eg_widget_on_drag_begin(eg_button_as_widget(btn), on_drag_begin, (void *)label);
    eg_widget_on_drag_end(eg_button_as_widget(btn), on_drag_end, (void *)label);
    
    /* Tooltip explicativo */
    eg_widget_set_tooltip(eg_button_as_widget(btn), "Arraste este botão para a área de drop");
    
    /* Cursor de arrastar */
    eg_widget_set_cursor(eg_button_as_widget(btn), "grab");
    
    return btn;
}

/* Callback para limpar área de drop */
static void on_clear_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    drag_count = 0;
    eg_label_set_text(drop_label, "Solte itens aqui!\n\nArraste os botões\nda esquerda");
    eg_label_set_text(status_label, "Área limpa");
}

/* Callback para remover drag source */
static void on_toggle_drag(EgWidget *widget, void *user_data) {
    (void)widget;
    static bool drag_enabled = true;
    EgButton *btn = (EgButton *)user_data;
    
    if (drag_enabled) {
        eg_widget_remove_drag_source(eg_button_as_widget(btn));
        eg_label_set_text(status_label, "Drag desabilitado no Botão 1");
        eg_widget_set_cursor(eg_button_as_widget(btn), "default");
    } else {
        eg_widget_set_drag_source(
            eg_button_as_widget(btn),
            EG_DRAG_ACTION_COPY,
            on_drag_prepare,
            (void *)"Texto do Botão 1"
        );
        eg_widget_on_drag_begin(eg_button_as_widget(btn), on_drag_begin, (void *)"Botão 1");
        eg_widget_on_drag_end(eg_button_as_widget(btn), on_drag_end, (void *)"Botão 1");
        eg_label_set_text(status_label, "Drag habilitado no Botão 1");
        eg_widget_set_cursor(eg_button_as_widget(btn), "grab");
    }
    
    drag_enabled = !drag_enabled;
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Janela principal */
    EgWindow *window = eg_window_new(app, "Drag and Drop Demo", 600, 400);
    
    /* Box principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Título */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Drag and Drop</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Descrição */
    EgLabel *desc = eg_label_new("Arraste os botões da esquerda para a área de drop à direita");
    eg_widget_set_halign(eg_label_as_widget(desc), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(desc), "dim-label");
    
    /* Container horizontal para drag sources e drop target */
    EgBox *content_box = eg_box_new_horizontal(20);
    eg_widget_set_vexpand(eg_box_as_widget(content_box), true);
    
    /* === Coluna esquerda: Drag Sources === */
    EgBox *sources_box = eg_box_new_vertical(10);
    
    EgLabel *sources_title = eg_label_new(NULL);
    eg_label_set_markup(sources_title, "<b>Fontes de Drag</b>");
    eg_widget_set_halign(eg_label_as_widget(sources_title), EG_ALIGN_START);
    
    /* Botões arrastáveis */
    EgButton *btn1 = create_draggable_button("Botão 1", "Texto do Botão 1");
    EgButton *btn2 = create_draggable_button("Botão 2", "Conteúdo do Botão 2");
    EgButton *btn3 = create_draggable_button("Emoji 🎉", "Parabéns! 🎉🎊");
    EgButton *btn4 = create_draggable_button("Código", "printf(\"Hello, World!\");");
    
    eg_box_append(sources_box, eg_label_as_widget(sources_title));
    eg_box_append(sources_box, eg_button_as_widget(btn1));
    eg_box_append(sources_box, eg_button_as_widget(btn2));
    eg_box_append(sources_box, eg_button_as_widget(btn3));
    eg_box_append(sources_box, eg_button_as_widget(btn4));
    
    /* === Coluna direita: Drop Target === */
    EgBox *target_box = eg_box_new_vertical(10);
    eg_widget_set_hexpand(eg_box_as_widget(target_box), true);
    
    EgLabel *target_title = eg_label_new(NULL);
    eg_label_set_markup(target_title, "<b>Área de Drop</b>");
    eg_widget_set_halign(eg_label_as_widget(target_title), EG_ALIGN_START);
    
    /* Label que recebe drops */
    drop_label = eg_label_new("Solte itens aqui!\n\nArraste os botões\nda esquerda");
    eg_widget_set_halign(eg_label_as_widget(drop_label), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_label_as_widget(drop_label), EG_ALIGN_CENTER);
    eg_widget_set_hexpand(eg_label_as_widget(drop_label), true);
    eg_widget_set_vexpand(eg_label_as_widget(drop_label), true);
    eg_widget_set_name(eg_label_as_widget(drop_label), "drop-area");
    eg_widget_add_css_class(eg_label_as_widget(drop_label), "card");
    eg_widget_set_margin(eg_label_as_widget(drop_label), 10);
    
    /* Configura como destino de drop */
    eg_widget_set_drop_target(
        eg_label_as_widget(drop_label),
        EG_DRAG_ACTION_COPY,
        on_drop,
        NULL
    );
    
    eg_box_append(target_box, eg_label_as_widget(target_title));
    eg_box_append(target_box, eg_label_as_widget(drop_label));
    
    /* Monta content box */
    eg_box_append(content_box, eg_box_as_widget(sources_box));
    eg_box_append(content_box, eg_box_as_widget(target_box));
    
    /* === Controles === */
    EgBox *controls_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(controls_box), EG_ALIGN_CENTER);
    
    EgButton *btn_clear = eg_button_new("Limpar Área");
    eg_button_on_click(btn_clear, on_clear_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_clear), "Limpa a área de drop");
    
    EgButton *btn_toggle = eg_button_new("Toggle Drag (Botão 1)");
    eg_button_on_click(btn_toggle, on_toggle_drag, btn1);
    eg_widget_set_tooltip(eg_button_as_widget(btn_toggle), 
                          "Habilita/desabilita drag no Botão 1");
    
    eg_box_append(controls_box, eg_button_as_widget(btn_clear));
    eg_box_append(controls_box, eg_button_as_widget(btn_toggle));
    
    /* === Status === */
    status_label = eg_label_new("Pronto - arraste um botão para começar");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* Monta UI principal */
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
