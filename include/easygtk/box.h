/**
 * EasyGTK - Box
 * 
 * Widget container linear (horizontal ou vertical).
 */

#ifndef EASYGTK_BOX_H
#define EASYGTK_BOX_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo box com orientação especificada.
 * 
 * @param orientation Orientação do box
 * @param spacing Espaçamento entre widgets
 * @return Ponteiro para o box ou NULL em caso de erro
 */
EgBox *eg_box_new(EgOrientation orientation, int spacing);

/**
 * Cria um novo box vertical.
 * 
 * @param spacing Espaçamento entre widgets
 * @return Ponteiro para o box ou NULL em caso de erro
 */
EgBox *eg_box_new_vertical(int spacing);

/**
 * Cria um novo box horizontal.
 * 
 * @param spacing Espaçamento entre widgets
 * @return Ponteiro para o box ou NULL em caso de erro
 */
EgBox *eg_box_new_horizontal(int spacing);

/**
 * Libera os recursos do box.
 * 
 * @param box Ponteiro para o box
 */
void eg_box_free(EgBox *box);

/**
 * Adiciona um widget ao final do box.
 * 
 * @param box Ponteiro para o box
 * @param child Widget filho
 */
void eg_box_append(EgBox *box, EgWidget *child);

/**
 * Adiciona um widget ao início do box.
 * 
 * @param box Ponteiro para o box
 * @param child Widget filho
 */
void eg_box_prepend(EgBox *box, EgWidget *child);

/**
 * Remove um widget do box.
 * 
 * @param box Ponteiro para o box
 * @param child Widget filho a remover
 */
void eg_box_remove(EgBox *box, EgWidget *child);

/**
 * Define o espaçamento entre widgets.
 * 
 * @param box Ponteiro para o box
 * @param spacing Novo espaçamento
 */
void eg_box_set_spacing(EgBox *box, int spacing);

/**
 * Obtém o espaçamento entre widgets.
 * 
 * @param box Ponteiro para o box
 * @return Espaçamento atual
 */
int eg_box_get_spacing(EgBox *box);

/**
 * Define se o espaço é distribuído igualmente.
 * 
 * @param box Ponteiro para o box
 * @param homogeneous true para distribuição igual
 */
void eg_box_set_homogeneous(EgBox *box, bool homogeneous);

/**
 * Obtém o widget base do box.
 * 
 * @param box Ponteiro para o box
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_box_as_widget(EgBox *box);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param box Ponteiro para o box
 * @return Ponteiro para GtkBox
 */
void *eg_box_get_native(EgBox *box);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_BOX_H */
