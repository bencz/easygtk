/**
 * EasyGTK - CheckButton
 * 
 * Widget de checkbox (caixa de seleção).
 */

#ifndef EASYGTK_CHECK_BUTTON_H
#define EASYGTK_CHECK_BUTTON_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo check button.
 * 
 * @param label Texto do label (pode ser NULL)
 * @return Ponteiro para o check button ou NULL
 */
EgCheckButton *eg_check_button_new(const char *label);

/**
 * Cria um novo check button com markup.
 * 
 * @param markup Texto com markup Pango
 * @return Ponteiro para o check button ou NULL
 */
EgCheckButton *eg_check_button_new_with_markup(const char *markup);

/**
 * Libera os recursos do check button.
 * 
 * @param check_button Ponteiro para o check button
 */
void eg_check_button_free(EgCheckButton *check_button);

/**
 * Define se o check button está marcado.
 * 
 * @param check_button Ponteiro para o check button
 * @param active true para marcado
 */
void eg_check_button_set_active(EgCheckButton *check_button, bool active);

/**
 * Verifica se o check button está marcado.
 * 
 * @param check_button Ponteiro para o check button
 * @return true se marcado
 */
bool eg_check_button_get_active(EgCheckButton *check_button);

/**
 * Define o texto do label.
 * 
 * @param check_button Ponteiro para o check button
 * @param label Novo texto
 */
void eg_check_button_set_label(EgCheckButton *check_button, const char *label);

/**
 * Obtém o texto do label.
 * 
 * @param check_button Ponteiro para o check button
 * @return Texto atual
 */
const char *eg_check_button_get_label(EgCheckButton *check_button);

/**
 * Define callback para quando o estado mudar.
 * 
 * @param check_button Ponteiro para o check button
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_check_button_on_toggled(EgCheckButton *check_button, EgCallback callback, void *user_data);

/**
 * Agrupa check buttons (radio button behavior).
 * 
 * @param check_button Ponteiro para o check button
 * @param group Outro check button do grupo (ou NULL para criar novo grupo)
 */
void eg_check_button_set_group(EgCheckButton *check_button, EgCheckButton *group);

/**
 * Obtém o widget base do check button.
 * 
 * @param check_button Ponteiro para o check button
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_check_button_as_widget(EgCheckButton *check_button);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param check_button Ponteiro para o check button
 * @return Ponteiro para GtkCheckButton
 */
void *eg_check_button_get_native(EgCheckButton *check_button);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_CHECK_BUTTON_H */
