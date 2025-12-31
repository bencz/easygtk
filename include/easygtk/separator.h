/**
 * EasyGTK - Separator Widget
 * 
 * Widget para criar linhas separadoras horizontais ou verticais.
 */

#ifndef EASYGTK_SEPARATOR_H
#define EASYGTK_SEPARATOR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo separador.
 * 
 * @param orientation Orientação (EG_ORIENTATION_HORIZONTAL ou EG_ORIENTATION_VERTICAL)
 * @return Ponteiro para o separador ou NULL em caso de erro
 */
EgSeparator *eg_separator_new(EgOrientation orientation);

/**
 * Cria um novo separador horizontal.
 * 
 * @return Ponteiro para o separador ou NULL em caso de erro
 */
EgSeparator *eg_separator_new_horizontal(void);

/**
 * Cria um novo separador vertical.
 * 
 * @return Ponteiro para o separador ou NULL em caso de erro
 */
EgSeparator *eg_separator_new_vertical(void);

/**
 * Libera os recursos do separador.
 * 
 * @param separator Ponteiro para o separador
 */
void eg_separator_free(EgSeparator *separator);

/**
 * Converte para EgWidget.
 * 
 * @param separator Ponteiro para o separador
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_separator_as_widget(EgSeparator *separator);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param separator Ponteiro para o separador
 * @return Ponteiro para GtkSeparator
 */
void *eg_separator_get_native(EgSeparator *separator);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SEPARATOR_H */
