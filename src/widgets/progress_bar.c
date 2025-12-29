/**
 * EasyGTK - ProgressBar
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/progress_bar.h>

/* Funções da vtable */
static void progress_bar_destroy(EgWidget *widget);
static void *progress_bar_get_native(EgWidget *widget);
static void progress_bar_set_visible(EgWidget *widget, bool visible);
static bool progress_bar_get_visible(EgWidget *widget);
static void progress_bar_set_sensitive(EgWidget *widget, bool sensitive);
static bool progress_bar_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_progress_bar_vtable = {
    .type = EG_WIDGET_TYPE_PROGRESS_BAR,
    .type_name = "EgProgressBar",
    .destroy = progress_bar_destroy,
    .get_native = progress_bar_get_native,
    .set_visible = progress_bar_set_visible,
    .get_visible = progress_bar_get_visible,
    .set_sensitive = progress_bar_set_sensitive,
    .get_sensitive = progress_bar_get_sensitive
};

static void progress_bar_destroy(EgWidget *widget) {
    EgProgressBar *pb = (EgProgressBar *)widget;
    if (pb == NULL) return;
    eg_free(pb);
}

static void *progress_bar_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void progress_bar_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool progress_bar_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void progress_bar_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool progress_bar_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgProgressBar *eg_progress_bar_new(void) {
    EgProgressBar *pb = EG_ALLOC(EgProgressBar);
    if (pb == NULL) return NULL;
    
    GtkWidget *gtk_pb = gtk_progress_bar_new();
    if (gtk_pb == NULL) {
        eg_free(pb);
        return NULL;
    }
    
    eg_widget_init(&pb->base, EG_WIDGET_TYPE_PROGRESS_BAR, gtk_pb, &eg_progress_bar_vtable);
    
    return pb;
}

void eg_progress_bar_free(EgProgressBar *progress_bar) {
    progress_bar_destroy((EgWidget *)progress_bar);
}

void eg_progress_bar_set_fraction(EgProgressBar *progress_bar, double fraction) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    
    /* Clamp entre 0.0 e 1.0 */
    if (fraction < 0.0) fraction = 0.0;
    if (fraction > 1.0) fraction = 1.0;
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar->base.native), fraction);
}

double eg_progress_bar_get_fraction(EgProgressBar *progress_bar) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return 0.0;
    return gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_bar->base.native));
}

void eg_progress_bar_set_text(EgProgressBar *progress_bar, const char *text) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar->base.native), text);
}

const char *eg_progress_bar_get_text(EgProgressBar *progress_bar) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return NULL;
    return gtk_progress_bar_get_text(GTK_PROGRESS_BAR(progress_bar->base.native));
}

void eg_progress_bar_set_show_text(EgProgressBar *progress_bar, bool show_text) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar->base.native), show_text);
}

void eg_progress_bar_pulse(EgProgressBar *progress_bar) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progress_bar->base.native));
}

void eg_progress_bar_set_pulse_step(EgProgressBar *progress_bar, double fraction) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(progress_bar->base.native), fraction);
}

void eg_progress_bar_set_inverted(EgProgressBar *progress_bar, bool inverted) {
    if (progress_bar == NULL || progress_bar->base.native == NULL) return;
    gtk_progress_bar_set_inverted(GTK_PROGRESS_BAR(progress_bar->base.native), inverted);
}

EgWidget *eg_progress_bar_as_widget(EgProgressBar *progress_bar) {
    return (EgWidget *)progress_bar;
}

void *eg_progress_bar_get_native(EgProgressBar *progress_bar) {
    if (progress_bar == NULL) return NULL;
    return progress_bar->base.native;
}
