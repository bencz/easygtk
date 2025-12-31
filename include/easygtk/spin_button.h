/**
 * EasyGTK - SpinButton
 * 
 * Campo numérico com botões de incremento/decremento.
 */

#ifndef EASYGTK_SPIN_BUTTON_H
#define EASYGTK_SPIN_BUTTON_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria um novo spin button.
 * 
 * @param min Valor mínimo
 * @param max Valor máximo
 * @param step Incremento por clique
 * @return Ponteiro para o spin button ou NULL
 */
EgSpinButton *eg_spin_button_new(double min, double max, double step);

/**
 * Cria um spin button para valores inteiros.
 * 
 * @param min Valor mínimo
 * @param max Valor máximo
 * @param step Incremento por clique
 * @return Ponteiro para o spin button ou NULL
 */
EgSpinButton *eg_spin_button_new_int(int min, int max, int step);

/**
 * Libera o spin button.
 */
void eg_spin_button_free(EgSpinButton *spin_button);

/**
 * Define o valor atual.
 */
void eg_spin_button_set_value(EgSpinButton *spin_button, double value);

/**
 * Obtém o valor atual.
 */
double eg_spin_button_get_value(EgSpinButton *spin_button);

/**
 * Obtém o valor como inteiro.
 */
int eg_spin_button_get_value_as_int(EgSpinButton *spin_button);

/**
 * Define o intervalo de valores.
 */
void eg_spin_button_set_range(EgSpinButton *spin_button, double min, double max);

/**
 * Define o incremento.
 */
void eg_spin_button_set_increments(EgSpinButton *spin_button, double step, double page);

/**
 * Define o número de casas decimais exibidas.
 */
void eg_spin_button_set_digits(EgSpinButton *spin_button, unsigned int digits);

/**
 * Define se o valor deve circular (wrap around).
 */
void eg_spin_button_set_wrap(EgSpinButton *spin_button, bool wrap);

/**
 * Define callback para quando o valor mudar.
 */
void eg_spin_button_on_value_changed(EgSpinButton *spin_button, EgCallback callback, void *user_data);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_spin_button_as_widget(EgSpinButton *spin_button);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_spin_button_get_native(EgSpinButton *spin_button);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SPIN_BUTTON_H */
