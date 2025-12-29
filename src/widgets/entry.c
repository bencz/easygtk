/**
 * EasyGTK - Entry
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* Funções da vtable */
static void entry_destroy(EgWidget *widget);
static void *entry_get_native(EgWidget *widget);
static void entry_set_visible(EgWidget *widget, bool visible);
static bool entry_get_visible(EgWidget *widget);
static void entry_set_sensitive(EgWidget *widget, bool sensitive);
static bool entry_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_entry_vtable = {
    .type = EG_WIDGET_TYPE_ENTRY,
    .type_name = "EgEntry",
    .destroy = entry_destroy,
    .get_native = entry_get_native,
    .set_visible = entry_set_visible,
    .get_visible = entry_get_visible,
    .set_sensitive = entry_set_sensitive,
    .get_sensitive = entry_get_sensitive
};

static void entry_destroy(EgWidget *widget) {
    EgEntry *entry = (EgEntry *)widget;
    if (entry == NULL) return;
    eg_free(entry);
}

static void *entry_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void entry_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool entry_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void entry_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool entry_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
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
