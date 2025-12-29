/**
 * EasyGTK - Timers
 * 
 * Sistema de timers e callbacks assíncronos.
 */

#ifndef EASYGTK_TIMER_H
#define EASYGTK_TIMER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ID do timer para cancelamento */
typedef uint32_t EgTimerId;

/* Callback de timer. Retorna true para continuar, false para parar */
typedef bool (*EgTimerCallback)(void *user_data);

/**
 * Adiciona um timer que dispara após um intervalo.
 * O callback será chamado repetidamente até retornar false.
 * 
 * @param interval_ms Intervalo em milissegundos
 * @param callback Função de callback
 * @param user_data Dados do usuário
 * @return Timer ID para cancelamento (0 se falhar)
 */
EgTimerId eg_timeout_add(uint32_t interval_ms, EgTimerCallback callback, void *user_data);

/**
 * Adiciona um timer que dispara apenas uma vez.
 * 
 * @param interval_ms Intervalo em milissegundos
 * @param callback Função de callback
 * @param user_data Dados do usuário
 * @return Timer ID para cancelamento (0 se falhar)
 */
EgTimerId eg_timeout_add_once(uint32_t interval_ms, EgTimerCallback callback, void *user_data);

/**
 * Adiciona um callback para ser executado quando o main loop estiver idle.
 * O callback será chamado repetidamente até retornar false.
 * 
 * @param callback Função de callback
 * @param user_data Dados do usuário
 * @return Timer ID para cancelamento (0 se falhar)
 */
EgTimerId eg_idle_add(EgTimerCallback callback, void *user_data);

/**
 * Adiciona um callback idle que executa apenas uma vez.
 * 
 * @param callback Função de callback
 * @param user_data Dados do usuário
 * @return Timer ID para cancelamento (0 se falhar)
 */
EgTimerId eg_idle_add_once(EgTimerCallback callback, void *user_data);

/**
 * Remove/cancela um timer ou idle callback.
 * 
 * @param timer_id ID retornado por eg_timeout_add ou eg_idle_add
 * @return true se removeu com sucesso
 */
bool eg_source_remove(EgTimerId timer_id);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_TIMER_H */
