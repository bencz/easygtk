/**
 * EasyGTK - Funções base de widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* VTable base padrão */
static void widget_destroy_default(EgWidget *widget);
static void *widget_get_native_default(EgWidget *widget);
static void widget_set_visible_default(EgWidget *widget, bool visible);
static bool widget_get_visible_default(EgWidget *widget);
static void widget_set_sensitive_default(EgWidget *widget, bool sensitive);
static bool widget_get_sensitive_default(EgWidget *widget);

const EgWidgetVTable eg_widget_vtable = {
    .type = EG_WIDGET_TYPE_BASE,
    .type_name = "EgWidget",
    .destroy = widget_destroy_default,
    .get_native = widget_get_native_default,
    .set_visible = widget_set_visible_default,
    .get_visible = widget_get_visible_default,
    .set_sensitive = widget_set_sensitive_default,
    .get_sensitive = widget_get_sensitive_default
};

static void widget_destroy_default(EgWidget *widget) {
    if (widget == NULL) return;
    
    if (widget->user_data_destroy != NULL && widget->user_data != NULL) {
        widget->user_data_destroy(widget->user_data);
    }
    
    eg_free(widget);
}

static void *widget_get_native_default(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void widget_set_visible_default(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool widget_get_visible_default(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void widget_set_sensitive_default(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool widget_get_sensitive_default(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Inicialização de widget */
void eg_widget_init(EgWidget *widget, EgWidgetType type, GtkWidget *native, const EgWidgetVTable *vtable) {
    if (widget == NULL) return;
    
    widget->type = type;
    widget->native = native;
    widget->vtable = vtable ? vtable : &eg_widget_vtable;
    widget->user_data = NULL;
    widget->user_data_destroy = NULL;
}

void eg_widget_destroy_internal(EgWidget *widget) {
    if (widget == NULL) return;
    
    if (widget->vtable != NULL && widget->vtable->destroy != NULL) {
        widget->vtable->destroy(widget);
    } else {
        widget_destroy_default(widget);
    }
}

/* Conversão de alinhamento */
GtkAlign eg_align_to_gtk(EgAlign align) {
    switch (align) {
        case EG_ALIGN_FILL:   return GTK_ALIGN_FILL;
        case EG_ALIGN_START:  return GTK_ALIGN_START;
        case EG_ALIGN_END:    return GTK_ALIGN_END;
        case EG_ALIGN_CENTER: return GTK_ALIGN_CENTER;
        default:              return GTK_ALIGN_FILL;
    }
}

EgAlign gtk_align_to_eg(GtkAlign align) {
    switch (align) {
        case GTK_ALIGN_FILL:     return EG_ALIGN_FILL;
        case GTK_ALIGN_START:    return EG_ALIGN_START;
        case GTK_ALIGN_END:      return EG_ALIGN_END;
        case GTK_ALIGN_CENTER:   return EG_ALIGN_CENTER;
        case GTK_ALIGN_BASELINE_FILL: return EG_ALIGN_FILL;
        default:                 return EG_ALIGN_FILL;
    }
}

/* API pública de widget */
const char *eg_version(void) {
    return "1.0.0";
}

EgWidgetType eg_widget_get_type(EgWidget *widget) {
    if (widget == NULL) return EG_WIDGET_TYPE_BASE;
    return widget->type;
}

void eg_widget_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->vtable == NULL) return;
    widget->vtable->set_visible(widget, visible);
}

bool eg_widget_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return false;
    return widget->vtable->get_visible(widget);
}

void eg_widget_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->vtable == NULL) return;
    widget->vtable->set_sensitive(widget, sensitive);
}

bool eg_widget_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return false;
    return widget->vtable->get_sensitive(widget);
}

void eg_widget_set_margins(EgWidget *widget, EgMargins margins) {
    if (widget == NULL || widget->native == NULL) return;
    
    gtk_widget_set_margin_top(widget->native, margins.top);
    gtk_widget_set_margin_bottom(widget->native, margins.bottom);
    gtk_widget_set_margin_start(widget->native, margins.start);
    gtk_widget_set_margin_end(widget->native, margins.end);
}

void eg_widget_set_margin(EgWidget *widget, int margin) {
    EgMargins margins = { margin, margin, margin, margin };
    eg_widget_set_margins(widget, margins);
}

void eg_widget_set_halign(EgWidget *widget, EgAlign align) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_halign(widget->native, eg_align_to_gtk(align));
}

void eg_widget_set_valign(EgWidget *widget, EgAlign align) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_valign(widget->native, eg_align_to_gtk(align));
}

void eg_widget_set_hexpand(EgWidget *widget, bool expand) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_hexpand(widget->native, expand);
}

void eg_widget_set_vexpand(EgWidget *widget, bool expand) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_vexpand(widget->native, expand);
}

void eg_widget_set_size_request(EgWidget *widget, int width, int height) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_size_request(widget->native, width, height);
}

void eg_widget_add_css_class(EgWidget *widget, const char *class_name) {
    if (widget == NULL || widget->native == NULL || class_name == NULL) return;
    gtk_widget_add_css_class(widget->native, class_name);
}

void eg_widget_remove_css_class(EgWidget *widget, const char *class_name) {
    if (widget == NULL || widget->native == NULL || class_name == NULL) return;
    gtk_widget_remove_css_class(widget->native, class_name);
}

void eg_widget_set_tooltip(EgWidget *widget, const char *tooltip) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_tooltip_text(widget->native, tooltip);
}

void eg_widget_set_tooltip_markup(EgWidget *widget, const char *markup) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_tooltip_markup(widget->native, markup);
}

void *eg_widget_get_native(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return NULL;
    return widget->vtable->get_native(widget);
}

void eg_widget_free(EgWidget *widget) {
    eg_widget_destroy_internal(widget);
}
