/**
 * EasyGTK - ColumnView
 *
 * Tabela com múltiplas colunas.
 * Usa GtkColumnView internamente.
 */

#ifndef EASYGTK_COLUMN_VIEW_H
#define EASYGTK_COLUMN_VIEW_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback para obter o valor de uma célula.
 *
 * @param row_index Índice da linha
 * @param user_data Dados do usuário passados ao adicionar a coluna
 * @return String a ser exibida (a biblioteca faz cópia interna)
 */
typedef const char *(*EgColumnValueFunc)(unsigned int row_index, void *user_data);

/**
 * Cria uma nova tabela vazia.
 *
 * @param selection_mode Modo de seleção (NONE, SINGLE, MULTIPLE)
 * @return Ponteiro para a tabela ou NULL em caso de erro
 */
EgColumnView *eg_column_view_new(EgSelectionMode selection_mode);

/**
 * Libera os recursos da tabela.
 *
 * @param column_view Ponteiro para a tabela
 */
void eg_column_view_free(EgColumnView *column_view);

/**
 * Adiciona uma coluna de texto à tabela.
 *
 * @param column_view Ponteiro para a tabela
 * @param title Título da coluna
 * @param column_index Índice da coluna nos dados (para acesso via eg_column_view_set_cell)
 * @return ID da coluna ou -1 em caso de erro
 */
int eg_column_view_add_column(EgColumnView *column_view, const char *title, int column_index);

/**
 * Adiciona uma coluna com largura fixa.
 *
 * @param column_view Ponteiro para a tabela
 * @param title Título da coluna
 * @param column_index Índice da coluna nos dados
 * @param fixed_width Largura fixa em pixels (-1 para automático)
 * @return ID da coluna ou -1 em caso de erro
 */
int eg_column_view_add_column_with_width(EgColumnView *column_view, const char *title,
                                          int column_index, int fixed_width);

/**
 * Define se uma coluna é redimensionável.
 *
 * @param column_view Ponteiro para a tabela
 * @param column_id ID da coluna
 * @param resizable true para permitir redimensionar
 */
void eg_column_view_set_column_resizable(EgColumnView *column_view, int column_id, bool resizable);

/**
 * Define se uma coluna é expansível (ocupa espaço extra).
 *
 * @param column_view Ponteiro para a tabela
 * @param column_id ID da coluna
 * @param expand true para expandir
 */
void eg_column_view_set_column_expand(EgColumnView *column_view, int column_id, bool expand);

/**
 * Adiciona uma linha à tabela.
 *
 * @param column_view Ponteiro para a tabela
 * @param values Array de strings (uma por coluna), terminado em NULL
 * @return Índice da nova linha ou -1 em caso de erro
 */
int eg_column_view_append_row(EgColumnView *column_view, const char *const *values);

/**
 * Adiciona uma linha com valores variádicos.
 * O último argumento deve ser NULL.
 *
 * @param column_view Ponteiro para a tabela
 * @param first_value Primeiro valor
 * @param ... Valores restantes, terminados em NULL
 * @return Índice da nova linha ou -1 em caso de erro
 */
int eg_column_view_append_rowv(EgColumnView *column_view, const char *first_value, ...);

/**
 * Define o valor de uma célula específica.
 *
 * @param column_view Ponteiro para a tabela
 * @param row Índice da linha
 * @param column Índice da coluna
 * @param value Novo valor
 */
void eg_column_view_set_cell(EgColumnView *column_view, unsigned int row,
                              unsigned int column, const char *value);

/**
 * Obtém o valor de uma célula específica.
 *
 * @param column_view Ponteiro para a tabela
 * @param row Índice da linha
 * @param column Índice da coluna
 * @return Valor da célula ou NULL
 */
const char *eg_column_view_get_cell(EgColumnView *column_view, unsigned int row,
                                     unsigned int column);

/**
 * Remove uma linha da tabela.
 *
 * @param column_view Ponteiro para a tabela
 * @param row Índice da linha a remover
 */
void eg_column_view_remove_row(EgColumnView *column_view, unsigned int row);

/**
 * Remove todas as linhas da tabela.
 *
 * @param column_view Ponteiro para a tabela
 */
void eg_column_view_clear(EgColumnView *column_view);

/**
 * Obtém o número de linhas.
 *
 * @param column_view Ponteiro para a tabela
 * @return Número de linhas
 */
unsigned int eg_column_view_get_row_count(EgColumnView *column_view);

/**
 * Obtém o número de colunas.
 *
 * @param column_view Ponteiro para a tabela
 * @return Número de colunas
 */
unsigned int eg_column_view_get_column_count(EgColumnView *column_view);

