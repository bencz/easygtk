/**
 * EasyGTK - ColumnView
 *
 * Tabela com múltiplas colunas usando GtkColumnView.
 * Usa um GListStore com objetos customizados para armazenar linhas.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/column_view.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Número máximo de colunas suportadas */
#define MAX_COLUMNS 32

/* Objeto para representar uma linha da tabela */
#define EG_TYPE_ROW_DATA (eg_row_data_get_type())
G_DECLARE_FINAL_TYPE(EgRowData, eg_row_data, EG, ROW_DATA, GObject)

struct _EgRowData {
    GObject parent_instance;
    char *values[MAX_COLUMNS];
    int n_columns;
};

G_DEFINE_TYPE(EgRowData, eg_row_data, G_TYPE_OBJECT)

static void eg_row_data_finalize(GObject *object) {
    EgRowData *self = EG_ROW_DATA(object);
    for (int i = 0; i < self->n_columns; i++) {
        g_free(self->values[i]);
    }
    G_OBJECT_CLASS(eg_row_data_parent_class)->finalize(object);
}

static void eg_row_data_class_init(EgRowDataClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = eg_row_data_finalize;
}

static void eg_row_data_init(EgRowData *self) {
    self->n_columns = 0;
    for (int i = 0; i < MAX_COLUMNS; i++) {
        self->values[i] = NULL;
    }
}

static EgRowData *eg_row_data_new(int n_columns) {
    EgRowData *row = g_object_new(EG_TYPE_ROW_DATA, NULL);
    row->n_columns = n_columns;
    return row;
}

static void eg_row_data_set_value(EgRowData *row, int column, const char *value) {
    if (row == NULL || column < 0 || column >= MAX_COLUMNS) return;
    g_free(row->values[column]);
    row->values[column] = g_strdup(value);
    if (column >= row->n_columns) {
        row->n_columns = column + 1;
    }
}

static const char *eg_row_data_get_value(EgRowData *row, int column) {
    if (row == NULL || column < 0 || column >= row->n_columns) return NULL;
    return row->values[column];
}

/* Estrutura para dados do factory */
typedef struct {
    int column_index;
} ColumnFactoryData;

/* Wrapper para g_free compatível com GClosureNotify */
static void factory_data_free(gpointer data, GClosure *closure) {
    (void)closure;
    g_free(data);
}

/* Funções da vtable */
static void column_view_destroy(EgWidget *widget);
static void *column_view_get_native(EgWidget *widget);
static void column_view_set_visible(EgWidget *widget, bool visible);
static bool column_view_get_visible(EgWidget *widget);
static void column_view_set_sensitive(EgWidget *widget, bool sensitive);
static bool column_view_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_column_view_vtable = {
    .type = EG_WIDGET_TYPE_COLUMN_VIEW,
    .type_name = "EgColumnView",
    .destroy = column_view_destroy,
    .get_native = column_view_get_native,
    .set_visible = column_view_set_visible,
    .get_visible = column_view_get_visible,
    .set_sensitive = column_view_set_sensitive,
    .get_sensitive = column_view_get_sensitive
};

static void column_view_destroy(EgWidget *widget) {
    EgColumnView *cv = (EgColumnView *)widget;
    if (cv == NULL) return;

    if (cv->sort_model) {
        g_object_unref(cv->sort_model);
    }
    if (cv->store) {
        if (cv->store->native) {
            g_object_unref(cv->store->native);
        }
        eg_free(cv->store);
    }
    eg_free(cv);
}

static void *column_view_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void column_view_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool column_view_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void column_view_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool column_view_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para seleção */
static void cv_selection_changed_callback(GtkSelectionModel *model, guint position, guint n_items, gpointer user_data) {
    (void)model;
    (void)position;
    (void)n_items;
    EgColumnView *cv = (EgColumnView *)user_data;
    if (cv && cv->on_selection_changed) {
        cv->on_selection_changed((EgWidget *)cv, cv->selection_changed_data);
    }
}

/* Callback para ativação */
static void cv_activate_callback(GtkColumnView *column_view, guint position, gpointer user_data) {
    (void)column_view;
    (void)position;
    EgColumnView *cv = (EgColumnView *)user_data;
    if (cv && cv->on_activate) {
        cv->on_activate((EgWidget *)cv, cv->activate_data);
    }
}

/* Setup para factory de coluna */
static void setup_column_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    (void)factory;
    (void)user_data;

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_margin_start(label, 8);
    gtk_widget_set_margin_end(label, 8);
    gtk_widget_set_margin_top(label, 6);
    gtk_widget_set_margin_bottom(label, 6);
    gtk_list_item_set_child(list_item, label);
}

