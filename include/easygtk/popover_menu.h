/**
 * EasyGTK - PopoverMenu
 *
 * Menu popup que pode ser associado a um MenuButton.
 * Usa GMenu/GtkPopoverMenu internamente.
 */

#ifndef EASYGTK_POPOVER_MENU_H
#define EASYGTK_POPOVER_MENU_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback para ações de menu.
 *
 * @param action_name Nome da ação que foi ativada
 * @param user_data Dados do usuário
 */
typedef void (*EgMenuActionCallback)(const char *action_name, void *user_data);

/**
 * Cria um novo menu popover vazio.
 *
 * @return Ponteiro para o novo menu ou NULL em caso de erro
 */
EgPopoverMenu *eg_popover_menu_new(void);

/**
 * Libera os recursos do menu.
 *
 * @param menu Ponteiro para o menu
 */
void eg_popover_menu_free(EgPopoverMenu *menu);

/**
 * Adiciona um item ao menu.
 *
 * @param menu Ponteiro para o menu
 * @param label Texto do item
 * @param action_name Nome único da ação (sem prefixo "menu.")
 * @param callback Callback a ser chamado quando o item for clicado
 * @param user_data Dados do usuário para o callback
 */
void eg_popover_menu_add_item(EgPopoverMenu *menu, const char *label,
                               const char *action_name, EgMenuActionCallback callback,
                               void *user_data);

/**
 * Adiciona um item com ícone ao menu.
 *
 * @param menu Ponteiro para o menu
 * @param label Texto do item
 * @param icon_name Nome do ícone (ex: "edit-copy", "document-open")
 * @param action_name Nome único da ação (sem prefixo "menu.")
 * @param callback Callback a ser chamado quando o item for clicado
 * @param user_data Dados do usuário para o callback
 */
void eg_popover_menu_add_item_with_icon(EgPopoverMenu *menu, const char *label,
                                         const char *icon_name, const char *action_name,
                                         EgMenuActionCallback callback, void *user_data);

/**
 * Adiciona um separador ao menu.
 *
 * @param menu Ponteiro para o menu
 */
void eg_popover_menu_add_separator(EgPopoverMenu *menu);

/**
 * Adiciona uma seção com título ao menu.
 * Os itens adicionados após esta chamada farão parte desta seção
 * até que uma nova seção seja criada.
 *
 * @param menu Ponteiro para o menu
 * @param title Título da seção (pode ser NULL para seção sem título)
 */
void eg_popover_menu_add_section(EgPopoverMenu *menu, const char *title);

/**
 * Habilita ou desabilita um item do menu.
 *
 * @param menu Ponteiro para o menu
 * @param action_name Nome da ação
 * @param enabled true para habilitar, false para desabilitar
 */
void eg_popover_menu_set_item_enabled(EgPopoverMenu *menu, const char *action_name,
                                       bool enabled);

/**
 * Mostra o menu popover.
 *
 * @param menu Ponteiro para o menu
 */
void eg_popover_menu_popup(EgPopoverMenu *menu);

/**
 * Esconde o menu popover.
 *
 * @param menu Ponteiro para o menu
 */
void eg_popover_menu_popdown(EgPopoverMenu *menu);

/**
 * Converte para EgWidget.
 *
 * @param menu Ponteiro para o menu
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_popover_menu_as_widget(EgPopoverMenu *menu);

/**
 * Obtém o ponteiro nativo GTK.
 *
 * @param menu Ponteiro para o menu
 * @return Ponteiro para GtkPopoverMenu
 */
void *eg_popover_menu_get_native(EgPopoverMenu *menu);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_POPOVER_MENU_H */
