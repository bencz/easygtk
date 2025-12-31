/**
 * EasyGTK - ListView
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include "internal/vtable.h"
#include <easygtk/list_view.h>
#include <stdlib.h>

/* Funções da vtable */
static void list_view_destroy(EgWidget *widget);
static void *list_view_get_native(EgWidget *widget);
static void list_view_set_visible(EgWidget *widget, bool visible);
static bool list_view_get_visible(EgWidget *widget);
static void list_view_set_sensitive(EgWidget *widget, bool sensitive);
static bool list_view_get_sensitive(EgWidget *widget);

/* Funções de binding */
static void *list_view_bind_value(EgWidget *widget, const EgBindingContext *ctx);
static void list_view_unbind(EgWidget *widget, void *binding_data);

/* Capacidades de binding - selection binding com INT */
static const EgBindingCapabilities eg_list_view_binding_caps = {
    .primary_type = EG_PROPERTY_TYPE_INT,
    .default_mode = EG_BINDING_MODE_TWO_WAY,
    .supports_command = false
};

const EgWidgetVTable eg_list_view_vtable = {
    .type = EG_WIDGET_TYPE_LIST_VIEW,
    .type_name = "EgListView",
    .destroy = list_view_destroy,
    .get_native = list_view_get_native,
    .set_visible = list_view_set_visible,
    .get_visible = list_view_get_visible,
    .set_sensitive = list_view_set_sensitive,
    .get_sensitive = list_view_get_sensitive,
    /* Binding support */
    .binding_caps = &eg_list_view_binding_caps,
    .bind_value = list_view_bind_value,
    .bind_command = NULL,
    .unbind = list_view_unbind
};

static void list_view_destroy(EgWidget *widget) {
    EgListView *lv = (EgListView *)widget;
    if (lv == NULL) return;

    if (lv->store) {
        eg_free(lv->store);
    }
    eg_free(lv);
}

static void *list_view_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void list_view_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool list_view_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void list_view_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool list_view_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para seleção */
static void selection_changed_callback(GtkSelectionModel *model, guint position, guint n_items, gpointer user_data) {
    (void)model;
    (void)position;
    (void)n_items;
    EgListView *lv = (EgListView *)user_data;
    if (lv && lv->on_selection_changed) {
        lv->on_selection_changed((EgWidget *)lv, lv->selection_changed_data);
    }
}

/* Callback para ativação */
static void activate_callback(GtkListView *list_view, guint position, gpointer user_data) {
    (void)list_view;
    (void)position;
    EgListView *lv = (EgListView *)user_data;
    if (lv && lv->on_activate) {
        lv->on_activate((EgWidget *)lv, lv->activate_data);
    }
}

/* Setup para o factory */
static void setup_list_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    (void)factory;
    (void)user_data;

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_widget_set_margin_start(label, 8);
    gtk_widget_set_margin_end(label, 8);
    gtk_widget_set_margin_top(label, 6);
    gtk_widget_set_margin_bottom(label, 6);
    gtk_list_item_set_child(list_item, label);
}

/* Bind para o factory */
static void bind_list_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    (void)factory;
    (void)user_data;

    GtkWidget *label = gtk_list_item_get_child(list_item);
    GtkStringObject *obj = GTK_STRING_OBJECT(gtk_list_item_get_item(list_item));

    if (obj) {
        gtk_label_set_text(GTK_LABEL(label), gtk_string_object_get_string(obj));
    }
}

/* Cria selection model baseado no modo */
static GtkSelectionModel *create_selection_model(GListModel *model, EgSelectionMode mode) {
    switch (mode) {
        case EG_SELECTION_NONE:
            return GTK_SELECTION_MODEL(gtk_no_selection_new(model));
        case EG_SELECTION_SINGLE:
            return GTK_SELECTION_MODEL(gtk_single_selection_new(model));
        case EG_SELECTION_MULTIPLE:
            return GTK_SELECTION_MODEL(gtk_multi_selection_new(model));
        default:
            return GTK_SELECTION_MODEL(gtk_single_selection_new(model));
    }
}

