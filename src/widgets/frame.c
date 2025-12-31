/**
 * EasyGTK - Frame
 *
 * Container com borda e label opcional, suporta 1 filho.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/frame.h>

/* ============================================
 * Frame Container VTable
 * ============================================ */

static void frame_destroy(EgWidget *widget);
static void frame_add_child(EgWidget *container, EgWidget *child);
static void frame_remove_child(EgWidget *container, EgWidget *child);
static size_t frame_get_child_count(EgWidget *container);
static EgWidget *frame_get_child_at(EgWidget *container, size_t index);
static void frame_clear(EgWidget *container);

static const EgContainerCapabilities frame_caps = {
    .supports_multiple = false,
    .supports_named = false,
    .supports_positioned = false,
    .max_children = 1
};

const EgContainerVTable eg_frame_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_FRAME,
        .type_name = "EgFrame",
        .destroy = frame_destroy,
        .get_native = eg_widget_get_native_default,
        .set_visible = eg_widget_set_visible_default,
        .get_visible = eg_widget_get_visible_default,
        .set_sensitive = eg_widget_set_sensitive_default,
        .get_sensitive = eg_widget_get_sensitive_default,
        .binding_caps = NULL,
        .bind_value = NULL,
        .bind_command = NULL,
        .unbind = NULL,
        .events = NULL,
        .validation = NULL
    },
    .caps = &frame_caps,
    .add_child = frame_add_child,
    .remove_child = frame_remove_child,
    .add_named = NULL,
    .get_by_name = NULL,
    .get_child_count = frame_get_child_count,
    .get_child_at = frame_get_child_at,
    .clear = frame_clear
};

/* ============================================
 * VTable Implementations
 * ============================================ */

static void frame_destroy(EgWidget *widget) {
    EgFrame *frame = (EgFrame *)widget;
    if (frame == NULL) return;

    /* Destrói o filho se existir */
    if (frame->child != NULL && frame->child->vtable != NULL &&
        frame->child->vtable->destroy != NULL) {
        frame->child->vtable->destroy(frame->child);
    }

    eg_free(frame);
}

static void frame_add_child(EgWidget *container, EgWidget *child) {
    EgFrame *frame = (EgFrame *)container;
    if (frame == NULL || frame->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_frame_set_child(GTK_FRAME(frame->base.native), child->native);
    frame->child = child;
}

static void frame_remove_child(EgWidget *container, EgWidget *child) {
    EgFrame *frame = (EgFrame *)container;
    if (frame == NULL || frame->base.native == NULL) return;
    if (frame->child != child) return;

    gtk_frame_set_child(GTK_FRAME(frame->base.native), NULL);
    frame->child = NULL;
}

static size_t frame_get_child_count(EgWidget *container) {
    EgFrame *frame = (EgFrame *)container;
    if (frame == NULL) return 0;
    return frame->child != NULL ? 1 : 0;
}

static EgWidget *frame_get_child_at(EgWidget *container, size_t index) {
    EgFrame *frame = (EgFrame *)container;
    if (frame == NULL || index != 0) return NULL;
    return frame->child;
}

static void frame_clear(EgWidget *container) {
    EgFrame *frame = (EgFrame *)container;
    if (frame == NULL || frame->base.native == NULL) return;

    gtk_frame_set_child(GTK_FRAME(frame->base.native), NULL);
    frame->child = NULL;
}

/* ============================================
 * API Pública
 * ============================================ */

EgFrame *eg_frame_new(const char *label) {
    EgFrame *frame = EG_ALLOC(EgFrame);
    if (frame == NULL) return NULL;

    GtkWidget *gtk_frame = gtk_frame_new(label);
    if (gtk_frame == NULL) {
        eg_free(frame);
        return NULL;
    }

    eg_widget_init(&frame->base, EG_WIDGET_TYPE_FRAME, gtk_frame, &eg_frame_vtable.base);
    frame->child = NULL;

    return frame;
}

void eg_frame_free(EgFrame *frame) {
    frame_destroy((EgWidget *)frame);
}

void eg_frame_set_child(EgFrame *frame, EgWidget *child) {
    frame_add_child((EgWidget *)frame, child);
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
