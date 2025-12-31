/**
 * EasyGTK - ProgressBar
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"
#include <easygtk/progress_bar.h>

/* Funções da vtable */
static void progress_bar_destroy(EgWidget *widget);
static void *progress_bar_get_native(EgWidget *widget);
static void progress_bar_set_visible(EgWidget *widget, bool visible);
static bool progress_bar_get_visible(EgWidget *widget);
static void progress_bar_set_sensitive(EgWidget *widget, bool sensitive);
static bool progress_bar_get_sensitive(EgWidget *widget);

/* Funções de binding */
static void *progress_bar_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void progress_bar_unbind(EgWidget *widget, void *binding_data);

/* Capacidades de binding */
static const EgBindingCapabilities eg_progress_bar_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_DOUBLE,
    .default_mode = EG_BINDING_MODE_ONE_WAY,
    .supports_command = false
};

const EgWidgetVTable eg_progress_bar_vtable = {
    .type = EG_WIDGET_TYPE_PROGRESS_BAR,
    .type_name = "EgProgressBar",
    .destroy = progress_bar_destroy,
    .get_native = progress_bar_get_native,
    .set_visible = progress_bar_set_visible,
    .get_visible = progress_bar_get_visible,
    .set_sensitive = progress_bar_set_sensitive,
    .get_sensitive = progress_bar_get_sensitive,
    /* Binding support */
    .binding_caps = &eg_progress_bar_binding_caps,
    .bind_value = progress_bar_bind_value,
    .bind_command = NULL,
    .unbind = progress_bar_unbind
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

/* ============================================
 * Binding Implementation
 * ============================================ */

typedef struct {
    EgProgressBar *progress_bar;
    EgProperty *property;
    EgPropertyType prop_type;
    EgHandlerId property_handler;
} ProgressBarBindingData;

static void progress_bar_binding_on_property_changed(EgProperty *property, void *user_data) {
    ProgressBarBindingData *data = (ProgressBarBindingData *)user_data;
    if (data == NULL || data->progress_bar == NULL) return;

    double value;
    if (data->prop_type == EG_PROPERTY_TYPE_INT) {
        value = (double)eg_property_get_int(property) / 100.0;  /* Assume int 0-100 */
    } else {
        value = eg_property_get_double(property);
    }
    eg_progress_bar_set_fraction(data->progress_bar, value);
}

static void *progress_bar_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgProgressBar *pb = (EgProgressBar *)widget;
    if (pb == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    EgPropertyType prop_type = eg_property_get_type(ctx->property);
    if (prop_type != EG_PROPERTY_TYPE_INT && prop_type != EG_PROPERTY_TYPE_DOUBLE) {
        return NULL;
    }

    ProgressBarBindingData *data = EG_ALLOC(ProgressBarBindingData);
    if (data == NULL) return NULL;

    data->progress_bar = pb;
    data->property = ctx->property;
    data->prop_type = prop_type;

    /* Property -> Widget (one-way) */
    data->property_handler = eg_property_on_changed(ctx->property,
                                                     progress_bar_binding_on_property_changed, data);

    /* Sincroniza valor inicial */
    progress_bar_binding_on_property_changed(ctx->property, data);

    return data;
}

static void progress_bar_unbind(EgWidget *widget, void *binding_data) {
    (void)widget;
    ProgressBarBindingData *data = (ProgressBarBindingData *)binding_data;
    if (data == NULL) return;

    if (data->property != NULL && data->property_handler != 0) {
        eg_property_disconnect(data->property, data->property_handler);
    }

    eg_free(data);
}
