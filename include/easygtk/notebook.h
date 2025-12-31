/**
 * EasyGTK - Notebook (Tabs)
 * 
 * Container com abas para múltiplas páginas.
 */

#ifndef EASYGTK_NOTEBOOK_H
#define EASYGTK_NOTEBOOK_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo notebook.
 */
EgNotebook *eg_notebook_new(void);

/**
 * Libera o notebook.
 */
void eg_notebook_free(EgNotebook *notebook);

/**
 * Adiciona uma página no final.
 * 
 * @param notebook O notebook
 * @param child Widget filho
 * @param label Texto da aba
 * @return Índice da página adicionada
 */
int eg_notebook_append_page(EgNotebook *notebook, EgWidget *child, const char *label);

/**
 * Adiciona uma página no início.
 */
int eg_notebook_prepend_page(EgNotebook *notebook, EgWidget *child, const char *label);

/**
 * Insere uma página em posição específica.
 */
int eg_notebook_insert_page(EgNotebook *notebook, EgWidget *child, const char *label, int position);

/**
 * Remove uma página.
 */
void eg_notebook_remove_page(EgNotebook *notebook, int page_num);

/**
 * Define a página atual.
 */
void eg_notebook_set_current_page(EgNotebook *notebook, int page_num);

/**
 * Obtém o índice da página atual.
 */
int eg_notebook_get_current_page(EgNotebook *notebook);

/**
 * Obtém o número de páginas.
 */
int eg_notebook_get_n_pages(EgNotebook *notebook);

/**
 * Define a posição das abas.
 * position: 0=left, 1=right, 2=top, 3=bottom
 */
void eg_notebook_set_tab_pos(EgNotebook *notebook, int position);

/**
 * Define se as abas são visíveis.
 */
void eg_notebook_set_show_tabs(EgNotebook *notebook, bool show);

/**
 * Define se a borda é visível.
 */
void eg_notebook_set_show_border(EgNotebook *notebook, bool show);

/**
 * Define se as abas podem ser roladas.
 */
void eg_notebook_set_scrollable(EgNotebook *notebook, bool scrollable);

/**
 * Define o texto da aba de uma página.
 */
void eg_notebook_set_tab_label_text(EgNotebook *notebook, EgWidget *child, const char *label);

/**
 * Define callback para quando a página mudar.
 */
void eg_notebook_on_page_changed(EgNotebook *notebook, EgCallback callback, void *user_data);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_notebook_as_widget(EgNotebook *notebook);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_notebook_get_native(EgNotebook *notebook);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_NOTEBOOK_H */