/* Bind para factory de coluna */
static void bind_column_item(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    (void)factory;
    ColumnFactoryData *data = (ColumnFactoryData *)user_data;

    GtkWidget *label = gtk_list_item_get_child(list_item);
    EgRowData *row = EG_ROW_DATA(gtk_list_item_get_item(list_item));

    if (row && data) {
        const char *value = eg_row_data_get_value(row, data->column_index);
        gtk_label_set_text(GTK_LABEL(label), value ? value : "");
    }
}

/* Cria selection model baseado no modo */
static GtkSelectionModel *cv_create_selection_model(GListModel *model, EgSelectionMode mode) {
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

EgColumnView *eg_column_view_new(EgSelectionMode selection_mode) {
    EgColumnView *cv = EG_ALLOC(EgColumnView);
    if (cv == NULL) return NULL;

    /* Cria o store */
    cv->store = EG_ALLOC(EgListStore);
    if (cv->store == NULL) {
        eg_free(cv);
        return NULL;
    }

    /* Cria GListStore para EgRowData */
    cv->store->native = g_list_store_new(EG_TYPE_ROW_DATA);
    cv->store->item_type = EG_TYPE_ROW_DATA;

    /* Cria o ColumnView primeiro (sem model) para obter o sorter */
    GtkWidget *column_view = gtk_column_view_new(NULL);
    if (column_view == NULL) {
        g_object_unref(cv->store->native);
        eg_free(cv->store);
        eg_free(cv);
        return NULL;
    }

    /* Cria GtkSortListModel que conecta o store ao sorter do column view */
    GtkSorter *cv_sorter = gtk_column_view_get_sorter(GTK_COLUMN_VIEW(column_view));
    cv->sort_model = gtk_sort_list_model_new(G_LIST_MODEL(cv->store->native), cv_sorter);
    if (cv_sorter) {
        g_object_ref(cv_sorter);  /* sort_list_model takes ownership, we need to keep ref */
    }

    /* Cria selection model a partir do sort model */
    cv->selection_mode = selection_mode;
    cv->selection_model = cv_create_selection_model(G_LIST_MODEL(cv->sort_model), selection_mode);

    /* Define o model no column view */
    gtk_column_view_set_model(GTK_COLUMN_VIEW(column_view), cv->selection_model);

    eg_widget_init(&cv->base, EG_WIDGET_TYPE_COLUMN_VIEW, column_view, &eg_column_view_vtable);

    cv->on_selection_changed = NULL;
    cv->selection_changed_data = NULL;
    cv->on_activate = NULL;
    cv->activate_data = NULL;

    /* Ordenação */
    cv->sort_column = -1;
    cv->sort_ascending = true;
    cv->custom_compare = NULL;
    cv->custom_compare_data = NULL;

    return cv;
}

void eg_column_view_free(EgColumnView *column_view) {
    column_view_destroy((EgWidget *)column_view);
}

int eg_column_view_add_column(EgColumnView *column_view, const char *title, int column_index) {
    return eg_column_view_add_column_with_width(column_view, title, column_index, -1);
}

int eg_column_view_add_column_with_width(EgColumnView *column_view, const char *title,
                                          int column_index, int fixed_width) {
    if (column_view == NULL || column_view->base.native == NULL || title == NULL) return -1;
    if (column_index < 0 || column_index >= MAX_COLUMNS) return -1;

    /* Cria factory para esta coluna */
    ColumnFactoryData *factory_data = g_new(ColumnFactoryData, 1);
    factory_data->column_index = column_index;

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_column_item), NULL);
    g_signal_connect_data(factory, "bind", G_CALLBACK(bind_column_item),
                          factory_data, factory_data_free, 0);

    /* Cria a coluna */
    GtkColumnViewColumn *column = gtk_column_view_column_new(title, factory);

    if (fixed_width > 0) {
        gtk_column_view_column_set_fixed_width(column, fixed_width);
    }

    gtk_column_view_column_set_resizable(column, TRUE);

    /* Adiciona ao column view */
    gtk_column_view_append_column(GTK_COLUMN_VIEW(column_view->base.native), column);

    /* Retorna o índice da coluna */
    GListModel *columns = gtk_column_view_get_columns(GTK_COLUMN_VIEW(column_view->base.native));
    return (int)g_list_model_get_n_items(columns) - 1;
}

