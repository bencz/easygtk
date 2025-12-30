/**
 * EasyGTK - Sistema de Properties Observáveis
 * 
 * Base para data binding e MVVM. Properties notificam
 * automaticamente quando seu valor muda.
 */

#ifndef EASYGTK_PROPERTY_H
#define EASYGTK_PROPERTY_H

#include "types.h"
#include "signal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipos opacos */
typedef struct EgProperty EgProperty;
typedef struct EgBinding EgBinding;

/* Tipos de propriedade */
typedef enum EgPropertyType {
    EG_PROPERTY_TYPE_INT = 0,
    EG_PROPERTY_TYPE_DOUBLE,
    EG_PROPERTY_TYPE_BOOL,
    EG_PROPERTY_TYPE_STRING,
    EG_PROPERTY_TYPE_POINTER
} EgPropertyType;

/* Flags de binding */
typedef enum EgBindingFlags {
    EG_BINDING_ONE_WAY = 0,         /* Source -> Target */
    EG_BINDING_TWO_WAY = 1,         /* Source <-> Target */
    EG_BINDING_ONE_WAY_TO_SOURCE = 2 /* Target -> Source */
} EgBindingFlags;

/* Callback para mudança de valor */
typedef void (*EgPropertyChangedCallback)(EgProperty *property, void *user_data);

/* Callback para transformação de valor em bindings */
typedef bool (*EgBindingTransform)(const void *source_value, void *target_value, void *user_data);

/* Callback para computed properties - recalcula valor baseado em outras properties */
typedef void (*EgPropertyComputeFunc)(EgProperty *computed_property, void *user_data);

/* ============================================
 * Criação de Properties
 * ============================================ */

/**
 * Cria uma property do tipo int.
 */
EgProperty *eg_property_new_int(const char *name, int initial_value);

/**
 * Cria uma property do tipo double.
 */
EgProperty *eg_property_new_double(const char *name, double initial_value);

/**
 * Cria uma property do tipo bool.
 */
EgProperty *eg_property_new_bool(const char *name, bool initial_value);

/**
 * Cria uma property do tipo string.
 */
EgProperty *eg_property_new_string(const char *name, const char *initial_value);

/**
 * Cria uma property do tipo pointer.
 */
EgProperty *eg_property_new_pointer(const char *name, void *initial_value);

/**
 * Libera a property.
 */
void eg_property_free(EgProperty *property);

/* ============================================
 * Getters
 * ============================================ */

int eg_property_get_int(EgProperty *property);
double eg_property_get_double(EgProperty *property);
bool eg_property_get_bool(EgProperty *property);
const char *eg_property_get_string(EgProperty *property);
void *eg_property_get_pointer(EgProperty *property);

/* ============================================
 * Setters (disparam notificação se valor mudar)
 * ============================================ */

void eg_property_set_int(EgProperty *property, int value);
void eg_property_set_double(EgProperty *property, double value);
void eg_property_set_bool(EgProperty *property, bool value);
void eg_property_set_string(EgProperty *property, const char *value);
void eg_property_set_pointer(EgProperty *property, void *value);

/* ============================================
 * Metadados
 * ============================================ */

/**
 * Obtém o nome da property.
 */
const char *eg_property_get_name(EgProperty *property);

/**
 * Obtém o tipo da property.
 */
EgPropertyType eg_property_get_type(EgProperty *property);

/* ============================================
 * Observação de mudanças
 * ============================================ */

/**
 * Conecta um callback para ser chamado quando o valor mudar.
 * 
 * @return Handler ID para desconexão
 */
EgHandlerId eg_property_on_changed(EgProperty *property, EgPropertyChangedCallback callback, void *user_data);

/**
 * Desconecta um callback.
 */
bool eg_property_disconnect(EgProperty *property, EgHandlerId handler_id);

/**
 * Obtém o signal interno (para uso avançado).
 */
EgSignal *eg_property_get_signal(EgProperty *property);

/* ============================================
 * Data Binding
 * ============================================ */

/**
 * Cria um binding entre duas properties.
 * 
 * @param source Property fonte
 * @param target Property destino
 * @param flags Direção do binding
 * @return Ponteiro para o binding ou NULL
 */
EgBinding *eg_property_bind(EgProperty *source, EgProperty *target, EgBindingFlags flags);

/**
 * Cria um binding com transformação de valor.
 * 
 * @param source Property fonte
 * @param target Property destino
 * @param flags Direção do binding
 * @param transform Função de transformação (source -> target)
 * @param reverse_transform Função reversa (target -> source), pode ser NULL
 * @param user_data Dados passados às funções de transformação
 * @return Ponteiro para o binding ou NULL
 */
EgBinding *eg_property_bind_transform(
    EgProperty *source, 
    EgProperty *target, 
    EgBindingFlags flags,
    EgBindingTransform transform,
    EgBindingTransform reverse_transform,
    void *user_data
);

/**
 * Remove o binding.
 */
void eg_binding_unbind(EgBinding *binding);

/* ============================================
 * Computed Properties
 * ============================================ */

/**
 * Define uma property como computada (derivada de outras).
 * O compute_func será chamado sempre que qualquer dependência mudar.
 *
 * @param computed_property Property que será computada
 * @param compute_func Função que recalcula o valor
 * @param dependencies Array de properties das quais depende
 * @param dependency_count Número de dependências
 * @param user_data Dados do usuário
 * @return true se configurou com sucesso
 */
bool eg_property_set_computed(
    EgProperty *computed_property,
    EgPropertyComputeFunc compute_func,
    EgProperty **dependencies,
    size_t dependency_count,
    void *user_data
);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_PROPERTY_H */
