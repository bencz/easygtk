/**
 * EasyGTK - FileChooser Dialog
 * 
 * Diálogos para seleção de arquivos e pastas.
 */

#ifndef EASYGTK_FILE_CHOOSER_H
#define EASYGTK_FILE_CHOOSER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declaration */

/* Callback para resultado do diálogo de arquivo */
typedef void (*EgFileChooserCallback)(const char *path, void *user_data);

/* Callback para resultado com múltiplos arquivos */
typedef void (*EgFileChooserMultiCallback)(char **paths, int count, void *user_data);

/**
 * Cria um novo filtro de arquivo.
 * 
 * @param name Nome do filtro (ex: "Imagens")
 * @return Ponteiro para o filtro ou NULL
 */
EgFileFilter *eg_file_filter_new(const char *name);

/**
 * Libera o filtro de arquivo.
 * 
 * @param filter Ponteiro para o filtro
 */
void eg_file_filter_free(EgFileFilter *filter);

/**
 * Adiciona um padrão MIME ao filtro.
 * 
 * @param filter Ponteiro para o filtro
 * @param mime_type Tipo MIME (ex: "image/png", "text/plain")
 */
void eg_file_filter_add_mime_type(EgFileFilter *filter, const char *mime_type);

/**
 * Adiciona um padrão de nome ao filtro.
 * 
 * @param filter Ponteiro para o filtro
 * @param pattern Padrão glob (ex: "*.png", "*.txt")
 */
void eg_file_filter_add_pattern(EgFileFilter *filter, const char *pattern);

/**
 * Adiciona um sufixo ao filtro.
 * 
 * @param filter Ponteiro para o filtro
 * @param suffix Sufixo sem ponto (ex: "png", "txt")
 */
void eg_file_filter_add_suffix(EgFileFilter *filter, const char *suffix);

/**
 * Abre diálogo para abrir arquivo.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param callback Função chamada com o caminho selecionado (NULL se cancelado)
 * @param user_data Dados do usuário para o callback
 */
void eg_file_chooser_open(EgWindow *parent, const char *title,
                          EgFileChooserCallback callback, void *user_data);

/**
 * Abre diálogo para abrir arquivo com filtros.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param filters Array de filtros (NULL-terminated)
 * @param callback Função chamada com o caminho selecionado
 * @param user_data Dados do usuário para o callback
 */
void eg_file_chooser_open_with_filters(EgWindow *parent, const char *title,
                                        EgFileFilter **filters,
                                        EgFileChooserCallback callback, void *user_data);

/**
 * Abre diálogo para abrir múltiplos arquivos.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param callback Função chamada com os caminhos selecionados
 * @param user_data Dados do usuário para o callback
 */
void eg_file_chooser_open_multiple(EgWindow *parent, const char *title,
                                    EgFileChooserMultiCallback callback, void *user_data);

/**
 * Abre diálogo para salvar arquivo.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param initial_name Nome inicial sugerido (pode ser NULL)
 * @param callback Função chamada com o caminho selecionado
 * @param user_data Dados do usuário para o callback
 */
void eg_file_chooser_save(EgWindow *parent, const char *title,
                          const char *initial_name,
                          EgFileChooserCallback callback, void *user_data);

/**
 * Abre diálogo para selecionar pasta.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param callback Função chamada com o caminho selecionado
 * @param user_data Dados do usuário para o callback
 */
void eg_file_chooser_select_folder(EgWindow *parent, const char *title,
                                    EgFileChooserCallback callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_FILE_CHOOSER_H */
