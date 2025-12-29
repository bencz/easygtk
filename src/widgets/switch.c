/**
 * EasyGTK - Switch
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/switch.h>

/* Funções da vtable */
static void switch_destroy(EgWidget *widget);
static void *switch_get_native(EgWidget *widget);
static void switch_set_visible(EgWidget *widget, bool visible);
static bool switch_get_visible(EgWidget *widget);
static void switch_set_sensitive(EgWidget *widget, bool sensitive);
static bool switch_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_switch_vtable = {
    .type = EG_WIDGET_TYPE_SWITCH,
    .type_name = "EgSwitch",
    .destroy = switch_destroy,
    .get_native = switch_get_native,
    .set_visible = switch_set_visible,
    .get_visible = switch_get_visible,
    .set_sensitive = switch_set_sensitive,
    .get_sensitive = switch_get_sensitive
};

static void switch_destroy(EgWidget *widget) {
    EgSwitch *sw = (EgSwitch *)widget;
    if (sw == NULL) return;
    eg_free(sw);
}

static void *switch_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void switch_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool switch_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void switch_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool switch_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para state-set */
static gboolean switch_state_set_callback(GtkSwitch *gtk_sw, gboolean state, gpointer user_data) {
    (void)gtk_sw;
    (void)state;
    EgSwitch *sw = (EgSwitch *)user_data;
    if (sw != NULL && sw->on_state_changed != NULL) {
        sw->on_state_changed((EgWidget *)sw, sw->state_changed_data);
    }
    return FALSE; /* Permite que o GTK atualize o estado */
}

EgSwitch *eg_switch_new(void) {
    EgSwitch *sw = EG_ALLOC(EgSwitch);
    if (sw == NULL) return NULL;
    
    GtkWidget *gtk_sw = gtk_switch_new();
    if (gtk_sw == NULL) {
        eg_free(sw);
        return NULL;
    }
    
    eg_widget_init(&sw->base, EG_WIDGET_TYPE_SWITCH, gtk_sw, &eg_switch_vtable);
    
    sw->on_state_changed = NULL;
    sw->state_changed_data = NULL;
    
    return sw;
}

void eg_switch_free(EgSwitch *sw) {
    switch_destroy((EgWidget *)sw);
}

void eg_switch_set_active(EgSwitch *sw, bool active) {
    if (sw == NULL || sw->base.native == NULL) return;
    gtk_switch_set_active(GTK_SWITCH(sw->base.native), active);
}

bool eg_switch_get_active(EgSwitch *sw) {
    if (sw == NULL || sw->base.native == NULL) return false;
    return gtk_switch_get_active(GTK_SWITCH(sw->base.native));
}

void eg_switch_on_state_changed(EgSwitch *sw, EgCallback callback, void *user_data) {
    if (sw == NULL) return;
    
    sw->on_state_changed = callback;
    sw->state_changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(sw->base.native, "state-set", 
                        G_CALLBACK(switch_state_set_callback), sw);
    }
}

EgWidget *eg_switch_as_widget(EgSwitch *sw) {
    return (EgWidget *)sw;
}

void *eg_switch_get_native(EgSwitch *sw) {
    if (sw == NULL) return NULL;
    return sw->base.native;
}
