/**
 * EasyGTK - CheckButton
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/check_button.h>

/* Funções da vtable */
static void check_button_destroy(EgWidget *widget);
static void *check_button_get_native(EgWidget *widget);
static void check_button_set_visible(EgWidget *widget, bool visible);
static bool check_button_get_visible(EgWidget *widget);
static void check_button_set_sensitive(EgWidget *widget, bool sensitive);
static bool check_button_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_check_button_vtable = {
    .type = EG_WIDGET_TYPE_CHECK_BUTTON,
    .type_name = "EgCheckButton",
    .destroy = check_button_destroy,
    .get_native = check_button_get_native,
    .set_visible = check_button_set_visible,
    .get_visible = check_button_get_visible,
    .set_sensitive = check_button_set_sensitive,
    .get_sensitive = check_button_get_sensitive
};

static void check_button_destroy(EgWidget *widget) {
    EgCheckButton *cb = (EgCheckButton *)widget;
    if (cb == NULL) return;
    eg_free(cb);
}

static void *check_button_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void check_button_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool check_button_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void check_button_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool check_button_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para toggled */
static void check_button_toggled_callback(GtkCheckButton *gtk_cb, gpointer user_data) {
    (void)gtk_cb;
    EgCheckButton *cb = (EgCheckButton *)user_data;
    if (cb != NULL && cb->on_toggled != NULL) {
        cb->on_toggled((EgWidget *)cb, cb->toggled_data);
    }
}

EgCheckButton *eg_check_button_new(const char *label) {
    EgCheckButton *cb = EG_ALLOC(EgCheckButton);
    if (cb == NULL) return NULL;
    
    GtkWidget *gtk_cb = gtk_check_button_new_with_label(label ? label : "");
    if (gtk_cb == NULL) {
        eg_free(cb);
        return NULL;
    }
    
    eg_widget_init(&cb->base, EG_WIDGET_TYPE_CHECK_BUTTON, gtk_cb, &eg_check_button_vtable);
    
    cb->on_toggled = NULL;
    cb->toggled_data = NULL;
    
    return cb;
}

EgCheckButton *eg_check_button_new_with_markup(const char *markup) {
    EgCheckButton *cb = eg_check_button_new(NULL);
    if (cb != NULL && markup != NULL) {
        gtk_check_button_set_use_underline(GTK_CHECK_BUTTON(cb->base.native), TRUE);
        gtk_check_button_set_label(GTK_CHECK_BUTTON(cb->base.native), markup);
    }
    return cb;
}

void eg_check_button_free(EgCheckButton *check_button) {
    check_button_destroy((EgWidget *)check_button);
}

void eg_check_button_set_active(EgCheckButton *check_button, bool active) {
    if (check_button == NULL || check_button->base.native == NULL) return;
    gtk_check_button_set_active(GTK_CHECK_BUTTON(check_button->base.native), active);
}

bool eg_check_button_get_active(EgCheckButton *check_button) {
    if (check_button == NULL || check_button->base.native == NULL) return false;
    return gtk_check_button_get_active(GTK_CHECK_BUTTON(check_button->base.native));
}

void eg_check_button_set_label(EgCheckButton *check_button, const char *label) {
    if (check_button == NULL || check_button->base.native == NULL) return;
    gtk_check_button_set_label(GTK_CHECK_BUTTON(check_button->base.native), label);
}

const char *eg_check_button_get_label(EgCheckButton *check_button) {
    if (check_button == NULL || check_button->base.native == NULL) return NULL;
    return gtk_check_button_get_label(GTK_CHECK_BUTTON(check_button->base.native));
}

void eg_check_button_on_toggled(EgCheckButton *check_button, EgCallback callback, void *user_data) {
    if (check_button == NULL) return;
    
    check_button->on_toggled = callback;
    check_button->toggled_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(check_button->base.native, "toggled", 
                        G_CALLBACK(check_button_toggled_callback), check_button);
    }
}

void eg_check_button_set_group(EgCheckButton *check_button, EgCheckButton *group) {
    if (check_button == NULL || check_button->base.native == NULL) return;
    
    GtkCheckButton *gtk_group = NULL;
    if (group != NULL && group->base.native != NULL) {
        gtk_group = GTK_CHECK_BUTTON(group->base.native);
    }
    
    gtk_check_button_set_group(GTK_CHECK_BUTTON(check_button->base.native), gtk_group);
}

EgWidget *eg_check_button_as_widget(EgCheckButton *check_button) {
    return (EgWidget *)check_button;
}

void *eg_check_button_get_native(EgCheckButton *check_button) {
    if (check_button == NULL) return NULL;
    return check_button->base.native;
}