EgListView *eg_list_view_new(EgSelectionMode selection_mode) {
    EgListView *lv = EG_ALLOC(EgListView);
    if (lv == NULL) return NULL;

    /* Cria o store */
    lv->store = EG_ALLOC(EgListStore);
    if (lv->store == NULL) {
        eg_free(lv);
        return NULL;
    }

    /* Usa GtkStringList para simplicidade */
    GtkStringList *string_list = gtk_string_list_new(NULL);
    lv->store->native = G_LIST_STORE(g_object_ref(string_list));

    /* Cria selection model */
    lv->selection_mode = selection_mode;
    lv->selection_model = create_selection_model(G_LIST_MODEL(string_list), selection_mode);

    /* Cria factory para itens */
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item), NULL);

    /* Cria o ListView */
    GtkWidget *list_view = gtk_list_view_new(lv->selection_model, factory);
    if (list_view == NULL) {
        g_object_unref(lv->selection_model);
        g_object_unref(string_list);
        eg_free(lv->store);
        eg_free(lv);
        return NULL;
    }

    eg_widget_init(&lv->base, EG_WIDGET_TYPE_LIST_VIEW, list_view, &eg_list_view_vtable);

    lv->on_selection_changed = NULL;
    lv->selection_changed_data = NULL;
    lv->on_activate = NULL;
    lv->activate_data = NULL;

    /* Ordenação */
    lv->sorter = NULL;
    lv->sort_model = NULL;
    lv->auto_sort_enabled = false;
    lv->auto_sort_ascending = true;
    lv->custom_compare = NULL;
    lv->custom_compare_data = NULL;

    return lv;
}

EgListView *eg_list_view_new_with_items(const char *const *items, EgSelectionMode selection_mode) {
    EgListView *lv = eg_list_view_new(selection_mode);
    if (lv == NULL) return NULL;

    if (items) {
        for (int i = 0; items[i] != NULL; i++) {
            eg_list_view_append(lv, items[i]);
        }
    }

    return lv;
}

void eg_list_view_free(EgListView *list_view) {
    list_view_destroy((EgWidget *)list_view);
}

void eg_list_view_append(EgListView *list_view, const char *text) {
    if (list_view == NULL || list_view->store == NULL || text == NULL) return;

    /* Obtém o model do selection */
    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    if (GTK_IS_STRING_LIST(model)) {
        gtk_string_list_append(GTK_STRING_LIST(model), text);
    }
}

void eg_list_view_insert(EgListView *list_view, unsigned int position, const char *text) {
    if (list_view == NULL || list_view->store == NULL || text == NULL) return;

    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    if (GTK_IS_STRING_LIST(model)) {
        /* GTK String List não tem insert, então precisamos fazer splice */
        const char *items[] = { text, NULL };
        gtk_string_list_splice(GTK_STRING_LIST(model), position, 0, items);
    }
}

void eg_list_view_remove(EgListView *list_view, unsigned int position) {
    if (list_view == NULL || list_view->store == NULL) return;

    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    if (GTK_IS_STRING_LIST(model)) {
        gtk_string_list_remove(GTK_STRING_LIST(model), position);
    }
}

void eg_list_view_clear(EgListView *list_view) {
    if (list_view == NULL || list_view->store == NULL) return;

    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    if (GTK_IS_STRING_LIST(model)) {
        guint n = g_list_model_get_n_items(model);
        if (n > 0) {
            gtk_string_list_splice(GTK_STRING_LIST(model), 0, n, NULL);
        }
    }
}

unsigned int eg_list_view_get_count(EgListView *list_view) {
    if (list_view == NULL || list_view->selection_model == NULL) return 0;

    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    return g_list_model_get_n_items(model);
}

