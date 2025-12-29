/**
 * EasyGTK - Reference Counting
 * 
 * Sistema opcional de contagem de referências para widgets.
 */

#ifndef EASYGTK_REFCOUNT_H
#define EASYGTK_REFCOUNT_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Incrementa a contagem de referências do widget.
 * 
 * @param widget Widget
 * @return O mesmo widget (para encadeamento)
 */
EgWidget *eg_widget_ref(EgWidget *widget);

/**
 * Decrementa a contagem de referências do widget.
 * Se chegar a zero, o widget é destruído.
 * 
 * @param widget Widget
 */
void eg_widget_unref(EgWidget *widget);

/**
 * Obtém a contagem atual de referências.
 * 
 * @param widget Widget
 * @return Contagem de referências (0 se widget é NULL)
 */
uint32_t eg_widget_get_ref_count(EgWidget *widget);

/**
 * Marca o widget como "floating" (referência inicial não contada).
 * Útil para widgets que serão adicionados a containers.
 * 
 * @param widget Widget
 */
void eg_widget_set_floating(EgWidget *widget, bool floating);

/**
 * Verifica se o widget está em estado "floating".
 * 
 * @param widget Widget
 * @return true se floating
 */
bool eg_widget_is_floating(EgWidget *widget);

/**
 * "Afunda" um widget floating, assumindo a propriedade.
 * Se o widget não é floating, incrementa a referência.
 * 
 * @param widget Widget
 * @return O mesmo widget
 */
EgWidget *eg_widget_ref_sink(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_REFCOUNT_H */
