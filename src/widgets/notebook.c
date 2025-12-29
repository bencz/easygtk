/**
 * EasyGTK - Notebook (Tabs)
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/notebook.h>

/* Funções da vtable */
static void notebook_destroy(EgWidget *widget);
static void *notebook_get_native(EgWidget *widget);
static void notebook_set_visible(EgWidget *widget, bool visible);
static bool notebook_get_visible(EgWidget *widget);
static void notebook_set_sensitive(EgWidget *widget, bool sensitive);
static bool notebook_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_notebook_vtable = {
    .type = EG_WIDGET_TYPE_NOTEBOOK,
    .type_name = "EgNotebook",
    .destroy = notebook_destroy,
    .get_native = notebook_get_native,
    .set_visible = notebook_set_visible,
    .get_visible = notebook_get_visible,
    .set_sensitive = notebook_set_sensitive,
    .get_sensitive = notebook_get_sensitive
};

static void notebook_destroy(EgWidget *widget) {
    EgNotebook *notebook = (EgNotebook *)widget;
    if (notebook == NULL) return;
    eg_free(notebook);
}

static void *notebook_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void notebook_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool notebook_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void notebook_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool notebook_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback GTK para switch-page */
static void gtk_switch_page_callback(GtkNotebook *gtk_notebook, GtkWidget *page, 
                                      guint page_num, gpointer user_data) {
    (void)gtk_notebook; (void)page; (void)page_num;
    EgNotebook *notebook = (EgNotebook *)user_data;
    if (notebook != NULL && notebook->on_page_changed != NULL) {
        notebook->on_page_changed((EgWidget *)notebook, notebook->page_changed_data);
    }
}

EgNotebook *eg_notebook_new(void) {
    EgNotebook *notebook = EG_ALLOC(EgNotebook);
    if (notebook == NULL) return NULL;
    
    GtkWidget *gtk_notebook = gtk_notebook_new();
    if (gtk_notebook == NULL) {
        eg_free(notebook);
        return NULL;
    }
    
    eg_widget_init(&notebook->base, EG_WIDGET_TYPE_NOTEBOOK, gtk_notebook, &eg_notebook_vtable);
    notebook->on_page_changed = NULL;
    notebook->page_changed_data = NULL;
    
    return notebook;
}

void eg_notebook_free(EgNotebook *notebook) {
    notebook_destroy((EgWidget *)notebook);
}

int eg_notebook_append_page(EgNotebook *notebook, EgWidget *child, const char *label) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    if (child == NULL || child->native == NULL) return -1;
    
    GtkWidget *tab_label = gtk_label_new(label);
    return gtk_notebook_append_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label);
}

int eg_notebook_prepend_page(EgNotebook *notebook, EgWidget *child, const char *label) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    if (child == NULL || child->native == NULL) return -1;
    
    GtkWidget *tab_label = gtk_label_new(label);
    return gtk_notebook_prepend_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label);
}

int eg_notebook_insert_page(EgNotebook *notebook, EgWidget *child, const char *label, int position) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    if (child == NULL || child->native == NULL) return -1;
    
    GtkWidget *tab_label = gtk_label_new(label);
    return gtk_notebook_insert_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label, position);
}

void eg_notebook_remove_page(EgNotebook *notebook, int page_num) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook->base.native), page_num);
}

void eg_notebook_set_current_page(EgNotebook *notebook, int page_num) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook->base.native), page_num);
}

int eg_notebook_get_current_page(EgNotebook *notebook) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    return gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook->base.native));
}

int eg_notebook_get_n_pages(EgNotebook *notebook) {
    if (notebook == NULL || notebook->base.native == NULL) return 0;
    return gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook->base.native));
}

void eg_notebook_set_tab_pos(EgNotebook *notebook, int position) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    GtkPositionType pos = GTK_POS_TOP;
    switch (position) {
        case 0: pos = GTK_POS_LEFT; break;
        case 1: pos = GTK_POS_RIGHT; break;
        case 2: pos = GTK_POS_TOP; break;
        case 3: pos = GTK_POS_BOTTOM; break;
    }
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook->base.native), pos);
}

void eg_notebook_set_show_tabs(EgNotebook *notebook, bool show) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook->base.native), show);
}

void eg_notebook_set_show_border(EgNotebook *notebook, bool show) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook->base.native), show);
}

void eg_notebook_set_scrollable(EgNotebook *notebook, bool scrollable) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook->base.native), scrollable);
}

void eg_notebook_set_tab_label_text(EgNotebook *notebook, EgWidget *child, const char *label) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook->base.native), child->native, label);
}

void eg_notebook_on_page_changed(EgNotebook *notebook, EgCallback callback, void *user_data) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    
    notebook->on_page_changed = callback;
    notebook->page_changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(notebook->base.native, "switch-page",
                         G_CALLBACK(gtk_switch_page_callback), notebook);
    }
}

EgWidget *eg_notebook_as_widget(EgNotebook *notebook) {
    return (EgWidget *)notebook;
}

void *eg_notebook_get_native(EgNotebook *notebook) {
    if (notebook == NULL) return NULL;
    return notebook->base.native;
}
