/**
 * EasyGTK - Reference Counting
 */

#include "internal/internal.h"
#include <easygtk/refcount.h>

EgWidget *eg_widget_ref(EgWidget *widget) {
    if (widget == NULL) return NULL;
    widget->ref_count++;
    return widget;
}

void eg_widget_unref(EgWidget *widget) {
    if (widget == NULL) return;
    
    if (widget->ref_count > 0) {
        widget->ref_count--;
    }
    
    if (widget->ref_count == 0 && !widget->is_floating) {
        eg_widget_destroy_internal(widget);
    }
}

uint32_t eg_widget_get_ref_count(EgWidget *widget) {
    if (widget == NULL) return 0;
    return widget->ref_count;
}

void eg_widget_set_floating(EgWidget *widget, bool floating) {
    if (widget == NULL) return;
    widget->is_floating = floating;
}

bool eg_widget_is_floating(EgWidget *widget) {
    if (widget == NULL) return false;
    return widget->is_floating;
}

EgWidget *eg_widget_ref_sink(EgWidget *widget) {
    if (widget == NULL) return NULL;
    
    if (widget->is_floating) {
        widget->is_floating = false;
    } else {
        widget->ref_count++;
    }
    
    return widget;
}
