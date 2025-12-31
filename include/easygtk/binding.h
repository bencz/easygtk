/**
 * EasyGTK - Binding Declarativo Widget <-> ViewModel
 *
 * Facilita a conexão entre widgets e ViewModels sem código manual.
 */

#ifndef EASYGTK_BINDING_H
#define EASYGTK_BINDING_H

#include "types.h"
#include "viewmodel.h"
#include "property.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================
 * Binding Modes
 * ============================================ */

/**
 * Modo de binding entre widget e property.
 */
typedef enum EgBindingMode {
    EG_BINDING_MODE_NONE = 0,           /**< Widget não suporta binding */
    EG_BINDING_MODE_ONE_WAY = 1,        /**< Property -> Widget */
    EG_BINDING_MODE_TWO_WAY = 2,        /**< Property <-> Widget */
    EG_BINDING_MODE_ONE_WAY_TO_SOURCE = 3  /**< Widget -> Property */
} EgBindingMode;

/* ============================================
 * Widget Binding Handle
 * ============================================ */

/**
 * Handle para um binding ativo entre widget e property/command.
 * Usado para desfazer o binding posteriormente.
 */
typedef struct EgWidgetBinding EgWidgetBinding;

/* ============================================
 * Generic Binding API (VTable-based)
 * ============================================ */

/**
 * Cria binding genérico entre widget e property do ViewModel.
 * Usa a vtable do widget para determinar como fazer o binding.
 *
 * @param widget Widget a ser bindado (qualquer tipo)
 * @param vm ViewModel contendo a property
 * @param property_name Nome da property
 * @return Handle do binding ou NULL em caso de erro
 */
EgWidgetBinding *eg_bind(EgWidget *widget, EgViewModel *vm, const char *property_name);

/**
 * Cria binding com modo explícito (override do modo padrão).
 *
 * @param widget Widget a ser bindado
 * @param vm ViewModel
 * @param property_name Nome da property
 * @param mode Modo de binding desejado
 * @return Handle do binding ou NULL
 */
EgWidgetBinding *eg_bind_with_mode(EgWidget *widget, EgViewModel *vm,
                                    const char *property_name, EgBindingMode mode);

/**
 * Cria binding de command entre widget e command do ViewModel.
 * Conecta a ação do widget (ex: click) ao command.
 *
 * @param widget Widget a ser bindado (tipicamente botão)
 * @param vm ViewModel contendo o command
 * @param command_name Nome do command
 * @return Handle do binding ou NULL
 */
EgWidgetBinding *eg_bind_cmd(EgWidget *widget, EgViewModel *vm, const char *command_name);

/**
 * Remove um binding e libera recursos.
 *
 * @param binding Handle do binding a remover
 */
void eg_unbind(EgWidgetBinding *binding);

/**
 * Verifica se um widget suporta binding de valor.
 *
 * @param widget Widget a verificar
 * @return true se suporta binding
 */
bool eg_widget_supports_binding(EgWidget *widget);

/**
 * Verifica se um widget suporta binding de command.
 *
 * @param widget Widget a verificar
 * @return true se suporta command binding
 */
bool eg_widget_supports_command_binding(EgWidget *widget);

/**
 * Obtém o tipo de property esperado para binding de um widget.
 *
 * @param widget Widget a verificar
 * @return Tipo de property ou EG_PROPERTY_TYPE_STRING se não suporta
 */
EgPropertyType eg_widget_get_binding_type(EgWidget *widget);

/* ============================================
 * Legacy Binding API (Deprecated)
 * ============================================
 *
 * Estas funções são mantidas para compatibilidade.
 * Prefira usar eg_bind() e eg_bind_cmd() para novos códigos.
 *
 * A nova API genérica usa vtables para descobrir automaticamente
 * como fazer o binding para cada tipo de widget.
 */

#if defined(__GNUC__) || defined(__clang__)
#define EG_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define EG_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define EG_DEPRECATED(msg)
#endif

/**
 * @deprecated Use eg_bind(eg_entry_as_widget(entry), vm, property_name) instead.
 * Liga o texto de um EgEntry a uma property do ViewModel (two-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_entry_text(EgEntry *entry, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind(eg_label_as_widget(label), vm, property_name) instead.
 * Liga o texto de um EgLabel a uma property do ViewModel (one-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_label_text(EgLabel *label, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind(eg_check_button_as_widget(check_button), vm, property_name) instead.
 * Liga o estado de um EgCheckButton a uma property bool do ViewModel (two-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_check_button_active(EgCheckButton *check_button, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind(eg_switch_as_widget(switch_widget), vm, property_name) instead.
 * Liga o estado de um EgSwitch a uma property bool do ViewModel (two-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_switch_active(EgSwitch *switch_widget, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind(eg_spin_button_as_widget(spin_button), vm, property_name) instead.
 * Liga o valor de um EgSpinButton a uma property int/double do ViewModel (two-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_spin_button_value(EgSpinButton *spin_button, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind(eg_scale_as_widget(scale), vm, property_name) instead.
 * Liga o valor de um EgScale a uma property double do ViewModel (two-way).
 */
