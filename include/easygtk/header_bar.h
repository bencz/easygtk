/**
 * EasyGTK - HeaderBar Widget
 * 
 * Widget de barra de título personalizada para janelas.
 */

#ifndef EASYGTK_HEADER_BAR_H
#define EASYGTK_HEADER_BAR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova barra de título.
 * 
 * @return Ponteiro para a barra ou NULL em caso de erro
 */
EgHeaderBar *eg_header_bar_new(void);

/**
 * Libera os recursos da barra de título.
 * 
 * @param header_bar Ponteiro para a barra
 */
void eg_header_bar_free(EgHeaderBar *header_bar);

/**
 * Define o widget de título.
 * 
 * @param header_bar Ponteiro para a barra
 * @param title_widget Widget a ser usado como título
 */
void eg_header_bar_set_title_widget(EgHeaderBar *header_bar, EgWidget *title_widget);

/**
 * Obtém o widget de título.
 * 
 * @param header_bar Ponteiro para a barra
 * @return Widget de título ou NULL
 */
EgWidget *eg_header_bar_get_title_widget(EgHeaderBar *header_bar);

/**
 * Adiciona um widget no início (esquerda).
 * 
 * @param header_bar Ponteiro para a barra
 * @param child Widget a adicionar
 */
void eg_header_bar_pack_start(EgHeaderBar *header_bar, EgWidget *child);

/**
 * Adiciona um widget no fim (direita).
 * 
 * @param header_bar Ponteiro para a barra
 * @param child Widget a adicionar
 */
void eg_header_bar_pack_end(EgHeaderBar *header_bar, EgWidget *child);

/**
 * Remove um widget da barra.
 * 
 * @param header_bar Ponteiro para a barra
 * @param child Widget a remover
 */
void eg_header_bar_remove(EgHeaderBar *header_bar, EgWidget *child);

/**
 * Define se mostra os botões de controle da janela.
 * 
 * @param header_bar Ponteiro para a barra
 * @param show_title_buttons true para mostrar
 */
void eg_header_bar_set_show_title_buttons(EgHeaderBar *header_bar, bool show_title_buttons);

/**
 * Verifica se mostra os botões de controle da janela.
 * 
 * @param header_bar Ponteiro para a barra
 * @return true se mostra
 */
bool eg_header_bar_get_show_title_buttons(EgHeaderBar *header_bar);

/**
 * Define o layout dos botões de decoração.
 * 
 * @param header_bar Ponteiro para a barra
 * @param layout String de layout (ex: "icon:minimize,maximize,close")
 */
void eg_header_bar_set_decoration_layout(EgHeaderBar *header_bar, const char *layout);

/**
 * Obtém o layout dos botões de decoração.
 * 
 * @param header_bar Ponteiro para a barra
 * @return String de layout ou NULL
 */
const char *eg_header_bar_get_decoration_layout(EgHeaderBar *header_bar);

/**
 * Converte para EgWidget.
 * 
 * @param header_bar Ponteiro para a barra
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_header_bar_as_widget(EgHeaderBar *header_bar);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param header_bar Ponteiro para a barra
 * @return Ponteiro para GtkHeaderBar
 */
void *eg_header_bar_get_native(EgHeaderBar *header_bar);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_HEADER_BAR_H */
