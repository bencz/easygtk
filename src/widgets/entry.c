/**
 * EasyGTK - Entry
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"

/* Funções da vtable */
static void entry_destroy(EgWidget *widget);

/* Funções de binding */
static void *entry_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void entry_unbind(EgWidget *widget, void *binding_data);

/* Funções de validação */
static EgValue entry_get_value(EgWidget *widget);
static void entry_show_error(EgWidget *widget, const char *message);
static void entry_clear_error(EgWidget *widget);

/* Capacidades de binding */
static const EgBindingCapabilities eg_entry_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_STRING,
    .default_mode = EG_BINDING_MODE_TWO_WAY,
    .supports_command = false
};

/* Capacidades de validação */
static const EgValidationCapabilities eg_entry_validation_caps = {
    .value_type = EG_PROPERTY_TYPE_STRING,
    .supports_inline_error = true,
    .supports_error_style = true
};

/* VTable de validação */
static const EgValidationVTable eg_entry_validation_vtable = {
    .caps = &eg_entry_validation_caps,
    .get_value = entry_get_value,
    .show_error = entry_show_error,
    .clear_error = entry_clear_error
};

const EgWidgetVTable eg_entry_vtable = {
    .type = EG_WIDGET_TYPE_ENTRY,
    .type_name = "EgEntry",
    .destroy = entry_destroy,
    .get_native = eg_widget_get_native_default,
    .set_visible = eg_widget_set_visible_default,
    .get_visible = eg_widget_get_visible_default,
    .set_sensitive = eg_widget_set_sensitive_default,
    .get_sensitive = eg_widget_get_sensitive_default,
    /* Binding support */
    .binding_caps = &eg_entry_binding_caps,
    .bind_value = entry_bind_value,
    .bind_command = NULL,
    .unbind = entry_unbind,
    /* Event support - usa default */
    .events = NULL,
    /* Validation support */
    .validation = &eg_entry_validation_vtable
};

static void entry_destroy(EgWidget *widget) {
    EgEntry *entry = (EgEntry *)widget;
    if (entry == NULL) return;
    eg_free(entry);
}

/* Callback para changed */
static void entry_changed_callback(GtkEditable *editable, gpointer user_data) {
    (void)editable;
    EgEntry *entry = (EgEntry *)user_data;
    if (entry != NULL && entry->on_changed != NULL) {
        entry->on_changed((EgWidget *)entry, entry->changed_data);
    }
}

/* Callback para activate */
static void entry_activate_callback(GtkEntry *gtk_entry, gpointer user_data) {
    (void)gtk_entry;
    EgEntry *entry = (EgEntry *)user_data;
    if (entry != NULL && entry->on_activate != NULL) {
        entry->on_activate((EgWidget *)entry, entry->activate_data);
    }
}

EgEntry *eg_entry_new(void) {
    EgEntry *entry = EG_ALLOC(EgEntry);
    if (entry == NULL) return NULL;
    
    GtkWidget *gtk_entry = gtk_entry_new();
    if (gtk_entry == NULL) {
        eg_free(entry);
        return NULL;
    }
    
    eg_widget_init(&entry->base, EG_WIDGET_TYPE_ENTRY, gtk_entry, &eg_entry_vtable);
    
    entry->buffer = gtk_entry_get_buffer(GTK_ENTRY(gtk_entry));
    entry->on_changed = NULL;
    entry->changed_data = NULL;
    entry->on_activate = NULL;
    entry->activate_data = NULL;
    
    return entry;
}

static void entry_set_placeholder_internal(EgEntry *entry, const char *placeholder);

EgEntry *eg_entry_new_with_placeholder(const char *placeholder) {
    EgEntry *entry = eg_entry_new();
    if (entry != NULL && placeholder != NULL) {
        entry_set_placeholder_internal(entry, placeholder);
    }
    return entry;
}

static void entry_set_placeholder_internal(EgEntry *entry, const char *placeholder) {
    if (entry == NULL || entry->base.native == NULL) return;
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry->base.native), placeholder);
}

void eg_entry_free(EgEntry *entry) {
    entry_destroy((EgWidget *)entry);
}

void eg_entry_set_text(EgEntry *entry, const char *text) {
    if (entry == NULL || entry->buffer == NULL) return;
    gtk_entry_buffer_set_text(entry->buffer, text ? text : "", -1);
}

const char *eg_entry_get_text(EgEntry *entry) {
    if (entry == NULL || entry->buffer == NULL) return NULL;
    return gtk_entry_buffer_get_text(entry->buffer);
}

void eg_entry_set_placeholder(EgEntry *entry, const char *placeholder) {
    entry_set_placeholder_internal(entry, placeholder);
}

void eg_entry_set_max_length(EgEntry *entry, int max_length) {
    if (entry == NULL || entry->base.native == NULL) return;
    gtk_entry_set_max_length(GTK_ENTRY(entry->base.native), max_length);
}

