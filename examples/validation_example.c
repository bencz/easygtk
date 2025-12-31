/**
 * EasyGTK - Exemplo de Validacao
 *
 * Demonstra o sistema de validacao declarativo:
 * - Validadores built-in (required, min/max length, email, pattern, range)
 * - Validador customizado
 * - Validacao automatica ao perder foco
 * - Validacao em lote
 * - Feedback visual de erros
 */

#include <easygtk/easygtk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Widgets globais para validacao */
static EgEntry *entry_name = NULL;
static EgEntry *entry_email = NULL;
static EgEntry *entry_password = NULL;
static EgEntry *entry_confirm_password = NULL;
static EgSpinButton *spin_age = NULL;
static EgEntry *entry_phone = NULL;
static EgLabel *label_status = NULL;

/* ============================================
 * Validador Customizado: Confirmar Senha
 * ============================================ */

static EgValidationResult validate_password_match(const EgValidationValue *value, void *user_data) {
    (void)user_data;
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    const char *confirm = value->string_val;
    const char *password = eg_entry_get_text(entry_password);

    if (confirm && password && strcmp(confirm, password) != 0) {
        result.is_valid = false;
        result.error_message = "Senhas nao conferem";
    }

    return result;
}

/* ============================================
 * Validador Customizado: Senha Forte
 * ============================================ */

static EgValidationResult validate_strong_password(const EgValidationValue *value, void *user_data) {
    (void)user_data;
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    const char *pwd = value->string_val;
    if (pwd == NULL) {
        result.is_valid = false;
        result.error_message = "Senha obrigatoria";
        return result;
    }

    bool has_upper = false, has_lower = false, has_digit = false;
    for (const char *p = pwd; *p; p++) {
        if (isupper(*p)) has_upper = true;
        if (islower(*p)) has_lower = true;
        if (isdigit(*p)) has_digit = true;
    }

    if (!has_upper || !has_lower || !has_digit) {
        result.is_valid = false;
        result.error_message = "Senha deve ter maiuscula, minuscula e numero";
    }

    return result;
}

/* ============================================
 * Callbacks
 * ============================================ */

static void on_validate_all(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;

    /* Array de widgets para validar */
    EgWidget *widgets[] = {
        eg_entry_as_widget(entry_name),
        eg_entry_as_widget(entry_email),
        eg_entry_as_widget(entry_password),
        eg_entry_as_widget(entry_confirm_password),
        eg_spin_button_as_widget(spin_age),
        eg_entry_as_widget(entry_phone)
    };

    /* Valida todos */
    EgValidationResult results[6];
    bool all_valid = eg_validate_all(widgets, 6, results);

    if (all_valid) {
        eg_label_set_markup(label_status,
            "<span color='#a6e3a1' weight='bold'>Todos os campos estao validos!</span>");

        printf("=== Dados do Formulario ===\n");
        printf("Nome: %s\n", eg_entry_get_text(entry_name));
        printf("Email: %s\n", eg_entry_get_text(entry_email));
        printf("Idade: %d\n", eg_spin_button_get_value_as_int(spin_age));
        printf("Telefone: %s\n", eg_entry_get_text(entry_phone));
    } else {
        /* Encontra primeiro invalido e foca */
        int first_invalid = eg_validate_first_invalid(widgets, 6);
        if (first_invalid >= 0) {
            eg_widget_grab_focus(widgets[first_invalid]);

            char msg[256];
            snprintf(msg, sizeof(msg),
                "<span color='#f38ba8'>Campo %d invalido: %s</span>",
                first_invalid + 1,
                results[first_invalid].error_message ? results[first_invalid].error_message : "erro");
            eg_label_set_markup(label_status, msg);
        }
    }
}

static void on_clear(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;

    eg_entry_set_text(entry_name, "");
    eg_entry_set_text(entry_email, "");
    eg_entry_set_text(entry_password, "");
    eg_entry_set_text(entry_confirm_password, "");
    eg_spin_button_set_value(spin_age, 18);
    eg_entry_set_text(entry_phone, "");

    /* Limpa erros visuais */
    eg_widget_clear_validation_error(eg_entry_as_widget(entry_name));
    eg_widget_clear_validation_error(eg_entry_as_widget(entry_email));
    eg_widget_clear_validation_error(eg_entry_as_widget(entry_password));
    eg_widget_clear_validation_error(eg_entry_as_widget(entry_confirm_password));
    eg_widget_clear_validation_error(eg_spin_button_as_widget(spin_age));
    eg_widget_clear_validation_error(eg_entry_as_widget(entry_phone));

    eg_label_set_text(label_status, "");
}