void eg_column_view_set_column_resizable(EgColumnView *column_view, int column_id, bool resizable) {
    if (column_view == NULL || column_view->base.native == NULL) return;

    GListModel *columns = gtk_column_view_get_columns(GTK_COLUMN_VIEW(column_view->base.native));
    GtkColumnViewColumn *column = GTK_COLUMN_VIEW_COLUMN(g_list_model_get_item(columns, column_id));
    if (column) {
        gtk_column_view_column_set_resizable(column, resizable);
        g_object_unref(column);
    }
}

void eg_column_view_set_column_expand(EgColumnView *column_view, int column_id, bool expand) {
    if (column_view == NULL || column_view->base.native == NULL) return;

    GListModel *columns = gtk_column_view_get_columns(GTK_COLUMN_VIEW(column_view->base.native));
    GtkColumnViewColumn *column = GTK_COLUMN_VIEW_COLUMN(g_list_model_get_item(columns, column_id));
    if (column) {
        gtk_column_view_column_set_expand(column, expand);
        g_object_unref(column);
    }
}

int eg_column_view_append_row(EgColumnView *column_view, const char *const *values) {
    if (column_view == NULL || column_view->store == NULL || values == NULL) return -1;

    /* Conta colunas */
    int n = 0;
    while (values[n] != NULL) n++;

    EgRowData *row = eg_row_data_new(n);
    for (int i = 0; i < n; i++) {
        eg_row_data_set_value(row, i, values[i]);
    }

    g_list_store_append(column_view->store->native, row);
    g_object_unref(row);

    return (int)g_list_model_get_n_items(G_LIST_MODEL(column_view->store->native)) - 1;
}

int eg_column_view_append_rowv(EgColumnView *column_view, const char *first_value, ...) {
    if (column_view == NULL || column_view->store == NULL) return -1;

    /* Coleta valores do varargs */
    const char *values[MAX_COLUMNS + 1];
    int n = 0;

    values[n++] = first_value;

    va_list args;
    va_start(args, first_value);

    const char *val;
    while ((val = va_arg(args, const char *)) != NULL && n < MAX_COLUMNS) {
        values[n++] = val;
    }
    values[n] = NULL;

    va_end(args);

    return eg_column_view_append_row(column_view, values);
}

void eg_column_view_set_cell(EgColumnView *column_view, unsigned int row,
                              unsigned int column, const char *value) {
    if (column_view == NULL || column_view->store == NULL) return;

    EgRowData *row_data = EG_ROW_DATA(g_list_model_get_item(
        G_LIST_MODEL(column_view->store->native), row));

    if (row_data) {
        eg_row_data_set_value(row_data, (int)column, value);
        g_object_unref(row_data);

        /* Notifica mudança para atualizar a view */
        g_list_store_remove(column_view->store->native, row);
        g_list_store_insert(column_view->store->native, row, row_data);
    }
}

const char *eg_column_view_get_cell(EgColumnView *column_view, unsigned int row,
                                     unsigned int column) {
    if (column_view == NULL || column_view->store == NULL) return NULL;

    EgRowData *row_data = EG_ROW_DATA(g_list_model_get_item(
        G_LIST_MODEL(column_view->store->native), row));

    if (row_data == NULL) return NULL;

    const char *value = eg_row_data_get_value(row_data, (int)column);
    g_object_unref(row_data);
    return value;
}

void eg_column_view_remove_row(EgColumnView *column_view, unsigned int row) {
    if (column_view == NULL || column_view->store == NULL) return;
    g_list_store_remove(column_view->store->native, row);
}

void eg_column_view_clear(EgColumnView *column_view) {
    if (column_view == NULL || column_view->store == NULL) return;
    g_list_store_remove_all(column_view->store->native);
}

unsigned int eg_column_view_get_row_count(EgColumnView *column_view) {
    if (column_view == NULL || column_view->store == NULL) return 0;
    return g_list_model_get_n_items(G_LIST_MODEL(column_view->store->native));
}

unsigned int eg_column_view_get_column_count(EgColumnView *column_view) {
    if (column_view == NULL || column_view->base.native == NULL) return 0;
    GListModel *columns = gtk_column_view_get_columns(GTK_COLUMN_VIEW(column_view->base.native));
    return g_list_model_get_n_items(columns);
}

void eg_column_view_select_row(EgColumnView *column_view, unsigned int row) {
    if (column_view == NULL || column_view->selection_model == NULL) return;
    gtk_selection_model_select_item(column_view->selection_model, row, TRUE);
}

