/**
 * EasyGTK - ProgressBar
 * 
 * Widget de barra de progresso.
 */

#ifndef EASYGTK_PROGRESS_BAR_H
#define EASYGTK_PROGRESS_BAR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova barra de progresso.
 * 
 * @return Ponteiro para a progress bar ou NULL
 */
EgProgressBar *eg_progress_bar_new(void);

/**
 * Libera os recursos da progress bar.
 * 
 * @param progress_bar Ponteiro para a progress bar
 */
void eg_progress_bar_free(EgProgressBar *progress_bar);

/**
 * Define a fração de progresso (0.0 a 1.0).
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @param fraction Valor entre 0.0 e 1.0
 */
void eg_progress_bar_set_fraction(EgProgressBar *progress_bar, double fraction);

/**
 * Obtém a fração de progresso atual.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @return Valor entre 0.0 e 1.0
 */
double eg_progress_bar_get_fraction(EgProgressBar *progress_bar);

/**
 * Define o texto exibido na progress bar.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @param text Texto a exibir (NULL para ocultar)
 */
void eg_progress_bar_set_text(EgProgressBar *progress_bar, const char *text);

/**
 * Obtém o texto da progress bar.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @return Texto atual
 */
const char *eg_progress_bar_get_text(EgProgressBar *progress_bar);

/**
 * Define se o texto deve ser exibido.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @param show_text true para exibir texto
 */
void eg_progress_bar_set_show_text(EgProgressBar *progress_bar, bool show_text);

/**
 * Avança a barra em modo indeterminado (pulse).
 * Chame repetidamente para criar animação de "carregando".
 * 
 * @param progress_bar Ponteiro para a progress bar
 */
void eg_progress_bar_pulse(EgProgressBar *progress_bar);

/**
 * Define o tamanho do passo do pulse.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @param fraction Tamanho do passo (0.0 a 1.0)
 */
void eg_progress_bar_set_pulse_step(EgProgressBar *progress_bar, double fraction);

/**
 * Define se a barra é invertida (direita para esquerda).
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @param inverted true para inverter
 */
void eg_progress_bar_set_inverted(EgProgressBar *progress_bar, bool inverted);

/**
 * Obtém o widget base da progress bar.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_progress_bar_as_widget(EgProgressBar *progress_bar);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param progress_bar Ponteiro para a progress bar
 * @return Ponteiro para GtkProgressBar
 */
void *eg_progress_bar_get_native(EgProgressBar *progress_bar);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_PROGRESS_BAR_H */
