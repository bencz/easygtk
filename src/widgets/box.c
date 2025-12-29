/**
 * EasyGTK - Box
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* Funções da vtable */
static void box_destroy(EgWidget *widget);
static void *box_get_native(EgWidget *widget);
static void box_set_visible(EgWidget *widget, bool visible);
static bool box_get_visible(EgWidget *widget);
static void box_set_sensitive(EgWidget *widget, bool sensitive);
static bool box_get_sensitive(EgWidget *widget);
static void box_add_child(EgWidget *container, EgWidget *child);
static void box_remove_child(EgWidget *container, EgWidget *child);

const EgContainerVTable eg_box_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_BOX,
        .type_name = "EgBox",
        .destroy = box_destroy,
        .get_native = box_get_native,
        .set_visible = box_set_visible,
        .get_visible = box_get_visible,
        .set_sensitive = box_set_sensitive,
        .get_sensitive = box_get_sensitive
    },
    .add_child = box_add_child,
    .remove_child = box_remove_child
};

static void box_destroy(EgWidget *widget) {
    EgBox *box = (EgBox *)widget;
    if (box == NULL) return;
    eg_free(box);
}

static void *box_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void box_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool box_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void box_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool box_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

static void box_add_child(EgWidget *container, EgWidget *child) {
    if (container == NULL || container->native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_box_append(GTK_BOX(container->native), child->native);
}

static void box_remove_child(EgWidget *container, EgWidget *child) {
    if (container == NULL || container->native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_box_remove(GTK_BOX(container->native), child->native);
}

EgBox *eg_box_new(EgOrientation orientation, int spacing) {
    EgBox *box = EG_ALLOC(EgBox);
    if (box == NULL) return NULL;
    
    GtkOrientation gtk_orient = (orientation == EG_ORIENTATION_VERTICAL) 
        ? GTK_ORIENTATION_VERTICAL 
        : GTK_ORIENTATION_HORIZONTAL;
    
    GtkWidget *gtk_box = gtk_box_new(gtk_orient, spacing);
    if (gtk_box == NULL) {
        eg_free(box);
        return NULL;
    }
    
    eg_widget_init(&box->base, EG_WIDGET_TYPE_BOX, gtk_box, &eg_box_vtable.base);
    
    return box;
}

EgBox *eg_box_new_vertical(int spacing) {
    return eg_box_new(EG_ORIENTATION_VERTICAL, spacing);
}

EgBox *eg_box_new_horizontal(int spacing) {
    return eg_box_new(EG_ORIENTATION_HORIZONTAL, spacing);
}

void eg_box_free(EgBox *box) {
    box_destroy((EgWidget *)box);
}

void eg_box_append(EgBox *box, EgWidget *child) {
    if (box == NULL || box->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_box_append(GTK_BOX(box->base.native), child->native);
}

void eg_box_prepend(EgBox *box, EgWidget *child) {
    if (box == NULL || box->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_box_prepend(GTK_BOX(box->base.native), child->native);
}

void eg_box_remove(EgBox *box, EgWidget *child) {
    if (box == NULL || box->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_box_remove(GTK_BOX(box->base.native), child->native);
}

void eg_box_set_spacing(EgBox *box, int spacing) {
    if (box == NULL || box->base.native == NULL) return;
    gtk_box_set_spacing(GTK_BOX(box->base.native), spacing);
}

int eg_box_get_spacing(EgBox *box) {
    if (box == NULL || box->base.native == NULL) return 0;
    return gtk_box_get_spacing(GTK_BOX(box->base.native));
}

void eg_box_set_homogeneous(EgBox *box, bool homogeneous) {
    if (box == NULL || box->base.native == NULL) return;
    gtk_box_set_homogeneous(GTK_BOX(box->base.native), homogeneous);
}

EgWidget *eg_box_as_widget(EgBox *box) {
    return (EgWidget *)box;
}

void *eg_box_get_native(EgBox *box) {
    if (box == NULL) return NULL;
    return box->base.native;
}
