/**
 * EasyGTK - Type Casting Helpers
 * 
 * Funções seguras para conversão de tipos de widgets.
 */

#ifndef EASYGTK_CAST_H
#define EASYGTK_CAST_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Converte EgWidget para EgWindow (retorna NULL se tipo incorreto).
 */
EgWindow *eg_widget_cast_to_window(EgWidget *widget);

/**
 * Converte EgWidget para EgButton (retorna NULL se tipo incorreto).
 */
EgButton *eg_widget_cast_to_button(EgWidget *widget);

/**
 * Converte EgWidget para EgLabel (retorna NULL se tipo incorreto).
 */
EgLabel *eg_widget_cast_to_label(EgWidget *widget);

/**
 * Converte EgWidget para EgEntry (retorna NULL se tipo incorreto).
 */
EgEntry *eg_widget_cast_to_entry(EgWidget *widget);

/**
 * Converte EgWidget para EgBox (retorna NULL se tipo incorreto).
 */
EgBox *eg_widget_cast_to_box(EgWidget *widget);

/**
 * Converte EgWidget para EgGrid (retorna NULL se tipo incorreto).
 */
EgGrid *eg_widget_cast_to_grid(EgWidget *widget);

/**
 * Converte EgWidget para EgCheckButton (retorna NULL se tipo incorreto).
 */
EgCheckButton *eg_widget_cast_to_check_button(EgWidget *widget);

/**
 * Converte EgWidget para EgSwitch (retorna NULL se tipo incorreto).
 */
EgSwitch *eg_widget_cast_to_switch(EgWidget *widget);

/**
 * Converte EgWidget para EgProgressBar (retorna NULL se tipo incorreto).
 */
EgProgressBar *eg_widget_cast_to_progress_bar(EgWidget *widget);

/**
 * Converte EgWidget para EgComboBox (retorna NULL se tipo incorreto).
 */
EgComboBox *eg_widget_cast_to_combo_box(EgWidget *widget);

/**
 * Converte EgWidget para EgScrolledWindow (retorna NULL se tipo incorreto).
 */
EgScrolledWindow *eg_widget_cast_to_scrolled_window(EgWidget *widget);

/**
 * Verifica se o widget é do tipo especificado.
 * 
 * @param widget Widget a verificar
 * @param type Tipo esperado
 * @return true se o widget é do tipo especificado
 */
bool eg_widget_is_type(EgWidget *widget, EgWidgetType type);

/**
 * Verifica se o widget é um container (Box, Grid, ScrolledWindow).
 * 
 * @param widget Widget a verificar
 * @return true se é um container
 */
bool eg_widget_is_container(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_CAST_H */
