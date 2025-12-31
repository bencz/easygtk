/**
 * EasyGTK - ListView
 *
 * Lista vertical de itens com suporte a seleção simples ou múltipla.
 * Usa GtkListView internamente com GtkStringList para itens de texto.
 */

#ifndef EASYGTK_LIST_VIEW_H
#define EASYGTK_LIST_VIEW_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback chamado quando a seleção muda.
 * Use eg_list_view_get_selected() ou eg_list_view_get_selected_indices() para obter a seleção.
 */

/**
 * Cria uma nova lista vazia.
 *
 * @param selection_mode Modo de seleção (NONE, SINGLE, MULTIPLE)
 * @return Ponteiro para a nova lista ou NULL em caso de erro
 */
EgListView *eg_list_view_new(EgSelectionMode selection_mode);

/**
 * Cria uma nova lista com itens iniciais.
 *
 * @param items Array de strings terminado em NULL
 * @param selection_mode Modo de seleção
 * @return Ponteiro para a nova lista ou NULL em caso de erro
 */
EgListView *eg_list_view_new_with_items(const char *const *items, EgSelectionMode selection_mode);

/**
 * Libera os recursos da lista.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_free(EgListView *list_view);

/**
 * Adiciona um item ao final da lista.
 *
 * @param list_view Ponteiro para a lista
 * @param text Texto do item
 */
void eg_list_view_append(EgListView *list_view, const char *text);

/**
 * Insere um item em uma posição específica.
 *
 * @param list_view Ponteiro para a lista
 * @param position Posição onde inserir (0 = início)
 * @param text Texto do item
 */
void eg_list_view_insert(EgListView *list_view, unsigned int position, const char *text);

/**
 * Remove um item da lista.
 *
 * @param list_view Ponteiro para a lista
 * @param position Posição do item a remover
 */
void eg_list_view_remove(EgListView *list_view, unsigned int position);

/**
 * Remove todos os itens da lista.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_clear(EgListView *list_view);

/**
 * Obtém o número de itens na lista.
 *
 * @param list_view Ponteiro para a lista
 * @return Número de itens
 */
unsigned int eg_list_view_get_count(EgListView *list_view);

/**
 * Obtém o texto de um item.
 *
 * @param list_view Ponteiro para a lista
 * @param position Posição do item
 * @return Texto do item ou NULL se posição inválida
 */
const char *eg_list_view_get_item(EgListView *list_view, unsigned int position);

/**
 * Seleciona um item (para seleção simples).
 *
 * @param list_view Ponteiro para a lista
 * @param position Posição do item a selecionar
 */
void eg_list_view_select(EgListView *list_view, unsigned int position);

/**
 * Remove a seleção de todos os itens.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_unselect_all(EgListView *list_view);

/**
 * Seleciona todos os itens (apenas para seleção múltipla).
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_select_all(EgListView *list_view);

/**
 * Obtém o índice do item selecionado (para seleção simples).
 *
 * @param list_view Ponteiro para a lista
 * @return Índice do item selecionado ou -1 se nenhum
 */
int eg_list_view_get_selected(EgListView *list_view);

/**
 * Obtém o texto do item selecionado (para seleção simples).
 *
 * @param list_view Ponteiro para a lista
 * @return Texto do item selecionado ou NULL
 */
const char *eg_list_view_get_selected_text(EgListView *list_view);

/**
 * Obtém os índices dos itens selecionados (para seleção múltipla).
 * O array retornado deve ser liberado com free().
 *
 * @param list_view Ponteiro para a lista
 * @param count Ponteiro para receber o número de itens selecionados
 * @return Array de índices ou NULL se nenhum selecionado
 */
unsigned int *eg_list_view_get_selected_indices(EgListView *list_view, unsigned int *count);

/**
 * Define callback para quando a seleção muda.
 *
 * @param list_view Ponteiro para a lista
 * @param callback Função callback
 * @param user_data Dados do usuário
 */
void eg_list_view_on_selection_changed(EgListView *list_view, EgCallback callback, void *user_data);

/**
 * Define callback para quando um item é ativado (duplo clique ou Enter).
 *
 * @param list_view Ponteiro para a lista
 * @param callback Função callback
 * @param user_data Dados do usuário
 */
void eg_list_view_on_activate(EgListView *list_view, EgCallback callback, void *user_data);

/**
 * Define se a lista deve mostrar separadores entre itens.
 *
 * @param list_view Ponteiro para a lista
 * @param show_separators true para mostrar separadores
 */
void eg_list_view_set_show_separators(EgListView *list_view, bool show_separators);

/**
 * Define se clique único deve selecionar e ativar.
 *
 * @param list_view Ponteiro para a lista
 * @param single_click true para ativar com clique único
 */
void eg_list_view_set_single_click_activate(EgListView *list_view, bool single_click);

/**
 * Rola a lista para mostrar um item específico.
 *
 * @param list_view Ponteiro para a lista
 * @param position Posição do item
 */
void eg_list_view_scroll_to(EgListView *list_view, unsigned int position);

/* ============================================
 * Ordenação
 * ============================================ */

/**
 * Ordena a lista em ordem alfabética ascendente.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_sort_ascending(EgListView *list_view);

/**
 * Ordena a lista em ordem alfabética descendente.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_sort_descending(EgListView *list_view);

/**
 * Ordena a lista usando uma função de comparação customizada.
 *
 * @param list_view Ponteiro para a lista
 * @param compare_func Função de comparação
 * @param user_data Dados do usuário passados à função de comparação
 */
void eg_list_view_sort_custom(EgListView *list_view, EgListViewCompareFunc compare_func, void *user_data);

/**
 * Habilita ordenação automática ao adicionar itens.
 * Quando habilitada, a lista mantém os itens sempre ordenados.
 *
 * @param list_view Ponteiro para a lista
 * @param ascending true para ordem ascendente, false para descendente
 */
void eg_list_view_set_auto_sort(EgListView *list_view, bool ascending);

/**
 * Desabilita ordenação automática.
 *
 * @param list_view Ponteiro para a lista
 */
void eg_list_view_disable_auto_sort(EgListView *list_view);

/**
 * Converte para EgWidget.
 *
 * @param list_view Ponteiro para a lista
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_list_view_as_widget(EgListView *list_view);

/**
 * Obtém o ponteiro nativo GTK.
 *
 * @param list_view Ponteiro para a lista
 * @return Ponteiro para GtkListView
 */
void *eg_list_view_get_native(EgListView *list_view);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_LIST_VIEW_H */
