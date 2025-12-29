/**
 * EasyGTK - Headers internos
 * 
 * Definições internas da biblioteca, não expostas na API pública.
 */

#ifndef EASYGTK_INTERNAL_H
#define EASYGTK_INTERNAL_H

#include <gtk/gtk.h>
#include <easygtk/types.h>
#include "vtable.h"

/* Macro interna para alocação segura */
#define EG_ALLOC(type) ((type *)eg_alloc(sizeof(type)))
#define EG_ALLOC_ARRAY(type, n) ((type *)eg_alloc(sizeof(type) * (n)))

/* Estrutura base de widget */
struct EgWidget {
    EgWidgetType type;
    GtkWidget *native;
    const EgWidgetVTable *vtable;
    void *user_data;
    EgDestroyNotify user_data_destroy;
    uint32_t ref_count;
    bool is_floating;
};

/* Estrutura de aplicação */
struct EgApp {
    GtkApplication *native;
    EgCallback on_activate;
    void *activate_data;
    int argc;
    char **argv;
};

/* Estrutura de janela */
struct EgWindow {
    EgWidget base;
    EgApp *app;
    EgCallback on_close;
    void *close_data;
    EgCallback on_destroy;
    void *destroy_data;
};

/* Estrutura de botão */
struct EgButton {
    EgWidget base;
    EgCallback on_click;
    void *click_data;
};

/* Estrutura de label */
struct EgLabel {
    EgWidget base;
};

/* Estrutura de entry */
struct EgEntry {
    EgWidget base;
    GtkEntryBuffer *buffer;
    EgCallback on_changed;
    void *changed_data;
    EgCallback on_activate;
    void *activate_data;
};

/* Lista de filhos para containers */
typedef struct EgChildList {
    EgWidget **children;
    size_t count;
    size_t capacity;
} EgChildList;

/* Estrutura de box */
struct EgBox {
    EgWidget base;
    EgChildList children;
};

/* Estrutura de grid */
struct EgGrid {
    EgWidget base;
    EgChildList children;
};

/* Estrutura de check button */
struct EgCheckButton {
    EgWidget base;
    EgCallback on_toggled;
    void *toggled_data;
};

/* Estrutura de switch */
struct EgSwitch {
    EgWidget base;
    EgCallback on_state_changed;
    void *state_changed_data;
};

/* Estrutura de progress bar */
struct EgProgressBar {
    EgWidget base;
};

/* Estrutura de combo box */
struct EgComboBox {
    EgWidget base;
    GtkStringList *model;
    EgCallback on_changed;
    void *changed_data;
};

/* Estrutura de scrolled window */
struct EgScrolledWindow {
    EgWidget base;
};

/* Estrutura de spin button */
struct EgSpinButton {
    EgWidget base;
    EgCallback on_value_changed;
    void *value_changed_data;
};

/* Estrutura de scale */
struct EgScale {
    EgWidget base;
    EgCallback on_value_changed;
    void *value_changed_data;
};

/* Estrutura de text view */
struct EgTextView {
    EgWidget base;
    GtkTextBuffer *buffer;
    EgCallback on_changed;
    void *changed_data;
};

/* Estrutura de stack */
struct EgStack {
    EgWidget base;
};

/* Estrutura de stack switcher */
struct EgStackSwitcher {
    EgWidget base;
};

/* Estrutura de notebook */
struct EgNotebook {
    EgWidget base;
    EgCallback on_page_changed;
    void *page_changed_data;
};

/* Estrutura de paned */
struct EgPaned {
    EgWidget base;
};

/* Estrutura de frame */
struct EgFrame {
    EgWidget base;
};

/* Estrutura de expander */
struct EgExpander {
    EgWidget base;
};

/* Estrutura para dados de callback */
typedef struct {
    EgCallback callback;
    void *user_data;
    EgWidget *widget;
} EgCallbackData;

/* Funções internas de memória */
void *eg_alloc(size_t size);
void *eg_realloc(void *ptr, size_t size);
void eg_free(void *ptr);
char *eg_strdup(const char *str);

/* Funções internas de widget */
void eg_widget_init(EgWidget *widget, EgWidgetType type, GtkWidget *native, const EgWidgetVTable *vtable);
void eg_widget_destroy_internal(EgWidget *widget);

/* Funções de conversão de alinhamento */
GtkAlign eg_align_to_gtk(EgAlign align);
EgAlign gtk_align_to_eg(GtkAlign align);

/* Criação de dados de callback */
EgCallbackData *eg_callback_data_new(EgCallback callback, void *user_data, EgWidget *widget);
void eg_callback_data_free(EgCallbackData *data);

#endif /* EASYGTK_INTERNAL_H */
