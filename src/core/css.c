/**
 * EasyGTK - Sistema de CSS
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/css.h>

/* Estrutura do provider */
struct EgCssProvider {
    GtkCssProvider *native;
};

EgCssProvider *eg_css_load_from_string(const char *css_string) {
    if (css_string == NULL) return NULL;
    
    EgCssProvider *provider = EG_ALLOC(EgCssProvider);
    if (provider == NULL) return NULL;
    
    provider->native = gtk_css_provider_new();
    if (provider->native == NULL) {
        eg_free(provider);
        return NULL;
    }
    
    gtk_css_provider_load_from_string(provider->native, css_string);
    
    /* Aplica globalmente */
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider->native),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    return provider;
}

EgCssProvider *eg_css_load_from_file(const char *file_path) {
    if (file_path == NULL) return NULL;
    
    EgCssProvider *provider = EG_ALLOC(EgCssProvider);
    if (provider == NULL) return NULL;
    
    provider->native = gtk_css_provider_new();
    if (provider->native == NULL) {
        eg_free(provider);
        return NULL;
    }
    
    GFile *file = g_file_new_for_path(file_path);
    gtk_css_provider_load_from_file(provider->native, file);
    g_object_unref(file);
    
    /* Aplica globalmente */
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider->native),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    return provider;
}

void eg_css_provider_free(EgCssProvider *provider) {
    if (provider == NULL) return;
    
    if (provider->native != NULL) {
        gtk_style_context_remove_provider_for_display(
            gdk_display_get_default(),
            GTK_STYLE_PROVIDER(provider->native)
        );
        g_object_unref(provider->native);
    }
    
    eg_free(provider);
}

bool eg_widget_set_css(EgWidget *widget, const char *css_string) {
    if (widget == NULL || widget->native == NULL || css_string == NULL) {
        return false;
    }
    
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css_string);
    
    /* GTK4: usar display provider ao invés de style context (deprecated) */
    GdkDisplay *display = gtk_widget_get_display(widget->native);
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    /* Provider é referenciado pelo display, podemos liberar nossa referência */
    g_object_unref(provider);
    
    return true;
}
