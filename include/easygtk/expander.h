/**
 * EasyGTK - Expander
 * 
 * Container expansível/recolhível com título.
 */

#ifndef EASYGTK_EXPANDER_H
#define EASYGTK_EXPANDER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo expander.
 * 
 * @param label Texto do título
 */
EgExpander *eg_expander_new(const char *label);

/**
 * Cria um expander com markup no título.
 */
EgExpander *eg_expander_new_with_markup(const char *markup);

/**
 * Libera o expander.
 */
void eg_expander_free(EgExpander *expander);

/**
 * Define o filho do expander.
 */
void eg_expander_set_child(EgExpander *expander, EgWidget *child);

/**
 * Define se está expandido.
 */
void eg_expander_set_expanded(EgExpander *expander, bool expanded);

/**
 * Verifica se está expandido.
 */
bool eg_expander_get_expanded(EgExpander *expander);

/**
 * Define o texto do título.
 */
void eg_expander_set_label(EgExpander *expander, const char *label);

/**
 * Obtém o texto do título.
 */
const char *eg_expander_get_label(EgExpander *expander);

/**
 * Define se o título usa markup.
 */
void eg_expander_set_use_markup(EgExpander *expander, bool use_markup);

/**
 * Define se o título usa underline para mnemonic.
 */
void eg_expander_set_use_underline(EgExpander *expander, bool use_underline);

/**
 * Define se redimensiona o pai ao expandir.
 */
void eg_expander_set_resize_toplevel(EgExpander *expander, bool resize);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_expander_as_widget(EgExpander *expander);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_expander_get_native(EgExpander *expander);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_EXPANDER_H */
