/**
 * EasyGTK - Notebook (Tabs)
 *
 * Container com abas, suporta múltiplos filhos nomeados.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/notebook.h>

/* ============================================
 * Notebook Container VTable
 * ============================================ */

static void notebook_destroy(EgWidget *widget);
static void notebook_add_child(EgWidget *container, EgWidget *child);
static void notebook_remove_child(EgWidget *container, EgWidget *child);
static void notebook_add_named(EgWidget *container, EgWidget *child, const char *name);
static size_t notebook_get_child_count(EgWidget *container);
static EgWidget *notebook_get_child_at(EgWidget *container, size_t index);
static void notebook_clear(EgWidget *container);

static const EgContainerCapabilities notebook_caps = {
    .supports_multiple = true,
    .supports_named = true,
    .supports_positioned = false,
    .max_children = 0  /* Ilimitado */
};

const EgContainerVTable eg_notebook_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_NOTEBOOK,
        .type_name = "EgNotebook",
        .destroy = notebook_destroy,
        .get_native = eg_widget_get_native_default,
        .set_visible = eg_widget_set_visible_default,
        .get_visible = eg_widget_get_visible_default,
        .set_sensitive = eg_widget_set_sensitive_default,
        .get_sensitive = eg_widget_get_sensitive_default,
        .binding_caps = NULL,
        .bind_value = NULL,
        .bind_command = NULL,
        .unbind = NULL,
        .events = NULL,
        .validation = NULL
    },
    .caps = &notebook_caps,
    .add_child = notebook_add_child,
    .remove_child = notebook_remove_child,
    .add_named = notebook_add_named,
    .get_by_name = NULL,
    .get_child_count = notebook_get_child_count,
    .get_child_at = notebook_get_child_at,
    .clear = notebook_clear
};

/* ============================================
 * VTable Implementations
 * ============================================ */

static void notebook_destroy(EgWidget *widget) {
    EgNotebook *notebook = (EgNotebook *)widget;
    if (notebook == NULL) return;

    /* Destrói todos os filhos */
    for (size_t i = 0; i < notebook->children.count; i++) {
        EgWidget *child = notebook->children.children[i];
        if (child != NULL && child->vtable != NULL && child->vtable->destroy != NULL) {
            child->vtable->destroy(child);
        }
    }

    eg_child_list_free(&notebook->children);
    eg_free(notebook);
}

static void notebook_add_child(EgWidget *container, EgWidget *child) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL || notebook->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook->base.native), child->native, NULL);
    eg_child_list_add(&notebook->children, child);
}

static void notebook_remove_child(EgWidget *container, EgWidget *child) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL || notebook->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    int page_num = gtk_notebook_page_num(GTK_NOTEBOOK(notebook->base.native), child->native);
    if (page_num >= 0) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook->base.native), page_num);
        eg_child_list_remove(&notebook->children, child);
    }
}

static void notebook_add_named(EgWidget *container, EgWidget *child, const char *name) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL || notebook->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    GtkWidget *tab_label = gtk_label_new(name);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label);
    eg_child_list_add(&notebook->children, child);
}

static size_t notebook_get_child_count(EgWidget *container) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL) return 0;
    return notebook->children.count;
}

static EgWidget *notebook_get_child_at(EgWidget *container, size_t index) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL) return NULL;
    return eg_child_list_get(&notebook->children, index);
}

static void notebook_clear(EgWidget *container) {
    EgNotebook *notebook = (EgNotebook *)container;
    if (notebook == NULL || notebook->base.native == NULL) return;

    /* Remove todas as páginas */
    while (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook->base.native)) > 0) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(notebook->base.native), 0);
    }

    eg_child_list_clear(&notebook->children);
}

/* ============================================
 * Callback GTK
 * ============================================ */

static void gtk_switch_page_callback(GtkNotebook *gtk_notebook, GtkWidget *page,
                                      guint page_num, gpointer user_data) {
    (void)gtk_notebook; (void)page; (void)page_num;
    EgNotebook *notebook = (EgNotebook *)user_data;
    if (notebook != NULL && notebook->on_page_changed != NULL) {
        notebook->on_page_changed((EgWidget *)notebook, notebook->page_changed_data);
    }
}

/* ============================================
 * API Pública
 * ============================================ */

EgNotebook *eg_notebook_new(void) {
    EgNotebook *notebook = EG_ALLOC(EgNotebook);
    if (notebook == NULL) return NULL;

    GtkWidget *gtk_notebook = gtk_notebook_new();
    if (gtk_notebook == NULL) {
        eg_free(notebook);
        return NULL;
    }

    eg_widget_init(&notebook->base, EG_WIDGET_TYPE_NOTEBOOK, gtk_notebook, &eg_notebook_vtable.base);
    eg_child_list_init(&notebook->children);
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
    int result = gtk_notebook_append_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label);
    if (result >= 0) {
        eg_child_list_add(&notebook->children, child);
    }
    return result;
}

int eg_notebook_prepend_page(EgNotebook *notebook, EgWidget *child, const char *label) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    if (child == NULL || child->native == NULL) return -1;

    GtkWidget *tab_label = gtk_label_new(label);
    int result = gtk_notebook_prepend_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label);
    if (result >= 0) {
        eg_child_list_add(&notebook->children, child);
    }
    return result;
}

int eg_notebook_insert_page(EgNotebook *notebook, EgWidget *child, const char *label, int position) {
    if (notebook == NULL || notebook->base.native == NULL) return -1;
    if (child == NULL || child->native == NULL) return -1;

    GtkWidget *tab_label = gtk_label_new(label);
    int result = gtk_notebook_insert_page(GTK_NOTEBOOK(notebook->base.native), child->native, tab_label, position);
    if (result >= 0) {
        eg_child_list_add(&notebook->children, child);
    }
    return result;
}

void eg_notebook_remove_page(EgNotebook *notebook, int page_num) {
    if (notebook == NULL || notebook->base.native == NULL) return;
    if (page_num < 0 || (size_t)page_num >= notebook->children.count) return;

    EgWidget *child = eg_child_list_get(&notebook->children, (size_t)page_num);
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook->base.native), page_num);
    if (child != NULL) {
        eg_child_list_remove(&notebook->children, child);
    }
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
