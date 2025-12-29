/**
 * EasyGTK - Label
 * 
 * Widget de texto estático.
 */

#ifndef EASYGTK_LABEL_H
#define EASYGTK_LABEL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova label.
 * 
 * @param text Texto inicial
 * @return Ponteiro para a label ou NULL em caso de erro
 */
EgLabel *eg_label_new(const char *text);

/**
 * Libera os recursos da label.
 * 
 * @param label Ponteiro para a label
 */
void eg_label_free(EgLabel *label);

/**
 * Define o texto da label.
 * 
 * @param label Ponteiro para a label
 * @param text Novo texto
 */
void eg_label_set_text(EgLabel *label, const char *text);

/**
 * Obtém o texto da label.
 * 
 * @param label Ponteiro para a label
 * @return Texto atual
 */
const char *eg_label_get_text(EgLabel *label);

/**
 * Define o texto com markup Pango.
 * 
 * @param label Ponteiro para a label
 * @param markup Texto com markup
 */
void eg_label_set_markup(EgLabel *label, const char *markup);

/**
 * Define o alinhamento horizontal do texto.
 * 
 * @param label Ponteiro para a label
 * @param align Alinhamento
 */
void eg_label_set_halign(EgLabel *label, EgAlign align);

/**
 * Define o alinhamento vertical do texto.
 * 
 * @param label Ponteiro para a label
 * @param align Alinhamento
 */
void eg_label_set_valign(EgLabel *label, EgAlign align);

/**
 * Define se o texto pode ser selecionado.
 * 
 * @param label Ponteiro para a label
 * @param selectable true para permitir seleção
 */
void eg_label_set_selectable(EgLabel *label, bool selectable);

/**
 * Define se o texto deve quebrar linhas.
 * 
 * @param label Ponteiro para a label
 * @param wrap true para quebrar linhas
 */
void eg_label_set_wrap(EgLabel *label, bool wrap);

/**
 * Obtém o widget base da label.
 * 
 * @param label Ponteiro para a label
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_label_as_widget(EgLabel *label);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param label Ponteiro para a label
 * @return Ponteiro para GtkLabel
 */
void *eg_label_get_native(EgLabel *label);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_LABEL_H */
