/**
 * EasyGTK - Model (MVVM)
 */

#include <string.h>
#include "internal/internal.h"
#include <easygtk/model.h>

/* Nó da lista de properties */
typedef struct PropertyNode {
    EgProperty *property;
    struct PropertyNode *next;
} PropertyNode;

/* Estrutura do Model */
struct EgModel {
    const EgModelVTable *vtable;
    PropertyNode *properties;
    void *user_data;
};

/* VTable padrão */
static void default_dispose(EgModel *model) { (void)model; }
static void default_init(EgModel *model) { (void)model; }
static bool default_validate(EgModel *model) { (void)model; return true; }

static const EgModelVTable default_vtable = {
    .type_name = "EgModel",
    .dispose = default_dispose,
    .init = default_init,
    .validate = default_validate
};

/* ============================================
 * Model
 * ============================================ */

EgModel *eg_model_new(void) {
    return eg_model_new_with_vtable(&default_vtable, sizeof(EgModel));
}

EgModel *eg_model_new_with_vtable(const EgModelVTable *vtable, size_t instance_size) {
    if (instance_size < sizeof(EgModel)) {
        instance_size = sizeof(EgModel);
    }

    EgModel *model = (EgModel *)eg_alloc(instance_size);
    if (model == NULL) return NULL;

    model->vtable = vtable ? vtable : &default_vtable;
    model->properties = NULL;
    model->user_data = NULL;

    /* Chama init da vtable */
    if (model->vtable->init != NULL) {
        model->vtable->init(model);
    }

    return model;
}

void eg_model_free(EgModel *model) {
    if (model == NULL) return;

    /* Chama dispose da vtable */
    if (model->vtable != NULL && model->vtable->dispose != NULL) {
        model->vtable->dispose(model);
    }

    /* Libera properties */
    PropertyNode *node = model->properties;
    while (node != NULL) {
        PropertyNode *next = node->next;
        eg_property_free(node->property);
        eg_free(node);
        node = next;
    }

    eg_free(model);
}

bool eg_model_add_property(EgModel *model, EgProperty *property) {
    if (model == NULL || property == NULL) return false;

    PropertyNode *node = EG_ALLOC(PropertyNode);
    if (node == NULL) return false;

    node->property = property;
    node->next = model->properties;
    model->properties = node;

    return true;
}

EgProperty *eg_model_get_property(EgModel *model, const char *name) {
    if (model == NULL || name == NULL) return NULL;

    PropertyNode *node = model->properties;
    while (node != NULL) {
        const char *prop_name = eg_property_get_name(node->property);
        if (prop_name != NULL && strcmp(prop_name, name) == 0) {
            return node->property;
        }
        node = node->next;
    }

    return NULL;
}

bool eg_model_set_int(EgModel *model, const char *name, int value) {
    EgProperty *prop = eg_model_get_property(model, name);
    if (prop == NULL) return false;
    eg_property_set_int(prop, value);
    return true;
}

int eg_model_get_int(EgModel *model, const char *name) {
    EgProperty *prop = eg_model_get_property(model, name);
    return eg_property_get_int(prop);
}

bool eg_model_set_string(EgModel *model, const char *name, const char *value) {
    EgProperty *prop = eg_model_get_property(model, name);
    if (prop == NULL) return false;
    eg_property_set_string(prop, value);
    return true;
}

const char *eg_model_get_string(EgModel *model, const char *name) {
    EgProperty *prop = eg_model_get_property(model, name);
    return eg_property_get_string(prop);
}

bool eg_model_set_bool(EgModel *model, const char *name, bool value) {
    EgProperty *prop = eg_model_get_property(model, name);
    if (prop == NULL) return false;
    eg_property_set_bool(prop, value);
    return true;
}

bool eg_model_get_bool(EgModel *model, const char *name) {
    EgProperty *prop = eg_model_get_property(model, name);
    return eg_property_get_bool(prop);
}

bool eg_model_set_double(EgModel *model, const char *name, double value) {
    EgProperty *prop = eg_model_get_property(model, name);
    if (prop == NULL) return false;
    eg_property_set_double(prop, value);
    return true;
}

double eg_model_get_double(EgModel *model, const char *name) {
    EgProperty *prop = eg_model_get_property(model, name);
    return eg_property_get_double(prop);
}

bool eg_model_validate(EgModel *model) {
    if (model == NULL) return false;
    if (model->vtable == NULL || model->vtable->validate == NULL) return true;
    return model->vtable->validate(model);
}

void *eg_model_get_data(EgModel *model) {
    if (model == NULL) return NULL;
    /* Para Models derivados, os dados extras ficam após a struct base */
    return (char *)model + sizeof(EgModel);
}
