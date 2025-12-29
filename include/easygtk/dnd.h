/**
 * EasyGTK - Drag and Drop
 * 
 * Sistema de arrastar e soltar.
 */

#ifndef EASYGTK_DND_H
#define EASYGTK_DND_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Ações de drag */
typedef enum EgDragAction {
    EG_DRAG_ACTION_COPY = 1 << 0,
    EG_DRAG_ACTION_MOVE = 1 << 1,
    EG_DRAG_ACTION_LINK = 1 << 2
} EgDragAction;

/* Callback para preparar dados do drag */
typedef const char *(*EgDragPrepareCallback)(EgWidget *widget, void *user_data);

/* Callback para quando o drag começa */
typedef void (*EgDragBeginCallback)(EgWidget *widget, void *user_data);

/* Callback para quando o drag termina */
typedef void (*EgDragEndCallback)(EgWidget *widget, bool success, void *user_data);

/* Callback para quando dados são soltos */
typedef void (*EgDropCallback)(EgWidget *widget, const char *data, double x, double y, void *user_data);

/* Callback para verificar se aceita o drop */
typedef bool (*EgDropAcceptCallback)(EgWidget *widget, void *user_data);

/**
 * Configura um widget como fonte de drag (pode ser arrastado).
 * 
 * @param widget Widget fonte
 * @param actions Ações permitidas (COPY, MOVE, LINK)
 * @param prepare_callback Callback para obter os dados a serem arrastados
 * @param user_data Dados do usuário
 */
void eg_widget_set_drag_source(EgWidget *widget, EgDragAction actions,
                                EgDragPrepareCallback prepare_callback, void *user_data);

/**
 * Define callback para quando o drag começa.
 * 
 * @param widget Widget fonte
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_drag_begin(EgWidget *widget, EgDragBeginCallback callback, void *user_data);

/**
 * Define callback para quando o drag termina.
 * 
 * @param widget Widget fonte
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_drag_end(EgWidget *widget, EgDragEndCallback callback, void *user_data);

/**
 * Configura um widget como destino de drop (pode receber itens arrastados).
 * 
 * @param widget Widget destino
 * @param actions Ações aceitas
 * @param drop_callback Callback quando dados são soltos
 * @param user_data Dados do usuário
 */
void eg_widget_set_drop_target(EgWidget *widget, EgDragAction actions,
                                EgDropCallback drop_callback, void *user_data);

/**
 * Remove a configuração de drag source do widget.
 * 
 * @param widget Widget
 */
void eg_widget_remove_drag_source(EgWidget *widget);

/**
 * Remove a configuração de drop target do widget.
 * 
 * @param widget Widget
 */
void eg_widget_remove_drop_target(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_DND_H */
