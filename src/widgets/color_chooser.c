/**
 * EasyGTK - ColorChooser Dialog
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include "internal/internal.h"
#include <easygtk/color_chooser.h>
#include <easygtk/window.h>

/* Dados para callback assíncrono */
typedef struct {
    EgColorChooserCallback callback;
    void *user_data;
} ColorChooserData;

/* Callback para GtkColorDialog */
static void color_dialog_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    ColorChooserData *data = (ColorChooserData *)user_data;
    GtkColorDialog *dialog = GTK_COLOR_DIALOG(source);
    
    GError *error = NULL;
    GdkRGBA *rgba = gtk_color_dialog_choose_rgba_finish(dialog, result, &error);
    
    if (rgba != NULL) {
        EgColor color = {
            .red = rgba->red,
            .green = rgba->green,
            .blue = rgba->blue,
            .alpha = rgba->alpha
        };
        
        if (data->callback != NULL) {
            data->callback(&color, data->user_data);
        }
        
        gdk_rgba_free(rgba);
    } else {
        if (data->callback != NULL) {
            data->callback(NULL, data->user_data);
        }
        if (error != NULL) {
            g_error_free(error);
        }
    }
    
    eg_free(data);
}

void eg_color_chooser_dialog(EgWindow *parent, const char *title,
                              const EgColor *initial_color,
                              EgColorChooserCallback callback, void *user_data) {
    eg_color_chooser_dialog_with_alpha(parent, title, initial_color, false, callback, user_data);
}

void eg_color_chooser_dialog_with_alpha(EgWindow *parent, const char *title,
                                         const EgColor *initial_color,
                                         bool with_alpha,
                                         EgColorChooserCallback callback, void *user_data) {
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    if (title != NULL) {
        gtk_color_dialog_set_title(dialog, title);
    }
    
    gtk_color_dialog_set_with_alpha(dialog, with_alpha);
    
    GdkRGBA initial_rgba = { 0.0, 0.0, 0.0, 1.0 };
    if (initial_color != NULL) {
        initial_rgba.red = initial_color->red;
        initial_rgba.green = initial_color->green;
        initial_rgba.blue = initial_color->blue;
        initial_rgba.alpha = initial_color->alpha;
    }
    
    ColorChooserData *data = EG_ALLOC(ColorChooserData);
    if (data == NULL) {
        g_object_unref(dialog);
        return;
    }
    data->callback = callback;
    data->user_data = user_data;
    
    GtkWindow *gtk_parent = parent ? GTK_WINDOW(eg_window_get_native(parent)) : NULL;
    gtk_color_dialog_choose_rgba(dialog, gtk_parent, &initial_rgba, NULL, color_dialog_callback, data);
    
    g_object_unref(dialog);
}

EgColor eg_color_from_rgb(int red, int green, int blue) {
    EgColor color = {
        .red = red / 255.0,
        .green = green / 255.0,
        .blue = blue / 255.0,
        .alpha = 1.0
    };
    return color;
}

EgColor eg_color_from_rgba(int red, int green, int blue, double alpha) {
    EgColor color = {
        .red = red / 255.0,
        .green = green / 255.0,
        .blue = blue / 255.0,
        .alpha = alpha
    };
    return color;
}

EgColor eg_color_from_string(const char *hex) {
    EgColor color = { 0.0, 0.0, 0.0, 1.0 };
    
    if (hex == NULL) return color;
    
    GdkRGBA rgba;
    if (gdk_rgba_parse(&rgba, hex)) {
        color.red = rgba.red;
        color.green = rgba.green;
        color.blue = rgba.blue;
        color.alpha = rgba.alpha;
    }
    
    return color;
}

char *eg_color_to_string(const EgColor *color, char *buffer, size_t buffer_size) {
    if (color == NULL || buffer == NULL || buffer_size < 8) return NULL;
    
    int r = (int)(color->red * 255);
    int g = (int)(color->green * 255);
    int b = (int)(color->blue * 255);
    
    if (color->alpha < 1.0 && buffer_size >= 10) {
        int a = (int)(color->alpha * 255);
        snprintf(buffer, buffer_size, "#%02X%02X%02X%02X", r, g, b, a);
    } else {
        snprintf(buffer, buffer_size, "#%02X%02X%02X", r, g, b);
    }
    
    return buffer;
}
