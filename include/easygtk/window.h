/**
 * EasyGTK - Janela
 * 
 * Widget de janela principal da aplicação.
 */

#ifndef EASYGTK_WINDOW_H
#define EASYGTK_WINDOW_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova janela.
 * 
 * @param app Aplicação dona da janela
 * @param title Título da janela
 * @param width Largura inicial
 * @param height Altura inicial
 * @return Ponteiro para a janela ou NULL em caso de erro
 */
EgWindow *eg_window_new(EgApp *app, const char *title, int width, int height);

/**
 * Libera os recursos da janela.
 * 
 * @param window Ponteiro para a janela
 */
void eg_window_free(EgWindow *window);

/**
 * Exibe a janela.
 * 
 * @param window Ponteiro para a janela
 */
void eg_window_show(EgWindow *window);

/**
 * Oculta a janela.
 * 
 * @param window Ponteiro para a janela
 */
void eg_window_hide(EgWindow *window);

/**
 * Fecha a janela.
 * 
 * @param window Ponteiro para a janela
 */
void eg_window_close(EgWindow *window);

/**
 * Define o título da janela.
 * 
 * @param window Ponteiro para a janela
 * @param title Novo título
 */
void eg_window_set_title(EgWindow *window, const char *title);

/**
 * Obtém o título da janela.
 * 
 * @param window Ponteiro para a janela
 * @return Título atual
 */
const char *eg_window_get_title(EgWindow *window);

/**
 * Define o tamanho da janela.
 * 
 * @param window Ponteiro para a janela
 * @param width Nova largura
 * @param height Nova altura
 */
void eg_window_set_size(EgWindow *window, int width, int height);

/**
 * Obtém o tamanho da janela.
 * 
 * @param window Ponteiro para a janela
 * @return Estrutura com largura e altura
 */
EgSize eg_window_get_size(EgWindow *window);

/**
 * Define se a janela pode ser redimensionada.
 * 
 * @param window Ponteiro para a janela
 * @param resizable true para permitir redimensionamento
 */
void eg_window_set_resizable(EgWindow *window, bool resizable);

/**
 * Define o widget filho da janela.
 * 
 * @param window Ponteiro para a janela
 * @param child Widget filho
 */
void eg_window_set_child(EgWindow *window, EgWidget *child);

/**
 * Define callback para quando a janela for fechada.
 * 
 * @param window Ponteiro para a janela
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_window_on_close(EgWindow *window, EgCallback callback, void *user_data);

/**
 * Define callback para quando a janela for destruída.
 * 
 * @param window Ponteiro para a janela
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_window_on_destroy(EgWindow *window, EgCallback callback, void *user_data);

/**
 * Obtém o widget base da janela.
 * 
 * @param window Ponteiro para a janela
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_window_as_widget(EgWindow *window);

/**
 * Obtém o ponteiro nativo GTK.
 * 
 * @param window Ponteiro para a janela
 * @return Ponteiro para GtkWindow
 */
void *eg_window_get_native(EgWindow *window);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_WINDOW_H */
