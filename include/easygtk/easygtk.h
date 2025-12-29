/**
 * EasyGTK - Header principal
 * 
 * Inclui todos os headers públicos da biblioteca.
 * Basta incluir este arquivo para usar toda a API.
 */

#ifndef EASYGTK_H
#define EASYGTK_H

#include "types.h"
#include "signal.h"
#include "property.h"
#include "timer.h"
#include "css.h"
#include "viewmodel.h"
#include "app.h"
#include "window.h"
#include "button.h"
#include "label.h"
#include "entry.h"
#include "box.h"
#include "grid.h"
#include "check_button.h"
#include "switch.h"
#include "progress_bar.h"
#include "combo_box.h"
#include "scrolled_window.h"
#include "dialog.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Obtém a versão da biblioteca.
 * 
 * @return String com a versão (ex: "1.0.0")
 */
const char *eg_version(void);

/* ============================================
 * Funções genéricas de widget
 * ============================================ */

/**
 * Obtém o tipo do widget.
 * 
 * @param widget Ponteiro para o widget
 * @return Tipo do widget
 */
EgWidgetType eg_widget_get_type(EgWidget *widget);

/**
 * Define a visibilidade do widget.
 * 
 * @param widget Ponteiro para o widget
 * @param visible true para visível
 */
void eg_widget_set_visible(EgWidget *widget, bool visible);

/**
 * Verifica se o widget está visível.
 * 
 * @param widget Ponteiro para o widget
 * @return true se visível
 */
bool eg_widget_get_visible(EgWidget *widget);

/**
 * Define se o widget está habilitado.
 * 
 * @param widget Ponteiro para o widget
 * @param sensitive true para habilitado
 */
void eg_widget_set_sensitive(EgWidget *widget, bool sensitive);

/**
 * Verifica se o widget está habilitado.
 * 
 * @param widget Ponteiro para o widget
 * @return true se habilitado
 */
bool eg_widget_get_sensitive(EgWidget *widget);

/**
 * Define as margens do widget.
 * 
 * @param widget Ponteiro para o widget
 * @param margins Estrutura com as margens
 */
void eg_widget_set_margins(EgWidget *widget, EgMargins margins);

/**
 * Define todas as margens com o mesmo valor.
 * 
 * @param widget Ponteiro para o widget
 * @param margin Valor da margem
 */
void eg_widget_set_margin(EgWidget *widget, int margin);

/**
 * Define o alinhamento horizontal.
 * 
 * @param widget Ponteiro para o widget
 * @param align Alinhamento
 */
void eg_widget_set_halign(EgWidget *widget, EgAlign align);

/**
 * Define o alinhamento vertical.
 * 
 * @param widget Ponteiro para o widget
 * @param align Alinhamento
 */
void eg_widget_set_valign(EgWidget *widget, EgAlign align);

/**
 * Define se o widget expande horizontalmente.
 * 
 * @param widget Ponteiro para o widget
 * @param expand true para expandir
 */
void eg_widget_set_hexpand(EgWidget *widget, bool expand);

/**
 * Define se o widget expande verticalmente.
 * 
 * @param widget Ponteiro para o widget
 * @param expand true para expandir
 */
void eg_widget_set_vexpand(EgWidget *widget, bool expand);

/**
 * Define o tamanho mínimo do widget.
 * 
 * @param widget Ponteiro para o widget
 * @param width Largura mínima (-1 para natural)
 * @param height Altura mínima (-1 para natural)
 */
void eg_widget_set_size_request(EgWidget *widget, int width, int height);

/**
 * Adiciona uma classe CSS ao widget.
 * 
 * @param widget Ponteiro para o widget
 * @param class_name Nome da classe CSS
 */
void eg_widget_add_css_class(EgWidget *widget, const char *class_name);

/**
 * Remove uma classe CSS do widget.
 * 
 * @param widget Ponteiro para o widget
 * @param class_name Nome da classe CSS
 */
void eg_widget_remove_css_class(EgWidget *widget, const char *class_name);

/**
 * Define o tooltip do widget.
 * 
 * @param widget Ponteiro para o widget
 * @param tooltip Texto do tooltip (NULL para remover)
 */
void eg_widget_set_tooltip(EgWidget *widget, const char *tooltip);

/**
 * Define o tooltip com markup Pango.
 * 
 * @param widget Ponteiro para o widget
 * @param markup Texto com markup (NULL para remover)
 */
void eg_widget_set_tooltip_markup(EgWidget *widget, const char *markup);

/**
 * Obtém o ponteiro nativo GTK do widget.
 * 
 * @param widget Ponteiro para o widget
 * @return Ponteiro para GtkWidget
 */
void *eg_widget_get_native(EgWidget *widget);

/**
 * Libera os recursos do widget.
 * Chama o destrutor apropriado baseado no tipo.
 * 
 * @param widget Ponteiro para o widget
 */
void eg_widget_free(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_H */
