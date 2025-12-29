/**
 * EasyGTK - Grid
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "internal/internal.h"

/* Funções da vtable */
static void grid_destroy(EgWidget *widget);
static void *grid_get_native(EgWidget *widget);
static void grid_set_visible(EgWidget *widget, bool visible);
static bool grid_get_visible(EgWidget *widget);
static void grid_set_sensitive(EgWidget *widget, bool sensitive);
static bool grid_get_sensitive(EgWidget *widget);
static void grid_add_child(EgWidget *container, EgWidget *child);
static void grid_remove_child(EgWidget *container, EgWidget *child);

const EgContainerVTable eg_grid_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_GRID,
        .type_name = "EgGrid",
        .destroy = grid_destroy,
        .get_native = grid_get_native,
        .set_visible = grid_set_visible,
        .get_visible = grid_get_visible,
        .set_sensitive = grid_set_sensitive,
        .get_sensitive = grid_get_sensitive
    },
    .add_child = grid_add_child,
    .remove_child = grid_remove_child
};

/* Funções auxiliares para lista de filhos */
static void child_list_init(EgChildList *list) {
    list->children = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void child_list_add(EgChildList *list, EgWidget *child) {
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity == 0 ? 4 : list->capacity * 2;
        EgWidget **new_arr = realloc(list->children, new_cap * sizeof(EgWidget *));
        if (new_arr == NULL) return;
        list->children = new_arr;
        list->capacity = new_cap;
    }
    list->children[list->count++] = child;
}

static void child_list_remove(EgChildList *list, EgWidget *child) {
    for (size_t i = 0; i < list->count; i++) {
        if (list->children[i] == child) {
            memmove(&list->children[i], &list->children[i + 1], 
                    (list->count - i - 1) * sizeof(EgWidget *));
            list->count--;
            return;
        }
    }
}

static void child_list_free(EgChildList *list) {
    free(list->children);
    list->children = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void grid_destroy(EgWidget *widget) {
    EgGrid *grid = (EgGrid *)widget;
    if (grid == NULL) return;
    child_list_free(&grid->children);
    eg_free(grid);
}

static void *grid_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void grid_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool grid_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void grid_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool grid_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

static void grid_add_child(EgWidget *container, EgWidget *child) {
    /* Grid precisa de posição, então adiciona em 0,0 por padrão */
    if (container == NULL || container->native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_grid_attach(GTK_GRID(container->native), child->native, 0, 0, 1, 1);
}

static void grid_remove_child(EgWidget *container, EgWidget *child) {
    if (container == NULL || container->native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_grid_remove(GTK_GRID(container->native), child->native);
}

EgGrid *eg_grid_new(void) {
    EgGrid *grid = EG_ALLOC(EgGrid);
    if (grid == NULL) return NULL;
    
    GtkWidget *gtk_grid = gtk_grid_new();
    if (gtk_grid == NULL) {
        eg_free(grid);
        return NULL;
    }
    
    eg_widget_init(&grid->base, EG_WIDGET_TYPE_GRID, gtk_grid, &eg_grid_vtable.base);
    child_list_init(&grid->children);
    
    return grid;
}

void eg_grid_free(EgGrid *grid) {
    grid_destroy((EgWidget *)grid);
}

void eg_grid_attach(EgGrid *grid, EgWidget *child, int column, int row, int width, int height) {
    if (grid == NULL || grid->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_grid_attach(GTK_GRID(grid->base.native), child->native, column, row, width, height);
    child_list_add(&grid->children, child);
}

void eg_grid_remove(EgGrid *grid, EgWidget *child) {
    if (grid == NULL || grid->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_grid_remove(GTK_GRID(grid->base.native), child->native);
    child_list_remove(&grid->children, child);
}

void eg_grid_set_column_spacing(EgGrid *grid, int spacing) {
    if (grid == NULL || grid->base.native == NULL) return;
    gtk_grid_set_column_spacing(GTK_GRID(grid->base.native), spacing);
}

void eg_grid_set_row_spacing(EgGrid *grid, int spacing) {
    if (grid == NULL || grid->base.native == NULL) return;
    gtk_grid_set_row_spacing(GTK_GRID(grid->base.native), spacing);
}

void eg_grid_set_column_homogeneous(EgGrid *grid, bool homogeneous) {
    if (grid == NULL || grid->base.native == NULL) return;
    gtk_grid_set_column_homogeneous(GTK_GRID(grid->base.native), homogeneous);
}

void eg_grid_set_row_homogeneous(EgGrid *grid, bool homogeneous) {
    if (grid == NULL || grid->base.native == NULL) return;
    gtk_grid_set_row_homogeneous(GTK_GRID(grid->base.native), homogeneous);
}

EgWidget *eg_grid_as_widget(EgGrid *grid) {
    return (EgWidget *)grid;
}

void *eg_grid_get_native(EgGrid *grid) {
    if (grid == NULL) return NULL;
    return grid->base.native;
}
