/**
 * EasyGTK - Container API Genérica
 *
 * Funções para operar em qualquer container via vtable.
 * Permite código genérico que funciona com Box, Stack, Frame, etc.
 */

#ifndef EASYGTK_CONTAINER_H
#define EASYGTK_CONTAINER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Verifica se um widget é um container.
 *
 * @param widget Widget a verificar
 * @return true se é container
 */
bool eg_widget_is_container(EgWidget *widget);

/**
 * Adiciona um filho ao container.
 * Equivalente a eg_box_append, eg_frame_set_child, etc.
 *
 * @param container Container
 * @param child Widget filho
 */
void eg_container_add(EgWidget *container, EgWidget *child);

/**
 * Remove um filho do container.
 *
 * @param container Container
 * @param child Widget filho a remover
 */
void eg_container_remove(EgWidget *container, EgWidget *child);

/**
 * Adiciona um filho com nome (para Stack, Notebook).
 * Se o container não suportar nomes, adiciona sem nome.
 *
 * @param container Container
 * @param child Widget filho
 * @param name Nome do filho
 */
void eg_container_add_named(EgWidget *container, EgWidget *child, const char *name);

/**
 * Obtém o número de filhos.
 *
 * @param container Container
 * @return Número de filhos
 */
size_t eg_container_get_child_count(EgWidget *container);

/**
 * Obtém um filho pelo índice.
 *
 * @param container Container
 * @param index Índice do filho
 * @return Widget filho ou NULL
 */
EgWidget *eg_container_get_child_at(EgWidget *container, size_t index);

/**
 * Remove todos os filhos do container.
 *
 * @param container Container
 */
void eg_container_clear(EgWidget *container);

/**
 * Verifica se o container suporta múltiplos filhos.
 *
 * @param container Container
 * @return true se suporta múltiplos (Box, Grid), false se único (Frame)
 */
bool eg_container_supports_multiple(EgWidget *container);

/**
 * Verifica se o container suporta filhos nomeados.
 *
 * @param container Container
 * @return true se suporta nomes (Stack, Notebook)
 */
bool eg_container_supports_named(EgWidget *container);

/**
 * Obtém o número máximo de filhos.
 *
 * @param container Container
 * @return Máximo (0 = ilimitado, 1 = Frame, 2 = Paned)
 */
size_t eg_container_max_children(EgWidget *container);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_CONTAINER_H */
