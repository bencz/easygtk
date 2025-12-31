/**
 * EasyGTK - Sistema de Validacao
 *
 * Implementacao do sistema de validacao declarativo.
 */

#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "internal/internal.h"
#include <easygtk/validation.h>

/* ============================================
 * Estruturas Internas
 * ============================================ */

/**
 * Validador individual na chain.
 */
typedef struct EgValidator {
    EgValidatorType type;
    union {
        struct { size_t min; size_t max; } length;
        struct { int min; int max; } range_int;
        struct { double min; double max; } range_double;
        char *pattern;
        struct { EgValidateFunc func; void *user_data; } custom;
    } params;
    char *error_message;
    struct EgValidator *next;
} EgValidator;

/**
 * Chain de validadores.
 */
struct EgValidatorChain {
    EgValidator *head;
    EgValidator *tail;
};

/**
 * Dados de validacao associados a um widget.
 */
typedef struct EgWidgetValidationData {
    EgValidatorChain *chain;
    bool validate_on_focus_out;
    bool validate_on_change;
    gulong focus_out_handler;
    gulong change_handler;
} EgWidgetValidationData;

/* ============================================
 * Mensagens de Erro Default
 * ============================================ */

static const char *MSG_REQUIRED = "Campo obrigatorio";
static const char *MSG_MIN_LENGTH = "Minimo %zu caracteres";
static const char *MSG_MAX_LENGTH = "Maximo %zu caracteres";
static const char *MSG_PATTERN = "Formato invalido";
static const char *MSG_RANGE_INT = "Valor deve estar entre %d e %d";
static const char *MSG_RANGE_DOUBLE = "Valor deve estar entre %.2f e %.2f";
static const char *MSG_EMAIL = "Email invalido";
static const char *MSG_CUSTOM = "Validacao falhou";

/* ============================================
 * Helpers Internos
 * ============================================ */

static EgValidator *validator_new(EgValidatorType type, const char *error_msg) {
    EgValidator *v = EG_ALLOC(EgValidator);
    if (v == NULL) return NULL;

    v->type = type;
    v->error_message = error_msg ? eg_strdup(error_msg) : NULL;
    v->next = NULL;
    return v;
}

static void validator_free(EgValidator *v) {
    if (v == NULL) return;
    if (v->error_message != NULL) eg_free(v->error_message);
    if (v->type == EG_VALIDATOR_PATTERN && v->params.pattern != NULL) {
        eg_free(v->params.pattern);
    }
    eg_free(v);
}

static void chain_append(EgValidatorChain *chain, EgValidator *v) {
    if (chain == NULL || v == NULL) return;

    if (chain->head == NULL) {
        chain->head = v;
        chain->tail = v;
    } else {
        chain->tail->next = v;
        chain->tail = v;
    }
}

static const EgValidationVTable *get_validation_vtable(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return NULL;
    return widget->vtable->validation;
}

/**
 * Validacao simples de email (verifica @ e .).
 */
static bool is_valid_email(const char *str) {
    if (str == NULL || *str == '\0') return false;

    const char *at = strchr(str, '@');
    if (at == NULL || at == str) return false;

    const char *dot = strrchr(at, '.');
    if (dot == NULL || dot == at + 1 || *(dot + 1) == '\0') return false;

    return true;
}

/**
 * Validacao de pattern usando GRegex.
 */
static bool matches_pattern(const char *str, const char *pattern) {
    if (str == NULL || pattern == NULL) return false;

    GError *error = NULL;
    GRegex *regex = g_regex_new(pattern, 0, 0, &error);
    if (regex == NULL) {
        if (error != NULL) g_error_free(error);
        return false;
    }

    gboolean result = g_regex_match(regex, str, 0, NULL);
    g_regex_unref(regex);
    return result;
}

/**
 * Formata mensagem de erro com parametros.
 */
