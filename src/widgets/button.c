/**
 * EasyGTK - Botão
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"
#include <easygtk/viewmodel.h>

/* Funções da vtable */
static void button_destroy(EgWidget *widget);
static void *button_get_native(EgWidget *widget);
static void button_set_visible(EgWidget *widget, bool visible);
static bool button_get_visible(EgWidget *widget);
static void button_set_sensitive(EgWidget *widget, bool sensitive);
static bool button_get_sensitive(EgWidget *widget);

/* Funções de binding */
static void *button_bind_command(EgWidget *widget, EgCommand *command);
static void button_unbind(EgWidget *widget, void *binding_data);

/* Capacidades de binding */
static const EgBindingCapabilities eg_button_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_NONE,
    .default_mode = EG_BINDING_MODE_NONE,
    .supports_command = true
};

const EgWidgetVTable eg_button_vtable = {
    .type = EG_WIDGET_TYPE_BUTTON,
    .type_name = "EgButton",
    .destroy = button_destroy,
    .get_native = button_get_native,
    .set_visible = button_set_visible,
    .get_visible = button_get_visible,
    .set_sensitive = button_set_sensitive,
    .get_sensitive = button_get_sensitive,
    /* Binding support */
    .binding_caps = &eg_button_binding_caps,
    .bind_value = NULL,
    .bind_command = button_bind_command,
    .unbind = button_unbind
};

static void button_destroy(EgWidget *widget) {
    EgButton *button = (EgButton *)widget;
    if (button == NULL) return;
    eg_free(button);
}

static void *button_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void button_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool button_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void button_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool button_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para clique */
static void button_clicked_callback(GtkButton *gtk_button, gpointer user_data) {
    (void)gtk_button;
    EgButton *button = (EgButton *)user_data;
    if (button != NULL && button->on_click != NULL) {
        button->on_click((EgWidget *)button, button->click_data);
    }
}

EgButton *eg_button_new(const char *label) {
    EgButton *button = EG_ALLOC(EgButton);
    if (button == NULL) return NULL;
    
    GtkWidget *gtk_button = gtk_button_new_with_label(label ? label : "");
    if (gtk_button == NULL) {
        eg_free(button);
        return NULL;
    }
    
    eg_widget_init(&button->base, EG_WIDGET_TYPE_BUTTON, gtk_button, &eg_button_vtable);
    
    button->on_click = NULL;
    button->click_data = NULL;
    
    return button;
}

EgButton *eg_button_new_with_icon(const char *icon_name) {
    EgButton *button = EG_ALLOC(EgButton);
    if (button == NULL) return NULL;
    
    GtkWidget *gtk_button = gtk_button_new_from_icon_name(icon_name);
    if (gtk_button == NULL) {
        eg_free(button);
        return NULL;
    }
    
    eg_widget_init(&button->base, EG_WIDGET_TYPE_BUTTON, gtk_button, &eg_button_vtable);
    
    button->on_click = NULL;
    button->click_data = NULL;
    
    return button;
}

void eg_button_free(EgButton *button) {
    button_destroy((EgWidget *)button);
}

void eg_button_set_label(EgButton *button, const char *label) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_button_set_label(GTK_BUTTON(button->base.native), label);
}

const char *eg_button_get_label(EgButton *button) {
    if (button == NULL || button->base.native == NULL) return NULL;
    return gtk_button_get_label(GTK_BUTTON(button->base.native));
}

void eg_button_on_click(EgButton *button, EgCallback callback, void *user_data) {
    if (button == NULL) return;
    
    button->on_click = callback;
    button->click_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(button->base.native, "clicked", 
                        G_CALLBACK(button_clicked_callback), button);
    }
}

void eg_button_set_sensitive(EgButton *button, bool sensitive) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_widget_set_sensitive(button->base.native, sensitive);
}

bool eg_button_get_sensitive(EgButton *button) {
    if (button == NULL || button->base.native == NULL) return false;
    return gtk_widget_get_sensitive(button->base.native);
}

EgWidget *eg_button_as_widget(EgButton *button) {
    return (EgWidget *)button;
}

void *eg_button_get_native(EgButton *button) {
    if (button == NULL) return NULL;
    return button->base.native;
}

/* ============================================
 * Binding Implementation (Command)
 * ============================================ */

typedef struct {
    EgButton *button;
    EgCommand *command;
    EgSignal *can_execute_signal;
    EgHandlerId can_execute_handler;
    gulong gtk_signal_id;
} ButtonBindingData;

static void button_binding_on_can_execute_changed(void *sender, void *event_data, void *user_data) {
    (void)sender;
    (void)event_data;
    ButtonBindingData *data = (ButtonBindingData *)user_data;
    if (data == NULL || data->button == NULL || data->command == NULL) return;

    bool can_execute = eg_command_can_execute(data->command, NULL);
    eg_button_set_sensitive(data->button, can_execute);
}

static void button_binding_on_clicked(GtkButton *gtk_button, gpointer user_data) {
    (void)gtk_button;
    ButtonBindingData *data = (ButtonBindingData *)user_data;
    if (data == NULL || data->command == NULL) return;

    eg_command_execute(data->command, NULL);
}

static void *button_bind_command(EgWidget *widget, EgCommand *command) {
    EgButton *button = (EgButton *)widget;
    if (button == NULL || command == NULL) return NULL;

    ButtonBindingData *data = EG_ALLOC(ButtonBindingData);
    if (data == NULL) return NULL;

    data->button = button;
    data->command = command;

    /* Conecta ao sinal can-execute-changed */
    data->can_execute_signal = eg_command_get_can_execute_changed_signal(command);
    if (data->can_execute_signal != NULL) {
        data->can_execute_handler = eg_signal_connect(data->can_execute_signal,
                                                       button_binding_on_can_execute_changed, data);
    }

    /* Conecta ao clique do botão */
    data->gtk_signal_id = g_signal_connect(widget->native, "clicked",
                                            G_CALLBACK(button_binding_on_clicked), data);

    /* Sincroniza estado inicial */
    button_binding_on_can_execute_changed(NULL, NULL, data);

    return data;
}

static void button_unbind(EgWidget *widget, void *binding_data) {
    ButtonBindingData *data = (ButtonBindingData *)binding_data;
    if (data == NULL) return;

    if (data->can_execute_signal != NULL && data->can_execute_handler != 0) {
        eg_signal_disconnect(data->can_execute_signal, data->can_execute_handler);
    }

    if (widget != NULL && widget->native != NULL && data->gtk_signal_id != 0) {
        g_signal_handler_disconnect(widget->native, data->gtk_signal_id);
    }

    eg_free(data);
}
