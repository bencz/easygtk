/**
 * EasyGTK - Botão
 * 
 * Widget de botão clicável.
 */

#ifndef EASYGTK_BUTTON_H
#define EASYGTK_BUTTON_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo botão com texto.
 * 
 * @param label Texto do botão
 * @return Ponteiro para o botão ou NULL em caso de erro
 */
EgButton *eg_button_new(const char *label);

/**
 * Cria um novo botão com ícone.
 * 
 * @param icon_name Nome do ícone
 * @return Ponteiro para o botão ou NULL em caso de erro
 */
EgButton *eg_button_new_with_icon(const char *icon_name);

/**
 * Libera os recursos do botão.
 * 
 * @param button Ponteiro para o botão
 */
void eg_button_free(EgButton *button);

/**
 * Define o texto do botão.
 * 
 * @param button Ponteiro para o botão
 * @param label Novo texto
 */
void eg_button_set_label(EgButton *button, const char *label);

/**
 * Obtém o texto do botão.
 * 
 * @param button Ponteiro para o botão
 * @return Texto atual
 */
const char *eg_button_get_label(EgButton *button);

/**
 * Define callback para clique no botão.
 * 
 * @param button Ponteiro para o botão
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_button_on_click(EgButton *button, EgCallback callback, void *user_data);

/**
 * Define se o botão está habilitado.
 * 
 * @param button Ponteiro para o botão
 * @param sensitive true para habilitar
 */
void eg_button_set_sensitive(EgButton *button, bool sensitive);

/**
 * Verifica se o botão está habilitado.
 * 
 * @param button Ponteiro para o botão
 * @return true se habilitado
 */
bool eg_button_get_sensitive(EgButton *button);

/**
 * Obtém o widget base do botão.
 * 
 * @param button Ponteiro para o botão
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_button_as_widget(EgButton *button);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param button Ponteiro para o botão
 * @return Ponteiro para GtkButton
 */
void *eg_button_get_native(EgButton *button);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_BUTTON_H */