const char *eg_list_view_get_item(EgListView *list_view, unsigned int position) {
    if (list_view == NULL || list_view->selection_model == NULL) return NULL;

    GListModel *model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    GtkStringObject *obj = GTK_STRING_OBJECT(g_list_model_get_item(model, position));
    if (obj == NULL) return NULL;

    const char *str = gtk_string_object_get_string(obj);
    g_object_unref(obj);
    return str;
}

void eg_list_view_select(EgListView *list_view, unsigned int position) {
    if (list_view == NULL || list_view->selection_model == NULL) return;

    gtk_selection_model_select_item(list_view->selection_model, position, TRUE);
}

void eg_list_view_unselect_all(EgListView *list_view) {
    if (list_view == NULL || list_view->selection_model == NULL) return;

    gtk_selection_model_unselect_all(list_view->selection_model);
}

void eg_list_view_select_all(EgListView *list_view) {
    if (list_view == NULL || list_view->selection_model == NULL) return;

    if (list_view->selection_mode == EG_SELECTION_MULTIPLE) {
        gtk_selection_model_select_all(list_view->selection_model);
    }
}

int eg_list_view_get_selected(EgListView *list_view) {
    if (list_view == NULL || list_view->selection_model == NULL) return -1;

    if (GTK_IS_SINGLE_SELECTION(list_view->selection_model)) {
        guint pos = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(list_view->selection_model));
        if (pos == GTK_INVALID_LIST_POSITION) return -1;
        return (int)pos;
    }

    /* Para multi-seleção, retorna o primeiro selecionado */
    GtkBitset *bitset = gtk_selection_model_get_selection(list_view->selection_model);
    if (gtk_bitset_is_empty(bitset)) return -1;

    guint first = gtk_bitset_get_minimum(bitset);
    return (int)first;
}

const char *eg_list_view_get_selected_text(EgListView *list_view) {
    int pos = eg_list_view_get_selected(list_view);
    if (pos < 0) return NULL;

    return eg_list_view_get_item(list_view, (unsigned int)pos);
}

unsigned int *eg_list_view_get_selected_indices(EgListView *list_view, unsigned int *count) {
    if (count) *count = 0;
    if (list_view == NULL || list_view->selection_model == NULL) return NULL;

    GtkBitset *bitset = gtk_selection_model_get_selection(list_view->selection_model);
    guint64 n = gtk_bitset_get_size(bitset);

    if (n == 0) return NULL;

    unsigned int *indices = (unsigned int *)malloc(n * sizeof(unsigned int));
    if (indices == NULL) return NULL;

    GtkBitsetIter iter;
    guint value;
    unsigned int i = 0;

    if (gtk_bitset_iter_init_first(&iter, bitset, &value)) {
        do {
            indices[i++] = value;
        } while (gtk_bitset_iter_next(&iter, &value));
    }

    if (count) *count = (unsigned int)n;
    return indices;
}

void eg_list_view_on_selection_changed(EgListView *list_view, EgCallback callback, void *user_data) {
    if (list_view == NULL) return;

    list_view->on_selection_changed = callback;
    list_view->selection_changed_data = user_data;

    if (callback && list_view->selection_model) {
        g_signal_connect(list_view->selection_model, "selection-changed",
                         G_CALLBACK(selection_changed_callback), list_view);
    }
}

void eg_list_view_on_activate(EgListView *list_view, EgCallback callback, void *user_data) {
    if (list_view == NULL) return;

    list_view->on_activate = callback;
    list_view->activate_data = user_data;

    if (callback && list_view->base.native) {
        g_signal_connect(list_view->base.native, "activate",
                         G_CALLBACK(activate_callback), list_view);
    }
}

void eg_list_view_set_show_separators(EgListView *list_view, bool show_separators) {
    if (list_view == NULL || list_view->base.native == NULL) return;
    gtk_list_view_set_show_separators(GTK_LIST_VIEW(list_view->base.native), show_separators);
}

