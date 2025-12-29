/**
 * EasyGTK - SpinButton
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/spin_button.h>

/* Funções da vtable */
static void spin_button_destroy(EgWidget *widget);
static void *spin_button_get_native(EgWidget *widget);
static void spin_button_set_visible(EgWidget *widget, bool visible);
static bool spin_button_get_visible(EgWidget *widget);
static void spin_button_set_sensitive(EgWidget *widget, bool sensitive);
static bool spin_button_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_spin_button_vtable = {
    .type = EG_WIDGET_TYPE_SPIN_BUTTON,
    .type_name = "EgSpinButton",
    .destroy = spin_button_destroy,
    .get_native = spin_button_get_native,
    .set_visible = spin_button_set_visible,
    .get_visible = spin_button_get_visible,
    .set_sensitive = spin_button_set_sensitive,
    .get_sensitive = spin_button_get_sensitive
};

static void spin_button_destroy(EgWidget *widget) {
    EgSpinButton *spin = (EgSpinButton *)widget;
    if (spin == NULL) return;
    eg_free(spin);
}

static void *spin_button_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void spin_button_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool spin_button_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void spin_button_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool spin_button_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback GTK para value-changed */
static void gtk_value_changed_callback(GtkSpinButton *gtk_spin, gpointer user_data) {
    (void)gtk_spin;
    EgSpinButton *spin = (EgSpinButton *)user_data;
    if (spin != NULL && spin->on_value_changed != NULL) {
        spin->on_value_changed((EgWidget *)spin, spin->value_changed_data);
    }
}

EgSpinButton *eg_spin_button_new(double min, double max, double step) {
    EgSpinButton *spin = EG_ALLOC(EgSpinButton);
    if (spin == NULL) return NULL;
    
    GtkAdjustment *adjustment = gtk_adjustment_new(min, min, max, step, step * 10, 0);
    GtkWidget *gtk_spin = gtk_spin_button_new(adjustment, step, 2);
    
    if (gtk_spin == NULL) {
        eg_free(spin);
        return NULL;
    }
    
    eg_widget_init(&spin->base, EG_WIDGET_TYPE_SPIN_BUTTON, gtk_spin, &eg_spin_button_vtable);
    spin->on_value_changed = NULL;
    spin->value_changed_data = NULL;
    
    return spin;
}

EgSpinButton *eg_spin_button_new_int(int min, int max, int step) {
    EgSpinButton *spin = eg_spin_button_new((double)min, (double)max, (double)step);
    if (spin != NULL) {
        gtk_spin_button_set_digits(GTK_SPIN_BUTTON(spin->base.native), 0);
    }
    return spin;
}

void eg_spin_button_free(EgSpinButton *spin_button) {
    spin_button_destroy((EgWidget *)spin_button);
}

void eg_spin_button_set_value(EgSpinButton *spin_button, double value) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button->base.native), value);
}

double eg_spin_button_get_value(EgSpinButton *spin_button) {
    if (spin_button == NULL || spin_button->base.native == NULL) return 0.0;
    return gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_button->base.native));
}

int eg_spin_button_get_value_as_int(EgSpinButton *spin_button) {
    if (spin_button == NULL || spin_button->base.native == NULL) return 0;
    return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button->base.native));
}

void eg_spin_button_set_range(EgSpinButton *spin_button, double min, double max) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spin_button->base.native), min, max);
}

void eg_spin_button_set_increments(EgSpinButton *spin_button, double step, double page) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    gtk_spin_button_set_increments(GTK_SPIN_BUTTON(spin_button->base.native), step, page);
}

void eg_spin_button_set_digits(EgSpinButton *spin_button, unsigned int digits) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(spin_button->base.native), digits);
}

void eg_spin_button_set_wrap(EgSpinButton *spin_button, bool wrap) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(spin_button->base.native), wrap);
}

void eg_spin_button_on_value_changed(EgSpinButton *spin_button, EgCallback callback, void *user_data) {
    if (spin_button == NULL || spin_button->base.native == NULL) return;
    
    spin_button->on_value_changed = callback;
    spin_button->value_changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(spin_button->base.native, "value-changed",
                         G_CALLBACK(gtk_value_changed_callback), spin_button);
    }
}

EgWidget *eg_spin_button_as_widget(EgSpinButton *spin_button) {
    return (EgWidget *)spin_button;
}

void *eg_spin_button_get_native(EgSpinButton *spin_button) {
    if (spin_button == NULL) return NULL;
    return spin_button->base.native;
}
