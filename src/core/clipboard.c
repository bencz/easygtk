/**
 * EasyGTK - Clipboard
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/clipboard.h>

/* Estrutura para dados do callback assíncrono */
typedef struct {
    EgClipboardTextCallback callback;
    void *user_data;
} ClipboardCallbackData;

/* Callback para leitura assíncrona */
static void clipboard_read_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    GdkClipboard *clipboard = GDK_CLIPBOARD(source);
    ClipboardCallbackData *data = (ClipboardCallbackData *)user_data;
    
    char *text = gdk_clipboard_read_text_finish(clipboard, result, NULL);
    
    if (data != NULL && data->callback != NULL) {
        data->callback(text, data->user_data);
    }
    
    g_free(text);
    eg_free(data);
}

void eg_clipboard_set_text(EgWidget *widget, const char *text) {
    if (widget == NULL || widget->native == NULL || text == NULL) return;
    
    GdkClipboard *clipboard = gtk_widget_get_clipboard(widget->native);
    if (clipboard == NULL) return;
    
    gdk_clipboard_set_text(clipboard, text);
}

void eg_clipboard_get_text(EgWidget *widget, EgClipboardTextCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    GdkClipboard *clipboard = gtk_widget_get_clipboard(widget->native);
    if (clipboard == NULL) return;
    
    ClipboardCallbackData *data = EG_ALLOC(ClipboardCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    
    gdk_clipboard_read_text_async(clipboard, NULL, clipboard_read_callback, data);
}

void eg_clipboard_clear(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return;
    
    GdkClipboard *clipboard = gtk_widget_get_clipboard(widget->native);
    if (clipboard == NULL) return;
    
    /* Definir conteúdo vazio limpa o clipboard */
    gdk_clipboard_set_text(clipboard, "");
}
