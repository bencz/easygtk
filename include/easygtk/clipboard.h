/**
 * EasyGTK - Clipboard
 * 
 * Sistema de área de transferência (copiar/colar).
 */

#ifndef EASYGTK_CLIPBOARD_H
#define EASYGTK_CLIPBOARD_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Callback para quando o texto é obtido do clipboard */
typedef void (*EgClipboardTextCallback)(const char *text, void *user_data);

/**
 * Copia texto para o clipboard.
 * 
 * @param widget Widget de contexto (para obter o display)
 * @param text Texto a copiar
 */
void eg_clipboard_set_text(EgWidget *widget, const char *text);

/**
 * Obtém texto do clipboard de forma assíncrona.
 * 
 * @param widget Widget de contexto (para obter o display)
 * @param callback Função chamada com o texto
 * @param user_data Dados do usuário
 */
void eg_clipboard_get_text(EgWidget *widget, EgClipboardTextCallback callback, void *user_data);

/**
 * Limpa o clipboard.
 * 
 * @param widget Widget de contexto
 */
void eg_clipboard_clear(EgWidget *widget);

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_CLIPBOARD_H */
