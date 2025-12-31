/**
 * EasyGTK - ScrolledWindow
 *
 * Container com scrollbars, suporta 1 filho.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/scrolled_window.h>

/* ============================================
 * ScrolledWindow Container VTable
 * ============================================ */

static void scrolled_window_destroy(EgWidget *widget);
static void scrolled_window_add_child(EgWidget *container, EgWidget *child);
static void scrolled_window_remove_child(EgWidget *container, EgWidget *child);
static size_t scrolled_window_get_child_count(EgWidget *container);
static EgWidget *scrolled_window_get_child_at(EgWidget *container, size_t index);
static void scrolled_window_clear(EgWidget *container);

static const EgContainerCapabilities scrolled_window_caps = {
    .supports_multiple = false,
    .supports_named = false,
    .supports_positioned = false,
    .max_children = 1
};

const EgContainerVTable eg_scrolled_window_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_SCROLLED_WINDOW,
        .type_name = "EgScrolledWindow",
        .destroy = scrolled_window_destroy,
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
    .caps = &scrolled_window_caps,
    .add_child = scrolled_window_add_child,
    .remove_child = scrolled_window_remove_child,
    .add_named = NULL,
    .get_by_name = NULL,
    .get_child_count = scrolled_window_get_child_count,
    .get_child_at = scrolled_window_get_child_at,
    .clear = scrolled_window_clear
};

/* ============================================
 * VTable Implementations
 * ============================================ */

static void scrolled_window_destroy(EgWidget *widget) {
    EgScrolledWindow *sw = (EgScrolledWindow *)widget;
    if (sw == NULL) return;

    /* Destrói o filho se existir */
    if (sw->child != NULL && sw->child->vtable != NULL &&
        sw->child->vtable->destroy != NULL) {
        sw->child->vtable->destroy(sw->child);
    }

    eg_free(sw);
}

static void scrolled_window_add_child(EgWidget *container, EgWidget *child) {
    EgScrolledWindow *sw = (EgScrolledWindow *)container;
    if (sw == NULL || sw->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw->base.native), child->native);
    sw->child = child;
}

static void scrolled_window_remove_child(EgWidget *container, EgWidget *child) {
    EgScrolledWindow *sw = (EgScrolledWindow *)container;
    if (sw == NULL || sw->base.native == NULL) return;
    if (sw->child != child) return;

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw->base.native), NULL);
    sw->child = NULL;
}

static size_t scrolled_window_get_child_count(EgWidget *container) {
    EgScrolledWindow *sw = (EgScrolledWindow *)container;
    if (sw == NULL) return 0;
    return sw->child != NULL ? 1 : 0;
}

static EgWidget *scrolled_window_get_child_at(EgWidget *container, size_t index) {
    EgScrolledWindow *sw = (EgScrolledWindow *)container;
    if (sw == NULL || index != 0) return NULL;
    return sw->child;
}

static void scrolled_window_clear(EgWidget *container) {
    EgScrolledWindow *sw = (EgScrolledWindow *)container;
    if (sw == NULL || sw->base.native == NULL) return;

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(sw->base.native), NULL);
    sw->child = NULL;
}

/* ============================================
 * Conversão de Policy
 * ============================================ */

static GtkPolicyType eg_policy_to_gtk(EgScrollPolicy policy) {
    switch (policy) {
        case EG_SCROLL_AUTOMATIC: return GTK_POLICY_AUTOMATIC;
        case EG_SCROLL_ALWAYS:    return GTK_POLICY_ALWAYS;
        case EG_SCROLL_NEVER:     return GTK_POLICY_NEVER;
        default:                  return GTK_POLICY_AUTOMATIC;
    }
}

/* ============================================
 * API Pública
 * ============================================ */

EgScrolledWindow *eg_scrolled_window_new(void) {
    EgScrolledWindow *sw = EG_ALLOC(EgScrolledWindow);
    if (sw == NULL) return NULL;

    GtkWidget *gtk_sw = gtk_scrolled_window_new();
    if (gtk_sw == NULL) {
        eg_free(sw);
        return NULL;
    }

    eg_widget_init(&sw->base, EG_WIDGET_TYPE_SCROLLED_WINDOW, gtk_sw, &eg_scrolled_window_vtable.base);
    sw->child = NULL;

    return sw;
}

void eg_scrolled_window_free(EgScrolledWindow *scrolled_window) {
    scrolled_window_destroy((EgWidget *)scrolled_window);
}

void eg_scrolled_window_set_child(EgScrolledWindow *scrolled_window, EgWidget *child) {
    scrolled_window_add_child((EgWidget *)scrolled_window, child);
}

void eg_scrolled_window_set_policy(EgScrolledWindow *scrolled_window,
                                    EgScrollPolicy hscroll, EgScrollPolicy vscroll) {
    if (scrolled_window == NULL || scrolled_window->base.native == NULL) return;

    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native),
        eg_policy_to_gtk(hscroll),
        eg_policy_to_gtk(vscroll)
    );
}

void eg_scrolled_window_set_min_content_size(EgScrolledWindow *scrolled_window,
                                              int width, int height) {
    if (scrolled_window == NULL || scrolled_window->base.native == NULL) return;

    gtk_scrolled_window_set_min_content_width(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native), width);
    gtk_scrolled_window_set_min_content_height(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native), height);
}

void eg_scrolled_window_set_max_content_size(EgScrolledWindow *scrolled_window,
                                              int width, int height) {
    if (scrolled_window == NULL || scrolled_window->base.native == NULL) return;

    gtk_scrolled_window_set_max_content_width(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native), width);
    gtk_scrolled_window_set_max_content_height(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native), height);
}

void eg_scrolled_window_set_overlay_scrolling(EgScrolledWindow *scrolled_window, bool overlay) {
    if (scrolled_window == NULL || scrolled_window->base.native == NULL) return;
    gtk_scrolled_window_set_overlay_scrolling(
        GTK_SCROLLED_WINDOW(scrolled_window->base.native), overlay);
}

EgWidget *eg_scrolled_window_as_widget(EgScrolledWindow *scrolled_window) {
    return (EgWidget *)scrolled_window;
}

void *eg_scrolled_window_get_native(EgScrolledWindow *scrolled_window) {
    if (scrolled_window == NULL) return NULL;
    return scrolled_window->base.native;
}
