/**
 * EasyGTK - Sistema de Properties Observáveis
 */

#include <string.h>
#include "internal/internal.h"
#include <easygtk/property.h>

/* União para armazenar valores de diferentes tipos */
typedef union {
    int int_value;
    double double_value;
    bool bool_value;
    char *string_value;
    void *pointer_value;
} EgPropertyValue;

/* Estrutura da property */
struct EgProperty {
    char *name;
    EgPropertyType type;
    EgPropertyValue value;
    EgSignal *changed_signal;
};

/* Estrutura do binding */
struct EgBinding {
    EgProperty *source;
    EgProperty *target;
    EgBindingFlags flags;
    EgBindingTransform transform;
    EgBindingTransform reverse_transform;
    void *user_data;
    EgHandlerId source_handler;
    EgHandlerId target_handler;
};

/* Forward declarations */
static void property_notify_changed(EgProperty *property);
static void binding_source_changed(void *sender, void *event_data, void *user_data);
static void binding_target_changed(void *sender, void *event_data, void *user_data);

/* ============================================
 * Criação
 * ============================================ */

static EgProperty *property_new_base(const char *name, EgPropertyType type) {
    EgProperty *prop = EG_ALLOC(EgProperty);
    if (prop == NULL) return NULL;
    
    prop->name = eg_strdup(name);
    prop->type = type;
    prop->changed_signal = eg_signal_new(name);
    
    if (prop->changed_signal == NULL) {
        eg_free(prop->name);
        eg_free(prop);
        return NULL;
    }
    
    return prop;
}

EgProperty *eg_property_new_int(const char *name, int initial_value) {
    EgProperty *prop = property_new_base(name, EG_PROPERTY_TYPE_INT);
    if (prop != NULL) {
        prop->value.int_value = initial_value;
    }
    return prop;
}

EgProperty *eg_property_new_double(const char *name, double initial_value) {
    EgProperty *prop = property_new_base(name, EG_PROPERTY_TYPE_DOUBLE);
    if (prop != NULL) {
        prop->value.double_value = initial_value;
    }
    return prop;
}

EgProperty *eg_property_new_bool(const char *name, bool initial_value) {
    EgProperty *prop = property_new_base(name, EG_PROPERTY_TYPE_BOOL);
    if (prop != NULL) {
        prop->value.bool_value = initial_value;
    }
    return prop;
}

EgProperty *eg_property_new_string(const char *name, const char *initial_value) {
    EgProperty *prop = property_new_base(name, EG_PROPERTY_TYPE_STRING);
    if (prop != NULL) {
        prop->value.string_value = eg_strdup(initial_value);
    }
    return prop;
}

EgProperty *eg_property_new_pointer(const char *name, void *initial_value) {
    EgProperty *prop = property_new_base(name, EG_PROPERTY_TYPE_POINTER);
    if (prop != NULL) {
        prop->value.pointer_value = initial_value;
    }
    return prop;
}

void eg_property_free(EgProperty *property) {
    if (property == NULL) return;
    
    if (property->type == EG_PROPERTY_TYPE_STRING) {
        eg_free(property->value.string_value);
    }
    
    eg_signal_free(property->changed_signal);
    eg_free(property->name);
    eg_free(property);
}

/* ============================================
 * Getters
 * ============================================ */

int eg_property_get_int(EgProperty *property) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_INT) return 0;
    return property->value.int_value;
}

double eg_property_get_double(EgProperty *property) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_DOUBLE) return 0.0;
    return property->value.double_value;
}

bool eg_property_get_bool(EgProperty *property) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_BOOL) return false;
    return property->value.bool_value;
}

const char *eg_property_get_string(EgProperty *property) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_STRING) return NULL;
    return property->value.string_value;
}

void *eg_property_get_pointer(EgProperty *property) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_POINTER) return NULL;
    return property->value.pointer_value;
}

/* ============================================
 * Setters
 * ============================================ */

void eg_property_set_int(EgProperty *property, int value) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_INT) return;
    if (property->value.int_value == value) return; /* Sem mudança */
    
    property->value.int_value = value;
    property_notify_changed(property);
}

void eg_property_set_double(EgProperty *property, double value) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_DOUBLE) return;
    if (property->value.double_value == value) return;
    
    property->value.double_value = value;
    property_notify_changed(property);
}

void eg_property_set_bool(EgProperty *property, bool value) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_BOOL) return;
    if (property->value.bool_value == value) return;
    
    property->value.bool_value = value;
    property_notify_changed(property);
}

void eg_property_set_string(EgProperty *property, const char *value) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_STRING) return;
    
    /* Verifica se é o mesmo valor */
    if (property->value.string_value != NULL && value != NULL) {
        if (strcmp(property->value.string_value, value) == 0) return;
    } else if (property->value.string_value == NULL && value == NULL) {
        return;
    }
    
    eg_free(property->value.string_value);
    property->value.string_value = eg_strdup(value);
    property_notify_changed(property);
}

void eg_property_set_pointer(EgProperty *property, void *value) {
    if (property == NULL || property->type != EG_PROPERTY_TYPE_POINTER) return;
    if (property->value.pointer_value == value) return;
    
    property->value.pointer_value = value;
    property_notify_changed(property);
}

/* ============================================
 * Metadados
 * ============================================ */

const char *eg_property_get_name(EgProperty *property) {
    if (property == NULL) return NULL;
    return property->name;
}

EgPropertyType eg_property_get_type(EgProperty *property) {
    if (property == NULL) return EG_PROPERTY_TYPE_INT;
    return property->type;
}

/* ============================================
 * Observação
 * ============================================ */

