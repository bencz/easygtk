/**
 * EasyGTK - Exemplo de Formulário
 * 
 * Demonstra o uso de grid, entry e labels para criar um formulário.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgEntry *entry_nome = NULL;
static EgEntry *entry_email = NULL;
static EgEntry *entry_senha = NULL;
static EgLabel *label_status = NULL;

static void on_submit(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    const char *nome = eg_entry_get_text(entry_nome);
    const char *email = eg_entry_get_text(entry_email);
    
    printf("Nome: %s\n", nome);
    printf("Email: %s\n", email);
    
    eg_label_set_markup(label_status, 
        "<span color='green'>Formulário enviado com sucesso!</span>");
}

static void on_clear(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    
    eg_entry_set_text(entry_nome, "");
    eg_entry_set_text(entry_email, "");
    eg_entry_set_text(entry_senha, "");
    eg_label_set_text(label_status, "");
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Cria a janela */
    EgWindow *window = eg_window_new(app, "Formulário de Cadastro", 450, 350);
    eg_window_set_resizable(window, false);
    
    /* Box principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 25);
    
    /* Título */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='xx-large' weight='bold'>Cadastro</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Grid para o formulário */
    EgGrid *grid = eg_grid_new();
    eg_grid_set_row_spacing(grid, 12);
    eg_grid_set_column_spacing(grid, 15);
    
    /* Campo Nome */
    EgLabel *lbl_nome = eg_label_new("Nome:");
    eg_widget_set_halign(eg_label_as_widget(lbl_nome), EG_ALIGN_END);
    entry_nome = eg_entry_new_with_placeholder("Digite seu nome");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_nome), true);
    
    /* Campo Email */
    EgLabel *lbl_email = eg_label_new("Email:");
    eg_widget_set_halign(eg_label_as_widget(lbl_email), EG_ALIGN_END);
    entry_email = eg_entry_new_with_placeholder("exemplo@email.com");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_email), true);
    
    /* Campo Senha */
    EgLabel *lbl_senha = eg_label_new("Senha:");
    eg_widget_set_halign(eg_label_as_widget(lbl_senha), EG_ALIGN_END);
    entry_senha = eg_entry_new_with_placeholder("Digite sua senha");
    eg_entry_set_visibility(entry_senha, false);
    eg_widget_set_hexpand(eg_entry_as_widget(entry_senha), true);
    
    /* Adiciona campos ao grid */
    eg_grid_attach(grid, eg_label_as_widget(lbl_nome), 0, 0, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_nome), 1, 0, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_email), 0, 1, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_email), 1, 1, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_senha), 0, 2, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_senha), 1, 2, 1, 1);
    
    /* Box para botões */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);
    
    EgButton *btn_submit = eg_button_new("Enviar");
    eg_button_on_click(btn_submit, on_submit, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_submit), "suggested-action");
    
    EgButton *btn_clear = eg_button_new("Limpar");
    eg_button_on_click(btn_clear, on_clear, NULL);
    
    eg_box_append(button_box, eg_button_as_widget(btn_submit));
    eg_box_append(button_box, eg_button_as_widget(btn_clear));
    
    /* Label de status */
    label_status = eg_label_new("");
    eg_widget_set_halign(eg_label_as_widget(label_status), EG_ALIGN_CENTER);
    
    /* Monta a interface */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_grid_as_widget(grid));
    eg_box_append(main_box, eg_box_as_widget(button_box));
    eg_box_append(main_box, eg_label_as_widget(label_status));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    EgApp *app = eg_app_new("com.example.form");
    if (app == NULL) {
        return 1;
    }
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    eg_app_free(app);
    
    return status;
}