void eg_list_view_set_single_click_activate(EgListView *list_view, bool single_click) {
    if (list_view == NULL || list_view->base.native == NULL) return;
    gtk_list_view_set_single_click_activate(GTK_LIST_VIEW(list_view->base.native), single_click);
}

void eg_list_view_scroll_to(EgListView *list_view, unsigned int position) {
    if (list_view == NULL || list_view->base.native == NULL) return;
    gtk_list_view_scroll_to(GTK_LIST_VIEW(list_view->base.native), position,
                            GTK_LIST_SCROLL_SELECT, NULL);
}

EgWidget *eg_list_view_as_widget(EgListView *list_view) {
    return (EgWidget *)list_view;
}

void *eg_list_view_get_native(EgListView *list_view) {
    if (list_view == NULL) return NULL;
    return list_view->base.native;
}

/* ============================================
 * Ordenação
 * ============================================ */

/* Estrutura auxiliar para QuickSort */
typedef struct {
    char **items;
    int count;
    EgListViewCompareFunc compare;
    void *user_data;
    bool ascending;
} SortContext;

/* Comparação padrão (strcmp) */
static int default_compare(const char *a, const char *b, void *user_data) {
    (void)user_data;
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return strcmp(a, b);
}

/* Função de comparação para qsort */
static int qsort_compare(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return default_compare(str_a, str_b, NULL);
}

static int qsort_compare_desc(const void *a, const void *b) {
    return -qsort_compare(a, b);
}

