/**
 * EasyGTK - HeaderBar Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/header_bar.h>

/* Funções da vtable */
static void header_bar_destroy(EgWidget *widget);
static void *header_bar_get_native(EgWidget *widget);
static void header_bar_set_visible(EgWidget *widget, bool visible);
static bool header_bar_get_visible(EgWidget *widget);
static void header_bar_set_sensitive(EgWidget *widget, bool sensitive);
static bool header_bar_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_header_bar_vtable = {
    .type = EG_WIDGET_TYPE_HEADER_BAR,
    .type_name = "EgHeaderBar",
    .destroy = header_bar_destroy,
    .get_native = header_bar_get_native,
    .set_visible = header_bar_set_visible,
    .get_visible = header_bar_get_visible,
    .set_sensitive = header_bar_set_sensitive,
    .get_sensitive = header_bar_get_sensitive
};

static void header_bar_destroy(EgWidget *widget) {
    EgHeaderBar *header_bar = (EgHeaderBar *)widget;
    if (header_bar == NULL) return;
    eg_free(header_bar);
}

static void *header_bar_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void header_bar_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool header_bar_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void header_bar_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool header_bar_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgHeaderBar *eg_header_bar_new(void) {
    EgHeaderBar *header_bar = EG_ALLOC(EgHeaderBar);
    if (header_bar == NULL) return NULL;
    
    GtkWidget *gtk_header_bar = gtk_header_bar_new();
    if (gtk_header_bar == NULL) {
        eg_free(header_bar);
        return NULL;
    }
    
    eg_widget_init(&header_bar->base, EG_WIDGET_TYPE_HEADER_BAR, gtk_header_bar, &eg_header_bar_vtable);
    
    return header_bar;
}

void eg_header_bar_free(EgHeaderBar *header_bar) {
    header_bar_destroy((EgWidget *)header_bar);
}

void eg_header_bar_set_title_widget(EgHeaderBar *header_bar, EgWidget *title_widget) {
    if (header_bar == NULL || header_bar->base.native == NULL) return;
    GtkWidget *gtk_title = title_widget ? title_widget->native : NULL;
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar->base.native), gtk_title);
}

EgWidget *eg_header_bar_get_title_widget(EgHeaderBar *header_bar) {
    (void)header_bar;
    return NULL;
}

void eg_header_bar_pack_start(EgHeaderBar *header_bar, EgWidget *child) {
    if (header_bar == NULL || header_bar->base.native == NULL || child == NULL) return;
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar->base.native), child->native);
}

void eg_header_bar_pack_end(EgHeaderBar *header_bar, EgWidget *child) {
    if (header_bar == NULL || header_bar->base.native == NULL || child == NULL) return;
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar->base.native), child->native);
}

void eg_header_bar_remove(EgHeaderBar *header_bar, EgWidget *child) {
    if (header_bar == NULL || header_bar->base.native == NULL || child == NULL) return;
    gtk_header_bar_remove(GTK_HEADER_BAR(header_bar->base.native), child->native);
}

void eg_header_bar_set_show_title_buttons(EgHeaderBar *header_bar, bool show_title_buttons) {
    if (header_bar == NULL || header_bar->base.native == NULL) return;
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_bar->base.native), show_title_buttons);
}

bool eg_header_bar_get_show_title_buttons(EgHeaderBar *header_bar) {
    if (header_bar == NULL || header_bar->base.native == NULL) return false;
    return gtk_header_bar_get_show_title_buttons(GTK_HEADER_BAR(header_bar->base.native));
}

void eg_header_bar_set_decoration_layout(EgHeaderBar *header_bar, const char *layout) {
    if (header_bar == NULL || header_bar->base.native == NULL) return;
    gtk_header_bar_set_decoration_layout(GTK_HEADER_BAR(header_bar->base.native), layout);
}

const char *eg_header_bar_get_decoration_layout(EgHeaderBar *header_bar) {
    if (header_bar == NULL || header_bar->base.native == NULL) return NULL;
    return gtk_header_bar_get_decoration_layout(GTK_HEADER_BAR(header_bar->base.native));
}

EgWidget *eg_header_bar_as_widget(EgHeaderBar *header_bar) {
    return (EgWidget *)header_bar;
}

void *eg_header_bar_get_native(EgHeaderBar *header_bar) {
    if (header_bar == NULL) return NULL;
    return header_bar->base.native;
}
