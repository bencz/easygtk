/**
 * EasyGTK - Picture Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/picture.h>

/* Funções da vtable */
static void picture_destroy(EgWidget *widget);
static void *picture_get_native(EgWidget *widget);
static void picture_set_visible(EgWidget *widget, bool visible);
static bool picture_get_visible(EgWidget *widget);
static void picture_set_sensitive(EgWidget *widget, bool sensitive);
static bool picture_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_picture_vtable = {
    .type = EG_WIDGET_TYPE_PICTURE,
    .type_name = "EgPicture",
    .destroy = picture_destroy,
    .get_native = picture_get_native,
    .set_visible = picture_set_visible,
    .get_visible = picture_get_visible,
    .set_sensitive = picture_set_sensitive,
    .get_sensitive = picture_get_sensitive
};

static void picture_destroy(EgWidget *widget) {
    EgPicture *picture = (EgPicture *)widget;
    if (picture == NULL) return;
    eg_free(picture);
}

static void *picture_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void picture_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool picture_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void picture_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool picture_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Converte EgContentFit para GtkContentFit */
static GtkContentFit eg_content_fit_to_gtk(EgContentFit fit) {
    switch (fit) {
        case EG_CONTENT_FIT_FILL: return GTK_CONTENT_FIT_FILL;
        case EG_CONTENT_FIT_CONTAIN: return GTK_CONTENT_FIT_CONTAIN;
        case EG_CONTENT_FIT_COVER: return GTK_CONTENT_FIT_COVER;
        case EG_CONTENT_FIT_SCALE_DOWN: return GTK_CONTENT_FIT_SCALE_DOWN;
        default: return GTK_CONTENT_FIT_CONTAIN;
    }
}

/* Converte GtkContentFit para EgContentFit */
static EgContentFit gtk_content_fit_to_eg(GtkContentFit fit) {
    switch (fit) {
        case GTK_CONTENT_FIT_FILL: return EG_CONTENT_FIT_FILL;
        case GTK_CONTENT_FIT_CONTAIN: return EG_CONTENT_FIT_CONTAIN;
        case GTK_CONTENT_FIT_COVER: return EG_CONTENT_FIT_COVER;
        case GTK_CONTENT_FIT_SCALE_DOWN: return EG_CONTENT_FIT_SCALE_DOWN;
        default: return EG_CONTENT_FIT_CONTAIN;
    }
}

EgPicture *eg_picture_new(void) {
    EgPicture *picture = EG_ALLOC(EgPicture);
    if (picture == NULL) return NULL;
    
    GtkWidget *gtk_picture = gtk_picture_new();
    if (gtk_picture == NULL) {
        eg_free(picture);
        return NULL;
    }
    
    eg_widget_init(&picture->base, EG_WIDGET_TYPE_PICTURE, gtk_picture, &eg_picture_vtable);
    
    return picture;
}

EgPicture *eg_picture_new_from_file(const char *file_path) {
    EgPicture *picture = EG_ALLOC(EgPicture);
    if (picture == NULL) return NULL;
    
    GFile *file = g_file_new_for_path(file_path);
    GtkWidget *gtk_picture = gtk_picture_new_for_file(file);
    g_object_unref(file);
    
    if (gtk_picture == NULL) {
        eg_free(picture);
        return NULL;
    }
    
    eg_widget_init(&picture->base, EG_WIDGET_TYPE_PICTURE, gtk_picture, &eg_picture_vtable);
    
    return picture;
}

EgPicture *eg_picture_new_from_resource(const char *resource_path) {
    EgPicture *picture = EG_ALLOC(EgPicture);
    if (picture == NULL) return NULL;
    
    GtkWidget *gtk_picture = gtk_picture_new_for_resource(resource_path);
    if (gtk_picture == NULL) {
        eg_free(picture);
        return NULL;
    }
    
    eg_widget_init(&picture->base, EG_WIDGET_TYPE_PICTURE, gtk_picture, &eg_picture_vtable);
    
    return picture;
}

void eg_picture_free(EgPicture *picture) {
    picture_destroy((EgWidget *)picture);
}

void eg_picture_set_filename(EgPicture *picture, const char *file_path) {
    if (picture == NULL || picture->base.native == NULL) return;
    gtk_picture_set_filename(GTK_PICTURE(picture->base.native), file_path);
}

const char *eg_picture_get_filename(EgPicture *picture) {
    if (picture == NULL || picture->base.native == NULL) return NULL;
    GFile *file = gtk_picture_get_file(GTK_PICTURE(picture->base.native));
    if (file == NULL) return NULL;
    return g_file_get_path(file);
}

void eg_picture_set_resource(EgPicture *picture, const char *resource_path) {
    if (picture == NULL || picture->base.native == NULL) return;
    gtk_picture_set_resource(GTK_PICTURE(picture->base.native), resource_path);
}

void eg_picture_set_content_fit(EgPicture *picture, EgContentFit fit) {
    if (picture == NULL || picture->base.native == NULL) return;
    gtk_picture_set_content_fit(GTK_PICTURE(picture->base.native), eg_content_fit_to_gtk(fit));
}

EgContentFit eg_picture_get_content_fit(EgPicture *picture) {
    if (picture == NULL || picture->base.native == NULL) return EG_CONTENT_FIT_CONTAIN;
    return gtk_content_fit_to_eg(gtk_picture_get_content_fit(GTK_PICTURE(picture->base.native)));
}

void eg_picture_set_can_shrink(EgPicture *picture, bool can_shrink) {
    if (picture == NULL || picture->base.native == NULL) return;
    gtk_picture_set_can_shrink(GTK_PICTURE(picture->base.native), can_shrink);
}

bool eg_picture_get_can_shrink(EgPicture *picture) {
    if (picture == NULL || picture->base.native == NULL) return false;
    return gtk_picture_get_can_shrink(GTK_PICTURE(picture->base.native));
}

void eg_picture_set_alternative_text(EgPicture *picture, const char *alt_text) {
    if (picture == NULL || picture->base.native == NULL) return;
    gtk_picture_set_alternative_text(GTK_PICTURE(picture->base.native), alt_text);
}

const char *eg_picture_get_alternative_text(EgPicture *picture) {
    if (picture == NULL || picture->base.native == NULL) return NULL;
    return gtk_picture_get_alternative_text(GTK_PICTURE(picture->base.native));
}

EgWidget *eg_picture_as_widget(EgPicture *picture) {
    return (EgWidget *)picture;
}

void *eg_picture_get_native(EgPicture *picture) {
    if (picture == NULL) return NULL;
    return picture->base.native;
}
