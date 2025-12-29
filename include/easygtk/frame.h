/**
 * EasyGTK - Frame
 * 
 * Container com borda e título opcional.
 */

#ifndef EASYGTK_FRAME_H
#define EASYGTK_FRAME_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgFrame EgFrame;

/**
 * Cria um novo frame.
 * 
 * @param label Texto do título (pode ser NULL)
 */
EgFrame *eg_frame_new(const char *label);

/**
 * Libera o frame.
 */
void eg_frame_free(EgFrame *frame);

/**
 * Define o filho do frame.
 */
void eg_frame_set_child(EgFrame *frame, EgWidget *child);

/**
 * Define o texto do título.
 */
void eg_frame_set_label(EgFrame *frame, const char *label);

/**
 * Obtém o texto do título.
 */
const char *eg_frame_get_label(EgFrame *frame);

/**
 * Define o alinhamento do título.
 * 
 * @param xalign Alinhamento horizontal (0.0 = esquerda, 1.0 = direita)
 */
void eg_frame_set_label_align(EgFrame *frame, float xalign);

/**
 * Converte para EgWidget.
 */
EgWidget *eg_frame_as_widget(EgFrame *frame);

/**
 * Obtém o widget GTK nativo.
 */
void *eg_frame_get_native(EgFrame *frame);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_FRAME_H */
