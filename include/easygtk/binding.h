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

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_BINDING_H */
