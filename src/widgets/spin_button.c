/**
 * EasyGTK - SpinButton
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"
#include <easygtk/spin_button.h>

/* Funções da vtable */
static void spin_button_destroy(EgWidget *widget);

/* Funções de binding */
static void *spin_button_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void spin_button_unbind(EgWidget *widget, void *binding_data);

/* Funções de validação */
static EgValue spin_button_get_value_for_validation(EgWidget *widget);
static void spin_button_show_error(EgWidget *widget, const char *message);
static void spin_button_clear_error(EgWidget *widget);

/* Capacidades de binding */
static const EgBindingCapabilities eg_spin_button_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_DOUBLE,  /* Aceita int ou double */
    .default_mode = EG_BINDING_MODE_TWO_WAY,
    .supports_command = false
};

/* Capacidades de validação */
static const EgValidationCapabilities eg_spin_button_validation_caps = {
    .value_type = EG_PROPERTY_TYPE_DOUBLE,
    .supports_inline_error = true,
    .supports_error_style = true
};

/* VTable de validação */
static const EgValidationVTable eg_spin_button_validation_vtable = {
    .caps = &eg_spin_button_validation_caps,
    .get_value = spin_button_get_value_for_validation,
    .show_error = spin_button_show_error,
    .clear_error = spin_button_clear_error
};

const EgWidgetVTable eg_spin_button_vtable = {
    .type = EG_WIDGET_TYPE_SPIN_BUTTON,
    .type_name = "EgSpinButton",
    .destroy = spin_button_destroy,
    .get_native = eg_widget_get_native_default,
    .set_visible = eg_widget_set_visible_default,
    .get_visible = eg_widget_get_visible_default,
    .set_sensitive = eg_widget_set_sensitive_default,
    .get_sensitive = eg_widget_get_sensitive_default,
    /* Binding support */
    .binding_caps = &eg_spin_button_binding_caps,
    .bind_value = spin_button_bind_value,
    .bind_command = NULL,
    .unbind = spin_button_unbind,
    /* Event support - usa default */
    .events = NULL,
    /* Validation support */
    .validation = &eg_spin_button_validation_vtable
};

static void spin_button_destroy(EgWidget *widget) {
    EgSpinButton *spin = (EgSpinButton *)widget;
    if (spin == NULL) return;
    eg_free(spin);
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

/* ============================================
 * Binding Implementation
 * ============================================ */

typedef struct {
    EgSpinButton *spin_button;
    EgProperty *property;
    EgPropertyType prop_type;
    EgHandlerId property_handler;
    gulong gtk_signal_id;
    bool updating;
} SpinButtonBindingData;

static void spin_button_binding_on_property_changed(EgProperty *property, void *user_data) {
    SpinButtonBindingData *data = (SpinButtonBindingData *)user_data;
    if (data == NULL || data->spin_button == NULL || data->updating) return;

    data->updating = true;
    double value;
    if (data->prop_type == EG_PROPERTY_TYPE_INT) {
        value = (double)eg_property_get_int(property);
    } else {
        value = eg_property_get_double(property);
    }
    eg_spin_button_set_value(data->spin_button, value);
    data->updating = false;
}

static void spin_button_binding_on_value_changed(GtkSpinButton *gtk_spin, gpointer user_data) {
    (void)gtk_spin;
    SpinButtonBindingData *data = (SpinButtonBindingData *)user_data;
    if (data == NULL || data->spin_button == NULL || data->updating) return;

    data->updating = true;
    if (data->prop_type == EG_PROPERTY_TYPE_INT) {
        int value = eg_spin_button_get_value_as_int(data->spin_button);
        eg_property_set_int(data->property, value);
    } else {
        double value = eg_spin_button_get_value(data->spin_button);
        eg_property_set_double(data->property, value);
    }
    data->updating = false;
}

static void *spin_button_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgSpinButton *spin = (EgSpinButton *)widget;
    if (spin == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    EgPropertyType prop_type = eg_property_get_type(ctx->property);
    if (prop_type != EG_PROPERTY_TYPE_INT && prop_type != EG_PROPERTY_TYPE_DOUBLE) {
        return NULL;
    }

    SpinButtonBindingData *data = EG_ALLOC(SpinButtonBindingData);
    if (data == NULL) return NULL;

    data->spin_button = spin;
    data->property = ctx->property;
    data->prop_type = prop_type;
    data->updating = false;

    /* Property -> Widget */
    data->property_handler = eg_property_on_changed(ctx->property,
                                                     spin_button_binding_on_property_changed, data);

    /* Widget -> Property (se two-way) */
    if (ctx->mode == EG_BINDING_MODE_TWO_WAY) {
        data->gtk_signal_id = g_signal_connect(widget->native, "value-changed",
                                                G_CALLBACK(spin_button_binding_on_value_changed), data);
    }

    /* Sincroniza valor inicial */
    spin_button_binding_on_property_changed(ctx->property, data);

    return data;
}

static void spin_button_unbind(EgWidget *widget, void *binding_data) {
    SpinButtonBindingData *data = (SpinButtonBindingData *)binding_data;
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

static EgValue spin_button_get_value_for_validation(EgWidget *widget) {
    EgValue value = { .type = EG_PROPERTY_TYPE_DOUBLE, .double_val = 0.0 };

    EgSpinButton *spin = (EgSpinButton *)widget;
    if (spin != NULL && spin->base.native != NULL) {
        value.double_val = gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin->base.native));
    }

    return value;
}

static void spin_button_show_error(EgWidget *widget, const char *message) {
    if (widget == NULL || widget->native == NULL) return;

    /* Adiciona classe CSS de erro */
    gtk_widget_add_css_class(widget->native, "error");

    /* Mostra mensagem como tooltip */
    if (message != NULL) {
        gtk_widget_set_tooltip_text(widget->native, message);
    }
}

static void spin_button_clear_error(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return;

    /* Remove classe CSS de erro */
    gtk_widget_remove_css_class(widget->native, "error");

    /* Remove tooltip de erro */
    gtk_widget_set_tooltip_text(widget->native, NULL);
}
