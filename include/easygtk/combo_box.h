/**
 * EasyGTK - ComboBox (Dropdown)
 * 
 * Widget de seleção dropdown.
 */

#ifndef EASYGTK_COMBO_BOX_H
#define EASYGTK_COMBO_BOX_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgComboBox EgComboBox;

/**
 * Cria um novo combo box.
 * 
 * @return Ponteiro para o combo box ou NULL
 */
EgComboBox *eg_combo_box_new(void);

/**
 * Cria um novo combo box a partir de array de strings.
 * 
 * @param items Array de strings (NULL-terminated)
 * @return Ponteiro para o combo box ou NULL
 */
EgComboBox *eg_combo_box_new_with_items(const char *const *items);

/**
 * Libera os recursos do combo box.
 * 
 * @param combo_box Ponteiro para o combo box
 */
void eg_combo_box_free(EgComboBox *combo_box);

/**
 * Adiciona um item ao combo box.
 * 
 * @param combo_box Ponteiro para o combo box
 * @param text Texto do item
 */
void eg_combo_box_append(EgComboBox *combo_box, const char *text);

/**
 * Remove todos os itens do combo box.
 * 
 * @param combo_box Ponteiro para o combo box
 */
void eg_combo_box_clear(EgComboBox *combo_box);

/**
 * Define o item selecionado pelo índice.
 * 
 * @param combo_box Ponteiro para o combo box
 * @param index Índice do item (0-based, -1 para nenhum)
 */
void eg_combo_box_set_active(EgComboBox *combo_box, int index);

/**
 * Obtém o índice do item selecionado.
 * 
 * @param combo_box Ponteiro para o combo box
 * @return Índice do item selecionado (-1 se nenhum)
 */
int eg_combo_box_get_active(EgComboBox *combo_box);

/**
 * Obtém o texto do item selecionado.
 * 
 * @param combo_box Ponteiro para o combo box
 * @return Texto do item selecionado (não liberar) ou NULL
 */
const char *eg_combo_box_get_active_text(EgComboBox *combo_box);

/**
 * Define callback para quando a seleção mudar.
 * 
 * @param combo_box Ponteiro para o combo box
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_combo_box_on_changed(EgComboBox *combo_box, EgCallback callback, void *user_data);

/**
 * Obtém o widget base do combo box.
 * 
 * @param combo_box Ponteiro para o combo box
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_combo_box_as_widget(EgComboBox *combo_box);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param combo_box Ponteiro para o combo box
 * @return Ponteiro para GtkDropDown
 */
void *eg_combo_box_get_native(EgComboBox *combo_box);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_COMBO_BOX_H */
