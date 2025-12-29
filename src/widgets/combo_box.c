/**
 * EasyGTK - ComboBox (Dropdown)
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/combo_box.h>

/* Funções da vtable */
static void combo_box_destroy(EgWidget *widget);
static void *combo_box_get_native(EgWidget *widget);
static void combo_box_set_visible(EgWidget *widget, bool visible);
static bool combo_box_get_visible(EgWidget *widget);
static void combo_box_set_sensitive(EgWidget *widget, bool sensitive);
static bool combo_box_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_combo_box_vtable = {
    .type = EG_WIDGET_TYPE_COMBO_BOX,
    .type_name = "EgComboBox",
    .destroy = combo_box_destroy,
    .get_native = combo_box_get_native,
    .set_visible = combo_box_set_visible,
    .get_visible = combo_box_get_visible,
    .set_sensitive = combo_box_set_sensitive,
    .get_sensitive = combo_box_get_sensitive
};

static void combo_box_destroy(EgWidget *widget) {
    EgComboBox *cb = (EgComboBox *)widget;
    if (cb == NULL) return;
    eg_free(cb);
}

static void *combo_box_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void combo_box_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool combo_box_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void combo_box_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool combo_box_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para notify::selected */
static void combo_box_selected_callback(GObject *gobject, GParamSpec *pspec, gpointer user_data) {
    (void)gobject;
    (void)pspec;
    EgComboBox *cb = (EgComboBox *)user_data;
    if (cb != NULL && cb->on_changed != NULL) {
        cb->on_changed((EgWidget *)cb, cb->changed_data);
    }
}

EgComboBox *eg_combo_box_new(void) {
    EgComboBox *cb = EG_ALLOC(EgComboBox);
    if (cb == NULL) return NULL;
    
    cb->model = gtk_string_list_new(NULL);
    GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(cb->model), NULL);
    
    if (dropdown == NULL) {
        g_object_unref(cb->model);
        eg_free(cb);
        return NULL;
    }
    
    eg_widget_init(&cb->base, EG_WIDGET_TYPE_COMBO_BOX, dropdown, &eg_combo_box_vtable);
    
    cb->on_changed = NULL;
    cb->changed_data = NULL;
    
    /* Seleciona nenhum item inicialmente */
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown), GTK_INVALID_LIST_POSITION);
    
    return cb;
}

EgComboBox *eg_combo_box_new_with_items(const char *const *items) {
    EgComboBox *cb = EG_ALLOC(EgComboBox);
    if (cb == NULL) return NULL;
    
    cb->model = gtk_string_list_new(items);
    GtkWidget *dropdown = gtk_drop_down_new(G_LIST_MODEL(cb->model), NULL);
    
    if (dropdown == NULL) {
        g_object_unref(cb->model);
        eg_free(cb);
        return NULL;
    }
    
    eg_widget_init(&cb->base, EG_WIDGET_TYPE_COMBO_BOX, dropdown, &eg_combo_box_vtable);
    
    cb->on_changed = NULL;
    cb->changed_data = NULL;
    
    return cb;
}

void eg_combo_box_free(EgComboBox *combo_box) {
    combo_box_destroy((EgWidget *)combo_box);
}

void eg_combo_box_append(EgComboBox *combo_box, const char *text) {
    if (combo_box == NULL || combo_box->model == NULL || text == NULL) return;
    gtk_string_list_append(combo_box->model, text);
}

void eg_combo_box_clear(EgComboBox *combo_box) {
    if (combo_box == NULL || combo_box->model == NULL) return;
    
    /* Remove todos os itens */
    guint n = g_list_model_get_n_items(G_LIST_MODEL(combo_box->model));
    if (n > 0) {
        gtk_string_list_splice(combo_box->model, 0, n, NULL);
    }
}

void eg_combo_box_set_active(EgComboBox *combo_box, int index) {
    if (combo_box == NULL || combo_box->base.native == NULL) return;
    
    guint pos = (index < 0) ? GTK_INVALID_LIST_POSITION : (guint)index;
    gtk_drop_down_set_selected(GTK_DROP_DOWN(combo_box->base.native), pos);
}

int eg_combo_box_get_active(EgComboBox *combo_box) {
    if (combo_box == NULL || combo_box->base.native == NULL) return -1;
    
    guint pos = gtk_drop_down_get_selected(GTK_DROP_DOWN(combo_box->base.native));
    if (pos == GTK_INVALID_LIST_POSITION) return -1;
    return (int)pos;
}

const char *eg_combo_box_get_active_text(EgComboBox *combo_box) {
    if (combo_box == NULL || combo_box->base.native == NULL) return NULL;
    
    GtkStringObject *obj = GTK_STRING_OBJECT(
        gtk_drop_down_get_selected_item(GTK_DROP_DOWN(combo_box->base.native))
    );
    
    if (obj == NULL) return NULL;
    return gtk_string_object_get_string(obj);
}

void eg_combo_box_on_changed(EgComboBox *combo_box, EgCallback callback, void *user_data) {
    if (combo_box == NULL) return;
    
    combo_box->on_changed = callback;
    combo_box->changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(combo_box->base.native, "notify::selected", 
                        G_CALLBACK(combo_box_selected_callback), combo_box);
    }
}

EgWidget *eg_combo_box_as_widget(EgComboBox *combo_box) {
    return (EgWidget *)combo_box;
}

void *eg_combo_box_get_native(EgComboBox *combo_box) {
    if (combo_box == NULL) return NULL;
    return combo_box->base.native;
}
