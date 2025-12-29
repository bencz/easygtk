/**
 * EasyGTK - AboutDialog
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/about_dialog.h>
#include <easygtk/window.h>

/* Estrutura interna */
struct EgAboutDialog {
    GtkAboutDialog *native;
};

EgAboutDialog *eg_about_dialog_new(void) {
    EgAboutDialog *dialog = EG_ALLOC(EgAboutDialog);
    if (dialog == NULL) return NULL;
    
    dialog->native = GTK_ABOUT_DIALOG(gtk_about_dialog_new());
    if (dialog->native == NULL) {
        eg_free(dialog);
        return NULL;
    }
    
    return dialog;
}

void eg_about_dialog_free(EgAboutDialog *dialog) {
    if (dialog == NULL) return;
    if (dialog->native != NULL) {
        gtk_window_destroy(GTK_WINDOW(dialog->native));
    }
    eg_free(dialog);
}

void eg_about_dialog_set_program_name(EgAboutDialog *dialog, const char *name) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_program_name(dialog->native, name);
}

void eg_about_dialog_set_version(EgAboutDialog *dialog, const char *version) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_version(dialog->native, version);
}

void eg_about_dialog_set_copyright(EgAboutDialog *dialog, const char *copyright) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_copyright(dialog->native, copyright);
}

void eg_about_dialog_set_comments(EgAboutDialog *dialog, const char *comments) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_comments(dialog->native, comments);
}

void eg_about_dialog_set_license(EgAboutDialog *dialog, const char *license) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_license(dialog->native, license);
}

void eg_about_dialog_set_wrap_license(EgAboutDialog *dialog, bool wrap_license) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_wrap_license(dialog->native, wrap_license);
}

void eg_about_dialog_set_website(EgAboutDialog *dialog, const char *website) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_website(dialog->native, website);
}

void eg_about_dialog_set_website_label(EgAboutDialog *dialog, const char *label) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_website_label(dialog->native, label);
}

void eg_about_dialog_set_authors(EgAboutDialog *dialog, const char **authors) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_authors(dialog->native, authors);
}

void eg_about_dialog_set_artists(EgAboutDialog *dialog, const char **artists) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_artists(dialog->native, artists);
}

void eg_about_dialog_set_documenters(EgAboutDialog *dialog, const char **documenters) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_documenters(dialog->native, documenters);
}

void eg_about_dialog_set_translator_credits(EgAboutDialog *dialog, const char *translator_credits) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_translator_credits(dialog->native, translator_credits);
}

void eg_about_dialog_set_logo_icon_name(EgAboutDialog *dialog, const char *icon_name) {
    if (dialog == NULL || dialog->native == NULL) return;
    gtk_about_dialog_set_logo_icon_name(dialog->native, icon_name);
}

void eg_about_dialog_show(EgAboutDialog *dialog, EgWindow *parent) {
    if (dialog == NULL || dialog->native == NULL) return;
    
    if (parent != NULL) {
        gtk_window_set_transient_for(GTK_WINDOW(dialog->native), 
                                      GTK_WINDOW(eg_window_get_native(parent)));
        gtk_window_set_modal(GTK_WINDOW(dialog->native), TRUE);
    }
    
    gtk_window_present(GTK_WINDOW(dialog->native));
}

void *eg_about_dialog_get_native(EgAboutDialog *dialog) {
    if (dialog == NULL) return NULL;
    return dialog->native;
}
