/**
 * EasyGTK - ColorChooser Dialog
 * 
 * Diálogo para seleção de cores.
 */

#ifndef EASYGTK_COLOR_CHOOSER_H
#define EASYGTK_COLOR_CHOOSER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration */
typedef struct EgWindow EgWindow;

/* Estrutura para representar uma cor RGBA */
typedef struct EgColor {
    double red;    /* 0.0 a 1.0 */
    double green;  /* 0.0 a 1.0 */
    double blue;   /* 0.0 a 1.0 */
    double alpha;  /* 0.0 a 1.0 */
} EgColor;

/* Callback para resultado do diálogo de cor */
typedef void (*EgColorChooserCallback)(const EgColor *color, void *user_data);

/**
 * Abre diálogo para selecionar cor.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param initial_color Cor inicial (pode ser NULL para usar padrão)
 * @param callback Função chamada com a cor selecionada (NULL se cancelado)
 * @param user_data Dados do usuário para o callback
 */
void eg_color_chooser_dialog(EgWindow *parent, const char *title,
                              const EgColor *initial_color,
                              EgColorChooserCallback callback, void *user_data);

/**
 * Abre diálogo para selecionar cor com suporte a alpha.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param initial_color Cor inicial (pode ser NULL)
 * @param with_alpha true para permitir seleção de transparência
 * @param callback Função chamada com a cor selecionada
 * @param user_data Dados do usuário para o callback
 */
void eg_color_chooser_dialog_with_alpha(EgWindow *parent, const char *title,
                                         const EgColor *initial_color,
                                         bool with_alpha,
                                         EgColorChooserCallback callback, void *user_data);

/**
 * Cria uma cor a partir de valores RGB (0-255).
 * 
 * @param red Componente vermelho (0-255)
 * @param green Componente verde (0-255)
 * @param blue Componente azul (0-255)
 * @return Estrutura EgColor
 */
EgColor eg_color_from_rgb(int red, int green, int blue);

/**
 * Cria uma cor a partir de valores RGBA (0-255, alpha 0.0-1.0).
 * 
 * @param red Componente vermelho (0-255)
 * @param green Componente verde (0-255)
 * @param blue Componente azul (0-255)
 * @param alpha Componente alpha (0.0-1.0)
 * @return Estrutura EgColor
 */
EgColor eg_color_from_rgba(int red, int green, int blue, double alpha);

/**
 * Cria uma cor a partir de string hexadecimal.
 * 
 * @param hex String hex (ex: "#FF0000", "#FF0000FF", "red")
 * @return Estrutura EgColor (preto se inválido)
 */
EgColor eg_color_from_string(const char *hex);

/**
 * Converte cor para string hexadecimal.
 * 
 * @param color Ponteiro para a cor
 * @param buffer Buffer para armazenar resultado (mínimo 10 chars para #RRGGBBAA)
 * @param buffer_size Tamanho do buffer
 * @return Ponteiro para buffer ou NULL em caso de erro
 */
char *eg_color_to_string(const EgColor *color, char *buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_COLOR_CHOOSER_H */