static char *format_error_message(const EgValidator *v) {
    char buffer[256];

    switch (v->type) {
        case EG_VALIDATOR_REQUIRED:
            return eg_strdup(v->error_message ? v->error_message : MSG_REQUIRED);

        case EG_VALIDATOR_MIN_LENGTH:
            if (v->error_message) return eg_strdup(v->error_message);
            snprintf(buffer, sizeof(buffer), MSG_MIN_LENGTH, v->params.length.min);
            return eg_strdup(buffer);

        case EG_VALIDATOR_MAX_LENGTH:
            if (v->error_message) return eg_strdup(v->error_message);
            snprintf(buffer, sizeof(buffer), MSG_MAX_LENGTH, v->params.length.max);
            return eg_strdup(buffer);

        case EG_VALIDATOR_PATTERN:
            return eg_strdup(v->error_message ? v->error_message : MSG_PATTERN);

        case EG_VALIDATOR_RANGE_INT:
            if (v->error_message) return eg_strdup(v->error_message);
            snprintf(buffer, sizeof(buffer), MSG_RANGE_INT,
                     v->params.range_int.min, v->params.range_int.max);
            return eg_strdup(buffer);

        case EG_VALIDATOR_RANGE_DOUBLE:
            if (v->error_message) return eg_strdup(v->error_message);
            snprintf(buffer, sizeof(buffer), MSG_RANGE_DOUBLE,
                     v->params.range_double.min, v->params.range_double.max);
            return eg_strdup(buffer);

        case EG_VALIDATOR_EMAIL:
            return eg_strdup(v->error_message ? v->error_message : MSG_EMAIL);

        case EG_VALIDATOR_CUSTOM:
            return eg_strdup(v->error_message ? v->error_message : MSG_CUSTOM);

        default:
            return eg_strdup("Erro de validacao");
    }
}

/**
 * Converte EgValue interno para EgValidationValue publico.
 */
static EgValidationValue value_to_validation_value(const EgValue *value) {
    EgValidationValue vv;
    vv.type = value->type;
    switch (value->type) {
        case EG_PROPERTY_TYPE_STRING:
            vv.string_val = value->string_val;
            break;
        case EG_PROPERTY_TYPE_INT:
            vv.int_val = value->int_val;
            break;
        case EG_PROPERTY_TYPE_DOUBLE:
            vv.double_val = value->double_val;
            break;
        case EG_PROPERTY_TYPE_BOOL:
            vv.bool_val = value->bool_val;
            break;
        default:
            vv.string_val = NULL;
            break;
    }
    return vv;
}

/**
 * Executa um validador individual.
 */
