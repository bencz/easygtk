/**
 * EasyGTK - ScrolledWindow
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/scrolled_window.h>

/* Funções da vtable */
static void scrolled_window_destroy(EgWidget *widget);
static void *scrolled_window_get_native(EgWidget *widget);
static void scrolled_window_set_visible(EgWidget *widget, bool visible);
static bool scrolled_window_get_visible(EgWidget *widget);
static void scrolled_window_set_sensitive(EgWidget *widget, bool sensitive);
static bool scrolled_window_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_scrolled_window_vtable = {
    .type = EG_WIDGET_TYPE_SCROLLED_WINDOW,
    .type_name = "EgScrolledWindow",
    .destroy = scrolled_window_destroy,
    .get_native = scrolled_window_get_native,
    .set_visible = scrolled_window_set_visible,
    .get_visible = scrolled_window_get_visible,
    .set_sensitive = scrolled_window_set_sensitive,
    .get_sensitive = scrolled_window_get_sensitive
};

static void scrolled_window_destroy(EgWidget *widget) {
    EgScrolledWindow *sw = (EgScrolledWindow *)widget;
    if (sw == NULL) return;
    eg_free(sw);
}

static void *scrolled_window_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void scrolled_window_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool scrolled_window_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void scrolled_window_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool scrolled_window_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Converte EgScrollPolicy para GtkPolicyType */
static GtkPolicyType eg_policy_to_gtk(EgScrollPolicy policy) {
    switch (policy) {
        case EG_SCROLL_AUTOMATIC: return GTK_POLICY_AUTOMATIC;
        case EG_SCROLL_ALWAYS:    return GTK_POLICY_ALWAYS;
        case EG_SCROLL_NEVER:     return GTK_POLICY_NEVER;
        default:                  return GTK_POLICY_AUTOMATIC;
    }
}

EgScrolledWindow *eg_scrolled_window_new(void) {
    EgScrolledWindow *sw = EG_ALLOC(EgScrolledWindow);
    if (sw == NULL) return NULL;
    
    GtkWidget *gtk_sw = gtk_scrolled_window_new();
    if (gtk_sw == NULL) {
        eg_free(sw);
        return NULL;
    }
    
    eg_widget_init(&sw->base, EG_WIDGET_TYPE_SCROLLED_WINDOW, gtk_sw, &eg_scrolled_window_vtable);
    
    return sw;
}

void eg_scrolled_window_free(EgScrolledWindow *scrolled_window) {
    scrolled_window_destroy((EgWidget *)scrolled_window);
}

void eg_scrolled_window_set_child(EgScrolledWindow *scrolled_window, EgWidget *child) {
    if (scrolled_window == NULL || scrolled_window->base.native == NULL) return;
    
    GtkWidget *gtk_child = NULL;
    if (child != NULL) {
        gtk_child = child->native;
    }
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window->base.native), gtk_child);
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
