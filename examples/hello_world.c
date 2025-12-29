/**
 * EasyGTK - Exemplo Hello World
 * 
 * Demonstra o uso básico da biblioteca.
 */

#include <easygtk/easygtk.h>

static EgWindow *window = NULL;
static EgLabel *label = NULL;

static void on_button_click(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    eg_label_set_text(label, "Botão foi clicado!");
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Cria a janela */
    window = eg_window_new(app, "Hello EasyGTK", 400, 300);
    
    /* Cria um box vertical */
    EgBox *box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(box), 20);
    eg_widget_set_halign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    
    /* Cria a label */
    label = eg_label_new("Olá, Mundo!");
    eg_label_set_markup(label, "<span size='x-large' weight='bold'>Olá, Mundo!</span>");
    
    /* Cria o botão */
    EgButton *button = eg_button_new("Clique aqui");
    eg_button_on_click(button, on_button_click, NULL);
    
    /* Adiciona widgets ao box */
    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_button_as_widget(button));
    
    /* Define o box como filho da janela */
    eg_window_set_child(window, eg_box_as_widget(box));
    
    /* Exibe a janela */
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    /* Cria a aplicação */
    EgApp *app = eg_app_new("com.example.helloworld");
    if (app == NULL) {
        return 1;
    }
    
    /* Define o callback de ativação */
    eg_app_on_activate(app, on_activate, app);
    
    /* Executa a aplicação */
    int status = eg_app_run(app);
    
    /* Limpa recursos */
    eg_app_free(app);
    
    return status;
}
