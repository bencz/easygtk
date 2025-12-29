/**
 * EasyGTK - Scale (Slider)
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/scale.h>

/* Funções da vtable */
static void scale_destroy(EgWidget *widget);
static void *scale_get_native(EgWidget *widget);
static void scale_set_visible(EgWidget *widget, bool visible);
static bool scale_get_visible(EgWidget *widget);
static void scale_set_sensitive(EgWidget *widget, bool sensitive);
static bool scale_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_scale_vtable = {
    .type = EG_WIDGET_TYPE_SCALE,
    .type_name = "EgScale",
    .destroy = scale_destroy,
    .get_native = scale_get_native,
    .set_visible = scale_set_visible,
    .get_visible = scale_get_visible,
    .set_sensitive = scale_set_sensitive,
    .get_sensitive = scale_get_sensitive
};

static void scale_destroy(EgWidget *widget) {
    EgScale *scale = (EgScale *)widget;
    if (scale == NULL) return;
    eg_free(scale);
}

static void *scale_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void scale_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool scale_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void scale_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool scale_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback GTK para value-changed */
static void gtk_value_changed_callback(GtkRange *range, gpointer user_data) {
    (void)range;
    EgScale *scale = (EgScale *)user_data;
    if (scale != NULL && scale->on_value_changed != NULL) {
        scale->on_value_changed((EgWidget *)scale, scale->value_changed_data);
    }
}

EgScale *eg_scale_new(EgOrientation orientation, double min, double max, double step) {
    EgScale *scale = EG_ALLOC(EgScale);
    if (scale == NULL) return NULL;
    
    GtkOrientation gtk_orient = (orientation == EG_ORIENTATION_VERTICAL) 
        ? GTK_ORIENTATION_VERTICAL 
        : GTK_ORIENTATION_HORIZONTAL;
    
    GtkAdjustment *adjustment = gtk_adjustment_new(min, min, max, step, step * 10, 0);
    GtkWidget *gtk_scale = gtk_scale_new(gtk_orient, adjustment);
    
    if (gtk_scale == NULL) {
        eg_free(scale);
        return NULL;
    }
    
    eg_widget_init(&scale->base, EG_WIDGET_TYPE_SCALE, gtk_scale, &eg_scale_vtable);
    scale->on_value_changed = NULL;
    scale->value_changed_data = NULL;
    
    return scale;
}

EgScale *eg_scale_new_horizontal(double min, double max, double step) {
    return eg_scale_new(EG_ORIENTATION_HORIZONTAL, min, max, step);
}

EgScale *eg_scale_new_vertical(double min, double max, double step) {
    return eg_scale_new(EG_ORIENTATION_VERTICAL, min, max, step);
}

void eg_scale_free(EgScale *scale) {
    scale_destroy((EgWidget *)scale);
}

void eg_scale_set_value(EgScale *scale, double value) {
    if (scale == NULL || scale->base.native == NULL) return;
    gtk_range_set_value(GTK_RANGE(scale->base.native), value);
}

double eg_scale_get_value(EgScale *scale) {
    if (scale == NULL || scale->base.native == NULL) return 0.0;
    return gtk_range_get_value(GTK_RANGE(scale->base.native));
}

void eg_scale_set_range(EgScale *scale, double min, double max) {
    if (scale == NULL || scale->base.native == NULL) return;
    gtk_range_set_range(GTK_RANGE(scale->base.native), min, max);
}

void eg_scale_set_draw_value(EgScale *scale, bool draw_value) {
    if (scale == NULL || scale->base.native == NULL) return;
    gtk_scale_set_draw_value(GTK_SCALE(scale->base.native), draw_value);
}

void eg_scale_set_value_pos(EgScale *scale, int position) {
    if (scale == NULL || scale->base.native == NULL) return;
    GtkPositionType pos = GTK_POS_TOP;
    switch (position) {
        case 0: pos = GTK_POS_LEFT; break;
        case 1: pos = GTK_POS_RIGHT; break;
        case 2: pos = GTK_POS_TOP; break;
        case 3: pos = GTK_POS_BOTTOM; break;
    }
    gtk_scale_set_value_pos(GTK_SCALE(scale->base.native), pos);
}

void eg_scale_set_digits(EgScale *scale, int digits) {
    if (scale == NULL || scale->base.native == NULL) return;
    gtk_scale_set_digits(GTK_SCALE(scale->base.native), digits);
}

void eg_scale_add_mark(EgScale *scale, double value, int position, const char *markup) {
    if (scale == NULL || scale->base.native == NULL) return;
    GtkPositionType pos = (position == 0) ? GTK_POS_BOTTOM : GTK_POS_TOP;
    gtk_scale_add_mark(GTK_SCALE(scale->base.native), value, pos, markup);
}

void eg_scale_clear_marks(EgScale *scale) {
    if (scale == NULL || scale->base.native == NULL) return;
    gtk_scale_clear_marks(GTK_SCALE(scale->base.native));
}

void eg_scale_on_value_changed(EgScale *scale, EgCallback callback, void *user_data) {
    if (scale == NULL || scale->base.native == NULL) return;
    
    scale->on_value_changed = callback;
    scale->value_changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(scale->base.native, "value-changed",
                         G_CALLBACK(gtk_value_changed_callback), scale);
    }
}

EgWidget *eg_scale_as_widget(EgScale *scale) {
    return (EgWidget *)scale;
}

void *eg_scale_get_native(EgScale *scale) {
    if (scale == NULL) return NULL;
    return scale->base.native;
}
