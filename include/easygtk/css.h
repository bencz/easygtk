/**
 * EasyGTK - Sistema de CSS
 * 
 * Carregamento e aplicação de estilos CSS.
 */

#ifndef EASYGTK_CSS_H
#define EASYGTK_CSS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipo opaco para provider CSS */
typedef struct EgCssProvider EgCssProvider;

/**
 * Carrega CSS de uma string e aplica globalmente.
 * 
 * @param css_string String contendo CSS
 * @return Ponteiro para o provider ou NULL em caso de erro
 */
EgCssProvider *eg_css_load_from_string(const char *css_string);

/**
 * Carrega CSS de um arquivo e aplica globalmente.
 * 
 * @param file_path Caminho para o arquivo CSS
 * @return Ponteiro para o provider ou NULL em caso de erro
 */
EgCssProvider *eg_css_load_from_file(const char *file_path);

/**
 * Remove o provider CSS (desfaz os estilos).
 * 
 * @param provider Ponteiro para o provider
 */
void eg_css_provider_free(EgCssProvider *provider);

/**
 * Aplica CSS inline a um widget específico.
 * 
 * @param widget Widget alvo
 * @param css_string CSS a aplicar
 * @return true se aplicou com sucesso
 */
bool eg_widget_set_css(EgWidget *widget, const char *css_string);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_CSS_H */
