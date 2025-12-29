/**
 * EasyGTK - Exemplo de Eventos
 * 
 * Demonstra eventos de teclado, mouse, foco, clipboard e outras
 * funcionalidades do Core.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *status_label = NULL;
static EgLabel *mouse_label = NULL;
static EgLabel *key_label = NULL;
static EgEntry *clipboard_entry = NULL;

/* Callback de tecla pressionada */
static bool on_key_press(EgWidget *widget, EgKeyEvent *event, void *user_data) {
    (void)widget; (void)user_data;
    
    char buf[128];
    const char *mod = "";
    if (event->modifiers & EG_MODIFIER_CTRL) mod = "Ctrl+";
    else if (event->modifiers & EG_MODIFIER_ALT) mod = "Alt+";
    else if (event->modifiers & EG_MODIFIER_SHIFT) mod = "Shift+";
    
    snprintf(buf, sizeof(buf), "Tecla: %s0x%X (code: %u)", 
             mod, event->keyval, event->keycode);
    eg_label_set_text(key_label, buf);
    
    /* Escape fecha a janela */
    if (event->keyval == EG_KEY_Escape) {
        printf("Escape pressionado!\n");
    }
    
    return false; /* Não bloqueia propagação */
}

/* Callback de movimento do mouse */
static void on_mouse_motion(EgWidget *widget, EgMouseEvent *event, void *user_data) {
    (void)widget; (void)user_data;
    
    char buf[64];
    snprintf(buf, sizeof(buf), "Mouse: (%.0f, %.0f)", event->x, event->y);
    eg_label_set_text(mouse_label, buf);
}

/* Callback de mouse enter */
static void on_mouse_enter(EgWidget *widget, double x, double y, void *user_data) {
    (void)widget; (void)x; (void)y;
    EgLabel *label = (EgLabel *)user_data;
    eg_label_set_text(label, "Mouse DENTRO da área");
    eg_widget_add_css_class(eg_label_as_widget(label), "success");
}

/* Callback de mouse leave */
static void on_mouse_leave(EgWidget *widget, double x, double y, void *user_data) {
    (void)widget; (void)x; (void)y;
    EgLabel *label = (EgLabel *)user_data;
    eg_label_set_text(label, "Mouse FORA da área");
    eg_widget_remove_css_class(eg_label_as_widget(label), "success");
}

/* Callback de foco ganho */
static void on_focus_in(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_label_set_text(status_label, "Entry tem foco");
}

/* Callback de foco perdido */
static void on_focus_out(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    eg_label_set_text(status_label, "Entry perdeu foco");
}

/* Callback de scroll */
static void on_scroll(EgWidget *widget, EgScrollEvent *event, void *user_data) {
    (void)widget; (void)user_data;
    
    char buf[64];
    snprintf(buf, sizeof(buf), "Scroll: dx=%.1f, dy=%.1f", event->delta_x, event->delta_y);
    eg_label_set_text(status_label, buf);
}

/* Callback de clique */
static bool on_click(EgWidget *widget, EgButtonEvent *event, void *user_data) {
    (void)widget; (void)user_data;
    
    char buf[64];
    const char *btn_name = "?";
    if (event->button == 1) btn_name = "Esquerdo";
    else if (event->button == 2) btn_name = "Meio";
    else if (event->button == 3) btn_name = "Direito";
    
    snprintf(buf, sizeof(buf), "Clique: %s (%u clicks) em (%.0f, %.0f)", 
             btn_name, event->n_press, event->x, event->y);
    eg_label_set_text(status_label, buf);
    
    return false;
}

/* Botão Copiar */
static void on_copy_click(EgWidget *widget, void *user_data) {
    (void)user_data;
    const char *text = eg_entry_get_text(clipboard_entry);
    eg_clipboard_set_text(widget, text);
    eg_label_set_text(status_label, "Texto copiado para clipboard!");
}

/* Callback do clipboard get */
static void on_clipboard_text(const char *text, void *user_data) {
    (void)user_data;
    if (text != NULL) {
        eg_entry_set_text(clipboard_entry, text);
        eg_label_set_text(status_label, "Texto colado do clipboard!");
    } else {
        eg_label_set_text(status_label, "Clipboard vazio");
    }
}

