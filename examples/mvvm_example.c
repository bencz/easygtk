/**
 * EasyGTK - Exemplo MVVM
 * 
 * Demonstra o uso de ViewModel, Properties observáveis,
 * Data Binding e Commands.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

/* ============================================
 * ViewModel do Contador
 * ============================================ */

static EgViewModel *vm = NULL;

/* Callback quando o contador muda - atualiza o label */
static void on_counter_changed(EgProperty *property, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    int value = eg_property_get_int(property);
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), 
        "<span size='xx-large' weight='bold'>%d</span>", value);
    eg_label_set_markup(label, buffer);
}

/* Command: Incrementar */
static void cmd_increment_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    int current = eg_view_model_get_int(vm, "counter");
    eg_view_model_set_int(vm, "counter", current + 1);
}

/* Command: Decrementar */
static void cmd_decrement_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    int current = eg_view_model_get_int(vm, "counter");
    eg_view_model_set_int(vm, "counter", current - 1);
}

/* Command: Reset */
static void cmd_reset_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    eg_view_model_set_int(vm, "counter", 0);
}

/* Wrappers para callbacks de botão (assinatura EgCallback) */
static void on_increment_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    cmd_increment_execute(NULL, NULL, NULL);
}

static void on_decrement_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    cmd_decrement_execute(NULL, NULL, NULL);
}

static void on_reset_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    cmd_reset_execute(NULL, NULL, NULL);
}

/* Verifica se pode decrementar (counter > 0) */
static bool cmd_decrement_can_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    return eg_view_model_get_int(vm, "counter") > 0;
}

/* Atualiza estado dos botões quando counter muda */
static void on_counter_changed_update_commands(EgProperty *property, void *user_data) {
    (void)property;
    EgButton *btn_dec = (EgButton *)user_data;
    
    /* Atualiza sensibilidade do botão decrementar */
    EgCommand *cmd = eg_view_model_get_command(vm, "decrement");
    bool can_dec = eg_command_can_execute(cmd, NULL);
    eg_button_set_sensitive(btn_dec, can_dec);
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* ========================================
     * Cria o ViewModel
     * ======================================== */
    vm = eg_view_model_new();
    
    /* Adiciona property "counter" */
    EgProperty *counter_prop = eg_property_new_int("counter", 0);
    eg_view_model_add_property(vm, counter_prop);
    
    /* Adiciona commands */
    EgCommand *cmd_inc = eg_command_new("increment", cmd_increment_execute, NULL, NULL);
    EgCommand *cmd_dec = eg_command_new("decrement", cmd_decrement_execute, cmd_decrement_can_execute, NULL);
    EgCommand *cmd_reset = eg_command_new("reset", cmd_reset_execute, NULL, NULL);
    
    eg_view_model_add_command(vm, cmd_inc);
    eg_view_model_add_command(vm, cmd_dec);
    eg_view_model_add_command(vm, cmd_reset);
    
    /* ========================================
     * Cria a UI
     * ======================================== */
    EgWindow *window = eg_window_new(app, "MVVM Counter", 350, 200);
    eg_window_set_resizable(window, false);
    
    EgBox *main_box = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(main_box), 30);
    eg_widget_set_halign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_box_as_widget(main_box), EG_ALIGN_CENTER);
    
    /* Título */
    EgLabel *title = eg_label_new("MVVM Pattern Demo");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* Label do contador (será atualizado via binding) */
    EgLabel *label_counter = eg_label_new("0");
    eg_widget_set_halign(eg_label_as_widget(label_counter), EG_ALIGN_CENTER);
    
    /* Conecta property ao label */
    eg_property_on_changed(counter_prop, on_counter_changed, label_counter);
    
    /* Atualiza label inicial */
    on_counter_changed(counter_prop, label_counter);
    
    /* Box de botões */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);
    
    /* Botões ligados aos commands */
    EgButton *btn_dec = eg_button_new("-");
    eg_widget_set_size_request(eg_button_as_widget(btn_dec), 50, -1);
    eg_button_on_click(btn_dec, on_decrement_click, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_dec), "destructive-action");
    eg_button_set_sensitive(btn_dec, false); /* Começa desabilitado (counter = 0) */
    
    EgButton *btn_reset = eg_button_new("Reset");
    eg_button_on_click(btn_reset, on_reset_click, NULL);
    
    EgButton *btn_inc = eg_button_new("+");
    eg_widget_set_size_request(eg_button_as_widget(btn_inc), 50, -1);
    eg_button_on_click(btn_inc, on_increment_click, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_inc), "suggested-action");
    
    /* Conecta property para atualizar estado do botão decrementar */
    eg_property_on_changed(counter_prop, on_counter_changed_update_commands, btn_dec);
    
    eg_box_append(button_box, eg_button_as_widget(btn_dec));
    eg_box_append(button_box, eg_button_as_widget(btn_reset));
    eg_box_append(button_box, eg_button_as_widget(btn_inc));
    
    /* Monta UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(label_counter));
    eg_box_append(main_box, eg_box_as_widget(button_box));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.mvvm");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    
    /* Cleanup */
    eg_view_model_free(vm);
    eg_app_free(app);
    
    return status;
}