static EgValidationResult validate_single(const EgValidator *v, const EgValue *value) {
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    switch (v->type) {
        case EG_VALIDATOR_REQUIRED:
            if (value->type == EG_PROPERTY_TYPE_STRING) {
                if (value->string_val == NULL || value->string_val[0] == '\0') {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_MIN_LENGTH:
            if (value->type == EG_PROPERTY_TYPE_STRING && value->string_val != NULL) {
                if (strlen(value->string_val) < v->params.length.min) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_MAX_LENGTH:
            if (value->type == EG_PROPERTY_TYPE_STRING && value->string_val != NULL) {
                if (strlen(value->string_val) > v->params.length.max) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_PATTERN:
            if (value->type == EG_PROPERTY_TYPE_STRING) {
                if (!matches_pattern(value->string_val, v->params.pattern)) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_RANGE_INT:
            if (value->type == EG_PROPERTY_TYPE_INT) {
                if (value->int_val < v->params.range_int.min ||
                    value->int_val > v->params.range_int.max) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_RANGE_DOUBLE:
            if (value->type == EG_PROPERTY_TYPE_DOUBLE) {
                if (value->double_val < v->params.range_double.min ||
                    value->double_val > v->params.range_double.max) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_EMAIL:
            if (value->type == EG_PROPERTY_TYPE_STRING) {
                if (!is_valid_email(value->string_val)) {
                    result.is_valid = false;
                }
            }
            break;

        case EG_VALIDATOR_CUSTOM:
            if (v->params.custom.func != NULL) {
                /* Converte para tipo publico antes de chamar callback */
                EgValidationValue vv = value_to_validation_value(value);
                result = v->params.custom.func(&vv, v->params.custom.user_data);
            }
            break;
    }

    return result;
}

/* ============================================
 * API Publica - Chain
 * ============================================ */

EgValidatorChain *eg_validator_chain_new(void) {
    EgValidatorChain *chain = EG_ALLOC(EgValidatorChain);
    if (chain == NULL) return NULL;

    chain->head = NULL;
    chain->tail = NULL;
    return chain;
}

void eg_validator_chain_free(EgValidatorChain *chain) {
    if (chain == NULL) return;

    EgValidator *v = chain->head;
    while (v != NULL) {
        EgValidator *next = v->next;
        validator_free(v);
        v = next;
    }

    eg_free(chain);
}

void eg_validator_add_required(EgValidatorChain *chain, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_REQUIRED, error_msg);
    chain_append(chain, v);
}

void eg_validator_add_min_length(EgValidatorChain *chain, size_t min_length, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_MIN_LENGTH, error_msg);
    if (v != NULL) {
        v->params.length.min = min_length;
        chain_append(chain, v);
    }
}

void eg_validator_add_max_length(EgValidatorChain *chain, size_t max_length, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_MAX_LENGTH, error_msg);
    if (v != NULL) {
        v->params.length.max = max_length;
        chain_append(chain, v);
    }
}

void eg_validator_add_pattern(EgValidatorChain *chain, const char *pattern, const char *error_msg) {
    if (chain == NULL || pattern == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_PATTERN, error_msg);
    if (v != NULL) {
        v->params.pattern = eg_strdup(pattern);
        chain_append(chain, v);
    }
}

void eg_validator_add_range_int(EgValidatorChain *chain, int min, int max, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_RANGE_INT, error_msg);
    if (v != NULL) {
        v->params.range_int.min = min;
        v->params.range_int.max = max;
        chain_append(chain, v);
    }
}

void eg_validator_add_range_double(EgValidatorChain *chain, double min, double max, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_RANGE_DOUBLE, error_msg);
    if (v != NULL) {
        v->params.range_double.min = min;
        v->params.range_double.max = max;
        chain_append(chain, v);
    }
}

void eg_validator_add_email(EgValidatorChain *chain, const char *error_msg) {
    if (chain == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_EMAIL, error_msg);
    chain_append(chain, v);
}

void eg_validator_add_custom(EgValidatorChain *chain, EgValidateFunc func,
                              void *user_data, const char *error_msg) {
    if (chain == NULL || func == NULL) return;

    EgValidator *v = validator_new(EG_VALIDATOR_CUSTOM, error_msg);
    if (v != NULL) {
        v->params.custom.func = func;
        v->params.custom.user_data = user_data;
        chain_append(chain, v);
    }
}

/**
 * Valida internamente usando EgValue.
 */
static EgValidationResult validate_chain_internal(EgValidatorChain *chain, const EgValue *value) {
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    if (chain == NULL || value == NULL) return result;

    EgValidator *v = chain->head;
    while (v != NULL) {
        EgValidationResult single = validate_single(v, value);
        if (!single.is_valid) {
            result.is_valid = false;
            result.error_message = format_error_message(v);
            break;
        }
        v = v->next;
    }

    return result;
}

EgValidationResult eg_validator_chain_validate(EgValidatorChain *chain, const EgValidationValue *value) {
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    if (chain == NULL || value == NULL) return result;

    /* Converte EgValidationValue para EgValue interno */
    EgValue internal_value;
    internal_value.type = value->type;
    switch (value->type) {
        case EG_PROPERTY_TYPE_STRING:
            internal_value.string_val = value->string_val;
            break;
        case EG_PROPERTY_TYPE_INT:
            internal_value.int_val = value->int_val;
            break;
        case EG_PROPERTY_TYPE_DOUBLE:
            internal_value.double_val = value->double_val;
            break;
        case EG_PROPERTY_TYPE_BOOL:
            internal_value.bool_val = value->bool_val;
            break;
        default:
            internal_value.string_val = NULL;
            break;
    }

    return validate_chain_internal(chain, &internal_value);
}

/* ============================================
 * Dados de Widget - Stored via g_object_set_data
 * ============================================ */

static const char *VALIDATION_DATA_KEY = "eg-validation-data";

static EgWidgetValidationData *get_widget_validation_data(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return NULL;
    return (EgWidgetValidationData *)g_object_get_data(G_OBJECT(widget->native), VALIDATION_DATA_KEY);
}

static void free_validation_data(gpointer data) {
    EgWidgetValidationData *vd = (EgWidgetValidationData *)data;
    if (vd == NULL) return;

    if (vd->chain != NULL) {
        eg_validator_chain_free(vd->chain);
    }
    eg_free(vd);
}

static EgWidgetValidationData *ensure_validation_data(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return NULL;

    EgWidgetValidationData *vd = get_widget_validation_data(widget);
    if (vd == NULL) {
        vd = EG_ALLOC(EgWidgetValidationData);
        if (vd == NULL) return NULL;

        vd->chain = NULL;
        vd->validate_on_focus_out = false;
        vd->validate_on_change = false;
        vd->focus_out_handler = 0;
        vd->change_handler = 0;

        g_object_set_data_full(G_OBJECT(widget->native), VALIDATION_DATA_KEY,
                               vd, free_validation_data);
    }
    return vd;
}

/* ============================================
 * API Publica - Widget Binding
 * ============================================ */

bool eg_widget_supports_validation(EgWidget *widget) {
    return get_validation_vtable(widget) != NULL;
}

bool eg_widget_set_validators(EgWidget *widget, EgValidatorChain *chain) {
    if (!eg_widget_supports_validation(widget)) return false;
    if (chain == NULL) return false;

    EgWidgetValidationData *vd = ensure_validation_data(widget);
    if (vd == NULL) return false;

    /* Libera chain anterior se existir */
    if (vd->chain != NULL) {
        eg_validator_chain_free(vd->chain);
    }

    vd->chain = chain;
    return true;
}

void eg_widget_clear_validators(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return;

    EgWidgetValidationData *vd = get_widget_validation_data(widget);
    if (vd != NULL && vd->chain != NULL) {
        eg_validator_chain_free(vd->chain);
        vd->chain = NULL;
    }
}

/* ============================================
 * API Publica - Validacao
 * ============================================ */

static EgValidationResult validate_widget_internal(EgWidget *widget, bool show_error) {
    EgValidationResult result = { .is_valid = true, .error_message = NULL };

    const EgValidationVTable *vtable = get_validation_vtable(widget);
    if (vtable == NULL) return result;

    EgWidgetValidationData *vd = get_widget_validation_data(widget);
    if (vd == NULL || vd->chain == NULL) return result;

    /* Obtem valor do widget (EgValue interno) */
    EgValue value = { .type = EG_PROPERTY_TYPE_STRING };
    if (vtable->get_value != NULL) {
        value = vtable->get_value(widget);
    }

    /* Valida usando funcao interna */
    result = validate_chain_internal(vd->chain, &value);

    /* Mostra/limpa erro visual */
    if (show_error) {
        if (!result.is_valid && vtable->show_error != NULL) {
            vtable->show_error(widget, result.error_message);
        } else if (result.is_valid && vtable->clear_error != NULL) {
            vtable->clear_error(widget);
        }
    }

    return result;
}

EgValidationResult eg_widget_validate(EgWidget *widget) {
    return validate_widget_internal(widget, true);
}

EgValidationResult eg_widget_validate_silent(EgWidget *widget) {
    return validate_widget_internal(widget, false);
}

void eg_widget_clear_validation_error(EgWidget *widget) {
    const EgValidationVTable *vtable = get_validation_vtable(widget);
    if (vtable != NULL && vtable->clear_error != NULL) {
        vtable->clear_error(widget);
    }
}

/* ============================================
 * Callbacks para Validacao Automatica
 * ============================================ */

static void on_focus_out_validate(EgWidget *widget, void *user_data) {
    (void)user_data;
    eg_widget_validate(widget);
}

/* Reservado para uso futuro em eg_widget_validate_on_change
static void on_change_validate(EgWidget *widget, void *user_data) {
    (void)user_data;
    eg_widget_validate(widget);
}
*/

void eg_widget_validate_on_focus_out(EgWidget *widget, bool enable) {
    if (!eg_widget_supports_validation(widget)) return;

    EgWidgetValidationData *vd = ensure_validation_data(widget);
    if (vd == NULL) return;

    if (enable && !vd->validate_on_focus_out) {
        eg_widget_on_focus_out(widget, on_focus_out_validate, NULL);
        vd->validate_on_focus_out = true;
    }
    /* Nota: desconectar handler eh mais complexo, por simplicidade nao implementamos */
}

void eg_widget_validate_on_change(EgWidget *widget, bool enable) {
    (void)widget;
    (void)enable;
    /* TODO: Implementar quando tivermos sistema de signals por tipo de widget */
    /* Por exemplo, Entry precisa conectar "changed", SpinButton "value-changed" */
}

/* ============================================
 * API Publica - Validacao em Lote
 * ============================================ */

bool eg_validate_all(EgWidget **widgets, size_t count, EgValidationResult *results) {
    if (widgets == NULL || count == 0) return true;

    bool all_valid = true;

    for (size_t i = 0; i < count; i++) {
        EgValidationResult result = eg_widget_validate(widgets[i]);
        if (results != NULL) {
            results[i] = result;
        }
        if (!result.is_valid) {
            all_valid = false;
        }
    }

    return all_valid;
}

int eg_validate_first_invalid(EgWidget **widgets, size_t count) {
    if (widgets == NULL || count == 0) return -1;

    for (size_t i = 0; i < count; i++) {
        EgValidationResult result = eg_widget_validate(widgets[i]);
        if (!result.is_valid) {
            return (int)i;
        }
    }

    return -1;
}