/* ============================================
 * Setup de Validadores
 * ============================================ */

static void setup_validators(void) {
    /* Nome: obrigatorio, 3-50 caracteres */
    EgValidatorChain *name_chain = eg_validator_chain_new();
    eg_validator_add_required(name_chain, "Nome e obrigatorio");
    eg_validator_add_min_length(name_chain, 3, "Nome deve ter pelo menos 3 caracteres");
    eg_validator_add_max_length(name_chain, 50, "Nome deve ter no maximo 50 caracteres");
    eg_widget_set_validators(eg_entry_as_widget(entry_name), name_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_name), true);

    /* Email: obrigatorio, formato valido */
    EgValidatorChain *email_chain = eg_validator_chain_new();
    eg_validator_add_required(email_chain, "Email e obrigatorio");
    eg_validator_add_email(email_chain, "Email invalido");
    eg_widget_set_validators(eg_entry_as_widget(entry_email), email_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_email), true);

    /* Senha: obrigatorio, minimo 8 chars, deve ser forte */
    EgValidatorChain *password_chain = eg_validator_chain_new();
    eg_validator_add_required(password_chain, "Senha e obrigatoria");
    eg_validator_add_min_length(password_chain, 8, "Senha deve ter pelo menos 8 caracteres");
    eg_validator_add_custom(password_chain, validate_strong_password, NULL, NULL);
    eg_widget_set_validators(eg_entry_as_widget(entry_password), password_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_password), true);

    /* Confirmar Senha: deve corresponder */
    EgValidatorChain *confirm_chain = eg_validator_chain_new();
    eg_validator_add_required(confirm_chain, "Confirmacao e obrigatoria");
    eg_validator_add_custom(confirm_chain, validate_password_match, NULL, NULL);
    eg_widget_set_validators(eg_entry_as_widget(entry_confirm_password), confirm_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_confirm_password), true);

    /* Idade: range 18-120 */
    EgValidatorChain *age_chain = eg_validator_chain_new();
    eg_validator_add_range_double(age_chain, 18.0, 120.0, "Idade deve ser entre 18 e 120 anos");
    eg_widget_set_validators(eg_spin_button_as_widget(spin_age), age_chain);
    eg_widget_validate_on_focus_out(eg_spin_button_as_widget(spin_age), true);

    /* Telefone: pattern (opcional - aceita vazio) */
    EgValidatorChain *phone_chain = eg_validator_chain_new();
    /* Pattern: formato (XX) XXXXX-XXXX ou vazio */
    eg_validator_add_pattern(phone_chain, "^(\\([0-9]{2}\\) [0-9]{5}-[0-9]{4})?$",
                              "Telefone deve estar no formato (XX) XXXXX-XXXX");
    eg_widget_set_validators(eg_entry_as_widget(entry_phone), phone_chain);
    eg_widget_validate_on_focus_out(eg_entry_as_widget(entry_phone), true);
}

