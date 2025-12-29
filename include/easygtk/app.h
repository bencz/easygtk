/**
 * EasyGTK - Aplicação
 * 
 * Gerencia o ciclo de vida da aplicação GTK.
 */

#ifndef EASYGTK_APP_H
#define EASYGTK_APP_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cria uma nova aplicação.
 * 
 * @param app_id Identificador da aplicação (ex: "com.example.app")
 * @return Ponteiro para a aplicação ou NULL em caso de erro
 */
EgApp *eg_app_new(const char *app_id);

/**
 * Libera os recursos da aplicação.
 * 
 * @param app Ponteiro para a aplicação
 */
void eg_app_free(EgApp *app);

/**
 * Executa o loop principal da aplicação.
 * 
 * @param app Ponteiro para a aplicação
 * @return Código de saída
 */
int eg_app_run(EgApp *app);

/**
 * Encerra a aplicação.
 * 
 * @param app Ponteiro para a aplicação
 */
void eg_app_quit(EgApp *app);

/**
 * Define callback para quando a aplicação for ativada.
 * 
 * @param app Ponteiro para a aplicação
 * @param callback Função de callback
 * @param user_data Dados do usuário passados ao callback
 */
void eg_app_on_activate(EgApp *app, EgCallback callback, void *user_data);

/**
 * Obtém o ponteiro interno do GtkApplication.
 * Para uso avançado quando necessário acesso direto ao GTK.
 * 
 * @param app Ponteiro para a aplicação
 * @return Ponteiro para GtkApplication
 */
void *eg_app_get_native(EgApp *app);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_APP_H */
