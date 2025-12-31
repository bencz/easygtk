/**
 * EasyGTK - Stack
 * 
 * Container que mostra apenas um filho por vez.
 */

#ifndef EASYGTK_STACK_H
#define EASYGTK_STACK_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo de transição */
typedef enum EgStackTransition {
    EG_STACK_TRANSITION_NONE = 0,
    EG_STACK_TRANSITION_CROSSFADE,
    EG_STACK_TRANSITION_SLIDE_RIGHT,
    EG_STACK_TRANSITION_SLIDE_LEFT,
    EG_STACK_TRANSITION_SLIDE_UP,
    EG_STACK_TRANSITION_SLIDE_DOWN,
    EG_STACK_TRANSITION_SLIDE_LEFT_RIGHT,
    EG_STACK_TRANSITION_SLIDE_UP_DOWN,
    EG_STACK_TRANSITION_OVER_UP,
    EG_STACK_TRANSITION_OVER_DOWN,
    EG_STACK_TRANSITION_OVER_LEFT,
    EG_STACK_TRANSITION_OVER_RIGHT,
    EG_STACK_TRANSITION_UNDER_UP,
    EG_STACK_TRANSITION_UNDER_DOWN,
    EG_STACK_TRANSITION_UNDER_LEFT,
    EG_STACK_TRANSITION_UNDER_RIGHT
} EgStackTransition;

/**
 * Cria um novo stack.
 */
EgStack *eg_stack_new(void);

/**
 * Libera o stack.
 */
void eg_stack_free(EgStack *stack);

/**
 * Adiciona um filho com nome.
 */
void eg_stack_add_named(EgStack *stack, EgWidget *child, const char *name);

/**
 * Adiciona um filho com nome e título.
 */
void eg_stack_add_titled(EgStack *stack, EgWidget *child, const char *name, const char *title);

/**
 * Remove um filho.
 */
void eg_stack_remove(EgStack *stack, EgWidget *child);

/**
 * Define o filho visível pelo nome.
 */
void eg_stack_set_visible_child_name(EgStack *stack, const char *name);

/**
 * Obtém o nome do filho visível.
 */
const char *eg_stack_get_visible_child_name(EgStack *stack);

/**
 * Define o filho visível.
 */
void eg_stack_set_visible_child(EgStack *stack, EgWidget *child);

/**
 * Obtém o filho visível.
 */
EgWidget *eg_stack_get_visible_child(EgStack *stack);

/**
 * Define o tipo de transição.
 */
void eg_stack_set_transition_type(EgStack *stack, EgStackTransition transition);

/**
 * Define a duração da transição em milissegundos.
 */
void eg_stack_set_transition_duration(EgStack *stack, unsigned int duration);

/**
 * Define se o stack é homogêneo (todos filhos mesmo tamanho).
 */
void eg_stack_set_hhomogeneous(EgStack *stack, bool homogeneous);
void eg_stack_set_vhomogeneous(EgStack *stack, bool homogeneous);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_stack_as_widget(EgStack *stack);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_stack_get_native(EgStack *stack);

/* ============================================
 * StackSwitcher
 * ============================================ */

/**
 * Cria um novo stack switcher.
 */
EgStackSwitcher *eg_stack_switcher_new(void);

/**
 * Libera o stack switcher.
 */
void eg_stack_switcher_free(EgStackSwitcher *switcher);

/**
 * Define o stack associado.
 */
void eg_stack_switcher_set_stack(EgStackSwitcher *switcher, EgStack *stack);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_stack_switcher_as_widget(EgStackSwitcher *switcher);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_stack_switcher_get_native(EgStackSwitcher *switcher);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_STACK_H */
