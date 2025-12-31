/**
 * EasyGTK - Switch
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"
#include <easygtk/switch.h>

/* Funções da vtable */
static void switch_destroy(EgWidget *widget);
static void *switch_get_native(EgWidget *widget);
static void switch_set_visible(EgWidget *widget, bool visible);
static bool switch_get_visible(EgWidget *widget);
static void switch_set_sensitive(EgWidget *widget, bool sensitive);
static bool switch_get_sensitive(EgWidget *widget);

/* Funções de binding */
static void *switch_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void switch_unbind(EgWidget *widget, void *binding_data);

/* Capacidades de binding */
static const EgBindingCapabilities eg_switch_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_BOOL,
    .default_mode = EG_BINDING_MODE_TWO_WAY,
    .supports_command = false
};

const EgWidgetVTable eg_switch_vtable = {
    .type = EG_WIDGET_TYPE_SWITCH,
    .type_name = "EgSwitch",
    .destroy = switch_destroy,
    .get_native = switch_get_native,
    .set_visible = switch_set_visible,
    .get_visible = switch_get_visible,
    .set_sensitive = switch_set_sensitive,
    .get_sensitive = switch_get_sensitive,
    /* Binding support */
    .binding_caps = &eg_switch_binding_caps,
    .bind_value = switch_bind_value,
    .bind_command = NULL,
    .unbind = switch_unbind
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

/* ============================================
 * Binding Implementation
 * ============================================ */

typedef struct {
    EgSwitch *switch_widget;
    EgProperty *property;
    EgHandlerId property_handler;
    gulong gtk_signal_id;
    bool updating;
} SwitchBindingData;

static void switch_binding_on_property_changed(EgProperty *property, void *user_data) {
    SwitchBindingData *data = (SwitchBindingData *)user_data;
    if (data == NULL || data->switch_widget == NULL || data->updating) return;

    data->updating = true;
    bool value = eg_property_get_bool(property);
    eg_switch_set_active(data->switch_widget, value);
    data->updating = false;
}

static gboolean switch_binding_on_state_set(GtkSwitch *gtk_sw, gboolean state, gpointer user_data) {
    (void)gtk_sw;
    SwitchBindingData *data = (SwitchBindingData *)user_data;
    if (data == NULL || data->switch_widget == NULL || data->updating) return FALSE;

    data->updating = true;
    eg_property_set_bool(data->property, state);
    data->updating = false;
    return FALSE;
}

static void *switch_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgSwitch *sw = (EgSwitch *)widget;
    if (sw == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    if (eg_property_get_type(ctx->property) != EG_PROPERTY_TYPE_BOOL) {
        return NULL;
    }

    SwitchBindingData *data = EG_ALLOC(SwitchBindingData);
    if (data == NULL) return NULL;

    data->switch_widget = sw;
    data->property = ctx->property;
    data->updating = false;

    /* Property -> Widget */
    data->property_handler = eg_property_on_changed(ctx->property,
                                                     switch_binding_on_property_changed, data);

    /* Widget -> Property (se two-way) */
    if (ctx->mode == EG_BINDING_MODE_TWO_WAY) {
        data->gtk_signal_id = g_signal_connect(widget->native, "state-set",
                                                G_CALLBACK(switch_binding_on_state_set), data);
    }

    /* Sincroniza valor inicial */
    switch_binding_on_property_changed(ctx->property, data);

    return data;
}

static void switch_unbind(EgWidget *widget, void *binding_data) {
    SwitchBindingData *data = (SwitchBindingData *)binding_data;
    if (data == NULL) return;

    if (data->property != NULL && data->property_handler != 0) {
        eg_property_disconnect(data->property, data->property_handler);
    }

    if (widget != NULL && widget->native != NULL && data->gtk_signal_id != 0) {
        g_signal_handler_disconnect(widget->native, data->gtk_signal_id);
    }

    eg_free(data);
}
