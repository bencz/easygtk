/**
 * EasyGTK - Spinner Widget
 * 
 * Widget de indicador de carregamento animado.
 */

#ifndef EASYGTK_SPINNER_H
#define EASYGTK_SPINNER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgSpinner EgSpinner;

/**
 * Cria um novo spinner.
 * 
 * @return Ponteiro para o spinner ou NULL em caso de erro
 */
EgSpinner *eg_spinner_new(void);

/**
 * Libera os recursos do spinner.
 * 
 * @param spinner Ponteiro para o spinner
 */
void eg_spinner_free(EgSpinner *spinner);

/**
 * Inicia a animação do spinner.
 * 
 * @param spinner Ponteiro para o spinner
 */
void eg_spinner_start(EgSpinner *spinner);

/**
 * Para a animação do spinner.
 * 
 * @param spinner Ponteiro para o spinner
 */
void eg_spinner_stop(EgSpinner *spinner);

/**
 * Define se o spinner está girando.
 * 
 * @param spinner Ponteiro para o spinner
 * @param spinning true para girar
 */
void eg_spinner_set_spinning(EgSpinner *spinner, bool spinning);

/**
 * Verifica se o spinner está girando.
 * 
 * @param spinner Ponteiro para o spinner
 * @return true se está girando
 */
bool eg_spinner_get_spinning(EgSpinner *spinner);

/**
 * Converte para EgWidget.
 * 
 * @param spinner Ponteiro para o spinner
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_spinner_as_widget(EgSpinner *spinner);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param spinner Ponteiro para o spinner
 * @return Ponteiro para GtkSpinner
 */
void *eg_spinner_get_native(EgSpinner *spinner);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SPINNER_H */