void eg_column_view_unselect_all(EgColumnView *column_view) {
    if (column_view == NULL || column_view->selection_model == NULL) return;
    gtk_selection_model_unselect_all(column_view->selection_model);
}

int eg_column_view_get_selected_row(EgColumnView *column_view) {
    if (column_view == NULL || column_view->selection_model == NULL) return -1;

    if (GTK_IS_SINGLE_SELECTION(column_view->selection_model)) {
        guint pos = gtk_single_selection_get_selected(GTK_SINGLE_SELECTION(column_view->selection_model));
        if (pos == GTK_INVALID_LIST_POSITION) return -1;
        return (int)pos;
    }

    GtkBitset *bitset = gtk_selection_model_get_selection(column_view->selection_model);
    if (gtk_bitset_is_empty(bitset)) return -1;

    return (int)gtk_bitset_get_minimum(bitset);
}

unsigned int *eg_column_view_get_selected_rows(EgColumnView *column_view, unsigned int *count) {
    if (count) *count = 0;
    if (column_view == NULL || column_view->selection_model == NULL) return NULL;

    GtkBitset *bitset = gtk_selection_model_get_selection(column_view->selection_model);
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

void eg_column_view_on_selection_changed(EgColumnView *column_view, EgCallback callback, void *user_data) {
    if (column_view == NULL) return;

    column_view->on_selection_changed = callback;
    column_view->selection_changed_data = user_data;

    if (callback && column_view->selection_model) {
        g_signal_connect(column_view->selection_model, "selection-changed",
                         G_CALLBACK(cv_selection_changed_callback), column_view);
    }
}

void eg_column_view_on_activate(EgColumnView *column_view, EgCallback callback, void *user_data) {
    if (column_view == NULL) return;

    column_view->on_activate = callback;
    column_view->activate_data = user_data;

    if (callback && column_view->base.native) {
        g_signal_connect(column_view->base.native, "activate",
                         G_CALLBACK(cv_activate_callback), column_view);
    }
}

void eg_column_view_set_show_column_headers(EgColumnView *column_view, bool show) {
    if (column_view == NULL || column_view->base.native == NULL) return;
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(column_view->base.native), show);
}

void eg_column_view_set_show_row_separators(EgColumnView *column_view, bool show) {
    if (column_view == NULL || column_view->base.native == NULL) return;
    gtk_column_view_set_show_row_separators(GTK_COLUMN_VIEW(column_view->base.native), show);
}

void eg_column_view_set_show_column_separators(EgColumnView *column_view, bool show) {
    if (column_view == NULL || column_view->base.native == NULL) return;
    gtk_column_view_set_show_column_separators(GTK_COLUMN_VIEW(column_view->base.native), show);
}

EgWidget *eg_column_view_as_widget(EgColumnView *column_view) {
    return (EgWidget *)column_view;
}

void *eg_column_view_get_native(EgColumnView *column_view) {
    if (column_view == NULL) return NULL;
    return column_view->base.native;
}

/* ============================================
 * Ordenação
 * ============================================ */

/* Estrutura para contexto de ordenação */
typedef struct {
    unsigned int column;
    bool ascending;
    EgColumnViewCompareFunc compare_func;
    void *user_data;
} ColumnSortContext;

/* Comparação padrão (strcmp) */
static int cv_default_compare(const char *a, const char *b, void *user_data) {
    (void)user_data;
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;
    return strcmp(a, b);
}

/* Função auxiliar para ordenar as linhas */
static void sort_column_view_rows(EgColumnView *column_view, unsigned int column, bool ascending,
                                   EgColumnViewCompareFunc compare_func, void *user_data) {
    if (column_view == NULL || column_view->store == NULL) return;

    GListStore *store = column_view->store->native;
    guint n = g_list_model_get_n_items(G_LIST_MODEL(store));
    if (n <= 1) return;

    /* Coleta todas as linhas */
    EgRowData **rows = (EgRowData **)malloc(n * sizeof(EgRowData *));
    if (rows == NULL) return;

    for (guint i = 0; i < n; i++) {
        rows[i] = EG_ROW_DATA(g_list_model_get_item(G_LIST_MODEL(store), i));
    }

    /* Bubble sort para ordenação com compare_func customizada */
    EgColumnViewCompareFunc cmp = compare_func ? compare_func : cv_default_compare;

    for (guint i = 0; i < n - 1; i++) {
        for (guint j = 0; j < n - i - 1; j++) {
            const char *val_a = eg_row_data_get_value(rows[j], (int)column);
            const char *val_b = eg_row_data_get_value(rows[j + 1], (int)column);

            int result = cmp(val_a, val_b, user_data);

            if ((ascending && result > 0) || (!ascending && result < 0)) {
                EgRowData *temp = rows[j];
                rows[j] = rows[j + 1];
                rows[j + 1] = temp;
            }
        }
    }

    /* Remove todas as linhas e reinsere ordenadas */
    g_list_store_remove_all(store);

    for (guint i = 0; i < n; i++) {
        g_list_store_append(store, rows[i]);
        g_object_unref(rows[i]);
    }

    free(rows);
}