void eg_entry_set_visibility(EgEntry *entry, bool visible) {
    if (entry == NULL || entry->base.native == NULL) return;
    gtk_entry_set_visibility(GTK_ENTRY(entry->base.native), visible);
}

void eg_entry_set_editable(EgEntry *entry, bool editable) {
    if (entry == NULL || entry->base.native == NULL) return;
    gtk_editable_set_editable(GTK_EDITABLE(entry->base.native), editable);
}

void eg_entry_on_changed(EgEntry *entry, EgCallback callback, void *user_data) {
    if (entry == NULL) return;
    
    entry->on_changed = callback;
    entry->changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(entry->base.native, "changed", 
                        G_CALLBACK(entry_changed_callback), entry);
    }
}

void eg_entry_on_activate(EgEntry *entry, EgCallback callback, void *user_data) {
    if (entry == NULL) return;
    
    entry->on_activate = callback;
    entry->activate_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(entry->base.native, "activate", 
                        G_CALLBACK(entry_activate_callback), entry);
    }
}

EgWidget *eg_entry_as_widget(EgEntry *entry) {
    return (EgWidget *)entry;
}

void *eg_entry_get_native(EgEntry *entry) {
    if (entry == NULL) return NULL;
    return entry->base.native;
}

/* ============================================
 * Binding Implementation
 * ============================================ */

typedef struct {
    EgEntry *entry;
    EgProperty *property;
    EgHandlerId property_handler;
    gulong gtk_signal_id;
    bool updating;
} EntryBindingData;

static void entry_binding_on_property_changed(EgProperty *property, void *user_data) {
    EntryBindingData *data = (EntryBindingData *)user_data;
    if (data == NULL || data->entry == NULL || data->updating) return;

    data->updating = true;
    const char *value = eg_property_get_string(property);
    eg_entry_set_text(data->entry, value ? value : "");
    data->updating = false;
}

static void entry_binding_on_changed(GtkEditable *editable, gpointer user_data) {
    (void)editable;
    EntryBindingData *data = (EntryBindingData *)user_data;
    if (data == NULL || data->entry == NULL || data->updating) return;

    data->updating = true;
    const char *text = eg_entry_get_text(data->entry);
    eg_property_set_string(data->property, text);
    data->updating = false;
}

static void *entry_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgEntry *entry = (EgEntry *)widget;
    if (entry == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    if (eg_property_get_type(ctx->property) != EG_PROPERTY_TYPE_STRING) {
        return NULL;
    }

    EntryBindingData *data = EG_ALLOC(EntryBindingData);
    if (data == NULL) return NULL;

    data->entry = entry;
    data->property = ctx->property;
    data->updating = false;

    /* Property -> Widget */
    data->property_handler = eg_property_on_changed(ctx->property,
                                                     entry_binding_on_property_changed, data);

    /* Widget -> Property (se two-way) */
    if (ctx->mode == EG_BINDING_MODE_TWO_WAY) {
        data->gtk_signal_id = g_signal_connect(widget->native, "changed",
                                                G_CALLBACK(entry_binding_on_changed), data);
    }

    /* Sincroniza valor inicial */
    entry_binding_on_property_changed(ctx->property, data);

    return data;
}

static void entry_unbind(EgWidget *widget, void *binding_data) {
    EntryBindingData *data = (EntryBindingData *)binding_data;
    if (data == NULL) return;

    if (data->property != NULL && data->property_handler != 0) {
        eg_property_disconnect(data->property, data->property_handler);
    }

    if (widget != NULL && widget->native != NULL && data->gtk_signal_id != 0) {
        g_signal_handler_disconnect(widget->native, data->gtk_signal_id);
    }

    eg_free(data);
}

/* ============================================
 * Validation Implementation
 * ============================================ */

static EgValue entry_get_value(EgWidget *widget) {
    EgValue value = { .type = EG_PROPERTY_TYPE_STRING, .string_val = NULL };

    EgEntry *entry = (EgEntry *)widget;
    if (entry != NULL && entry->buffer != NULL) {
        value.string_val = gtk_entry_buffer_get_text(entry->buffer);
    }

    return value;
}

static void entry_show_error(EgWidget *widget, const char *message) {
    if (widget == NULL || widget->native == NULL) return;

    /* Adiciona classe CSS de erro */
    gtk_widget_add_css_class(widget->native, "error");

    /* Mostra mensagem como tooltip */
    if (message != NULL) {
        gtk_widget_set_tooltip_text(widget->native, message);
    }
}

static void entry_clear_error(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return;

    /* Remove classe CSS de erro */
    gtk_widget_remove_css_class(widget->native, "error");

    /* Remove tooltip de erro */
    gtk_widget_set_tooltip_text(widget->native, NULL);
}