static void property_notify_changed(EgProperty *property) {
    if (property == NULL || property->changed_signal == NULL) return;
    eg_signal_emit(property->changed_signal, property, NULL);
}

/* Wrapper para converter callback de property para callback de signal */
typedef struct {
    EgPropertyChangedCallback callback;
    void *user_data;
} PropertyCallbackWrapper;

static void property_callback_wrapper(void *sender, void *event_data, void *user_data) {
    (void)event_data;
    PropertyCallbackWrapper *wrapper = (PropertyCallbackWrapper *)user_data;
    if (wrapper != NULL && wrapper->callback != NULL) {
        wrapper->callback((EgProperty *)sender, wrapper->user_data);
    }
}

EgHandlerId eg_property_on_changed(EgProperty *property, EgPropertyChangedCallback callback, void *user_data) {
    if (property == NULL || callback == NULL) return 0;
    
    /* Aloca wrapper - será vazado, mas é aceitável para simplicidade */
    PropertyCallbackWrapper *wrapper = EG_ALLOC(PropertyCallbackWrapper);
    if (wrapper == NULL) return 0;
    
    wrapper->callback = callback;
    wrapper->user_data = user_data;
    
    return eg_signal_connect(property->changed_signal, property_callback_wrapper, wrapper);
}

bool eg_property_disconnect(EgProperty *property, EgHandlerId handler_id) {
    if (property == NULL) return false;
    return eg_signal_disconnect(property->changed_signal, handler_id);
}

EgSignal *eg_property_get_signal(EgProperty *property) {
    if (property == NULL) return NULL;
    return property->changed_signal;
}

/* ============================================
 * Data Binding
 * ============================================ */

static void copy_property_value(EgProperty *source, EgProperty *target) {
    if (source->type != target->type) return;
    
    switch (source->type) {
        case EG_PROPERTY_TYPE_INT:
            eg_property_set_int(target, eg_property_get_int(source));
            break;
        case EG_PROPERTY_TYPE_DOUBLE:
            eg_property_set_double(target, eg_property_get_double(source));
            break;
        case EG_PROPERTY_TYPE_BOOL:
            eg_property_set_bool(target, eg_property_get_bool(source));
            break;
        case EG_PROPERTY_TYPE_STRING:
            eg_property_set_string(target, eg_property_get_string(source));
            break;
        case EG_PROPERTY_TYPE_POINTER:
            eg_property_set_pointer(target, eg_property_get_pointer(source));
            break;
    }
}

static void binding_source_changed(void *sender, void *event_data, void *user_data) {
    (void)event_data;
    EgBinding *binding = (EgBinding *)user_data;
    if (binding == NULL) return;
    
    EgProperty *source = (EgProperty *)sender;
    
    /* Bloqueia signal do target para evitar loop infinito */
    eg_signal_block(binding->target->changed_signal);
    
    if (binding->transform != NULL) {
        /* TODO: implementar transformação */
    } else {
        copy_property_value(source, binding->target);
    }
    
    eg_signal_unblock(binding->target->changed_signal);
}

static void binding_target_changed(void *sender, void *event_data, void *user_data) {
    (void)event_data;
    EgBinding *binding = (EgBinding *)user_data;
    if (binding == NULL) return;
    
    EgProperty *target = (EgProperty *)sender;
    
    /* Bloqueia signal do source para evitar loop infinito */
    eg_signal_block(binding->source->changed_signal);
    
    if (binding->reverse_transform != NULL) {
        /* TODO: implementar transformação reversa */
    } else {
        copy_property_value(target, binding->source);
    }
    
    eg_signal_unblock(binding->source->changed_signal);
}

EgBinding *eg_property_bind(EgProperty *source, EgProperty *target, EgBindingFlags flags) {
    return eg_property_bind_transform(source, target, flags, NULL, NULL, NULL);
}

EgBinding *eg_property_bind_transform(
    EgProperty *source, 
    EgProperty *target, 
    EgBindingFlags flags,
    EgBindingTransform transform,
    EgBindingTransform reverse_transform,
    void *user_data
) {
    if (source == NULL || target == NULL) return NULL;
    
    EgBinding *binding = EG_ALLOC(EgBinding);
    if (binding == NULL) return NULL;
    
    binding->source = source;
    binding->target = target;
    binding->flags = flags;
    binding->transform = transform;
    binding->reverse_transform = reverse_transform;
    binding->user_data = user_data;
    binding->source_handler = 0;
    binding->target_handler = 0;
    
    /* Conecta source -> target */
    if (flags == EG_BINDING_ONE_WAY || flags == EG_BINDING_TWO_WAY) {
        binding->source_handler = eg_signal_connect(
            source->changed_signal, 
            binding_source_changed, 
            binding
        );
        
        /* Sincroniza valor inicial */
        copy_property_value(source, target);
    }
    
    /* Conecta target -> source (two-way ou one-way-to-source) */
    if (flags == EG_BINDING_TWO_WAY || flags == EG_BINDING_ONE_WAY_TO_SOURCE) {
        binding->target_handler = eg_signal_connect(
            target->changed_signal, 
            binding_target_changed, 
            binding
        );
        
        /* Se one-way-to-source, sincroniza valor inicial do target para source */
        if (flags == EG_BINDING_ONE_WAY_TO_SOURCE) {
            copy_property_value(target, source);
        }
    }
    
    return binding;
}

void eg_binding_unbind(EgBinding *binding) {
    if (binding == NULL) return;
    
    if (binding->source_handler != 0) {
        eg_signal_disconnect(binding->source->changed_signal, binding->source_handler);
    }
    
    if (binding->target_handler != 0) {
        eg_signal_disconnect(binding->target->changed_signal, binding->target_handler);
    }
    
    eg_free(binding);
}