/**
 * Seleciona uma linha (para seleção simples).
 *
 * @param column_view Ponteiro para a tabela
 * @param row Índice da linha a selecionar
 */
void eg_column_view_select_row(EgColumnView *column_view, unsigned int row);

/**
 * Remove a seleção de todas as linhas.
 *
 * @param column_view Ponteiro para a tabela
 */
void eg_column_view_unselect_all(EgColumnView *column_view);

/**
 * Obtém o índice da linha selecionada (para seleção simples).
 *
 * @param column_view Ponteiro para a tabela
 * @return Índice da linha selecionada ou -1 se nenhuma
 */
int eg_column_view_get_selected_row(EgColumnView *column_view);

/**
 * Obtém os índices das linhas selecionadas (para seleção múltipla).
 * O array retornado deve ser liberado com free().
 *
 * @param column_view Ponteiro para a tabela
 * @param count Ponteiro para receber o número de linhas selecionadas
 * @return Array de índices ou NULL se nenhuma selecionada
 */
unsigned int *eg_column_view_get_selected_rows(EgColumnView *column_view, unsigned int *count);

/**
 * Define callback para quando a seleção muda.
 *
 * @param column_view Ponteiro para a tabela
 * @param callback Função callback
 * @param user_data Dados do usuário
 */
void eg_column_view_on_selection_changed(EgColumnView *column_view, EgCallback callback, void *user_data);

/**
 * Define callback para quando uma linha é ativada (duplo clique ou Enter).
 *
 * @param column_view Ponteiro para a tabela
 * @param callback Função callback
 * @param user_data Dados do usuário
 */
void eg_column_view_on_activate(EgColumnView *column_view, EgCallback callback, void *user_data);

/**
 * Define se deve mostrar cabeçalhos das colunas.
 *
 * @param column_view Ponteiro para a tabela
 * @param show true para mostrar
 */
void eg_column_view_set_show_column_headers(EgColumnView *column_view, bool show);

/**
 * Define se linhas devem ter separadores.
 *
 * @param column_view Ponteiro para a tabela
 * @param show true para mostrar separadores
 */
void eg_column_view_set_show_row_separators(EgColumnView *column_view, bool show);

/**
 * Define se colunas devem ter separadores.
 *
 * @param column_view Ponteiro para a tabela
 * @param show true para mostrar separadores
 */
void eg_column_view_set_show_column_separators(EgColumnView *column_view, bool show);

/* ============================================
 * Ordenação
 * ============================================ */

/**
 * Tipo da função de comparação para ordenação de colunas.
 * Retorna < 0 se a < b, 0 se a == b, > 0 se a > b.
 * Os parâmetros são os valores das células na coluna sendo ordenada.
 */
typedef int (*EgColumnViewCompareFunc)(const char *a, const char *b, void *user_data);

/**
 * Ordena a tabela pela coluna especificada em ordem ascendente.
 *
 * @param column_view Ponteiro para a tabela
 * @param column Índice da coluna para ordenar
 */
void eg_column_view_sort_by_column(EgColumnView *column_view, unsigned int column, bool ascending);

/**
 * Ordena a tabela usando uma função de comparação customizada.
 *
 * @param column_view Ponteiro para a tabela
 * @param column Índice da coluna para ordenar
 * @param compare_func Função de comparação
 * @param user_data Dados do usuário passados à função de comparação
 */
void eg_column_view_sort_custom(EgColumnView *column_view, unsigned int column,
                                 EgColumnViewCompareFunc compare_func, void *user_data);

/**
 * Habilita ordenação automática por uma coluna específica.
 * Quando habilitada, a tabela mantém as linhas ordenadas ao inserir/editar.
 *
 * @param column_view Ponteiro para a tabela
 * @param column Índice da coluna para ordenação (-1 para desabilitar)
 * @param ascending true para ordem ascendente, false para descendente
 */
void eg_column_view_set_auto_sort_column(EgColumnView *column_view, int column, bool ascending);

/**
 * Habilita a ordenação clicável nas colunas.
 * Quando habilitada, clicar no header da coluna ordena por ela.
 *
 * @param column_view Ponteiro para a tabela
 * @param column_id ID da coluna
 * @param sortable true para permitir ordenação por clique
 */
void eg_column_view_set_column_sortable(EgColumnView *column_view, int column_id, bool sortable);

/**
 * Converte para EgWidget.
 *
 * @param column_view Ponteiro para a tabela
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_column_view_as_widget(EgColumnView *column_view);

/**
 * Obtém o ponteiro nativo GTK.
 *
 * @param column_view Ponteiro para a tabela
 * @return Ponteiro para GtkColumnView
 */
void *eg_column_view_get_native(EgColumnView *column_view);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_COLUMN_VIEW_H */
