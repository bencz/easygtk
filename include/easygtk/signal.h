/**
 * EasyGTK - Sistema de Signals
 * 
 * Sistema de eventos com suporte a múltiplos listeners,
 * handler IDs para desconexão, e base para data binding.
 */

#ifndef EASYGTK_SIGNAL_H
#define EASYGTK_SIGNAL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Tipos opacos */
typedef uint32_t EgHandlerId;

/* Callback genérico para signals */
typedef void (*EgSignalCallback)(void *sender, void *event_data, void *user_data);

/**
 * Cria um novo signal.
 * 
 * @param name Nome do signal (para debug)
 * @return Ponteiro para o signal ou NULL
 */
EgSignal *eg_signal_new(const char *name);

/**
 * Libera o signal e todos os handlers.
 * 
 * @param signal Ponteiro para o signal
 */
void eg_signal_free(EgSignal *signal);

/**
 * Conecta um callback ao signal.
 * 
 * @param signal Ponteiro para o signal
 * @param callback Função de callback
 * @param user_data Dados do usuário
 * @return Handler ID para desconexão (0 se falhar)
 */
EgHandlerId eg_signal_connect(EgSignal *signal, EgSignalCallback callback, void *user_data);

/**
 * Desconecta um handler pelo ID.
 * 
 * @param signal Ponteiro para o signal
 * @param handler_id ID retornado por eg_signal_connect
 * @return true se desconectou com sucesso
 */
bool eg_signal_disconnect(EgSignal *signal, EgHandlerId handler_id);

/**
 * Desconecta todos os handlers.
 * 
 * @param signal Ponteiro para o signal
 */
void eg_signal_disconnect_all(EgSignal *signal);

/**
 * Emite o signal, chamando todos os handlers.
 * 
 * @param signal Ponteiro para o signal
 * @param sender Objeto que emitiu o signal
 * @param event_data Dados do evento (pode ser NULL)
 */
void eg_signal_emit(EgSignal *signal, void *sender, void *event_data);

/**
 * Retorna o número de handlers conectados.
 * 
 * @param signal Ponteiro para o signal
 * @return Número de handlers
 */
size_t eg_signal_get_handler_count(EgSignal *signal);

/**
 * Bloqueia temporariamente a emissão do signal.
 * 
 * @param signal Ponteiro para o signal
 */
void eg_signal_block(EgSignal *signal);

/**
 * Desbloqueia a emissão do signal.
 * 
 * @param signal Ponteiro para o signal
 */
void eg_signal_unblock(EgSignal *signal);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_SIGNAL_H */
