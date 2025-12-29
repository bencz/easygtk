/**
 * EasyGTK - Paned (Split View)
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/paned.h>

/* Funções da vtable */
static void paned_destroy(EgWidget *widget);
static void *paned_get_native(EgWidget *widget);
static void paned_set_visible(EgWidget *widget, bool visible);
static bool paned_get_visible(EgWidget *widget);
static void paned_set_sensitive(EgWidget *widget, bool sensitive);
static bool paned_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_paned_vtable = {
    .type = EG_WIDGET_TYPE_PANED,
    .type_name = "EgPaned",
    .destroy = paned_destroy,
    .get_native = paned_get_native,
    .set_visible = paned_set_visible,
    .get_visible = paned_get_visible,
    .set_sensitive = paned_set_sensitive,
    .get_sensitive = paned_get_sensitive
};

static void paned_destroy(EgWidget *widget) {
    EgPaned *paned = (EgPaned *)widget;
    if (paned == NULL) return;
    eg_free(paned);
}

static void *paned_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void paned_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool paned_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void paned_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool paned_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgPaned *eg_paned_new(EgOrientation orientation) {
    EgPaned *paned = EG_ALLOC(EgPaned);
    if (paned == NULL) return NULL;
    
    GtkOrientation gtk_orient = (orientation == EG_ORIENTATION_VERTICAL) 
        ? GTK_ORIENTATION_VERTICAL 
        : GTK_ORIENTATION_HORIZONTAL;
    
    GtkWidget *gtk_paned = gtk_paned_new(gtk_orient);
    if (gtk_paned == NULL) {
        eg_free(paned);
        return NULL;
    }
    
    eg_widget_init(&paned->base, EG_WIDGET_TYPE_PANED, gtk_paned, &eg_paned_vtable);
    return paned;
}

EgPaned *eg_paned_new_horizontal(void) {
    return eg_paned_new(EG_ORIENTATION_HORIZONTAL);
}

EgPaned *eg_paned_new_vertical(void) {
    return eg_paned_new(EG_ORIENTATION_VERTICAL);
}

void eg_paned_free(EgPaned *paned) {
    paned_destroy((EgWidget *)paned);
}

void eg_paned_set_start_child(EgPaned *paned, EgWidget *child) {
    if (paned == NULL || paned->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_paned_set_start_child(GTK_PANED(paned->base.native), gtk_child);
}

void eg_paned_set_end_child(EgPaned *paned, EgWidget *child) {
    if (paned == NULL || paned->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_paned_set_end_child(GTK_PANED(paned->base.native), gtk_child);
}

void eg_paned_set_position(EgPaned *paned, int position) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_position(GTK_PANED(paned->base.native), position);
}

int eg_paned_get_position(EgPaned *paned) {
    if (paned == NULL || paned->base.native == NULL) return 0;
    return gtk_paned_get_position(GTK_PANED(paned->base.native));
}

void eg_paned_set_start_child_resize(EgPaned *paned, bool resize) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_resize_start_child(GTK_PANED(paned->base.native), resize);
}

void eg_paned_set_end_child_resize(EgPaned *paned, bool resize) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_resize_end_child(GTK_PANED(paned->base.native), resize);
}

void eg_paned_set_start_child_shrink(EgPaned *paned, bool shrink) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_shrink_start_child(GTK_PANED(paned->base.native), shrink);
}

void eg_paned_set_end_child_shrink(EgPaned *paned, bool shrink) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_shrink_end_child(GTK_PANED(paned->base.native), shrink);
}

void eg_paned_set_wide_handle(EgPaned *paned, bool wide) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_wide_handle(GTK_PANED(paned->base.native), wide);
}

EgWidget *eg_paned_as_widget(EgPaned *paned) {
    return (EgWidget *)paned;
}

void *eg_paned_get_native(EgPaned *paned) {
    if (paned == NULL) return NULL;
    return paned->base.native;
}