void eg_column_view_sort_by_column(EgColumnView *column_view, unsigned int column, bool ascending) {
    sort_column_view_rows(column_view, column, ascending, NULL, NULL);
}

void eg_column_view_sort_custom(EgColumnView *column_view, unsigned int column,
                                 EgColumnViewCompareFunc compare_func, void *user_data) {
    if (compare_func == NULL) return;
    sort_column_view_rows(column_view, column, true, compare_func, user_data);
}

void eg_column_view_set_auto_sort_column(EgColumnView *column_view, int column, bool ascending) {
    if (column_view == NULL) return;
    column_view->sort_column = column;
    column_view->sort_ascending = ascending;

    if (column >= 0) {
        /* Ordena imediatamente */
        sort_column_view_rows(column_view, (unsigned int)column, ascending, NULL, NULL);
    }
}

/* ============================================
 * Sorter customizado para ordenação por clique
 * ============================================ */

#define EG_TYPE_COLUMN_SORTER (eg_column_sorter_get_type())
G_DECLARE_FINAL_TYPE(EgColumnSorter, eg_column_sorter, EG, COLUMN_SORTER, GtkSorter)

struct _EgColumnSorter {
    GtkSorter parent_instance;
    int column_index;
};

G_DEFINE_TYPE(EgColumnSorter, eg_column_sorter, GTK_TYPE_SORTER)

static GtkOrdering eg_column_sorter_compare(GtkSorter *sorter, gpointer item1, gpointer item2) {
    EgColumnSorter *self = EG_COLUMN_SORTER(sorter);

    EgRowData *row1 = EG_ROW_DATA(item1);
    EgRowData *row2 = EG_ROW_DATA(item2);

    const char *val1 = eg_row_data_get_value(row1, self->column_index);
    const char *val2 = eg_row_data_get_value(row2, self->column_index);

    if (val1 == NULL && val2 == NULL) return GTK_ORDERING_EQUAL;
    if (val1 == NULL) return GTK_ORDERING_SMALLER;
    if (val2 == NULL) return GTK_ORDERING_LARGER;

    int result = strcmp(val1, val2);
    if (result < 0) return GTK_ORDERING_SMALLER;
    if (result > 0) return GTK_ORDERING_LARGER;
    return GTK_ORDERING_EQUAL;
}

static GtkSorterOrder eg_column_sorter_get_order(GtkSorter *sorter) {
    (void)sorter;
    return GTK_SORTER_ORDER_PARTIAL;
}

static void eg_column_sorter_class_init(EgColumnSorterClass *klass) {
    GtkSorterClass *sorter_class = GTK_SORTER_CLASS(klass);
    sorter_class->compare = eg_column_sorter_compare;
    sorter_class->get_order = eg_column_sorter_get_order;
}

static void eg_column_sorter_init(EgColumnSorter *self) {
    self->column_index = 0;
}

static GtkSorter *eg_column_sorter_new(int column_index) {
    EgColumnSorter *sorter = g_object_new(EG_TYPE_COLUMN_SORTER, NULL);
    sorter->column_index = column_index;
    return GTK_SORTER(sorter);
}

void eg_column_view_set_column_sortable(EgColumnView *column_view, int column_id, bool sortable) {
    if (column_view == NULL || column_view->base.native == NULL) return;

    GListModel *columns = gtk_column_view_get_columns(GTK_COLUMN_VIEW(column_view->base.native));
    GtkColumnViewColumn *column = GTK_COLUMN_VIEW_COLUMN(g_list_model_get_item(columns, column_id));

    if (column) {
        if (sortable) {
            /* Cria um sorter customizado para esta coluna */
            GtkSorter *sorter = eg_column_sorter_new(column_id);
            gtk_column_view_column_set_sorter(column, sorter);
            g_object_unref(sorter);
        } else {
            gtk_column_view_column_set_sorter(column, NULL);
        }
        g_object_unref(column);
    }
}
