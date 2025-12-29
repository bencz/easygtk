/**
 * EasyGTK - ViewModel (MVVM)
 */

#include <string.h>
#include "internal/internal.h"
#include <easygtk/viewmodel.h>

/* Nó da lista de properties */
typedef struct PropertyNode {
    EgProperty *property;
    struct PropertyNode *next;
} PropertyNode;

/* Nó da lista de commands */
typedef struct CommandNode {
    EgCommand *command;
    struct CommandNode *next;
} CommandNode;

/* Estrutura do ViewModel */
struct EgViewModel {
    const EgViewModelVTable *vtable;
    PropertyNode *properties;
    CommandNode *commands;
    void *user_data;
};

/* Estrutura do Command */
struct EgCommand {
    char *name;
    EgCommandExecute execute;
    EgCommandCanExecute can_execute;
    void *user_data;
    EgSignal *can_execute_changed;
};

/* VTable padrão */
static void default_dispose(EgViewModel *vm) { (void)vm; }
static void default_init(EgViewModel *vm) { (void)vm; }

static const EgViewModelVTable default_vtable = {
    .type_name = "EgViewModel",
    .dispose = default_dispose,
    .init = default_init
};

/* ============================================
 * ViewModel
 * ============================================ */

EgViewModel *eg_view_model_new(void) {
    return eg_view_model_new_with_vtable(&default_vtable, sizeof(EgViewModel));
}

EgViewModel *eg_view_model_new_with_vtable(const EgViewModelVTable *vtable, size_t instance_size) {
    if (instance_size < sizeof(EgViewModel)) {
        instance_size = sizeof(EgViewModel);
    }
    
    EgViewModel *vm = (EgViewModel *)eg_alloc(instance_size);
    if (vm == NULL) return NULL;
    
    vm->vtable = vtable ? vtable : &default_vtable;
    vm->properties = NULL;
    vm->commands = NULL;
    vm->user_data = NULL;
    
    /* Chama init da vtable */
    if (vm->vtable->init != NULL) {
        vm->vtable->init(vm);
    }
    
    return vm;
}

void eg_view_model_free(EgViewModel *vm) {
    if (vm == NULL) return;
    
    /* Chama dispose da vtable */
    if (vm->vtable != NULL && vm->vtable->dispose != NULL) {
        vm->vtable->dispose(vm);
    }
    
    /* Libera properties */
    PropertyNode *prop_node = vm->properties;
    while (prop_node != NULL) {
        PropertyNode *next = prop_node->next;
        eg_property_free(prop_node->property);
        eg_free(prop_node);
        prop_node = next;
    }
    
    /* Libera commands */
    CommandNode *cmd_node = vm->commands;
    while (cmd_node != NULL) {
        CommandNode *next = cmd_node->next;
        eg_command_free(cmd_node->command);
        eg_free(cmd_node);
        cmd_node = next;
    }
    
    eg_free(vm);
}

bool eg_view_model_add_property(EgViewModel *vm, EgProperty *property) {
    if (vm == NULL || property == NULL) return false;
    
    PropertyNode *node = EG_ALLOC(PropertyNode);
    if (node == NULL) return false;
    
    node->property = property;
    node->next = vm->properties;
    vm->properties = node;
    
    return true;
}

EgProperty *eg_view_model_get_property(EgViewModel *vm, const char *name) {
    if (vm == NULL || name == NULL) return NULL;
    
    PropertyNode *node = vm->properties;
    while (node != NULL) {
        const char *prop_name = eg_property_get_name(node->property);
        if (prop_name != NULL && strcmp(prop_name, name) == 0) {
            return node->property;
        }
        node = node->next;
    }
    
    return NULL;
}

bool eg_view_model_set_int(EgViewModel *vm, const char *name, int value) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    if (prop == NULL) return false;
    eg_property_set_int(prop, value);
    return true;
}

int eg_view_model_get_int(EgViewModel *vm, const char *name) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    return eg_property_get_int(prop);
}

bool eg_view_model_set_string(EgViewModel *vm, const char *name, const char *value) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    if (prop == NULL) return false;
    eg_property_set_string(prop, value);
    return true;
}

const char *eg_view_model_get_string(EgViewModel *vm, const char *name) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    return eg_property_get_string(prop);
}

bool eg_view_model_set_bool(EgViewModel *vm, const char *name, bool value) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    if (prop == NULL) return false;
    eg_property_set_bool(prop, value);
    return true;
}

bool eg_view_model_get_bool(EgViewModel *vm, const char *name) {
    EgProperty *prop = eg_view_model_get_property(vm, name);
    return eg_property_get_bool(prop);
}

void *eg_view_model_get_data(EgViewModel *vm) {
    if (vm == NULL) return NULL;
    /* Para ViewModels derivados, os dados extras ficam após a struct base */
    return (char *)vm + sizeof(EgViewModel);
}

/* ============================================
 * Commands
 * ============================================ */

EgCommand *eg_command_new(
    const char *name,
    EgCommandExecute execute,
    EgCommandCanExecute can_execute,
    void *user_data
) {
    if (execute == NULL) return NULL;
    
    EgCommand *cmd = EG_ALLOC(EgCommand);
    if (cmd == NULL) return NULL;
    
    cmd->name = eg_strdup(name);
    cmd->execute = execute;
    cmd->can_execute = can_execute;
    cmd->user_data = user_data;
    cmd->can_execute_changed = eg_signal_new("can_execute_changed");
    
    return cmd;
}

void eg_command_free(EgCommand *command) {
    if (command == NULL) return;
    
    eg_signal_free(command->can_execute_changed);
    eg_free(command->name);
    eg_free(command);
}

void eg_command_execute(EgCommand *command, void *parameter) {
    if (command == NULL || command->execute == NULL) return;
    
    /* Verifica se pode executar */
    if (command->can_execute != NULL) {
        if (!command->can_execute(command, parameter, command->user_data)) {
            return;
        }
    }
    
    command->execute(command, parameter, command->user_data);
}

bool eg_command_can_execute(EgCommand *command, void *parameter) {
    if (command == NULL) return false;
    if (command->can_execute == NULL) return true; /* Sem verificação = sempre pode */
    return command->can_execute(command, parameter, command->user_data);
}

void eg_command_raise_can_execute_changed(EgCommand *command) {
    if (command == NULL) return;
    eg_signal_emit(command->can_execute_changed, command, NULL);
}

EgSignal *eg_command_get_can_execute_changed_signal(EgCommand *command) {
    if (command == NULL) return NULL;
    return command->can_execute_changed;
}

bool eg_view_model_add_command(EgViewModel *vm, EgCommand *command) {
    if (vm == NULL || command == NULL) return false;
    
    CommandNode *node = EG_ALLOC(CommandNode);
    if (node == NULL) return false;
    
    node->command = command;
    node->next = vm->commands;
    vm->commands = node;
    
    return true;
}

EgCommand *eg_view_model_get_command(EgViewModel *vm, const char *name) {
    if (vm == NULL || name == NULL) return NULL;
    
    CommandNode *node = vm->commands;
    while (node != NULL) {
        if (node->command->name != NULL && strcmp(node->command->name, name) == 0) {
            return node->command;
        }
        node = node->next;
    }
    
    return NULL;
}