EG_DEPRECATED("Use eg_bind() instead")
bool eg_bind_scale_value(EgScale *scale, EgViewModel *vm, const char *property_name);

/**
 * Liga a visibilidade de um Widget a uma property bool do ViewModel.
 * Nota: Esta função ainda é útil pois eg_bind() não faz binding de visibilidade.
 *
 * @param widget Widget a ligar
 * @param vm ViewModel
 * @param property_name Nome da property bool
 * @return true se criou binding com sucesso
 */
bool eg_bind_widget_visible(EgWidget *widget, EgViewModel *vm, const char *property_name);

/**
 * Liga a sensibilidade de um Widget a uma property bool do ViewModel.
 * Nota: Esta função ainda é útil pois eg_bind() não faz binding de sensibilidade.
 *
 * @param widget Widget a ligar
 * @param vm ViewModel
 * @param property_name Nome da property bool
 * @return true se criou binding com sucesso
 */
bool eg_bind_widget_sensitive(EgWidget *widget, EgViewModel *vm, const char *property_name);

/**
 * @deprecated Use eg_bind_cmd(eg_button_as_widget(button), vm, command_name) instead.
 * Liga um EgButton a um Command do ViewModel.
 * O botão executará o command ao ser clicado e sua sensibilidade
 * será atualizada automaticamente baseado em can_execute.
 */
EG_DEPRECATED("Use eg_bind_cmd() instead")
bool eg_bind_button_command(EgButton *button, EgViewModel *vm, const char *command_name);

/* ============================================
 * Binding para ListView e ColumnView
 * ============================================ */

/**
 * Callback chamado quando a seleção da lista muda.
 * Permite atualizar o ViewModel baseado na seleção.
 *
 * @param list_view Ponteiro para a lista
 * @param selected_index Índice selecionado (-1 se nenhum)
 * @param selected_text Texto do item selecionado (NULL se nenhum)
 * @param user_data Dados do usuário
 */
typedef void (*EgListViewSelectionCallback)(EgListView *list_view, int selected_index,
                                             const char *selected_text, void *user_data);

/**
 * Callback chamado quando a seleção da tabela muda.
 *
 * @param column_view Ponteiro para a tabela
 * @param selected_row Índice da linha selecionada (-1 se nenhuma)
 * @param user_data Dados do usuário
 */
typedef void (*EgColumnViewSelectionCallback)(EgColumnView *column_view, int selected_row,
                                               void *user_data);

/**
 * Liga um EgListView a uma property string_array do ViewModel.
 * Quando a property muda, a lista é atualizada automaticamente.
 *
 * @param list_view ListView a ligar
 * @param vm ViewModel
 * @param items_property_name Nome da property que contém os itens
 * @param selected_property_name Nome da property para receber o índice selecionado (pode ser NULL)
 * @return true se criou binding com sucesso
 */
bool eg_bind_list_view_items(EgListView *list_view, EgViewModel *vm,
                              const char *items_property_name,
                              const char *selected_property_name);

/**
 * Liga a seleção do ListView a um callback customizado.
 * Permite lógica MVVM mais complexa na seleção.
 *
 * @param list_view ListView a ligar
 * @param callback Função chamada quando a seleção muda
 * @param user_data Dados do usuário
 * @return true se criou binding com sucesso
 */
bool eg_bind_list_view_selection(EgListView *list_view,
                                  EgListViewSelectionCallback callback,
                                  void *user_data);

/**
 * Liga um EgColumnView a uma property table do ViewModel.
 * A property deve ser do tipo que contém os dados da tabela.
 *
 * @param column_view ColumnView a ligar
 * @param vm ViewModel
 * @param rows_property_name Nome da property que contém as linhas
 * @param selected_property_name Nome da property para receber o índice selecionado (pode ser NULL)
 * @return true se criou binding com sucesso
 */
bool eg_bind_column_view_rows(EgColumnView *column_view, EgViewModel *vm,
                               const char *rows_property_name,
                               const char *selected_property_name);

/**
 * Liga a seleção do ColumnView a um callback customizado.
 *
 * @param column_view ColumnView a ligar
 * @param callback Função chamada quando a seleção muda
 * @param user_data Dados do usuário
 * @return true se criou binding com sucesso
 */
bool eg_bind_column_view_selection(EgColumnView *column_view,
                                    EgColumnViewSelectionCallback callback,
                                    void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_BINDING_H */
