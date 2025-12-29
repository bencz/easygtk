/**
 * EasyGTK - Expander
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/expander.h>

/* Funções da vtable */
static void expander_destroy(EgWidget *widget);
static void *expander_get_native(EgWidget *widget);
static void expander_set_visible(EgWidget *widget, bool visible);
static bool expander_get_visible(EgWidget *widget);
static void expander_set_sensitive(EgWidget *widget, bool sensitive);
static bool expander_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_expander_vtable = {
    .type = EG_WIDGET_TYPE_EXPANDER,
    .type_name = "EgExpander",
    .destroy = expander_destroy,
    .get_native = expander_get_native,
    .set_visible = expander_set_visible,
    .get_visible = expander_get_visible,
    .set_sensitive = expander_set_sensitive,
    .get_sensitive = expander_get_sensitive
};

static void expander_destroy(EgWidget *widget) {
    EgExpander *expander = (EgExpander *)widget;
    if (expander == NULL) return;
    eg_free(expander);
}

static void *expander_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void expander_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool expander_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void expander_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool expander_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgExpander *eg_expander_new(const char *label) {
    EgExpander *expander = EG_ALLOC(EgExpander);
    if (expander == NULL) return NULL;
    
    GtkWidget *gtk_expander = gtk_expander_new(label);
    if (gtk_expander == NULL) {
        eg_free(expander);
        return NULL;
    }
    
    eg_widget_init(&expander->base, EG_WIDGET_TYPE_EXPANDER, gtk_expander, &eg_expander_vtable);
    return expander;
}

EgExpander *eg_expander_new_with_markup(const char *markup) {
    EgExpander *expander = EG_ALLOC(EgExpander);
    if (expander == NULL) return NULL;
    
    GtkWidget *gtk_expander = gtk_expander_new(markup);
    if (gtk_expander == NULL) {
        eg_free(expander);
        return NULL;
    }
    
    gtk_expander_set_use_markup(GTK_EXPANDER(gtk_expander), TRUE);
    
    eg_widget_init(&expander->base, EG_WIDGET_TYPE_EXPANDER, gtk_expander, &eg_expander_vtable);
    return expander;
}

void eg_expander_free(EgExpander *expander) {
    expander_destroy((EgWidget *)expander);
}

void eg_expander_set_child(EgExpander *expander, EgWidget *child) {
    if (expander == NULL || expander->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_expander_set_child(GTK_EXPANDER(expander->base.native), gtk_child);
}

void eg_expander_set_expanded(EgExpander *expander, bool expanded) {
    if (expander == NULL || expander->base.native == NULL) return;
    gtk_expander_set_expanded(GTK_EXPANDER(expander->base.native), expanded);
}

bool eg_expander_get_expanded(EgExpander *expander) {
    if (expander == NULL || expander->base.native == NULL) return false;
    return gtk_expander_get_expanded(GTK_EXPANDER(expander->base.native));
}

void eg_expander_set_label(EgExpander *expander, const char *label) {
    if (expander == NULL || expander->base.native == NULL) return;
    gtk_expander_set_label(GTK_EXPANDER(expander->base.native), label);
}

const char *eg_expander_get_label(EgExpander *expander) {
    if (expander == NULL || expander->base.native == NULL) return NULL;
    return gtk_expander_get_label(GTK_EXPANDER(expander->base.native));
}

void eg_expander_set_use_markup(EgExpander *expander, bool use_markup) {
    if (expander == NULL || expander->base.native == NULL) return;
    gtk_expander_set_use_markup(GTK_EXPANDER(expander->base.native), use_markup);
}

void eg_expander_set_use_underline(EgExpander *expander, bool use_underline) {
    if (expander == NULL || expander->base.native == NULL) return;
    gtk_expander_set_use_underline(GTK_EXPANDER(expander->base.native), use_underline);
}

void eg_expander_set_resize_toplevel(EgExpander *expander, bool resize) {
    if (expander == NULL || expander->base.native == NULL) return;
    gtk_expander_set_resize_toplevel(GTK_EXPANDER(expander->base.native), resize);
}

EgWidget *eg_expander_as_widget(EgExpander *expander) {
    return (EgWidget *)expander;
}

void *eg_expander_get_native(EgExpander *expander) {
    if (expander == NULL) return NULL;
    return expander->base.native;
}
