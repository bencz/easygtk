/**
 * EasyGTK - Widget Helpers
 *
 * Funções auxiliares compartilhadas por todos os widgets.
 * Reduz duplicação de código nas implementações de vtable.
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "internal.h"

/* ============================================
 * Widget VTable Helpers
 * ============================================ */

void *eg_widget_get_native_default(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

void eg_widget_set_visible_default(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

bool eg_widget_get_visible_default(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

void eg_widget_set_sensitive_default(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

bool eg_widget_get_sensitive_default(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

void eg_widget_destroy_simple(EgWidget *widget) {
    if (widget == NULL) return;
    eg_free(widget);
}

/* ============================================
 * Child List Management
 * ============================================ */

void eg_child_list_init(void *list_ptr) {
    EgChildList *list = (EgChildList *)list_ptr;
    list->children = NULL;
    list->count = 0;
    list->capacity = 0;
}

void eg_child_list_add(void *list_ptr, EgWidget *child) {
    EgChildList *list = (EgChildList *)list_ptr;
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity == 0 ? 4 : list->capacity * 2;
        EgWidget **new_arr = realloc(list->children, new_cap * sizeof(EgWidget *));
        if (new_arr == NULL) return;
        list->children = new_arr;
        list->capacity = new_cap;
    }
    list->children[list->count++] = child;
}

void eg_child_list_remove(void *list_ptr, EgWidget *child) {
    EgChildList *list = (EgChildList *)list_ptr;
    for (size_t i = 0; i < list->count; i++) {
        if (list->children[i] == child) {
            memmove(&list->children[i], &list->children[i + 1],
                    (list->count - i - 1) * sizeof(EgWidget *));
            list->count--;
            return;
        }
    }
}

void eg_child_list_free(void *list_ptr) {
    EgChildList *list = (EgChildList *)list_ptr;
    free(list->children);
    list->children = NULL;
    list->count = 0;
    list->capacity = 0;
}

size_t eg_child_list_count(void *list_ptr) {
    EgChildList *list = (EgChildList *)list_ptr;
    return list->count;
}

EgWidget *eg_child_list_get(void *list_ptr, size_t index) {
    EgChildList *list = (EgChildList *)list_ptr;
    if (index >= list->count) return NULL;
    return list->children[index];
}

void eg_child_list_clear(void *list_ptr) {
    EgChildList *list = (EgChildList *)list_ptr;
    list->count = 0;
}

/* ============================================
 * Container Helpers
 * ============================================ */

void eg_container_destroy_with_children(EgWidget *widget) {
    if (widget == NULL) return;

    /* Assume que o container tem EgChildList logo após EgWidget base */
    /* Isso funciona para Box, Grid, e containers similares */
    EgChildList *children = (EgChildList *)((char *)widget + sizeof(EgWidget));

    /* Destrói todos os filhos */
    for (size_t i = 0; i < children->count; i++) {
        EgWidget *child = children->children[i];
        if (child != NULL && child->vtable != NULL && child->vtable->destroy != NULL) {
            child->vtable->destroy(child);
        }
    }

    eg_child_list_free(children);
    eg_free(widget);
}