/* ============================================
 * Criacao da Interface
 * ============================================ */

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Janela principal */
    EgWindow *window = eg_window_new(app, "Validacao de Formulario", 500, 550);
    eg_window_set_resizable(window, false);

    /* Box principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 25);

    /* Titulo */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='xx-large' weight='bold'>Cadastro de Usuario</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);

    EgLabel *subtitle = eg_label_new("Os campos sao validados ao perder foco");
    eg_widget_set_halign(eg_label_as_widget(subtitle), EG_ALIGN_CENTER);
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "dim-label");

    /* Grid para campos */
    EgGrid *grid = eg_grid_new();
    eg_grid_set_row_spacing(grid, 12);
    eg_grid_set_column_spacing(grid, 15);

    /* Nome */
    EgLabel *lbl_name = eg_label_new("Nome *");
    eg_widget_set_halign(eg_label_as_widget(lbl_name), EG_ALIGN_END);
    entry_name = eg_entry_new_with_placeholder("Seu nome completo");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_name), true);

    /* Email */
    EgLabel *lbl_email = eg_label_new("Email *");
    eg_widget_set_halign(eg_label_as_widget(lbl_email), EG_ALIGN_END);
    entry_email = eg_entry_new_with_placeholder("usuario@exemplo.com");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_email), true);

    /* Senha */
    EgLabel *lbl_password = eg_label_new("Senha *");
    eg_widget_set_halign(eg_label_as_widget(lbl_password), EG_ALIGN_END);
    entry_password = eg_entry_new_with_placeholder("Minimo 8 caracteres");
    eg_entry_set_visibility(entry_password, false);
    eg_widget_set_hexpand(eg_entry_as_widget(entry_password), true);

    /* Confirmar Senha */
    EgLabel *lbl_confirm = eg_label_new("Confirmar *");
    eg_widget_set_halign(eg_label_as_widget(lbl_confirm), EG_ALIGN_END);
    entry_confirm_password = eg_entry_new_with_placeholder("Repita a senha");
    eg_entry_set_visibility(entry_confirm_password, false);
    eg_widget_set_hexpand(eg_entry_as_widget(entry_confirm_password), true);

    /* Idade */
    EgLabel *lbl_age = eg_label_new("Idade *");
    eg_widget_set_halign(eg_label_as_widget(lbl_age), EG_ALIGN_END);
    spin_age = eg_spin_button_new_int(0, 150, 1);
    eg_spin_button_set_value(spin_age, 18);
    eg_widget_set_hexpand(eg_spin_button_as_widget(spin_age), true);

    /* Telefone */
    EgLabel *lbl_phone = eg_label_new("Telefone");
    eg_widget_set_halign(eg_label_as_widget(lbl_phone), EG_ALIGN_END);
    entry_phone = eg_entry_new_with_placeholder("(XX) XXXXX-XXXX");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_phone), true);

    /* Monta grid */
    eg_grid_attach(grid, eg_label_as_widget(lbl_name), 0, 0, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_name), 1, 0, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_email), 0, 1, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_email), 1, 1, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_password), 0, 2, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_password), 1, 2, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_confirm), 0, 3, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_confirm_password), 1, 3, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_age), 0, 4, 1, 1);
    eg_grid_attach(grid, eg_spin_button_as_widget(spin_age), 1, 4, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_phone), 0, 5, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_phone), 1, 5, 1, 1);

    /* Legenda */
    EgLabel *legend = eg_label_new("* Campos obrigatorios");
    eg_widget_set_halign(eg_label_as_widget(legend), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(legend), "dim-label");

    /* Botoes */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);

    EgButton *btn_validate = eg_button_new("Validar e Enviar");
    eg_button_on_click(btn_validate, on_validate_all, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_validate), "suggested-action");

    EgButton *btn_clear = eg_button_new("Limpar");
    eg_button_on_click(btn_clear, on_clear, NULL);

    eg_box_append(button_box, eg_button_as_widget(btn_validate));
    eg_box_append(button_box, eg_button_as_widget(btn_clear));

    /* Status */
    label_status = eg_label_new("");
    eg_widget_set_halign(eg_label_as_widget(label_status), EG_ALIGN_CENTER);
    eg_label_set_wrap(label_status, true);

    /* Monta interface */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(subtitle));
    eg_box_append(main_box, eg_grid_as_widget(grid));
    eg_box_append(main_box, eg_label_as_widget(legend));
    eg_box_append(main_box, eg_box_as_widget(button_box));
    eg_box_append(main_box, eg_label_as_widget(label_status));

    /* Configura validadores APOS criar widgets */
    setup_validators();

    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

/* ============================================
 * Main
 * ============================================ */

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    /* CSS para estilo de erro */
    const char *css =
        ".error {\n"
        "    border-color: #f38ba8;\n"
        "    background-color: rgba(243, 139, 168, 0.1);\n"
        "}\n"
        ".dim-label {\n"
        "    opacity: 0.7;\n"
        "    font-size: 0.9em;\n"
        "}\n";

    EgApp *app = eg_app_new("com.example.validation");
    if (app == NULL) return 1;

    /* Carrega CSS */
    eg_css_load_from_string(css);

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);
    eg_app_free(app);

    return status;
}
