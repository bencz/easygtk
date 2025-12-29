/**
 * EasyGTK - Entry
 * 
 * Widget de campo de entrada de texto.
 */

#ifndef EASYGTK_ENTRY_H
#define EASYGTK_ENTRY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo campo de entrada.
 * 
 * @return Ponteiro para o entry ou NULL em caso de erro
 */
EgEntry *eg_entry_new(void);

/**
 * Cria um novo campo de entrada com placeholder.
 * 
 * @param placeholder Texto placeholder
 * @return Ponteiro para o entry ou NULL em caso de erro
 */
EgEntry *eg_entry_new_with_placeholder(const char *placeholder);

/**
 * Libera os recursos do entry.
 * 
 * @param entry Ponteiro para o entry
 */
void eg_entry_free(EgEntry *entry);

/**
 * Define o texto do entry.
 * 
 * @param entry Ponteiro para o entry
 * @param text Novo texto
 */
void eg_entry_set_text(EgEntry *entry, const char *text);

/**
 * Obtém o texto do entry.
 * 
 * @param entry Ponteiro para o entry
 * @return Texto atual (não deve ser liberado)
 */
const char *eg_entry_get_text(EgEntry *entry);

/**
 * Define o texto placeholder.
 * 
 * @param entry Ponteiro para o entry
 * @param placeholder Texto placeholder
 */
void eg_entry_set_placeholder(EgEntry *entry, const char *placeholder);

/**
 * Define o tamanho máximo de caracteres.
 * 
 * @param entry Ponteiro para o entry
 * @param max_length Tamanho máximo (0 para ilimitado)
 */
void eg_entry_set_max_length(EgEntry *entry, int max_length);

/**
 * Define se o texto é visível (para senhas).
 * 
 * @param entry Ponteiro para o entry
 * @param visible true para texto visível
 */
void eg_entry_set_visibility(EgEntry *entry, bool visible);

/**
 * Define se o entry é editável.
 * 
 * @param entry Ponteiro para o entry
 * @param editable true para editável
 */
void eg_entry_set_editable(EgEntry *entry, bool editable);

/**
 * Define callback para quando o texto mudar.
 * 
 * @param entry Ponteiro para o entry
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_entry_on_changed(EgEntry *entry, EgCallback callback, void *user_data);

/**
 * Define callback para quando Enter for pressionado.
 * 
 * @param entry Ponteiro para o entry
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_entry_on_activate(EgEntry *entry, EgCallback callback, void *user_data);

/**
 * Obtém o widget base do entry.
 * 
 * @param entry Ponteiro para o entry
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_entry_as_widget(EgEntry *entry);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param entry Ponteiro para o entry
 * @return Ponteiro para GtkEntry
 */
void *eg_entry_get_native(EgEntry *entry);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_ENTRY_H */
