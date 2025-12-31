/**
 * EasyGTK - Image Widget
 * 
 * Widget para exibição de imagens (ícones e arquivos).
 */

#ifndef EASYGTK_IMAGE_H
#define EASYGTK_IMAGE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova imagem vazia.
 * 
 * @return Ponteiro para a imagem ou NULL em caso de erro
 */
EgImage *eg_image_new(void);

/**
 * Cria uma nova imagem a partir de um arquivo.
 * 
 * @param file_path Caminho para o arquivo de imagem
 * @return Ponteiro para a imagem ou NULL em caso de erro
 */
EgImage *eg_image_new_from_file(const char *file_path);

/**
 * Cria uma nova imagem a partir de um ícone.
 * 
 * @param icon_name Nome do ícone (ex: "dialog-information", "edit-copy")
 * @return Ponteiro para a imagem ou NULL em caso de erro
 */
EgImage *eg_image_new_from_icon(const char *icon_name);

/**
 * Cria uma nova imagem a partir de um resource.
 * 
 * @param resource_path Caminho do resource (ex: "/com/example/app/icon.png")
 * @return Ponteiro para a imagem ou NULL em caso de erro
 */
EgImage *eg_image_new_from_resource(const char *resource_path);

/**
 * Libera os recursos da imagem.
 * 
 * @param image Ponteiro para a imagem
 */
void eg_image_free(EgImage *image);

/**
 * Define a imagem a partir de um arquivo.
 * 
 * @param image Ponteiro para a imagem
 * @param file_path Caminho para o arquivo
 */
void eg_image_set_from_file(EgImage *image, const char *file_path);

/**
 * Define a imagem a partir de um ícone.
 * 
 * @param image Ponteiro para a imagem
 * @param icon_name Nome do ícone
 */
void eg_image_set_from_icon(EgImage *image, const char *icon_name);

/**
 * Define a imagem a partir de um resource.
 * 
 * @param image Ponteiro para a imagem
 * @param resource_path Caminho do resource
 */
void eg_image_set_from_resource(EgImage *image, const char *resource_path);

/**
 * Define o tamanho do ícone em pixels.
 * 
 * @param image Ponteiro para a imagem
 * @param pixel_size Tamanho em pixels (-1 para tamanho padrão)
 */
void eg_image_set_pixel_size(EgImage *image, int pixel_size);

/**
 * Obtém o tamanho do ícone em pixels.
 * 
 * @param image Ponteiro para a imagem
 * @return Tamanho em pixels
 */
int eg_image_get_pixel_size(EgImage *image);

/**
 * Limpa a imagem.
 * 
 * @param image Ponteiro para a imagem
 */
void eg_image_clear(EgImage *image);

/**
 * Converte para EgWidget.
 * 
 * @param image Ponteiro para a imagem
 * @return Ponteiro para EgWidget
 */
EgWidget *eg_image_as_widget(EgImage *image);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param image Ponteiro para a imagem
 * @return Ponteiro para GtkImage
 */
void *eg_image_get_native(EgImage *image);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_IMAGE_H */
