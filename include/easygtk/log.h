/**
 * EasyGTK - Logging e Debug
 * 
 * Sistema de logging para debug e diagnóstico.
 */

#ifndef EASYGTK_LOG_H
#define EASYGTK_LOG_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Níveis de log */
typedef enum EgLogLevel {
    EG_LOG_LEVEL_DEBUG = 0,
    EG_LOG_LEVEL_INFO = 1,
    EG_LOG_LEVEL_WARNING = 2,
    EG_LOG_LEVEL_ERROR = 3,
    EG_LOG_LEVEL_NONE = 4
} EgLogLevel;

/* Callback customizado de log */
typedef void (*EgLogCallback)(EgLogLevel level, const char *domain, 
                               const char *message, void *user_data);

/**
 * Define o nível mínimo de log a ser exibido.
 * Mensagens abaixo deste nível serão ignoradas.
 * 
 * @param level Nível mínimo
 */
void eg_log_set_level(EgLogLevel level);

/**
 * Obtém o nível atual de log.
 * 
 * @return Nível atual
 */
EgLogLevel eg_log_get_level(void);

/**
 * Define um callback customizado para processar logs.
 * Se NULL, usa o handler padrão (stderr).
 * 
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_log_set_handler(EgLogCallback callback, void *user_data);

/**
 * Habilita/desabilita cores no output (apenas para handler padrão).
 * 
 * @param enabled true para habilitar cores
 */
void eg_log_set_colors(bool enabled);

/**
 * Loga uma mensagem de debug.
 * 
 * @param domain Domínio/módulo (ex: "EgButton", "EgWindow")
 * @param format Formato printf
 * @param ... Argumentos
 */
void eg_log_debug(const char *domain, const char *format, ...);

/**
 * Loga uma mensagem informativa.
 * 
 * @param domain Domínio/módulo
 * @param format Formato printf
 * @param ... Argumentos
 */
void eg_log_info(const char *domain, const char *format, ...);

/**
 * Loga um aviso.
 * 
 * @param domain Domínio/módulo
 * @param format Formato printf
 * @param ... Argumentos
 */
void eg_log_warning(const char *domain, const char *format, ...);

/**
 * Loga um erro.
 * 
 * @param domain Domínio/módulo
 * @param format Formato printf
 * @param ... Argumentos
 */
void eg_log_error(const char *domain, const char *format, ...);

/**
 * Imprime informações de debug sobre um widget.
 * Usa widget->vtable->type_name para obter o nome do tipo.
 * 
 * @param widget Widget a inspecionar
 */
void eg_widget_debug_print(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_LOG_H */
