/**
 * EasyGTK - ScrolledWindow
 * 
 * Container com barras de rolagem.
 */

#ifndef EASYGTK_SCROLLED_WINDOW_H
#define EASYGTK_SCROLLED_WINDOW_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco */
typedef struct EgScrolledWindow EgScrolledWindow;

/* Política de exibição das barras de rolagem */
typedef enum EgScrollPolicy {
    EG_SCROLL_AUTOMATIC = 0,  /* Mostra quando necessário */
    EG_SCROLL_ALWAYS = 1,     /* Sempre mostra */
    EG_SCROLL_NEVER = 2       /* Nunca mostra */
} EgScrollPolicy;

/**
 * Cria uma nova scrolled window.
 * 
 * @return Ponteiro para a scrolled window ou NULL
 */
EgScrolledWindow *eg_scrolled_window_new(void);

/**
 * Libera os recursos da scrolled window.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 */
void eg_scrolled_window_free(EgScrolledWindow *scrolled_window);

/**
 * Define o widget filho.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @param child Widget filho
 */
void eg_scrolled_window_set_child(EgScrolledWindow *scrolled_window, EgWidget *child);

/**
 * Define a política de scroll horizontal e vertical.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @param hscroll Política horizontal
 * @param vscroll Política vertical
 */
void eg_scrolled_window_set_policy(EgScrolledWindow *scrolled_window, 
                                    EgScrollPolicy hscroll, EgScrollPolicy vscroll);

/**
 * Define o tamanho mínimo do conteúdo.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @param width Largura mínima (-1 para natural)
 * @param height Altura mínima (-1 para natural)
 */
void eg_scrolled_window_set_min_content_size(EgScrolledWindow *scrolled_window, 
                                              int width, int height);

/**
 * Define o tamanho máximo do conteúdo.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @param width Largura máxima (-1 para ilimitado)
 * @param height Altura máxima (-1 para ilimitado)
 */
void eg_scrolled_window_set_max_content_size(EgScrolledWindow *scrolled_window, 
                                              int width, int height);

/**
 * Define se as bordas devem ter overlay.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @param overlay true para overlay
 */
void eg_scrolled_window_set_overlay_scrolling(EgScrolledWindow *scrolled_window, bool overlay);

/**
 * Obtém o widget base da scrolled window.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_scrolled_window_as_widget(EgScrolledWindow *scrolled_window);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param scrolled_window Ponteiro para a scrolled window
 * @return Ponteiro para GtkScrolledWindow
 */
void *eg_scrolled_window_get_native(EgScrolledWindow *scrolled_window);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SCROLLED_WINDOW_H */