/* Botão Colar */
static void on_paste_click(EgWidget *widget, void *user_data) {
    (void)user_data;
    eg_clipboard_get_text(widget, on_clipboard_text, NULL);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Janela principal */
    EgWindow *window = eg_window_new(app, "Eventos Demo", 500, 450);
    
    /* Box principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Título */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Sistema de Eventos</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === Seção Teclado === */
    EgLabel *key_title = eg_label_new("Eventos de Teclado:");
    eg_widget_set_halign(eg_label_as_widget(key_title), EG_ALIGN_START);
    
    key_label = eg_label_new("Pressione uma tecla...");
    eg_widget_set_halign(eg_label_as_widget(key_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(key_label), "monospace");
    
    /* === Seção Mouse === */
    EgLabel *mouse_title = eg_label_new("Eventos de Mouse:");
    eg_widget_set_halign(eg_label_as_widget(mouse_title), EG_ALIGN_START);
    
    mouse_label = eg_label_new("Mova o mouse...");
    eg_widget_set_halign(eg_label_as_widget(mouse_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(mouse_label), "monospace");
    
    /* Área de detecção de mouse enter/leave */
    EgLabel *hover_label = eg_label_new("Mouse FORA da área");
    eg_widget_set_halign(eg_label_as_widget(hover_label), EG_ALIGN_CENTER);
    eg_widget_set_margin(eg_label_as_widget(hover_label), 10);
    eg_widget_set_name(eg_label_as_widget(hover_label), "hover-area");
    
    /* Conectar eventos de enter/leave */
    eg_widget_on_enter(eg_label_as_widget(hover_label), on_mouse_enter, hover_label);
    eg_widget_on_leave(eg_label_as_widget(hover_label), on_mouse_leave, hover_label);
    
    /* === Seção Foco === */
    EgLabel *focus_title = eg_label_new("Eventos de Foco:");
    eg_widget_set_halign(eg_label_as_widget(focus_title), EG_ALIGN_START);
    
    EgEntry *focus_entry = eg_entry_new_with_placeholder("Clique aqui para focar");
    eg_widget_set_focusable(eg_entry_as_widget(focus_entry), true);
    eg_widget_on_focus_in(eg_entry_as_widget(focus_entry), on_focus_in, NULL);
    eg_widget_on_focus_out(eg_entry_as_widget(focus_entry), on_focus_out, NULL);
    
    /* === Seção Clipboard === */
    EgLabel *clip_title = eg_label_new("Clipboard:");
    eg_widget_set_halign(eg_label_as_widget(clip_title), EG_ALIGN_START);
    
    EgBox *clip_box = eg_box_new_horizontal(10);
    
    clipboard_entry = eg_entry_new_with_placeholder("Texto para copiar/colar");
    eg_widget_set_hexpand(eg_entry_as_widget(clipboard_entry), true);
    
    EgButton *btn_copy = eg_button_new("Copiar");
    eg_button_on_click(btn_copy, on_copy_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_copy), "Copiar texto para clipboard");
    
    EgButton *btn_paste = eg_button_new("Colar");
    eg_button_on_click(btn_paste, on_paste_click, NULL);
    eg_widget_set_tooltip(eg_button_as_widget(btn_paste), "Colar texto do clipboard");
    
    eg_box_append(clip_box, eg_entry_as_widget(clipboard_entry));
    eg_box_append(clip_box, eg_button_as_widget(btn_copy));
    eg_box_append(clip_box, eg_button_as_widget(btn_paste));
    
    /* === Seção Cursor e Opacity === */
    EgLabel *misc_title = eg_label_new("Cursor e Opacity:");
    eg_widget_set_halign(eg_label_as_widget(misc_title), EG_ALIGN_START);
    
    EgBox *cursor_box = eg_box_new_horizontal(10);
    
    EgButton *btn_pointer = eg_button_new("Pointer");
    eg_widget_set_cursor(eg_button_as_widget(btn_pointer), "pointer");
    
    EgButton *btn_crosshair = eg_button_new("Crosshair");
    eg_widget_set_cursor(eg_button_as_widget(btn_crosshair), "crosshair");
    
    EgButton *btn_wait = eg_button_new("Wait");
    eg_widget_set_cursor(eg_button_as_widget(btn_wait), "wait");
    
    EgButton *btn_opacity = eg_button_new("50% Opacity");
    eg_widget_set_opacity(eg_button_as_widget(btn_opacity), 0.5);
    
    eg_box_append(cursor_box, eg_button_as_widget(btn_pointer));
    eg_box_append(cursor_box, eg_button_as_widget(btn_crosshair));
    eg_box_append(cursor_box, eg_button_as_widget(btn_wait));
    eg_box_append(cursor_box, eg_button_as_widget(btn_opacity));
    
    /* === Status === */
    status_label = eg_label_new("Status: Pronto");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* Monta UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(key_title));
    eg_box_append(main_box, eg_label_as_widget(key_label));
    eg_box_append(main_box, eg_label_as_widget(mouse_title));
    eg_box_append(main_box, eg_label_as_widget(mouse_label));
    eg_box_append(main_box, eg_label_as_widget(hover_label));
    eg_box_append(main_box, eg_label_as_widget(focus_title));
    eg_box_append(main_box, eg_entry_as_widget(focus_entry));
    eg_box_append(main_box, eg_label_as_widget(clip_title));
    eg_box_append(main_box, eg_box_as_widget(clip_box));
    eg_box_append(main_box, eg_label_as_widget(misc_title));
    eg_box_append(main_box, eg_box_as_widget(cursor_box));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    
    /* Conectar eventos globais da janela */
    eg_widget_on_key_press(eg_window_as_widget(window), on_key_press, NULL);
    eg_widget_on_motion(eg_window_as_widget(window), on_mouse_motion, NULL);
    eg_widget_on_scroll(eg_window_as_widget(window), on_scroll, NULL);
    eg_widget_on_click(eg_window_as_widget(window), on_click, NULL);
    
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.events");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
