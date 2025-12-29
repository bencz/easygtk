/**
 * EasyGTK - Label
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* Funções da vtable */
static void label_destroy(EgWidget *widget);
static void *label_get_native(EgWidget *widget);
static void label_set_visible(EgWidget *widget, bool visible);
static bool label_get_visible(EgWidget *widget);
static void label_set_sensitive(EgWidget *widget, bool sensitive);
static bool label_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_label_vtable = {
    .type = EG_WIDGET_TYPE_LABEL,
    .type_name = "EgLabel",
    .destroy = label_destroy,
    .get_native = label_get_native,
    .set_visible = label_set_visible,
    .get_visible = label_get_visible,
    .set_sensitive = label_set_sensitive,
    .get_sensitive = label_get_sensitive
};

static void label_destroy(EgWidget *widget) {
    EgLabel *label = (EgLabel *)widget;
    if (label == NULL) return;
    eg_free(label);
}

static void *label_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void label_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool label_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void label_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool label_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgLabel *eg_label_new(const char *text) {
    EgLabel *label = EG_ALLOC(EgLabel);
    if (label == NULL) return NULL;
    
    GtkWidget *gtk_label = gtk_label_new(text);
    if (gtk_label == NULL) {
        eg_free(label);
        return NULL;
    }
    
    eg_widget_init(&label->base, EG_WIDGET_TYPE_LABEL, gtk_label, &eg_label_vtable);
    
    return label;
}

void eg_label_free(EgLabel *label) {
    label_destroy((EgWidget *)label);
}

void eg_label_set_text(EgLabel *label, const char *text) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_text(GTK_LABEL(label->base.native), text);
}

const char *eg_label_get_text(EgLabel *label) {
    if (label == NULL || label->base.native == NULL) return NULL;
    return gtk_label_get_text(GTK_LABEL(label->base.native));
}

void eg_label_set_markup(EgLabel *label, const char *markup) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_markup(GTK_LABEL(label->base.native), markup);
}

void eg_label_set_halign(EgLabel *label, EgAlign align) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_widget_set_halign(label->base.native, eg_align_to_gtk(align));
}

void eg_label_set_valign(EgLabel *label, EgAlign align) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_widget_set_valign(label->base.native, eg_align_to_gtk(align));
}

void eg_label_set_selectable(EgLabel *label, bool selectable) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_selectable(GTK_LABEL(label->base.native), selectable);
}

void eg_label_set_wrap(EgLabel *label, bool wrap) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_wrap(GTK_LABEL(label->base.native), wrap);
}

EgWidget *eg_label_as_widget(EgLabel *label) {
    return (EgWidget *)label;
}

void *eg_label_get_native(EgLabel *label) {
    if (label == NULL) return NULL;
    return label->base.native;
}
