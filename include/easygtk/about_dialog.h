/**
 * EasyGTK - AboutDialog
 * 
 * Diálogo "Sobre" para exibir informações do aplicativo.
 */

#ifndef EASYGTK_ABOUT_DIALOG_H
#define EASYGTK_ABOUT_DIALOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Cria um novo diálogo "Sobre".
 * 
 * @return Ponteiro para o diálogo ou NULL em caso de erro
 */
EgAboutDialog *eg_about_dialog_new(void);

/**
 * Libera os recursos do diálogo.
 * 
 * @param dialog Ponteiro para o diálogo
 */
void eg_about_dialog_free(EgAboutDialog *dialog);

/**
 * Define o nome do programa.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param name Nome do programa
 */
void eg_about_dialog_set_program_name(EgAboutDialog *dialog, const char *name);

/**
 * Define a versão do programa.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param version Versão (ex: "1.0.0")
 */
void eg_about_dialog_set_version(EgAboutDialog *dialog, const char *version);

/**
 * Define o copyright.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param copyright Texto de copyright
 */
void eg_about_dialog_set_copyright(EgAboutDialog *dialog, const char *copyright);

/**
 * Define os comentários/descrição.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param comments Comentários sobre o programa
 */
void eg_about_dialog_set_comments(EgAboutDialog *dialog, const char *comments);

/**
 * Define a licença.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param license Texto da licença
 */
void eg_about_dialog_set_license(EgAboutDialog *dialog, const char *license);

/**
 * Define se a licença deve quebrar linhas automaticamente.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param wrap_license true para quebrar linhas
 */
void eg_about_dialog_set_wrap_license(EgAboutDialog *dialog, bool wrap_license);

/**
 * Define o website.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param website URL do website
 */
void eg_about_dialog_set_website(EgAboutDialog *dialog, const char *website);

/**
 * Define o label do website.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param label Label para o link do website
 */
void eg_about_dialog_set_website_label(EgAboutDialog *dialog, const char *label);

/**
 * Define os autores.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param authors Array de strings NULL-terminated com nomes dos autores
 */
void eg_about_dialog_set_authors(EgAboutDialog *dialog, const char **authors);

/**
 * Define os artistas.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param artists Array de strings NULL-terminated com nomes dos artistas
 */
void eg_about_dialog_set_artists(EgAboutDialog *dialog, const char **artists);

/**
 * Define os documentadores.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param documenters Array de strings NULL-terminated
 */
void eg_about_dialog_set_documenters(EgAboutDialog *dialog, const char **documenters);

/**
 * Define os créditos de tradução.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param translator_credits Créditos de tradução
 */
void eg_about_dialog_set_translator_credits(EgAboutDialog *dialog, const char *translator_credits);

/**
 * Define o nome do ícone do logo.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param icon_name Nome do ícone
 */
void eg_about_dialog_set_logo_icon_name(EgAboutDialog *dialog, const char *icon_name);

/**
 * Exibe o diálogo.
 * 
 * @param dialog Ponteiro para o diálogo
 * @param parent Janela pai (pode ser NULL)
 */
void eg_about_dialog_show(EgAboutDialog *dialog, EgWindow *parent);

/**
 * Obtém o widget GTK nativo.
 * 
 * @param dialog Ponteiro para o diálogo
 * @return Ponteiro para GtkAboutDialog
 */
void *eg_about_dialog_get_native(EgAboutDialog *dialog);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_ABOUT_DIALOG_H */
