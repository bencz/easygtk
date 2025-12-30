/**
 * EasyGTK - Janela
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/app.h>

/* Funções da vtable */
static void window_destroy(EgWidget *widget);
static void *window_get_native(EgWidget *widget);
static void window_set_visible(EgWidget *widget, bool visible);
static bool window_get_visible(EgWidget *widget);
static void window_set_sensitive(EgWidget *widget, bool sensitive);
static bool window_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_window_vtable = {
    .type = EG_WIDGET_TYPE_WINDOW,
    .type_name = "EgWindow",
    .destroy = window_destroy,
    .get_native = window_get_native,
    .set_visible = window_set_visible,
    .get_visible = window_get_visible,
    .set_sensitive = window_set_sensitive,
    .get_sensitive = window_get_sensitive
};

static void window_destroy(EgWidget *widget) {
    EgWindow *window = (EgWindow *)widget;
    if (window == NULL) return;
    
    /* Nota: GTK gerencia o ciclo de vida do widget nativo */
    eg_free(window);
}

static void *window_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void window_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool window_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void window_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool window_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para close-request */
static gboolean window_close_request_callback(GtkWindow *gtk_window, gpointer user_data) {
    (void)gtk_window;
    EgWindow *window = (EgWindow *)user_data;
    if (window != NULL && window->on_close != NULL) {
        window->on_close((EgWidget *)window, window->close_data);
    }
    return FALSE; /* Permite o fechamento */
}

/* Callback para destroy */
static void window_destroy_callback(GtkWidget *gtk_widget, gpointer user_data) {
    (void)gtk_widget;
    EgWindow *window = (EgWindow *)user_data;
    if (window != NULL && window->on_destroy != NULL) {
        window->on_destroy((EgWidget *)window, window->destroy_data);
    }
}

EgWindow *eg_window_new(EgApp *app, const char *title, int width, int height) {
    if (app == NULL) return NULL;
    
    EgWindow *window = EG_ALLOC(EgWindow);
    if (window == NULL) return NULL;
    
    GtkWidget *gtk_window = gtk_application_window_new(GTK_APPLICATION(eg_app_get_native(app)));
    if (gtk_window == NULL) {
        eg_free(window);
        return NULL;
    }
    
    eg_widget_init(&window->base, EG_WIDGET_TYPE_WINDOW, gtk_window, &eg_window_vtable);
    
    window->app = app;
    window->on_close = NULL;
    window->close_data = NULL;
    window->on_destroy = NULL;
    window->destroy_data = NULL;
    
    if (title != NULL) {
        gtk_window_set_title(GTK_WINDOW(gtk_window), title);
    }
    
    gtk_window_set_default_size(GTK_WINDOW(gtk_window), width, height);
    
    return window;
}

void eg_window_free(EgWindow *window) {
    window_destroy((EgWidget *)window);
}

void eg_window_show(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_present(GTK_WINDOW(window->base.native));
}

void eg_window_hide(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_widget_set_visible(window->base.native, FALSE);
}

void eg_window_close(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_close(GTK_WINDOW(window->base.native));
}

void eg_window_set_title(EgWindow *window, const char *title) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_set_title(GTK_WINDOW(window->base.native), title);
}

const char *eg_window_get_title(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return NULL;
    return gtk_window_get_title(GTK_WINDOW(window->base.native));
}

void eg_window_set_size(EgWindow *window, int width, int height) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_set_default_size(GTK_WINDOW(window->base.native), width, height);
}

EgSize eg_window_get_size(EgWindow *window) {
    EgSize size = { 0, 0 };
    if (window == NULL || window->base.native == NULL) return size;
    
    gtk_window_get_default_size(GTK_WINDOW(window->base.native), &size.width, &size.height);
    return size;
}

void eg_window_set_resizable(EgWindow *window, bool resizable) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_set_resizable(GTK_WINDOW(window->base.native), resizable);
}

void eg_window_set_child(EgWindow *window, EgWidget *child) {
    if (window == NULL || window->base.native == NULL) return;
    if (child == NULL) {
        gtk_window_set_child(GTK_WINDOW(window->base.native), NULL);
    } else {
        gtk_window_set_child(GTK_WINDOW(window->base.native), child->native);
    }
}

void eg_window_set_titlebar(EgWindow *window, EgWidget *titlebar) {
    if (window == NULL || window->base.native == NULL) return;
    if (titlebar == NULL) {
        gtk_window_set_titlebar(GTK_WINDOW(window->base.native), NULL);
    } else {
        gtk_window_set_titlebar(GTK_WINDOW(window->base.native), titlebar->native);
    }
}

void eg_window_on_close(EgWindow *window, EgCallback callback, void *user_data) {
    if (window == NULL) return;
    
    window->on_close = callback;
    window->close_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(window->base.native, "close-request", 
                        G_CALLBACK(window_close_request_callback), window);
    }
}

void eg_window_on_destroy(EgWindow *window, EgCallback callback, void *user_data) {
    if (window == NULL) return;
    
    window->on_destroy = callback;
    window->destroy_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(window->base.native, "destroy", 
                        G_CALLBACK(window_destroy_callback), window);
    }
}

EgWidget *eg_window_as_widget(EgWindow *window) {
    return (EgWidget *)window;
}

void *eg_window_get_native(EgWindow *window) {
    if (window == NULL) return NULL;
    return window->base.native;
}

void eg_window_minimize(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_minimize(GTK_WINDOW(window->base.native));
}

void eg_window_maximize(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_maximize(GTK_WINDOW(window->base.native));
}

void eg_window_unmaximize(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_unmaximize(GTK_WINDOW(window->base.native));
}

bool eg_window_is_maximized(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return false;
    return gtk_window_is_maximized(GTK_WINDOW(window->base.native));
}

void eg_window_toggle_maximize(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    if (gtk_window_is_maximized(GTK_WINDOW(window->base.native))) {
        gtk_window_unmaximize(GTK_WINDOW(window->base.native));
    } else {
        gtk_window_maximize(GTK_WINDOW(window->base.native));
    }
}

void eg_window_fullscreen(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_fullscreen(GTK_WINDOW(window->base.native));
}

void eg_window_unfullscreen(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_unfullscreen(GTK_WINDOW(window->base.native));
}

bool eg_window_is_fullscreen(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return false;
    return gtk_window_is_fullscreen(GTK_WINDOW(window->base.native));
}

void eg_window_set_decorated(EgWindow *window, bool decorated) {
    if (window == NULL || window->base.native == NULL) return;
    gtk_window_set_decorated(GTK_WINDOW(window->base.native), decorated);
}

bool eg_window_get_decorated(EgWindow *window) {
    if (window == NULL || window->base.native == NULL) return true;
    return gtk_window_get_decorated(GTK_WINDOW(window->base.native));
}
