/**
 * EasyGTK - Switch
 * 
 * Widget de toggle switch (liga/desliga).
 */

#ifndef EASYGTK_SWITCH_H
#define EASYGTK_SWITCH_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgSwitch EgSwitch;

/**
 * Cria um novo switch.
 * 
 * @return Ponteiro para o switch ou NULL
 */
EgSwitch *eg_switch_new(void);

/**
 * Libera os recursos do switch.
 * 
 * @param sw Ponteiro para o switch
 */
void eg_switch_free(EgSwitch *sw);

/**
 * Define se o switch está ativo.
 * 
 * @param sw Ponteiro para o switch
 * @param active true para ativo
 */
void eg_switch_set_active(EgSwitch *sw, bool active);

/**
 * Verifica se o switch está ativo.
 * 
 * @param sw Ponteiro para o switch
 * @return true se ativo
 */
bool eg_switch_get_active(EgSwitch *sw);

/**
 * Define callback para quando o estado mudar.
 * 
 * @param sw Ponteiro para o switch
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_switch_on_state_changed(EgSwitch *sw, EgCallback callback, void *user_data);

/**
 * Obtém o widget base do switch.
 * 
 * @param sw Ponteiro para o switch
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_switch_as_widget(EgSwitch *sw);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param sw Ponteiro para o switch
 * @return Ponteiro para GtkSwitch
 */
void *eg_switch_get_native(EgSwitch *sw);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SWITCH_H */
