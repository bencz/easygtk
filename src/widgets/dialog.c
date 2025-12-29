/**
 * EasyGTK - Diálogos
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/dialog.h>
#include <easygtk/window.h>

/* Estrutura para dados do callback assíncrono */
typedef struct {
    EgDialogCallback callback;
    void *user_data;
} DialogCallbackData;

/* Callback para AlertDialog */
static void alert_dialog_response_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    GtkAlertDialog *dialog = GTK_ALERT_DIALOG(source);
    DialogCallbackData *data = (DialogCallbackData *)user_data;
    
    int response = gtk_alert_dialog_choose_finish(dialog, result, NULL);
    
    if (data != NULL && data->callback != NULL) {
        EgDialogResponse eg_response;
        switch (response) {
            case 0: eg_response = EG_RESPONSE_YES; break;    /* Primeiro botão */
            case 1: eg_response = EG_RESPONSE_NO; break;     /* Segundo botão */
            case 2: eg_response = EG_RESPONSE_CANCEL; break; /* Terceiro botão */
            default: eg_response = EG_RESPONSE_NONE; break;
        }
        data->callback(eg_response, data->user_data);
    }
    
    eg_free(data);
}

/* Callback simples para diálogos de informação */
static void simple_dialog_response_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    (void)user_data;
    GtkAlertDialog *dialog = GTK_ALERT_DIALOG(source);
    gtk_alert_dialog_choose_finish(dialog, result, NULL);
}

void eg_dialog_info(EgWindow *parent, const char *title, const char *message) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", message ? message : "");
    
    if (title != NULL) {
        gtk_alert_dialog_set_detail(dialog, title);
    }
    
    const char *buttons[] = { "OK", NULL };
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);
    gtk_alert_dialog_set_cancel_button(dialog, 0);
    
    GtkWindow *gtk_parent = NULL;
    if (parent != NULL) {
        gtk_parent = GTK_WINDOW(eg_window_get_native(parent));
    }
    
    gtk_alert_dialog_choose(dialog, gtk_parent, NULL, simple_dialog_response_callback, NULL);
}

void eg_dialog_warning(EgWindow *parent, const char *title, const char *message) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : "Aviso");
    
    if (message != NULL) {
        gtk_alert_dialog_set_detail(dialog, message);
    }
    
    const char *buttons[] = { "OK", NULL };
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);
    gtk_alert_dialog_set_cancel_button(dialog, 0);
    
    GtkWindow *gtk_parent = NULL;
    if (parent != NULL) {
        gtk_parent = GTK_WINDOW(eg_window_get_native(parent));
    }
    
    gtk_alert_dialog_choose(dialog, gtk_parent, NULL, simple_dialog_response_callback, NULL);
}

void eg_dialog_error(EgWindow *parent, const char *title, const char *message) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : "Erro");
    
    if (message != NULL) {
        gtk_alert_dialog_set_detail(dialog, message);
    }
    
    const char *buttons[] = { "OK", NULL };
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);
    gtk_alert_dialog_set_cancel_button(dialog, 0);
    
    GtkWindow *gtk_parent = NULL;
    if (parent != NULL) {
        gtk_parent = GTK_WINDOW(eg_window_get_native(parent));
    }
    
    gtk_alert_dialog_choose(dialog, gtk_parent, NULL, simple_dialog_response_callback, NULL);
}

void eg_dialog_question(EgWindow *parent, const char *title, const char *message,
                        EgDialogCallback callback, void *user_data) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : "Pergunta");
    
    if (message != NULL) {
        gtk_alert_dialog_set_detail(dialog, message);
    }
    
    const char *buttons[] = { "Sim", "Não", NULL };
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);
    gtk_alert_dialog_set_cancel_button(dialog, 1);
    
    GtkWindow *gtk_parent = NULL;
    if (parent != NULL) {
        gtk_parent = GTK_WINDOW(eg_window_get_native(parent));
    }
    
    DialogCallbackData *data = EG_ALLOC(DialogCallbackData);
    if (data != NULL) {
        data->callback = callback;
        data->user_data = user_data;
    }
    
    gtk_alert_dialog_choose(dialog, gtk_parent, NULL, alert_dialog_response_callback, data);
}

void eg_dialog_confirm(EgWindow *parent, const char *title, const char *message,
                       EgDialogCallback callback, void *user_data) {
    GtkAlertDialog *dialog = gtk_alert_dialog_new("%s", title ? title : "Confirmar");
    
    if (message != NULL) {
        gtk_alert_dialog_set_detail(dialog, message);
    }
    
    const char *buttons[] = { "OK", "Cancelar", NULL };
    gtk_alert_dialog_set_buttons(dialog, buttons);
    gtk_alert_dialog_set_default_button(dialog, 0);
    gtk_alert_dialog_set_cancel_button(dialog, 1);
    
    GtkWindow *gtk_parent = NULL;
    if (parent != NULL) {
        gtk_parent = GTK_WINDOW(eg_window_get_native(parent));
    }
    
    DialogCallbackData *data = EG_ALLOC(DialogCallbackData);
    if (data != NULL) {
        data->callback = callback;
        data->user_data = user_data;
    }
    
    /* Para confirm, mapeamos: 0=OK, 1=Cancel */
    gtk_alert_dialog_choose(dialog, gtk_parent, NULL, alert_dialog_response_callback, data);
}
