/**
 * EasyGTK - Container API Genérica
 *
 * Funções que operam em qualquer container via vtable.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* ============================================
 * Helpers internos
 * ============================================ */

static const EgContainerVTable *get_container_vtable(EgWidget *widget) {
    if (widget == NULL || widget->vtable == NULL) return NULL;

    /* Verifica se é um container baseado no tipo */
    switch (widget->type) {
        case EG_WIDGET_TYPE_BOX:
        case EG_WIDGET_TYPE_GRID:
        case EG_WIDGET_TYPE_STACK:
        case EG_WIDGET_TYPE_NOTEBOOK:
        case EG_WIDGET_TYPE_FRAME:
        case EG_WIDGET_TYPE_PANED:
        case EG_WIDGET_TYPE_SCROLLED_WINDOW:
            return (const EgContainerVTable *)widget->vtable;
        default:
            return NULL;
    }
}

/* ============================================
 * API Pública
 * ============================================ */

bool eg_widget_is_container(EgWidget *widget) {
    return get_container_vtable(widget) != NULL;
}

void eg_container_add(EgWidget *container, EgWidget *child) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->add_child == NULL) return;
    vtable->add_child(container, child);
}

void eg_container_remove(EgWidget *container, EgWidget *child) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->remove_child == NULL) return;
    vtable->remove_child(container, child);
}

void eg_container_add_named(EgWidget *container, EgWidget *child, const char *name) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL) return;

    if (vtable->add_named != NULL) {
        vtable->add_named(container, child, name);
    } else if (vtable->add_child != NULL) {
        /* Fallback para add simples se não suporta nomes */
        vtable->add_child(container, child);
    }
}

size_t eg_container_get_child_count(EgWidget *container) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->get_child_count == NULL) return 0;
    return vtable->get_child_count(container);
}

EgWidget *eg_container_get_child_at(EgWidget *container, size_t index) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->get_child_at == NULL) return NULL;
    return vtable->get_child_at(container, index);
}

void eg_container_clear(EgWidget *container) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->clear == NULL) return;
    vtable->clear(container);
}

bool eg_container_supports_multiple(EgWidget *container) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->caps == NULL) return false;
    return vtable->caps->supports_multiple;
}

bool eg_container_supports_named(EgWidget *container) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->caps == NULL) return false;
    return vtable->caps->supports_named;
}

size_t eg_container_max_children(EgWidget *container) {
    const EgContainerVTable *vtable = get_container_vtable(container);
    if (vtable == NULL || vtable->caps == NULL) return 0;
    return vtable->caps->max_children;
}
