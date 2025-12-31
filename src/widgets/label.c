/**
 * EasyGTK - Label
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include "internal/internal.h"
#include "internal/vtable.h"

/* Funções da vtable */
static void label_destroy(EgWidget *widget);
static void *label_get_native(EgWidget *widget);
static void label_set_visible(EgWidget *widget, bool visible);
static bool label_get_visible(EgWidget *widget);
static void label_set_sensitive(EgWidget *widget, bool sensitive);
static bool label_get_sensitive(EgWidget *widget);

/* Funções de binding */
static void *label_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void label_unbind(EgWidget *widget, void *binding_data);

/* Capacidades de binding do Label */
static const EgBindingCapabilities eg_label_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_STRING,  /* Aceita qualquer tipo, converte para string */
    .default_mode = EG_BINDING_MODE_ONE_WAY,  /* Label é apenas leitura */
    .supports_command = false
};

const EgWidgetVTable eg_label_vtable = {
    .type = EG_WIDGET_TYPE_LABEL,
    .type_name = "EgLabel",
    .destroy = label_destroy,
    .get_native = label_get_native,
    .set_visible = label_set_visible,
    .get_visible = label_get_visible,
    .set_sensitive = label_set_sensitive,
    .get_sensitive = label_get_sensitive,
    /* Binding support */
    .binding_caps = &eg_label_binding_caps,
    .bind_value = label_bind_value,
    .bind_command = NULL,
    .unbind = label_unbind
};

static void label_destroy(EgWidget *widget) {
    EgLabel *label = (EgLabel *)widget;
    if (label == NULL) return;
    eg_free(label);
}

static void *label_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void label_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool label_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void label_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool label_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgLabel *eg_label_new(const char *text) {
    EgLabel *label = EG_ALLOC(EgLabel);
    if (label == NULL) return NULL;
    
    GtkWidget *gtk_label = gtk_label_new(text);
    if (gtk_label == NULL) {
        eg_free(label);
        return NULL;
    }
    
    eg_widget_init(&label->base, EG_WIDGET_TYPE_LABEL, gtk_label, &eg_label_vtable);
    
    return label;
}

void eg_label_free(EgLabel *label) {
    label_destroy((EgWidget *)label);
}

void eg_label_set_text(EgLabel *label, const char *text) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_text(GTK_LABEL(label->base.native), text);
}

const char *eg_label_get_text(EgLabel *label) {
    if (label == NULL || label->base.native == NULL) return NULL;
    return gtk_label_get_text(GTK_LABEL(label->base.native));
}

void eg_label_set_markup(EgLabel *label, const char *markup) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_markup(GTK_LABEL(label->base.native), markup);
}

void eg_label_set_halign(EgLabel *label, EgAlign align) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_widget_set_halign(label->base.native, eg_align_to_gtk(align));
}

void eg_label_set_valign(EgLabel *label, EgAlign align) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_widget_set_valign(label->base.native, eg_align_to_gtk(align));
}

void eg_label_set_xalign(EgLabel *label, float xalign) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_xalign(GTK_LABEL(label->base.native), xalign);
}

void eg_label_set_yalign(EgLabel *label, float yalign) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_yalign(GTK_LABEL(label->base.native), yalign);
}

void eg_label_set_selectable(EgLabel *label, bool selectable) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_selectable(GTK_LABEL(label->base.native), selectable);
}

void eg_label_set_wrap(EgLabel *label, bool wrap) {
    if (label == NULL || label->base.native == NULL) return;
    gtk_label_set_wrap(GTK_LABEL(label->base.native), wrap);
}

EgWidget *eg_label_as_widget(EgLabel *label) {
    return (EgWidget *)label;
}

void *eg_label_get_native(EgLabel *label) {
    if (label == NULL) return NULL;
    return label->base.native;
}

/* ============================================
 * Binding Implementation
 * ============================================ */

typedef struct {
    EgLabel *label;
    EgProperty *property;
    EgHandlerId handler_id;
} LabelBindingData;

static void label_binding_on_property_changed(EgProperty *property, void *user_data) {
    LabelBindingData *data = (LabelBindingData *)user_data;
    if (data == NULL || data->label == NULL) return;

    EgPropertyType type = eg_property_get_type(property);
    char buffer[256];
    const char *text = NULL;

    switch (type) {
        case EG_PROPERTY_TYPE_STRING:
            text = eg_property_get_string(property);
            break;
        case EG_PROPERTY_TYPE_INT: {
            int val = eg_property_get_int(property);
            snprintf(buffer, sizeof(buffer), "%d", val);
            text = buffer;
            break;
        }
        case EG_PROPERTY_TYPE_DOUBLE: {
            double val = eg_property_get_double(property);
            snprintf(buffer, sizeof(buffer), "%.2f", val);
            text = buffer;
            break;
        }
        case EG_PROPERTY_TYPE_BOOL: {
            bool val = eg_property_get_bool(property);
            text = val ? "true" : "false";
            break;
        }
        default:
            text = "";
            break;
    }

    eg_label_set_text(data->label, text ? text : "");
}

static void *label_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgLabel *label = (EgLabel *)widget;
    if (label == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    LabelBindingData *data = EG_ALLOC(LabelBindingData);
    if (data == NULL) return NULL;

    data->label = label;
    data->property = ctx->property;

    /* Property -> Widget (one-way) */
    data->handler_id = eg_property_on_changed(ctx->property,
                                               label_binding_on_property_changed, data);

    /* Sincroniza valor inicial */
    label_binding_on_property_changed(ctx->property, data);

    return data;
}

static void label_unbind(EgWidget *widget, void *binding_data) {
    (void)widget;
    LabelBindingData *data = (LabelBindingData *)binding_data;
    if (data == NULL) return;

    if (data->property != NULL && data->handler_id != 0) {
        eg_property_disconnect(data->property, data->handler_id);
    }

    eg_free(data);
}
