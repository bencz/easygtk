/**
 * EasyGTK - TextView
 * 
 * Campo de texto multi-linha.
 */

#ifndef EASYGTK_TEXT_VIEW_H
#define EASYGTK_TEXT_VIEW_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo text view.
 * 
 * @return Ponteiro para o text view ou NULL
 */
EgTextView *eg_text_view_new(void);

/**
 * Libera o text view.
 */
void eg_text_view_free(EgTextView *text_view);

/**
 * Define o texto.
 */
void eg_text_view_set_text(EgTextView *text_view, const char *text);

/**
 * Obtém o texto.
 * 
 * @return Texto (deve ser liberado com free() pelo chamador)
 */
char *eg_text_view_get_text(EgTextView *text_view);

/**
 * Define se o texto é editável.
 */
void eg_text_view_set_editable(EgTextView *text_view, bool editable);

/**
 * Verifica se o texto é editável.
 */
bool eg_text_view_get_editable(EgTextView *text_view);

/**
 * Define se o cursor é visível.
 */
void eg_text_view_set_cursor_visible(EgTextView *text_view, bool visible);

/**
 * Define o modo de quebra de linha.
 * mode: 0=none, 1=char, 2=word, 3=word_char
 */
void eg_text_view_set_wrap_mode(EgTextView *text_view, int mode);

/**
 * Define a justificação do texto.
 * justification: 0=left, 1=right, 2=center, 3=fill
 */
void eg_text_view_set_justification(EgTextView *text_view, int justification);

/**
 * Define margem esquerda em pixels.
 */
void eg_text_view_set_left_margin(EgTextView *text_view, int margin);

/**
 * Define margem direita em pixels.
 */
void eg_text_view_set_right_margin(EgTextView *text_view, int margin);

/**
 * Define margem superior em pixels.
 */
void eg_text_view_set_top_margin(EgTextView *text_view, int margin);

/**
 * Define margem inferior em pixels.
 */
void eg_text_view_set_bottom_margin(EgTextView *text_view, int margin);

/**
 * Define se aceita tabs.
 */
void eg_text_view_set_accepts_tab(EgTextView *text_view, bool accepts_tab);

/**
 * Define fonte monoespaçada.
 */
void eg_text_view_set_monospace(EgTextView *text_view, bool monospace);

/**
 * Define callback para quando o texto mudar.
 */
void eg_text_view_on_changed(EgTextView *text_view, EgCallback callback, void *user_data);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_text_view_as_widget(EgTextView *text_view);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_text_view_get_native(EgTextView *text_view);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_TEXT_VIEW_H */
