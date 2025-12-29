/**
 * EasyGTK - LevelBar Widget
 * 
 * Widget para exibição de níveis/valores em forma de barra.
 */

#ifndef EASYGTK_LEVEL_BAR_H
#define EASYGTK_LEVEL_BAR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgLevelBar EgLevelBar;

/* Modo da barra de nível */
typedef enum EgLevelBarMode {
    EG_LEVEL_BAR_MODE_CONTINUOUS = 0,  /* Barra contínua */
    EG_LEVEL_BAR_MODE_DISCRETE = 1     /* Barra discreta (blocos) */
} EgLevelBarMode;

/**
 * Cria uma nova barra de nível.
 * 
 * @return Ponteiro para a barra ou NULL em caso de erro
 */
EgLevelBar *eg_level_bar_new(void);

/**
 * Cria uma nova barra de nível com intervalo.
 * 
 * @param min_value Valor mínimo
 * @param max_value Valor máximo
 * @return Ponteiro para a barra ou NULL em caso de erro
 */
EgLevelBar *eg_level_bar_new_for_interval(double min_value, double max_value);

/**
 * Libera os recursos da barra de nível.
 * 
 * @param level_bar Ponteiro para a barra
 */
void eg_level_bar_free(EgLevelBar *level_bar);

/**
 * Define o valor atual.
 * 
 * @param level_bar Ponteiro para a barra
 * @param value Valor atual
 */
void eg_level_bar_set_value(EgLevelBar *level_bar, double value);

/**
 * Obtém o valor atual.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Valor atual
 */
double eg_level_bar_get_value(EgLevelBar *level_bar);

/**
 * Define o valor mínimo.
 * 
 * @param level_bar Ponteiro para a barra
 * @param min_value Valor mínimo
 */
void eg_level_bar_set_min_value(EgLevelBar *level_bar, double min_value);

/**
 * Obtém o valor mínimo.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Valor mínimo
 */
double eg_level_bar_get_min_value(EgLevelBar *level_bar);

/**
 * Define o valor máximo.
 * 
 * @param level_bar Ponteiro para a barra
 * @param max_value Valor máximo
 */
void eg_level_bar_set_max_value(EgLevelBar *level_bar, double max_value);

/**
 * Obtém o valor máximo.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Valor máximo
 */
double eg_level_bar_get_max_value(EgLevelBar *level_bar);

/**
 * Define o modo da barra.
 * 
 * @param level_bar Ponteiro para a barra
 * @param mode Modo (contínuo ou discreto)
 */
void eg_level_bar_set_mode(EgLevelBar *level_bar, EgLevelBarMode mode);

/**
 * Obtém o modo da barra.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Modo atual
 */
EgLevelBarMode eg_level_bar_get_mode(EgLevelBar *level_bar);

/**
 * Define se a barra é invertida.
 * 
 * @param level_bar Ponteiro para a barra
 * @param inverted true para inverter
 */
void eg_level_bar_set_inverted(EgLevelBar *level_bar, bool inverted);

/**
 * Verifica se a barra é invertida.
 * 
 * @param level_bar Ponteiro para a barra
 * @return true se invertida
 */
bool eg_level_bar_get_inverted(EgLevelBar *level_bar);

/**
 * Adiciona um offset com nome.
 * 
 * @param level_bar Ponteiro para a barra
 * @param name Nome do offset (ex: "low", "high", "full")
 * @param value Valor do offset
 */
void eg_level_bar_add_offset_value(EgLevelBar *level_bar, const char *name, double value);

/**
 * Remove um offset pelo nome.
 * 
 * @param level_bar Ponteiro para a barra
 * @param name Nome do offset
 */
void eg_level_bar_remove_offset_value(EgLevelBar *level_bar, const char *name);

/**
 * Converte para EgWidget.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_level_bar_as_widget(EgLevelBar *level_bar);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param level_bar Ponteiro para a barra
 * @return Ponteiro para GtkLevelBar
 */
void *eg_level_bar_get_native(EgLevelBar *level_bar);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_LEVEL_BAR_H */
