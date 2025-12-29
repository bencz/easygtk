/**
 * EasyGTK - Type Casting Helpers
 * 
 * Usa a vtable para verificação de tipo, evitando switches.
 */

#include "internal/internal.h"
#include <easygtk/cast.h>

/* Função genérica de cast usando vtable */
static void *eg_widget_cast_internal(EgWidget *widget, EgWidgetType expected_type) {
    if (widget == NULL) return NULL;
    /* Usa vtable->type que é definido em cada widget */
    if (widget->vtable != NULL && widget->vtable->type == expected_type) {
        return widget;
    }
    return NULL;
}

EgWindow *eg_widget_cast_to_window(EgWidget *widget) {
    return (EgWindow *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_WINDOW);
}

EgButton *eg_widget_cast_to_button(EgWidget *widget) {
    return (EgButton *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_BUTTON);
}

EgLabel *eg_widget_cast_to_label(EgWidget *widget) {
    return (EgLabel *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_LABEL);
}

EgEntry *eg_widget_cast_to_entry(EgWidget *widget) {
    return (EgEntry *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_ENTRY);
}

EgBox *eg_widget_cast_to_box(EgWidget *widget) {
    return (EgBox *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_BOX);
}

EgGrid *eg_widget_cast_to_grid(EgWidget *widget) {
    return (EgGrid *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_GRID);
}

EgCheckButton *eg_widget_cast_to_check_button(EgWidget *widget) {
    return (EgCheckButton *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_CHECK_BUTTON);
}

EgSwitch *eg_widget_cast_to_switch(EgWidget *widget) {
    return (EgSwitch *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_SWITCH);
}

EgProgressBar *eg_widget_cast_to_progress_bar(EgWidget *widget) {
    return (EgProgressBar *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_PROGRESS_BAR);
}

EgComboBox *eg_widget_cast_to_combo_box(EgWidget *widget) {
    return (EgComboBox *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_COMBO_BOX);
}

EgScrolledWindow *eg_widget_cast_to_scrolled_window(EgWidget *widget) {
    return (EgScrolledWindow *)eg_widget_cast_internal(widget, EG_WIDGET_TYPE_SCROLLED_WINDOW);
}

bool eg_widget_is_type(EgWidget *widget, EgWidgetType type) {
    if (widget == NULL || widget->vtable == NULL) return false;
    return widget->vtable->type == type;
}

bool eg_widget_is_container(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return false;
    EgWidgetType type = widget->vtable->type;
    return type == EG_WIDGET_TYPE_BOX ||
           type == EG_WIDGET_TYPE_GRID ||
           type == EG_WIDGET_TYPE_SCROLLED_WINDOW;
}
