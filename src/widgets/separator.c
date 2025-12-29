/**
 * EasyGTK - Separator Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/separator.h>

/* Funções da vtable */
static void separator_destroy(EgWidget *widget);
static void *separator_get_native(EgWidget *widget);
static void separator_set_visible(EgWidget *widget, bool visible);
static bool separator_get_visible(EgWidget *widget);
static void separator_set_sensitive(EgWidget *widget, bool sensitive);
static bool separator_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_separator_vtable = {
    .type = EG_WIDGET_TYPE_SEPARATOR,
    .type_name = "EgSeparator",
    .destroy = separator_destroy,
    .get_native = separator_get_native,
    .set_visible = separator_set_visible,
    .get_visible = separator_get_visible,
    .set_sensitive = separator_set_sensitive,
    .get_sensitive = separator_get_sensitive
};

static void separator_destroy(EgWidget *widget) {
    EgSeparator *separator = (EgSeparator *)widget;
    if (separator == NULL) return;
    eg_free(separator);
}

static void *separator_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void separator_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool separator_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void separator_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool separator_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Converte EgOrientation para GtkOrientation */
static GtkOrientation eg_orientation_to_gtk(EgOrientation orientation) {
    return orientation == EG_ORIENTATION_VERTICAL ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
}

EgSeparator *eg_separator_new(EgOrientation orientation) {
    EgSeparator *separator = EG_ALLOC(EgSeparator);
    if (separator == NULL) return NULL;
    
    GtkWidget *gtk_separator = gtk_separator_new(eg_orientation_to_gtk(orientation));
    if (gtk_separator == NULL) {
        eg_free(separator);
        return NULL;
    }
    
    eg_widget_init(&separator->base, EG_WIDGET_TYPE_SEPARATOR, gtk_separator, &eg_separator_vtable);
    
    return separator;
}

EgSeparator *eg_separator_new_horizontal(void) {
    return eg_separator_new(EG_ORIENTATION_HORIZONTAL);
}

EgSeparator *eg_separator_new_vertical(void) {
    return eg_separator_new(EG_ORIENTATION_VERTICAL);
}

void eg_separator_free(EgSeparator *separator) {
    separator_destroy((EgWidget *)separator);
}

EgWidget *eg_separator_as_widget(EgSeparator *separator) {
    return (EgWidget *)separator;
}

void *eg_separator_get_native(EgSeparator *separator) {
    if (separator == NULL) return NULL;
    return separator->base.native;
}
