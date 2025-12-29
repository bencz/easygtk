/**
 * EasyGTK - Frame
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/frame.h>

/* Funções da vtable */
static void frame_destroy(EgWidget *widget);
static void *frame_get_native(EgWidget *widget);
static void frame_set_visible(EgWidget *widget, bool visible);
static bool frame_get_visible(EgWidget *widget);
static void frame_set_sensitive(EgWidget *widget, bool sensitive);
static bool frame_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_frame_vtable = {
    .type = EG_WIDGET_TYPE_FRAME,
    .type_name = "EgFrame",
    .destroy = frame_destroy,
    .get_native = frame_get_native,
    .set_visible = frame_set_visible,
    .get_visible = frame_get_visible,
    .set_sensitive = frame_set_sensitive,
    .get_sensitive = frame_get_sensitive
};

static void frame_destroy(EgWidget *widget) {
    EgFrame *frame = (EgFrame *)widget;
    if (frame == NULL) return;
    eg_free(frame);
}

static void *frame_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void frame_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool frame_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void frame_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool frame_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgFrame *eg_frame_new(const char *label) {
    EgFrame *frame = EG_ALLOC(EgFrame);
    if (frame == NULL) return NULL;
    
    GtkWidget *gtk_frame = gtk_frame_new(label);
    if (gtk_frame == NULL) {
        eg_free(frame);
        return NULL;
    }
    
    eg_widget_init(&frame->base, EG_WIDGET_TYPE_FRAME, gtk_frame, &eg_frame_vtable);
    return frame;
}

void eg_frame_free(EgFrame *frame) {
    frame_destroy((EgWidget *)frame);
}

void eg_frame_set_child(EgFrame *frame, EgWidget *child) {
    if (frame == NULL || frame->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_frame_set_child(GTK_FRAME(frame->base.native), gtk_child);
}

void eg_frame_set_label(EgFrame *frame, const char *label) {
    if (frame == NULL || frame->base.native == NULL) return;
    gtk_frame_set_label(GTK_FRAME(frame->base.native), label);
}

const char *eg_frame_get_label(EgFrame *frame) {
    if (frame == NULL || frame->base.native == NULL) return NULL;
    return gtk_frame_get_label(GTK_FRAME(frame->base.native));
}

void eg_frame_set_label_align(EgFrame *frame, float xalign) {
    if (frame == NULL || frame->base.native == NULL) return;
    gtk_frame_set_label_align(GTK_FRAME(frame->base.native), xalign);
}

EgWidget *eg_frame_as_widget(EgFrame *frame) {
    return (EgWidget *)frame;
}

void *eg_frame_get_native(EgFrame *frame) {
    if (frame == NULL) return NULL;
    return frame->base.native;
}
