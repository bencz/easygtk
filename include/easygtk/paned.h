/**
 * EasyGTK - Paned (Split View)
 * 
 * Container dividido em dois painéis redimensionáveis.
 */

#ifndef EASYGTK_PANED_H
#define EASYGTK_PANED_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo paned.
 * 
 * @param orientation Orientação (horizontal ou vertical)
 */
EgPaned *eg_paned_new(EgOrientation orientation);

/**
 * Cria um paned horizontal.
 */
EgPaned *eg_paned_new_horizontal(void);

/**
 * Cria um paned vertical.
 */
EgPaned *eg_paned_new_vertical(void);

/**
 * Libera o paned.
 */
void eg_paned_free(EgPaned *paned);

/**
 * Define o filho inicial (esquerda/topo).
 */
void eg_paned_set_start_child(EgPaned *paned, EgWidget *child);

/**
 * Define o filho final (direita/baixo).
 */
void eg_paned_set_end_child(EgPaned *paned, EgWidget *child);

/**
 * Define a posição do divisor em pixels.
 */
void eg_paned_set_position(EgPaned *paned, int position);

/**
 * Obtém a posição do divisor.
 */
int eg_paned_get_position(EgPaned *paned);

/**
 * Define se o filho inicial pode ser redimensionado.
 */
void eg_paned_set_start_child_resize(EgPaned *paned, bool resize);

/**
 * Define se o filho final pode ser redimensionado.
 */
void eg_paned_set_end_child_resize(EgPaned *paned, bool resize);

/**
 * Define se o filho inicial pode encolher.
 */
void eg_paned_set_start_child_shrink(EgPaned *paned, bool shrink);

/**
 * Define se o filho final pode encolher.
 */
void eg_paned_set_end_child_shrink(EgPaned *paned, bool shrink);

/**
 * Define se o divisor é largo (mais fácil de arrastar).
 */
void eg_paned_set_wide_handle(EgPaned *paned, bool wide);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_paned_as_widget(EgPaned *paned);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_paned_get_native(EgPaned *paned);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_PANED_H */
