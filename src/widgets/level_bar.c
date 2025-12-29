/**
 * EasyGTK - LevelBar Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/level_bar.h>

/* Funções da vtable */
static void level_bar_destroy(EgWidget *widget);
static void *level_bar_get_native(EgWidget *widget);
static void level_bar_set_visible(EgWidget *widget, bool visible);
static bool level_bar_get_visible(EgWidget *widget);
static void level_bar_set_sensitive(EgWidget *widget, bool sensitive);
static bool level_bar_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_level_bar_vtable = {
    .type = EG_WIDGET_TYPE_LEVEL_BAR,
    .type_name = "EgLevelBar",
    .destroy = level_bar_destroy,
    .get_native = level_bar_get_native,
    .set_visible = level_bar_set_visible,
    .get_visible = level_bar_get_visible,
    .set_sensitive = level_bar_set_sensitive,
    .get_sensitive = level_bar_get_sensitive
};

static void level_bar_destroy(EgWidget *widget) {
    EgLevelBar *level_bar = (EgLevelBar *)widget;
    if (level_bar == NULL) return;
    eg_free(level_bar);
}

static void *level_bar_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void level_bar_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool level_bar_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void level_bar_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool level_bar_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgLevelBar *eg_level_bar_new(void) {
    EgLevelBar *level_bar = EG_ALLOC(EgLevelBar);
    if (level_bar == NULL) return NULL;
    
    GtkWidget *gtk_level_bar = gtk_level_bar_new();
    if (gtk_level_bar == NULL) {
        eg_free(level_bar);
        return NULL;
    }
    
    eg_widget_init(&level_bar->base, EG_WIDGET_TYPE_LEVEL_BAR, gtk_level_bar, &eg_level_bar_vtable);
    
    return level_bar;
}

EgLevelBar *eg_level_bar_new_for_interval(double min_value, double max_value) {
    EgLevelBar *level_bar = EG_ALLOC(EgLevelBar);
    if (level_bar == NULL) return NULL;
    
    GtkWidget *gtk_level_bar = gtk_level_bar_new_for_interval(min_value, max_value);
    if (gtk_level_bar == NULL) {
        eg_free(level_bar);
        return NULL;
    }
    
    eg_widget_init(&level_bar->base, EG_WIDGET_TYPE_LEVEL_BAR, gtk_level_bar, &eg_level_bar_vtable);
    
    return level_bar;
}

void eg_level_bar_free(EgLevelBar *level_bar) {
    level_bar_destroy((EgWidget *)level_bar);
}

void eg_level_bar_set_value(EgLevelBar *level_bar, double value) {
    if (level_bar == NULL || level_bar->base.native == NULL) return;
    gtk_level_bar_set_value(GTK_LEVEL_BAR(level_bar->base.native), value);
}

double eg_level_bar_get_value(EgLevelBar *level_bar) {
    if (level_bar == NULL || level_bar->base.native == NULL) return 0.0;
    return gtk_level_bar_get_value(GTK_LEVEL_BAR(level_bar->base.native));
}

void eg_level_bar_set_min_value(EgLevelBar *level_bar, double min_value) {
    if (level_bar == NULL || level_bar->base.native == NULL) return;
    gtk_level_bar_set_min_value(GTK_LEVEL_BAR(level_bar->base.native), min_value);
}

double eg_level_bar_get_min_value(EgLevelBar *level_bar) {
    if (level_bar == NULL || level_bar->base.native == NULL) return 0.0;
    return gtk_level_bar_get_min_value(GTK_LEVEL_BAR(level_bar->base.native));
}

void eg_level_bar_set_max_value(EgLevelBar *level_bar, double max_value) {
    if (level_bar == NULL || level_bar->base.native == NULL) return;
    gtk_level_bar_set_max_value(GTK_LEVEL_BAR(level_bar->base.native), max_value);
}

double eg_level_bar_get_max_value(EgLevelBar *level_bar) {
    if (level_bar == NULL || level_bar->base.native == NULL) return 1.0;
    return gtk_level_bar_get_max_value(GTK_LEVEL_BAR(level_bar->base.native));
}

void eg_level_bar_set_mode(EgLevelBar *level_bar, EgLevelBarMode mode) {
    if (level_bar == NULL || level_bar->base.native == NULL) return;
    GtkLevelBarMode gtk_mode = (mode == EG_LEVEL_BAR_MODE_DISCRETE) ? 
        GTK_LEVEL_BAR_MODE_DISCRETE : GTK_LEVEL_BAR_MODE_CONTINUOUS;
    gtk_level_bar_set_mode(GTK_LEVEL_BAR(level_bar->base.native), gtk_mode);
}

EgLevelBarMode eg_level_bar_get_mode(EgLevelBar *level_bar) {
    if (level_bar == NULL || level_bar->base.native == NULL) return EG_LEVEL_BAR_MODE_CONTINUOUS;
    GtkLevelBarMode gtk_mode = gtk_level_bar_get_mode(GTK_LEVEL_BAR(level_bar->base.native));
    return (gtk_mode == GTK_LEVEL_BAR_MODE_DISCRETE) ? 
        EG_LEVEL_BAR_MODE_DISCRETE : EG_LEVEL_BAR_MODE_CONTINUOUS;
}

void eg_level_bar_set_inverted(EgLevelBar *level_bar, bool inverted) {
    if (level_bar == NULL || level_bar->base.native == NULL) return;
    gtk_level_bar_set_inverted(GTK_LEVEL_BAR(level_bar->base.native), inverted);
}

bool eg_level_bar_get_inverted(EgLevelBar *level_bar) {
    if (level_bar == NULL || level_bar->base.native == NULL) return false;
    return gtk_level_bar_get_inverted(GTK_LEVEL_BAR(level_bar->base.native));
}

void eg_level_bar_add_offset_value(EgLevelBar *level_bar, const char *name, double value) {
    if (level_bar == NULL || level_bar->base.native == NULL || name == NULL) return;
    gtk_level_bar_add_offset_value(GTK_LEVEL_BAR(level_bar->base.native), name, value);
}

void eg_level_bar_remove_offset_value(EgLevelBar *level_bar, const char *name) {
    if (level_bar == NULL || level_bar->base.native == NULL || name == NULL) return;
    gtk_level_bar_remove_offset_value(GTK_LEVEL_BAR(level_bar->base.native), name);
}

EgWidget *eg_level_bar_as_widget(EgLevelBar *level_bar) {
    return (EgWidget *)level_bar;
}

void *eg_level_bar_get_native(EgLevelBar *level_bar) {
    if (level_bar == NULL) return NULL;
    return level_bar->base.native;
}
