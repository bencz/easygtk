/**
 * EasyGTK - Binding Declarativo Widget <-> ViewModel
 */

#include "internal/internal.h"
#include <easygtk/binding.h>
#include <easygtk/entry.h>
#include <easygtk/label.h>
#include <easygtk/check_button.h>
#include <easygtk/switch.h>
#include <easygtk/spin_button.h>
#include <easygtk/scale.h>
#include <easygtk/button.h>
#include <stdio.h>

/* ============================================
 * Entry <-> String Property (Two-Way)
 * ============================================ */

typedef struct {
    EgEntry *entry;
    EgProperty *property;
} EntryBindingData;

static void entry_on_property_changed(EgProperty *property, void *user_data) {
    EntryBindingData *data = (EntryBindingData *)user_data;
    const char *value = eg_property_get_string(property);
    eg_entry_set_text(data->entry, value ? value : "");
}

static void entry_on_widget_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    EntryBindingData *data = (EntryBindingData *)user_data;
    const char *text = eg_entry_get_text(data->entry);
    eg_property_set_string(data->property, text);
}

bool eg_bind_entry_text(EgEntry *entry, EgViewModel *vm, const char *property_name) {
    if (entry == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    EntryBindingData *data = EG_ALLOC(EntryBindingData);
    if (data == NULL) return false;

    data->entry = entry;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, entry_on_property_changed, data);

    /* Widget -> Property */
    eg_entry_on_changed(entry, entry_on_widget_changed, data);

    /* Sincroniza valor inicial */
    entry_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Label <-> String Property (One-Way)
 * ============================================ */

typedef struct {
    EgLabel *label;
    EgProperty *property;
} LabelBindingData;

static void label_on_property_changed(EgProperty *property, void *user_data) {
    LabelBindingData *data = (LabelBindingData *)user_data;

    /* Suporta diferentes tipos de property */
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

bool eg_bind_label_text(EgLabel *label, EgViewModel *vm, const char *property_name) {
    if (label == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    LabelBindingData *data = EG_ALLOC(LabelBindingData);
    if (data == NULL) return false;

    data->label = label;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, label_on_property_changed, data);

    /* Sincroniza valor inicial */
    label_on_property_changed(property, data);

    return true;
}

/* ============================================
 * CheckButton <-> Bool Property (Two-Way)
 * ============================================ */

typedef struct {
    EgCheckButton *check_button;
    EgProperty *property;
} CheckButtonBindingData;

static void check_button_on_property_changed(EgProperty *property, void *user_data) {
    CheckButtonBindingData *data = (CheckButtonBindingData *)user_data;
    bool value = eg_property_get_bool(property);
    eg_check_button_set_active(data->check_button, value);
}

static void check_button_on_widget_toggled(EgWidget *widget, void *user_data) {
    (void)widget;
    CheckButtonBindingData *data = (CheckButtonBindingData *)user_data;
    bool active = eg_check_button_get_active(data->check_button);
    eg_property_set_bool(data->property, active);
}

bool eg_bind_check_button_active(EgCheckButton *check_button, EgViewModel *vm, const char *property_name) {
    if (check_button == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    CheckButtonBindingData *data = EG_ALLOC(CheckButtonBindingData);
    if (data == NULL) return false;

    data->check_button = check_button;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, check_button_on_property_changed, data);

    /* Widget -> Property */
    eg_check_button_on_toggled(check_button, check_button_on_widget_toggled, data);

    /* Sincroniza valor inicial */
    check_button_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Switch <-> Bool Property (Two-Way)
 * ============================================ */

typedef struct {
    EgSwitch *switch_widget;
    EgProperty *property;
} SwitchBindingData;

static void switch_on_property_changed(EgProperty *property, void *user_data) {
    SwitchBindingData *data = (SwitchBindingData *)user_data;
    bool value = eg_property_get_bool(property);
    eg_switch_set_active(data->switch_widget, value);
}

static void switch_on_widget_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    SwitchBindingData *data = (SwitchBindingData *)user_data;
    bool active = eg_switch_get_active(data->switch_widget);
    eg_property_set_bool(data->property, active);
}

bool eg_bind_switch_active(EgSwitch *switch_widget, EgViewModel *vm, const char *property_name) {
    if (switch_widget == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    SwitchBindingData *data = EG_ALLOC(SwitchBindingData);
    if (data == NULL) return false;

    data->switch_widget = switch_widget;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, switch_on_property_changed, data);

    /* Widget -> Property */
    eg_switch_on_state_changed(switch_widget, switch_on_widget_changed, data);

    /* Sincroniza valor inicial */
    switch_on_property_changed(property, data);

    return true;
}

/* ============================================
 * SpinButton <-> Double Property (Two-Way)
 * ============================================ */

typedef struct {
    EgSpinButton *spin_button;
    EgProperty *property;
} SpinButtonBindingData;

static void spin_button_on_property_changed(EgProperty *property, void *user_data) {
    SpinButtonBindingData *data = (SpinButtonBindingData *)user_data;

    /* Suporta int ou double */
    EgPropertyType type = eg_property_get_type(property);
    double value = 0.0;

    if (type == EG_PROPERTY_TYPE_INT) {
        value = (double)eg_property_get_int(property);
    } else if (type == EG_PROPERTY_TYPE_DOUBLE) {
        value = eg_property_get_double(property);
    }

    eg_spin_button_set_value(data->spin_button, value);
}

static void spin_button_on_widget_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    SpinButtonBindingData *data = (SpinButtonBindingData *)user_data;
    double value = eg_spin_button_get_value(data->spin_button);

    /* Suporta int ou double */
    EgPropertyType type = eg_property_get_type(data->property);

    if (type == EG_PROPERTY_TYPE_INT) {
        eg_property_set_int(data->property, (int)value);
    } else if (type == EG_PROPERTY_TYPE_DOUBLE) {
        eg_property_set_double(data->property, value);
    }
}

bool eg_bind_spin_button_value(EgSpinButton *spin_button, EgViewModel *vm, const char *property_name) {
    if (spin_button == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    SpinButtonBindingData *data = EG_ALLOC(SpinButtonBindingData);
    if (data == NULL) return false;

    data->spin_button = spin_button;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, spin_button_on_property_changed, data);

    /* Widget -> Property */
    eg_spin_button_on_value_changed(spin_button, spin_button_on_widget_changed, data);

    /* Sincroniza valor inicial */
    spin_button_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Scale <-> Double Property (Two-Way)
 * ============================================ */

typedef struct {
    EgScale *scale;
    EgProperty *property;
} ScaleBindingData;

static void scale_on_property_changed(EgProperty *property, void *user_data) {
    ScaleBindingData *data = (ScaleBindingData *)user_data;
    double value = eg_property_get_double(property);
    eg_scale_set_value(data->scale, value);
}

static void scale_on_widget_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    ScaleBindingData *data = (ScaleBindingData *)user_data;
    double value = eg_scale_get_value(data->scale);
    eg_property_set_double(data->property, value);
}

bool eg_bind_scale_value(EgScale *scale, EgViewModel *vm, const char *property_name) {
    if (scale == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    ScaleBindingData *data = EG_ALLOC(ScaleBindingData);
    if (data == NULL) return false;

    data->scale = scale;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, scale_on_property_changed, data);

    /* Widget -> Property */
    eg_scale_on_value_changed(scale, scale_on_widget_changed, data);

    /* Sincroniza valor inicial */
    scale_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Widget Visible <-> Bool Property
 * ============================================ */

typedef struct {
    EgWidget *widget;
    EgProperty *property;
} WidgetVisibleBindingData;

static void widget_visible_on_property_changed(EgProperty *property, void *user_data) {
    WidgetVisibleBindingData *data = (WidgetVisibleBindingData *)user_data;
    bool visible = eg_property_get_bool(property);
    gtk_widget_set_visible(data->widget->native, visible);
}

bool eg_bind_widget_visible(EgWidget *widget, EgViewModel *vm, const char *property_name) {
    if (widget == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    WidgetVisibleBindingData *data = EG_ALLOC(WidgetVisibleBindingData);
    if (data == NULL) return false;

    data->widget = widget;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, widget_visible_on_property_changed, data);

    /* Sincroniza valor inicial */
    widget_visible_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Widget Sensitive <-> Bool Property
 * ============================================ */

typedef struct {
    EgWidget *widget;
    EgProperty *property;
} WidgetSensitiveBindingData;

static void widget_sensitive_on_property_changed(EgProperty *property, void *user_data) {
    WidgetSensitiveBindingData *data = (WidgetSensitiveBindingData *)user_data;
    bool sensitive = eg_property_get_bool(property);
    gtk_widget_set_sensitive(data->widget->native, sensitive);
}

bool eg_bind_widget_sensitive(EgWidget *widget, EgViewModel *vm, const char *property_name) {
    if (widget == NULL || vm == NULL || property_name == NULL) return false;

    EgProperty *property = eg_view_model_get_property(vm, property_name);
    if (property == NULL) return false;

    WidgetSensitiveBindingData *data = EG_ALLOC(WidgetSensitiveBindingData);
    if (data == NULL) return false;

    data->widget = widget;
    data->property = property;

    /* Property -> Widget */
    eg_property_on_changed(property, widget_sensitive_on_property_changed, data);

    /* Sincroniza valor inicial */
    widget_sensitive_on_property_changed(property, data);

    return true;
}

/* ============================================
 * Button <-> Command
 * ============================================ */

typedef struct {
    EgButton *button;
    EgCommand *command;
} ButtonCommandBindingData;

static void button_command_on_click(EgWidget *widget, void *user_data) {
    (void)widget;
    ButtonCommandBindingData *data = (ButtonCommandBindingData *)user_data;
    eg_command_execute(data->command, NULL);
}

static void button_command_on_can_execute_changed(void *sender, void *event_data, void *user_data) {
    (void)sender;
    (void)event_data;
    ButtonCommandBindingData *data = (ButtonCommandBindingData *)user_data;

    bool can_execute = eg_command_can_execute(data->command, NULL);
    eg_button_set_sensitive(data->button, can_execute);
}

bool eg_bind_button_command(EgButton *button, EgViewModel *vm, const char *command_name) {
    if (button == NULL || vm == NULL || command_name == NULL) return false;

    EgCommand *command = eg_view_model_get_command(vm, command_name);
    if (command == NULL) return false;

    ButtonCommandBindingData *data = EG_ALLOC(ButtonCommandBindingData);
    if (data == NULL) return false;

    data->button = button;
    data->command = command;

    /* Click -> Execute */
    eg_button_on_click(button, button_command_on_click, data);

    /* CanExecute -> Sensitive */
    EgSignal *can_execute_signal = eg_command_get_can_execute_changed_signal(command);
    if (can_execute_signal != NULL) {
        eg_signal_connect(can_execute_signal, button_command_on_can_execute_changed, data);
    }

    /* Sincroniza estado inicial */
    button_command_on_can_execute_changed(NULL, NULL, data);

    return true;
}
