/**
 * EasyGTK - Grid
 * 
 * Widget container em grade.
 */

#ifndef EASYGTK_GRID_H
#define EASYGTK_GRID_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo grid.
 * 
 * @return Ponteiro para o grid ou NULL em caso de erro
 */
EgGrid *eg_grid_new(void);

/**
 * Libera os recursos do grid.
 * 
 * @param grid Ponteiro para o grid
 */
void eg_grid_free(EgGrid *grid);

/**
 * Adiciona um widget ao grid.
 * 
 * @param grid Ponteiro para o grid
 * @param child Widget filho
 * @param column Coluna (0-indexed)
 * @param row Linha (0-indexed)
 * @param width Largura em colunas
 * @param height Altura em linhas
 */
void eg_grid_attach(EgGrid *grid, EgWidget *child, int column, int row, int width, int height);

/**
 * Remove um widget do grid.
 * 
 * @param grid Ponteiro para o grid
 * @param child Widget filho a remover
 */
void eg_grid_remove(EgGrid *grid, EgWidget *child);

/**
 * Define o espaçamento entre colunas.
 * 
 * @param grid Ponteiro para o grid
 * @param spacing Espaçamento
 */
void eg_grid_set_column_spacing(EgGrid *grid, int spacing);

/**
 * Define o espaçamento entre linhas.
 * 
 * @param grid Ponteiro para o grid
 * @param spacing Espaçamento
 */
void eg_grid_set_row_spacing(EgGrid *grid, int spacing);

/**
 * Define se as colunas têm largura igual.
 * 
 * @param grid Ponteiro para o grid
 * @param homogeneous true para largura igual
 */
void eg_grid_set_column_homogeneous(EgGrid *grid, bool homogeneous);

/**
 * Define se as linhas têm altura igual.
 * 
 * @param grid Ponteiro para o grid
 * @param homogeneous true para altura igual
 */
void eg_grid_set_row_homogeneous(EgGrid *grid, bool homogeneous);

/**
 * Obtém o widget base do grid.
 * 
 * @param grid Ponteiro para o grid
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_grid_as_widget(EgGrid *grid);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param grid Ponteiro para o grid
 * @return Ponteiro para GtkGrid
 */
void *eg_grid_get_native(EgGrid *grid);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_GRID_H */
