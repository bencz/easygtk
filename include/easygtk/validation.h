/**
 * EasyGTK - Sistema de Validacao
 *
 * API para validacao declarativa de widgets de input.
 * Permite criar chains de validadores que verificam dados
 * automaticamente ao perder foco ou manualmente.
 */

#ifndef EASYGTK_VALIDATION_H
#define EASYGTK_VALIDATION_H

#include "types.h"
#include "property.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
typedef struct EgValidatorChain EgValidatorChain;

/* ============================================
 * Resultado de Validacao
 * ============================================ */

/**
 * Resultado de uma validacao.
 */
typedef struct EgValidationResult {
    bool is_valid;                  /**< true se passou em todos os validadores */
    const char *error_message;      /**< Mensagem de erro (NULL se valido) */
} EgValidationResult;

/* ============================================
 * Tipos de Validadores
 * ============================================ */

/**
 * Tipos de validadores built-in.
 */
typedef enum EgValidatorType {
    EG_VALIDATOR_REQUIRED,          /**< Campo obrigatorio (nao pode ser vazio) */
    EG_VALIDATOR_MIN_LENGTH,        /**< Tamanho minimo de string */
    EG_VALIDATOR_MAX_LENGTH,        /**< Tamanho maximo de string */
    EG_VALIDATOR_PATTERN,           /**< Expressao regular */
    EG_VALIDATOR_RANGE_INT,         /**< Range de inteiros [min, max] */
    EG_VALIDATOR_RANGE_DOUBLE,      /**< Range de doubles [min, max] */
    EG_VALIDATOR_EMAIL,             /**< Formato de email basico */
    EG_VALIDATOR_CUSTOM             /**< Validador customizado via callback */
} EgValidatorType;

/* ============================================
 * Valor Generico para Validacao
 * ============================================ */

/**
 * Valor generico usado para validacao.
 * Encapsula o valor do widget para passar ao validador.
 */
typedef struct EgValidationValue {
    EgPropertyType type;            /**< Tipo do valor */
    union {
        const char *string_val;     /**< Valor string */
        int int_val;                /**< Valor inteiro */
        double double_val;          /**< Valor double */
        bool bool_val;              /**< Valor booleano */
    };
} EgValidationValue;

/* ============================================
 * Callback para Validacao Customizada
 * ============================================ */

/**
 * Callback para validador customizado.
 *
 * @param value Valor a ser validado
 * @param user_data Dados do usuario
 * @return Resultado da validacao
 */
typedef EgValidationResult (*EgValidateFunc)(const EgValidationValue *value, void *user_data);

/* ============================================
 * Criacao e Destruicao de Chain
 * ============================================ */

/**
 * Cria uma nova chain de validadores.
 *
 * @return Nova chain ou NULL em caso de erro
 */
EgValidatorChain *eg_validator_chain_new(void);

/**
 * Libera a chain e todos os validadores.
 *
 * @param chain Chain a ser liberada
 */
void eg_validator_chain_free(EgValidatorChain *chain);

/* ============================================
 * Adicionar Validadores a Chain
 * ============================================ */

/**
 * Adiciona validador de campo obrigatorio.
 *
 * @param chain Chain de validadores
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_required(EgValidatorChain *chain, const char *error_msg);

/**
 * Adiciona validador de tamanho minimo.
 *
 * @param chain Chain de validadores
 * @param min_length Tamanho minimo
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_min_length(EgValidatorChain *chain, size_t min_length, const char *error_msg);

/**
 * Adiciona validador de tamanho maximo.
 *
 * @param chain Chain de validadores
 * @param max_length Tamanho maximo
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_max_length(EgValidatorChain *chain, size_t max_length, const char *error_msg);

/**
 * Adiciona validador de expressao regular.
 *
 * @param chain Chain de validadores
 * @param pattern Expressao regular (PCRE)
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_pattern(EgValidatorChain *chain, const char *pattern, const char *error_msg);

/**
 * Adiciona validador de range para inteiros.
 *
 * @param chain Chain de validadores
 * @param min Valor minimo
 * @param max Valor maximo
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_range_int(EgValidatorChain *chain, int min, int max, const char *error_msg);

/**
 * Adiciona validador de range para doubles.
 *
 * @param chain Chain de validadores
 * @param min Valor minimo
 * @param max Valor maximo
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_range_double(EgValidatorChain *chain, double min, double max, const char *error_msg);

/**
 * Adiciona validador de formato de email.
 *
 * @param chain Chain de validadores
 * @param error_msg Mensagem de erro (NULL para default)
 */
