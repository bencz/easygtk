/**
 * EasyGTK - MenuButton
 *
 * Botão que exibe um menu popover quando clicado.
 */

#ifndef EASYGTK_MENU_BUTTON_H
#define EASYGTK_MENU_BUTTON_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo botão de menu vazio.
 *
 * @return Ponteiro para o novo botão ou NULL em caso de erro
 */
EgMenuButton *eg_menu_button_new(void);

/**
 * Cria um novo botão de menu com label.
 *
 * @param label Texto do botão
 * @return Ponteiro para o novo botão ou NULL em caso de erro
 */
EgMenuButton *eg_menu_button_new_with_label(const char *label);

/**
 * Cria um novo botão de menu com ícone.
 *
 * @param icon_name Nome do ícone (ex: "open-menu", "view-more")
 * @return Ponteiro para o novo botão ou NULL em caso de erro
 */
EgMenuButton *eg_menu_button_new_with_icon(const char *icon_name);

/**
 * Libera os recursos do botão.
 *
 * @param button Ponteiro para o botão
 */
void eg_menu_button_free(EgMenuButton *button);

/**
 * Define o menu popover a ser exibido.
 *
 * @param button Ponteiro para o botão
 * @param menu Ponteiro para o menu popover
 */
void eg_menu_button_set_popover(EgMenuButton *button, EgPopoverMenu *menu);

/**
 * Obtém o menu popover associado.
 *
 * @param button Ponteiro para o botão
 * @return Ponteiro para o menu ou NULL
 */
EgPopoverMenu *eg_menu_button_get_popover(EgMenuButton *button);

/**
 * Define o label do botão.
 *
 * @param button Ponteiro para o botão
 * @param label Texto do botão
 */
void eg_menu_button_set_label(EgMenuButton *button, const char *label);

/**
 * Obtém o label do botão.
 *
 * @param button Ponteiro para o botão
 * @return Texto do botão ou NULL
 */
const char *eg_menu_button_get_label(EgMenuButton *button);

/**
 * Define o ícone do botão.
 *
 * @param button Ponteiro para o botão
 * @param icon_name Nome do ícone
 */
void eg_menu_button_set_icon_name(EgMenuButton *button, const char *icon_name);

/**
 * Obtém o nome do ícone do botão.
 *
 * @param button Ponteiro para o botão
 * @return Nome do ícone ou NULL
 */
const char *eg_menu_button_get_icon_name(EgMenuButton *button);

/**
 * Define a direção da seta indicadora do menu.
 *
 * @param button Ponteiro para o botão
 * @param direction Direção: 0=up, 1=down, 2=left, 3=right, 4=none
 */
void eg_menu_button_set_direction(EgMenuButton *button, int direction);

/**
 * Define se o botão deve ter um frame visível.
 *
 * @param button Ponteiro para o botão
 * @param has_frame true para mostrar frame
 */
void eg_menu_button_set_has_frame(EgMenuButton *button, bool has_frame);

/**
 * Verifica se o botão tem frame.
 *
 * @param button Ponteiro para o botão
 * @return true se tem frame
 */
bool eg_menu_button_get_has_frame(EgMenuButton *button);

/**
 * Abre o menu popover programaticamente.
 *
 * @param button Ponteiro para o botão
 */
void eg_menu_button_popup(EgMenuButton *button);

/**
 * Fecha o menu popover programaticamente.
 *
 * @param button Ponteiro para o botão
 */
void eg_menu_button_popdown(EgMenuButton *button);

/**
 * Converte para EgWidget.
 *
 * @param button Ponteiro para o botão
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_menu_button_as_widget(EgMenuButton *button);

/**
 * Obtém o ponteiro nativo GTK.
 *
 * @param button Ponteiro para o botão
 * @return Ponteiro para GtkMenuButton
 */
void *eg_menu_button_get_native(EgMenuButton *button);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_MENU_BUTTON_H */
