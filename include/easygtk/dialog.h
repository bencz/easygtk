/**
 * EasyGTK - Diálogos
 * 
 * Sistema de diálogos para mensagens, alertas e confirmações.
 * Usa AlertDialog do GTK4 (substitui GtkMessageDialog deprecated).
 */

#ifndef EASYGTK_DIALOG_H
#define EASYGTK_DIALOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipos de resposta do diálogo */
typedef enum EgDialogResponse {
    EG_RESPONSE_NONE = 0,
    EG_RESPONSE_OK = 1,
    EG_RESPONSE_CANCEL = 2,
    EG_RESPONSE_YES = 3,
    EG_RESPONSE_NO = 4,
    EG_RESPONSE_CLOSE = 5
} EgDialogResponse;

/* Callback para resposta do diálogo */
typedef void (*EgDialogCallback)(EgDialogResponse response, void *user_data);

/**
 * Exibe um diálogo de informação.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param message Mensagem a exibir
 */
void eg_dialog_info(EgWindow *parent, const char *title, const char *message);

/**
 * Exibe um diálogo de aviso.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param message Mensagem a exibir
 */
void eg_dialog_warning(EgWindow *parent, const char *title, const char *message);

/**
 * Exibe um diálogo de erro.
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param message Mensagem a exibir
 */
void eg_dialog_error(EgWindow *parent, const char *title, const char *message);

/**
 * Exibe um diálogo de confirmação (Sim/Não).
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param message Mensagem a exibir
 * @param callback Função chamada com a resposta
 * @param user_data Dados do usuário
 */
void eg_dialog_question(EgWindow *parent, const char *title, const char *message,
                        EgDialogCallback callback, void *user_data);

/**
 * Exibe um diálogo de confirmação (OK/Cancelar).
 * 
 * @param parent Janela pai (pode ser NULL)
 * @param title Título do diálogo
 * @param message Mensagem a exibir
 * @param callback Função chamada com a resposta
 * @param user_data Dados do usuário
 */
void eg_dialog_confirm(EgWindow *parent, const char *title, const char *message,
                       EgDialogCallback callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_DIALOG_H */