void eg_validator_add_email(EgValidatorChain *chain, const char *error_msg);

/**
 * Adiciona validador customizado.
 *
 * @param chain Chain de validadores
 * @param func Funcao de validacao
 * @param user_data Dados passados para a funcao
 * @param error_msg Mensagem de erro default (NULL para default)
 */
void eg_validator_add_custom(EgValidatorChain *chain, EgValidateFunc func,
                              void *user_data, const char *error_msg);

/* ============================================
 * Vinculacao de Validadores a Widgets
 * ============================================ */

/**
 * Vincula uma chain de validadores a um widget.
 * O widget assume ownership da chain.
 *
 * @param widget Widget de input (Entry, SpinButton, etc.)
 * @param chain Chain de validadores
 * @return true se vinculou com sucesso
 */
bool eg_widget_set_validators(EgWidget *widget, EgValidatorChain *chain);

/**
 * Remove validadores de um widget.
 *
 * @param widget Widget de input
 */
void eg_widget_clear_validators(EgWidget *widget);

/**
 * Verifica se o widget suporta validacao.
 *
 * @param widget Widget a verificar
 * @return true se suporta validacao
 */
bool eg_widget_supports_validation(EgWidget *widget);

/* ============================================
 * Validacao Manual
 * ============================================ */

/**
 * Valida o widget manualmente.
 * Mostra erro visual se invalido.
 *
 * @param widget Widget a validar
 * @return Resultado da validacao
 */
EgValidationResult eg_widget_validate(EgWidget *widget);

/**
 * Valida o widget sem mostrar erro visual.
 *
 * @param widget Widget a validar
 * @return Resultado da validacao
 */
EgValidationResult eg_widget_validate_silent(EgWidget *widget);

/**
 * Limpa o estado de erro visual do widget.
 *
 * @param widget Widget a limpar
 */
void eg_widget_clear_validation_error(EgWidget *widget);

/* ============================================
 * Validacao Automatica
 * ============================================ */

/**
 * Habilita/desabilita validacao automatica ao perder foco.
 *
 * @param widget Widget de input
 * @param enable true para habilitar
 */
void eg_widget_validate_on_focus_out(EgWidget *widget, bool enable);

/**
 * Habilita/desabilita validacao automatica ao digitar.
 *
 * @param widget Widget de input
 * @param enable true para habilitar
 */
void eg_widget_validate_on_change(EgWidget *widget, bool enable);

/* ============================================
 * Validacao em Lote
 * ============================================ */

/**
 * Valida multiplos widgets de uma vez.
 *
 * @param widgets Array de widgets
 * @param count Numero de widgets
 * @param results Array para receber resultados (pode ser NULL)
 * @return true se todos sao validos
 */
bool eg_validate_all(EgWidget **widgets, size_t count, EgValidationResult *results);

/**
 * Valida multiplos widgets e retorna o primeiro invalido.
 *
 * @param widgets Array de widgets
 * @param count Numero de widgets
 * @return Indice do primeiro invalido, ou -1 se todos validos
 */
int eg_validate_first_invalid(EgWidget **widgets, size_t count);

/* ============================================
 * Helpers para Validacao de Valor
 * ============================================ */

/**
 * Valida um valor diretamente contra uma chain.
 * Util para validacao fora do contexto de widget.
 *
 * @param chain Chain de validadores
 * @param value Valor a validar
 * @return Resultado da validacao
 */
EgValidationResult eg_validator_chain_validate(EgValidatorChain *chain, const EgValidationValue *value);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_VALIDATION_H */
