/**
 * EasyGTK - Scale (Slider)
 * 
 * Controle deslizante para seleção de valores numéricos.
 */

#ifndef EASYGTK_SCALE_H
#define EASYGTK_SCALE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgScale EgScale;

/**
 * Cria um novo scale.
 * 
 * @param orientation Orientação (horizontal ou vertical)
 * @param min Valor mínimo
 * @param max Valor máximo
 * @param step Incremento
 * @return Ponteiro para o scale ou NULL
 */
EgScale *eg_scale_new(EgOrientation orientation, double min, double max, double step);

/**
 * Cria um scale horizontal.
 */
EgScale *eg_scale_new_horizontal(double min, double max, double step);

/**
 * Cria um scale vertical.
 */
EgScale *eg_scale_new_vertical(double min, double max, double step);

/**
 * Libera o scale.
 */
void eg_scale_free(EgScale *scale);

/**
 * Define o valor atual.
 */
void eg_scale_set_value(EgScale *scale, double value);

/**
 * Obtém o valor atual.
 */
double eg_scale_get_value(EgScale *scale);

/**
 * Define o intervalo de valores.
 */
void eg_scale_set_range(EgScale *scale, double min, double max);

/**
 * Define se deve mostrar o valor atual.
 */
void eg_scale_set_draw_value(EgScale *scale, bool draw_value);

/**
 * Define a posição do valor exibido.
 * position: 0=left/top, 1=right/bottom, 2=top/left, 3=bottom/right
 */
void eg_scale_set_value_pos(EgScale *scale, int position);

/**
 * Define o número de casas decimais exibidas.
 */
void eg_scale_set_digits(EgScale *scale, int digits);

/**
 * Adiciona uma marca no scale.
 * 
 * @param value Posição da marca
 * @param position 0=bottom/right, 1=top/left
 * @param markup Texto da marca (pode ser NULL)
 */
void eg_scale_add_mark(EgScale *scale, double value, int position, const char *markup);

/**
 * Remove todas as marcas.
 */
void eg_scale_clear_marks(EgScale *scale);

/**
 * Define callback para quando o valor mudar.
 */
void eg_scale_on_value_changed(EgScale *scale, EgCallback callback, void *user_data);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_scale_as_widget(EgScale *scale);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_scale_get_native(EgScale *scale);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SCALE_H */
