/**
 * EasyGTK - Picture Widget
 * 
 * Widget para exibição de imagens com controle de escala e ajuste.
 * Diferente de EgImage, EgPicture é otimizado para exibir imagens maiores.
 */

#ifndef EASYGTK_PICTURE_H
#define EASYGTK_PICTURE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Modo de ajuste do conteúdo */
typedef enum EgContentFit {
    EG_CONTENT_FIT_FILL = 0,      /* Preenche todo o espaço (pode distorcer) */
    EG_CONTENT_FIT_CONTAIN = 1,   /* Mantém proporção, cabe inteiro */
    EG_CONTENT_FIT_COVER = 2,     /* Mantém proporção, cobre todo espaço */
    EG_CONTENT_FIT_SCALE_DOWN = 3 /* Como contain, mas nunca aumenta */
} EgContentFit;

/**
 * Cria uma nova picture vazia.
 * 
 * @return Ponteiro para a picture ou NULL em caso de erro
 */
EgPicture *eg_picture_new(void);

/**
 * Cria uma nova picture a partir de um arquivo.
 * 
 * @param file_path Caminho para o arquivo de imagem
 * @return Ponteiro para a picture ou NULL em caso de erro
 */
EgPicture *eg_picture_new_from_file(const char *file_path);

/**
 * Cria uma nova picture a partir de um resource.
 * 
 * @param resource_path Caminho do resource
 * @return Ponteiro para a picture ou NULL em caso de erro
 */
EgPicture *eg_picture_new_from_resource(const char *resource_path);

/**
 * Libera os recursos da picture.
 * 
 * @param picture Ponteiro para a picture
 */
void eg_picture_free(EgPicture *picture);

/**
 * Define a imagem a partir de um arquivo.
 * 
 * @param picture Ponteiro para a picture
 * @param file_path Caminho para o arquivo
 */
void eg_picture_set_filename(EgPicture *picture, const char *file_path);

/**
 * Obtém o caminho do arquivo.
 * 
 * @param picture Ponteiro para a picture
 * @return Caminho do arquivo ou NULL
 */
const char *eg_picture_get_filename(EgPicture *picture);

/**
 * Define a imagem a partir de um resource.
 * 
 * @param picture Ponteiro para a picture
 * @param resource_path Caminho do resource
 */
void eg_picture_set_resource(EgPicture *picture, const char *resource_path);

/**
 * Define o modo de ajuste do conteúdo.
 * 
 * @param picture Ponteiro para a picture
 * @param fit Modo de ajuste
 */
void eg_picture_set_content_fit(EgPicture *picture, EgContentFit fit);

/**
 * Obtém o modo de ajuste do conteúdo.
 * 
 * @param picture Ponteiro para a picture
 * @return Modo de ajuste
 */
EgContentFit eg_picture_get_content_fit(EgPicture *picture);

/**
 * Define se a picture pode ser reduzida.
 * 
 * @param picture Ponteiro para a picture
 * @param can_shrink true para permitir redução
 */
void eg_picture_set_can_shrink(EgPicture *picture, bool can_shrink);

/**
 * Verifica se a picture pode ser reduzida.
 * 
 * @param picture Ponteiro para a picture
 * @return true se pode ser reduzida
 */
bool eg_picture_get_can_shrink(EgPicture *picture);

/**
 * Define um texto alternativo para acessibilidade.
 * 
 * @param picture Ponteiro para a picture
 * @param alt_text Texto alternativo
 */
void eg_picture_set_alternative_text(EgPicture *picture, const char *alt_text);

/**
 * Obtém o texto alternativo.
 * 
 * @param picture Ponteiro para a picture
 * @return Texto alternativo ou NULL
 */
const char *eg_picture_get_alternative_text(EgPicture *picture);

/**
 * Converte para EgWidget.
 * 
 * @param picture Ponteiro para a picture
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_picture_as_widget(EgPicture *picture);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param picture Ponteiro para a picture
 * @return Ponteiro para GtkPicture
 */
void *eg_picture_get_native(EgPicture *picture);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_PICTURE_H */
