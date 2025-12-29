/**
 * EasyGTK - ViewModel (MVVM)
 * 
 * Base para implementação do padrão MVVM.
 * ViewModels contêm properties observáveis e commands.
 */

#ifndef EASYGTK_VIEWMODEL_H
#define EASYGTK_VIEWMODEL_H

#include "types.h"
#include "property.h"
#include "signal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipos opacos */
typedef struct EgViewModel EgViewModel;
typedef struct EgCommand EgCommand;

/* VTable para ViewModel - permite extensão */
typedef struct EgViewModelVTable {
    const char *type_name;
    void (*dispose)(EgViewModel *vm);
    void (*init)(EgViewModel *vm);
} EgViewModelVTable;

/* Callback para commands */
typedef void (*EgCommandExecute)(EgCommand *command, void *parameter, void *user_data);
typedef bool (*EgCommandCanExecute)(EgCommand *command, void *parameter, void *user_data);

/* ============================================
 * ViewModel
 * ============================================ */

/**
 * Cria um novo ViewModel base.
 * 
 * @return Ponteiro para o ViewModel ou NULL
 */
EgViewModel *eg_view_model_new(void);

/**
 * Cria um ViewModel com vtable customizada.
 * 
 * @param vtable VTable com funções customizadas
 * @param instance_size Tamanho da struct derivada
 * @return Ponteiro para o ViewModel ou NULL
 */
EgViewModel *eg_view_model_new_with_vtable(const EgViewModelVTable *vtable, size_t instance_size);

/**
 * Libera o ViewModel e todas suas properties.
 */
void eg_view_model_free(EgViewModel *vm);

/**
 * Adiciona uma property ao ViewModel.
 * O ViewModel assume ownership da property.
 * 
 * @param vm ViewModel
 * @param property Property a adicionar
 * @return true se adicionou com sucesso
 */
bool eg_view_model_add_property(EgViewModel *vm, EgProperty *property);

/**
 * Obtém uma property pelo nome.
 * 
 * @param vm ViewModel
 * @param name Nome da property
 * @return Ponteiro para a property ou NULL
 */
EgProperty *eg_view_model_get_property(EgViewModel *vm, const char *name);

/**
 * Atalho: define valor int de uma property.
 */
bool eg_view_model_set_int(EgViewModel *vm, const char *name, int value);

/**
 * Atalho: obtém valor int de uma property.
 */
int eg_view_model_get_int(EgViewModel *vm, const char *name);

/**
 * Atalho: define valor string de uma property.
 */
bool eg_view_model_set_string(EgViewModel *vm, const char *name, const char *value);

/**
 * Atalho: obtém valor string de uma property.
 */
const char *eg_view_model_get_string(EgViewModel *vm, const char *name);

/**
 * Atalho: define valor bool de uma property.
 */
bool eg_view_model_set_bool(EgViewModel *vm, const char *name, bool value);

/**
 * Atalho: obtém valor bool de uma property.
 */
bool eg_view_model_get_bool(EgViewModel *vm, const char *name);

/**
 * Obtém dados customizados do ViewModel.
 * Útil para ViewModels derivados.
 */
void *eg_view_model_get_data(EgViewModel *vm);

/* ============================================
 * Commands (para ações/botões)
 * ============================================ */

/**
 * Cria um novo command.
 * 
 * @param name Nome do command
 * @param execute Função executada ao invocar o command
 * @param can_execute Função que verifica se pode executar (pode ser NULL)
 * @param user_data Dados do usuário
 * @return Ponteiro para o command ou NULL
 */
EgCommand *eg_command_new(
    const char *name,
    EgCommandExecute execute,
    EgCommandCanExecute can_execute,
    void *user_data
);

/**
 * Libera o command.
 */
void eg_command_free(EgCommand *command);

/**
 * Executa o command.
 * 
 * @param command Command a executar
 * @param parameter Parâmetro opcional
 */
void eg_command_execute(EgCommand *command, void *parameter);

/**
 * Verifica se o command pode ser executado.
 * 
 * @param command Command a verificar
 * @param parameter Parâmetro opcional
 * @return true se pode executar
 */
bool eg_command_can_execute(EgCommand *command, void *parameter);

/**
 * Notifica que a condição de can_execute pode ter mudado.
 * Widgets ligados ao command devem atualizar seu estado.
 */
void eg_command_raise_can_execute_changed(EgCommand *command);

/**
 * Obtém o signal de can_execute_changed.
 */
EgSignal *eg_command_get_can_execute_changed_signal(EgCommand *command);

/**
 * Adiciona um command ao ViewModel.
 * 
 * @param vm ViewModel
 * @param command Command a adicionar
 * @return true se adicionou com sucesso
 */
bool eg_view_model_add_command(EgViewModel *vm, EgCommand *command);

/**
 * Obtém um command pelo nome.
 * 
 * @param vm ViewModel
 * @param name Nome do command
 * @return Ponteiro para o command ou NULL
 */
EgCommand *eg_view_model_get_command(EgViewModel *vm, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_VIEWMODEL_H */
