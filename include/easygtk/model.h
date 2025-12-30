/**
 * EasyGTK - Model (MVVM)
 *
 * Camada de dados para MVVM. Models representam entidades
 * de domínio com properties observáveis.
 */

#ifndef EASYGTK_MODEL_H
#define EASYGTK_MODEL_H

#include "types.h"
#include "property.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgModel EgModel;

/* VTable para Model - permite extensão */
typedef struct EgModelVTable {
    const char *type_name;
    void (*dispose)(EgModel *model);
    void (*init)(EgModel *model);
    bool (*validate)(EgModel *model);
} EgModelVTable;

/* ============================================
 * Model
 * ============================================ */

/**
 * Cria um novo Model base.
 *
 * @return Ponteiro para o Model ou NULL
 */
EgModel *eg_model_new(void);

/**
 * Cria um Model com vtable customizada.
 *
 * @param vtable VTable com funções customizadas
 * @param instance_size Tamanho da struct derivada
 * @return Ponteiro para o Model ou NULL
 */
EgModel *eg_model_new_with_vtable(const EgModelVTable *vtable, size_t instance_size);

/**
 * Libera o Model e todas suas properties.
 */
void eg_model_free(EgModel *model);

/**
 * Adiciona uma property ao Model.
 * O Model assume ownership da property.
 *
 * @param model Model
 * @param property Property a adicionar
 * @return true se adicionou com sucesso
 */
bool eg_model_add_property(EgModel *model, EgProperty *property);

/**
 * Obtém uma property pelo nome.
 *
 * @param model Model
 * @param name Nome da property
 * @return Ponteiro para a property ou NULL
 */
EgProperty *eg_model_get_property(EgModel *model, const char *name);

/**
 * Atalho: define valor int de uma property.
 */
bool eg_model_set_int(EgModel *model, const char *name, int value);

/**
 * Atalho: obtém valor int de uma property.
 */
int eg_model_get_int(EgModel *model, const char *name);

/**
 * Atalho: define valor string de uma property.
 */
bool eg_model_set_string(EgModel *model, const char *name, const char *value);

/**
 * Atalho: obtém valor string de uma property.
 */
const char *eg_model_get_string(EgModel *model, const char *name);

/**
 * Atalho: define valor bool de uma property.
 */
bool eg_model_set_bool(EgModel *model, const char *name, bool value);

/**
 * Atalho: obtém valor bool de uma property.
 */
bool eg_model_get_bool(EgModel *model, const char *name);

/**
 * Atalho: define valor double de uma property.
 */
bool eg_model_set_double(EgModel *model, const char *name, double value);

/**
 * Atalho: obtém valor double de uma property.
 */
double eg_model_get_double(EgModel *model, const char *name);

/**
 * Valida o Model.
 * Chama a função validate da vtable se existir.
 *
 * @param model Model a validar
 * @return true se válido
 */
bool eg_model_validate(EgModel *model);

/**
 * Obtém dados customizados do Model.
 * Útil para Models derivados.
 */
void *eg_model_get_data(EgModel *model);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_MODEL_H */
