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

/* Forward declarations */
typedef struct EgWidget EgWidget;
typedef struct EgEntry EgEntry;
typedef struct EgLabel EgLabel;
typedef struct EgCheckButton EgCheckButton;
typedef struct EgSwitch EgSwitch;
typedef struct EgSpinButton EgSpinButton;
typedef struct EgScale EgScale;
typedef struct EgButton EgButton;
typedef struct EgListView EgListView;
typedef struct EgColumnView EgColumnView;

/* ============================================
 * Binding Declarativo
 * ============================================ */

/**
 * Liga o texto de um EgEntry a uma property do ViewModel (two-way).
 *
 * @param entry Entry a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_entry_text(EgEntry *entry, EgViewModel *vm, const char *property_name);

/**
 * Liga o texto de um EgLabel a uma property do ViewModel (one-way).
 *
 * @param label Label a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_label_text(EgLabel *label, EgViewModel *vm, const char *property_name);

/**
 * Liga o estado de um EgCheckButton a uma property bool do ViewModel (two-way).
 *
 * @param check_button CheckButton a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_check_button_active(EgCheckButton *check_button, EgViewModel *vm, const char *property_name);

/**
 * Liga o estado de um EgSwitch a uma property bool do ViewModel (two-way).
 *
 * @param switch_widget Switch a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_switch_active(EgSwitch *switch_widget, EgViewModel *vm, const char *property_name);

/**
 * Liga o valor de um EgSpinButton a uma property int/double do ViewModel (two-way).
 *
 * @param spin_button SpinButton a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_spin_button_value(EgSpinButton *spin_button, EgViewModel *vm, const char *property_name);

/**
 * Liga o valor de um EgScale a uma property double do ViewModel (two-way).
 *
 * @param scale Scale a ligar
 * @param vm ViewModel
 * @param property_name Nome da property
 * @return true se criou binding com sucesso
 */
bool eg_bind_scale_value(EgScale *scale, EgViewModel *vm, const char *property_name);

/**
 * Liga a visibilidade de um Widget a uma property bool do ViewModel.
 *
 * @param widget Widget a ligar
 * @param vm ViewModel
 * @param property_name Nome da property bool
 * @return true se criou binding com sucesso
 */
bool eg_bind_widget_visible(EgWidget *widget, EgViewModel *vm, const char *property_name);

/**
 * Liga a sensibilidade de um Widget a uma property bool do ViewModel.
 *
 * @param widget Widget a ligar
 * @param vm ViewModel
 * @param property_name Nome da property bool
 * @return true se criou binding com sucesso
 */
bool eg_bind_widget_sensitive(EgWidget *widget, EgViewModel *vm, const char *property_name);

/**
 * Liga um EgButton a um Command do ViewModel.
 * O botão executará o command ao ser clicado e sua sensibilidade
 * será atualizada automaticamente baseado em can_execute.
 *
 * @param button Button a ligar
 * @param vm ViewModel
 * @param command_name Nome do command
 * @return true se criou binding com sucesso
 */
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
