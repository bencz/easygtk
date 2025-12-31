/**
 * EasyGTK - Sistema de Eventos
 *
 * API pública de eventos. Usa as funções default de event_helpers.c
 * ou vtable customizada do widget quando disponível.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/events.h>

/* ============================================
 * Helpers para obter vtable de eventos
 * ============================================ */

static const EgEventVTable *get_event_vtable(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return &eg_event_vtable_default;
    if (widget->vtable->events != NULL) return widget->vtable->events;
    return &eg_event_vtable_default;
}

/* ============================================
 * Implementação das funções públicas
 * ============================================ */

void eg_widget_on_key_press(EgWidget *widget, EgKeyCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_key != NULL) {
        vtable->connect_key(widget, callback, user_data, true);
    }
}

void eg_widget_on_key_release(EgWidget *widget, EgKeyCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_key != NULL) {
        vtable->connect_key(widget, callback, user_data, false);
    }
}

void eg_widget_on_motion(EgWidget *widget, EgMotionCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_motion != NULL) {
        vtable->connect_motion(widget, callback, user_data);
    }
}

void eg_widget_on_enter(EgWidget *widget, EgCrossingCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_crossing != NULL) {
        vtable->connect_crossing(widget, callback, user_data, true);
    }
}

void eg_widget_on_leave(EgWidget *widget, EgCrossingCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_crossing != NULL) {
        vtable->connect_crossing(widget, callback, user_data, false);
    }
}

void eg_widget_on_scroll(EgWidget *widget, EgScrollCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_scroll != NULL) {
        vtable->connect_scroll(widget, callback, user_data);
    }
}

void eg_widget_on_click(EgWidget *widget, EgButtonCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_click != NULL) {
        vtable->connect_click(widget, callback, user_data);
    }
}

void eg_widget_on_focus_in(EgWidget *widget, EgFocusCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_focus != NULL) {
        vtable->connect_focus(widget, callback, user_data, true);
    }
}

void eg_widget_on_focus_out(EgWidget *widget, EgFocusCallback callback, void *user_data) {
    const EgEventVTable *vtable = get_event_vtable(widget);
    if (vtable->connect_focus != NULL) {
        vtable->connect_focus(widget, callback, user_data, false);
    }
}

/* ============================================
 * Funções de foco (não usam vtable)
 * ============================================ */

void eg_widget_set_focusable(EgWidget *widget, bool focusable) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_focusable(widget->native, focusable);
}

bool eg_widget_get_focusable(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_focusable(widget->native);
}

bool eg_widget_grab_focus(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_grab_focus(widget->native);
}

bool eg_widget_has_focus(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_has_focus(widget->native);
}