/* Função auxiliar para ordenar a lista */
static void sort_list_items(EgListView *list_view, bool ascending,
                            EgListViewCompareFunc compare_func, void *user_data) {
    if (list_view == NULL || list_view->selection_model == NULL) return;

    /* Obtem modelo */
    GListModel *model = NULL;
    if (GTK_IS_SINGLE_SELECTION(list_view->selection_model)) {
        model = gtk_single_selection_get_model(GTK_SINGLE_SELECTION(list_view->selection_model));
    } else if (GTK_IS_MULTI_SELECTION(list_view->selection_model)) {
        model = gtk_multi_selection_get_model(GTK_MULTI_SELECTION(list_view->selection_model));
    } else if (GTK_IS_NO_SELECTION(list_view->selection_model)) {
        model = gtk_no_selection_get_model(GTK_NO_SELECTION(list_view->selection_model));
    }

    if (!GTK_IS_STRING_LIST(model)) return;

    GtkStringList *string_list = GTK_STRING_LIST(model);
    guint n = g_list_model_get_n_items(G_LIST_MODEL(string_list));
    if (n <= 1) return;

    /* Copia todos os itens para array */
    char **items = (char **)malloc(n * sizeof(char *));
    if (items == NULL) return;

    for (guint i = 0; i < n; i++) {
        GtkStringObject *obj = GTK_STRING_OBJECT(g_list_model_get_item(G_LIST_MODEL(string_list), i));
        items[i] = g_strdup(gtk_string_object_get_string(obj));
        g_object_unref(obj);
    }

    /* Ordena usando qsort (para simplicidade, não usa compare_func customizada no qsort) */
    if (compare_func == NULL) {
        if (ascending) {
            qsort(items, n, sizeof(char *), qsort_compare);
        } else {
            qsort(items, n, sizeof(char *), qsort_compare_desc);
        }
    } else {
        /* Para função customizada, usamos bubble sort para manter user_data acessível */
        for (guint i = 0; i < n - 1; i++) {
            for (guint j = 0; j < n - i - 1; j++) {
                int cmp = compare_func(items[j], items[j + 1], user_data);
                if ((ascending && cmp > 0) || (!ascending && cmp < 0)) {
                    char *temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }

    /* Limpa e repopula a lista */
    gtk_string_list_splice(string_list, 0, n, NULL);

    for (guint i = 0; i < n; i++) {
        gtk_string_list_append(string_list, items[i]);
        g_free(items[i]);
    }

    free(items);
}

void eg_list_view_sort_ascending(EgListView *list_view) {
    sort_list_items(list_view, true, NULL, NULL);
}

void eg_list_view_sort_descending(EgListView *list_view) {
    sort_list_items(list_view, false, NULL, NULL);
}

void eg_list_view_sort_custom(EgListView *list_view, EgListViewCompareFunc compare_func, void *user_data) {
    if (compare_func == NULL) return;
    sort_list_items(list_view, true, compare_func, user_data);
}

void eg_list_view_set_auto_sort(EgListView *list_view, bool ascending) {
    if (list_view == NULL) return;
    list_view->auto_sort_enabled = true;
    list_view->auto_sort_ascending = ascending;
    /* Ordena imediatamente */
    sort_list_items(list_view, ascending, NULL, NULL);
}

void eg_list_view_disable_auto_sort(EgListView *list_view) {
    if (list_view == NULL) return;
    list_view->auto_sort_enabled = false;
}

/* ============================================
 * Binding Implementation (Selection)
 * ============================================ */

typedef struct {
    EgListView *list_view;
    EgProperty *property;
    EgHandlerId property_handler;
    gulong gtk_signal_id;
    bool updating;
} ListViewBindingData;

static void list_view_binding_on_property_changed(EgProperty *property, void *user_data) {
    ListViewBindingData *data = (ListViewBindingData *)user_data;
    if (data == NULL || data->list_view == NULL || data->updating) return;

    data->updating = true;
    int value = eg_property_get_int(property);
    if (value >= 0) {
        eg_list_view_select(data->list_view, (unsigned int)value);
    } else {
        eg_list_view_unselect_all(data->list_view);
    }
    data->updating = false;
}

static void list_view_binding_on_selection_changed(GtkSelectionModel *model,
                                                    guint position, guint n_items,
                                                    gpointer user_data) {
    (void)model;
    (void)position;
    (void)n_items;
    ListViewBindingData *data = (ListViewBindingData *)user_data;
    if (data == NULL || data->list_view == NULL || data->updating) return;

    data->updating = true;
    int selected = eg_list_view_get_selected(data->list_view);
    eg_property_set_int(data->property, selected);
    data->updating = false;
}

static void *list_view_bind_value(EgWidget *widget, const EgBindingContext *ctx) {
    EgListView *lv = (EgListView *)widget;
    if (lv == NULL || ctx == NULL || ctx->property == NULL) return NULL;

    if (eg_property_get_type(ctx->property) != EG_PROPERTY_TYPE_INT) {
        return NULL;
    }

    ListViewBindingData *data = EG_ALLOC(ListViewBindingData);
    if (data == NULL) return NULL;

    data->list_view = lv;
    data->property = ctx->property;
    data->updating = false;

    /* Property -> Widget */
    data->property_handler = eg_property_on_changed(ctx->property,
                                                     list_view_binding_on_property_changed, data);

    /* Widget -> Property (se two-way) */
    if (ctx->mode == EG_BINDING_MODE_TWO_WAY && lv->selection_model != NULL) {
        data->gtk_signal_id = g_signal_connect(lv->selection_model, "selection-changed",
                                                G_CALLBACK(list_view_binding_on_selection_changed), data);
    }

    /* Sincroniza valor inicial */
    list_view_binding_on_property_changed(ctx->property, data);

    return data;
}

static void list_view_unbind(EgWidget *widget, void *binding_data) {
    ListViewBindingData *data = (ListViewBindingData *)binding_data;
    if (data == NULL) return;

    if (data->property != NULL && data->property_handler != 0) {
        eg_property_disconnect(data->property, data->property_handler);
    }

    EgListView *lv = (EgListView *)widget;
    if (lv != NULL && lv->selection_model != NULL && data->gtk_signal_id != 0) {
        g_signal_handler_disconnect(lv->selection_model, data->gtk_signal_id);
    }

    eg_free(data);
}
