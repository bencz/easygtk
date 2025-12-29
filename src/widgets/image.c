/**
 * EasyGTK - Image Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/image.h>

/* Funções da vtable */
static void image_destroy(EgWidget *widget);
static void *image_get_native(EgWidget *widget);
static void image_set_visible(EgWidget *widget, bool visible);
static bool image_get_visible(EgWidget *widget);
static void image_set_sensitive(EgWidget *widget, bool sensitive);
static bool image_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_image_vtable = {
    .type = EG_WIDGET_TYPE_IMAGE,
    .type_name = "EgImage",
    .destroy = image_destroy,
    .get_native = image_get_native,
    .set_visible = image_set_visible,
    .get_visible = image_get_visible,
    .set_sensitive = image_set_sensitive,
    .get_sensitive = image_get_sensitive
};

static void image_destroy(EgWidget *widget) {
    EgImage *image = (EgImage *)widget;
    if (image == NULL) return;
    eg_free(image);
}

static void *image_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void image_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool image_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void image_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool image_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgImage *eg_image_new(void) {
    EgImage *image = EG_ALLOC(EgImage);
    if (image == NULL) return NULL;
    
    GtkWidget *gtk_image = gtk_image_new();
    if (gtk_image == NULL) {
        eg_free(image);
        return NULL;
    }
    
    eg_widget_init(&image->base, EG_WIDGET_TYPE_IMAGE, gtk_image, &eg_image_vtable);
    
    return image;
}

EgImage *eg_image_new_from_file(const char *file_path) {
    EgImage *image = EG_ALLOC(EgImage);
    if (image == NULL) return NULL;
    
    GtkWidget *gtk_image = gtk_image_new_from_file(file_path);
    if (gtk_image == NULL) {
        eg_free(image);
        return NULL;
    }
    
    eg_widget_init(&image->base, EG_WIDGET_TYPE_IMAGE, gtk_image, &eg_image_vtable);
    
    return image;
}

EgImage *eg_image_new_from_icon(const char *icon_name) {
    EgImage *image = EG_ALLOC(EgImage);
    if (image == NULL) return NULL;
    
    GtkWidget *gtk_image = gtk_image_new_from_icon_name(icon_name);
    if (gtk_image == NULL) {
        eg_free(image);
        return NULL;
    }
    
    eg_widget_init(&image->base, EG_WIDGET_TYPE_IMAGE, gtk_image, &eg_image_vtable);
    
    return image;
}

EgImage *eg_image_new_from_resource(const char *resource_path) {
    EgImage *image = EG_ALLOC(EgImage);
    if (image == NULL) return NULL;
    
    GtkWidget *gtk_image = gtk_image_new_from_resource(resource_path);
    if (gtk_image == NULL) {
        eg_free(image);
        return NULL;
    }
    
    eg_widget_init(&image->base, EG_WIDGET_TYPE_IMAGE, gtk_image, &eg_image_vtable);
    
    return image;
}

void eg_image_free(EgImage *image) {
    image_destroy((EgWidget *)image);
}

void eg_image_set_from_file(EgImage *image, const char *file_path) {
    if (image == NULL || image->base.native == NULL) return;
    gtk_image_set_from_file(GTK_IMAGE(image->base.native), file_path);
}

void eg_image_set_from_icon(EgImage *image, const char *icon_name) {
    if (image == NULL || image->base.native == NULL) return;
    gtk_image_set_from_icon_name(GTK_IMAGE(image->base.native), icon_name);
}

void eg_image_set_from_resource(EgImage *image, const char *resource_path) {
    if (image == NULL || image->base.native == NULL) return;
    gtk_image_set_from_resource(GTK_IMAGE(image->base.native), resource_path);
}

void eg_image_set_pixel_size(EgImage *image, int pixel_size) {
    if (image == NULL || image->base.native == NULL) return;
    gtk_image_set_pixel_size(GTK_IMAGE(image->base.native), pixel_size);
}

int eg_image_get_pixel_size(EgImage *image) {
    if (image == NULL || image->base.native == NULL) return -1;
    return gtk_image_get_pixel_size(GTK_IMAGE(image->base.native));
}

void eg_image_clear(EgImage *image) {
    if (image == NULL || image->base.native == NULL) return;
    gtk_image_clear(GTK_IMAGE(image->base.native));
}

EgWidget *eg_image_as_widget(EgImage *image) {
    return (EgWidget *)image;
}

void *eg_image_get_native(EgImage *image) {
    if (image == NULL) return NULL;
    return image->base.native;
}
