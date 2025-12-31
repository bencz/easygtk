/**
 * EasyGTK - Paned (Split View)
 *
 * Container que divide espaço entre 2 filhos.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/paned.h>

/* ============================================
 * Paned Container VTable
 * ============================================ */

static void paned_destroy(EgWidget *widget);
static void paned_add_child(EgWidget *container, EgWidget *child);
static void paned_remove_child(EgWidget *container, EgWidget *child);
static size_t paned_get_child_count(EgWidget *container);
static EgWidget *paned_get_child_at(EgWidget *container, size_t index);
static void paned_clear(EgWidget *container);

static const EgContainerCapabilities paned_caps = {
    .supports_multiple = true,
    .supports_named = false,
    .supports_positioned = false,
    .max_children = 2
};

const EgContainerVTable eg_paned_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_PANED,
        .type_name = "EgPaned",
        .destroy = paned_destroy,
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
    .caps = &paned_caps,
    .add_child = paned_add_child,
    .remove_child = paned_remove_child,
    .add_named = NULL,
    .get_by_name = NULL,
    .get_child_count = paned_get_child_count,
    .get_child_at = paned_get_child_at,
    .clear = paned_clear
};

/* ============================================
 * VTable Implementations
 * ============================================ */

static void paned_destroy(EgWidget *widget) {
    EgPaned *paned = (EgPaned *)widget;
    if (paned == NULL) return;

    /* Destrói os filhos */
    if (paned->child1 != NULL && paned->child1->vtable != NULL &&
        paned->child1->vtable->destroy != NULL) {
        paned->child1->vtable->destroy(paned->child1);
    }
    if (paned->child2 != NULL && paned->child2->vtable != NULL &&
        paned->child2->vtable->destroy != NULL) {
        paned->child2->vtable->destroy(paned->child2);
    }

    eg_free(paned);
}

static void paned_add_child(EgWidget *container, EgWidget *child) {
    EgPaned *paned = (EgPaned *)container;
    if (paned == NULL || paned->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    /* Adiciona no primeiro slot livre */
    if (paned->child1 == NULL) {
        gtk_paned_set_start_child(GTK_PANED(paned->base.native), child->native);
        paned->child1 = child;
    } else if (paned->child2 == NULL) {
        gtk_paned_set_end_child(GTK_PANED(paned->base.native), child->native);
        paned->child2 = child;
    }
}

static void paned_remove_child(EgWidget *container, EgWidget *child) {
    EgPaned *paned = (EgPaned *)container;
    if (paned == NULL || paned->base.native == NULL) return;

    if (paned->child1 == child) {
        gtk_paned_set_start_child(GTK_PANED(paned->base.native), NULL);
        paned->child1 = NULL;
    } else if (paned->child2 == child) {
        gtk_paned_set_end_child(GTK_PANED(paned->base.native), NULL);
        paned->child2 = NULL;
    }
}

static size_t paned_get_child_count(EgWidget *container) {
    EgPaned *paned = (EgPaned *)container;
    if (paned == NULL) return 0;
    size_t count = 0;
    if (paned->child1 != NULL) count++;
    if (paned->child2 != NULL) count++;
    return count;
}

static EgWidget *paned_get_child_at(EgWidget *container, size_t index) {
    EgPaned *paned = (EgPaned *)container;
    if (paned == NULL) return NULL;
    if (index == 0) return paned->child1;
    if (index == 1) return paned->child2;
    return NULL;
}

static void paned_clear(EgWidget *container) {
    EgPaned *paned = (EgPaned *)container;
    if (paned == NULL || paned->base.native == NULL) return;

    gtk_paned_set_start_child(GTK_PANED(paned->base.native), NULL);
    gtk_paned_set_end_child(GTK_PANED(paned->base.native), NULL);
    paned->child1 = NULL;
    paned->child2 = NULL;
}

/* ============================================
 * API Pública
 * ============================================ */

EgPaned *eg_paned_new(EgOrientation orientation) {
    EgPaned *paned = EG_ALLOC(EgPaned);
    if (paned == NULL) return NULL;

    GtkOrientation gtk_orient = (orientation == EG_ORIENTATION_VERTICAL)
        ? GTK_ORIENTATION_VERTICAL
        : GTK_ORIENTATION_HORIZONTAL;

    GtkWidget *gtk_paned = gtk_paned_new(gtk_orient);
    if (gtk_paned == NULL) {
        eg_free(paned);
        return NULL;
    }

    eg_widget_init(&paned->base, EG_WIDGET_TYPE_PANED, gtk_paned, &eg_paned_vtable.base);
    paned->child1 = NULL;
    paned->child2 = NULL;

    return paned;
}

EgPaned *eg_paned_new_horizontal(void) {
    return eg_paned_new(EG_ORIENTATION_HORIZONTAL);
}

EgPaned *eg_paned_new_vertical(void) {
    return eg_paned_new(EG_ORIENTATION_VERTICAL);
}

void eg_paned_free(EgPaned *paned) {
    paned_destroy((EgWidget *)paned);
}

void eg_paned_set_start_child(EgPaned *paned, EgWidget *child) {
    if (paned == NULL || paned->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_paned_set_start_child(GTK_PANED(paned->base.native), gtk_child);
    paned->child1 = child;
}

void eg_paned_set_end_child(EgPaned *paned, EgWidget *child) {
    if (paned == NULL || paned->base.native == NULL) return;
    GtkWidget *gtk_child = (child != NULL) ? child->native : NULL;
    gtk_paned_set_end_child(GTK_PANED(paned->base.native), gtk_child);
    paned->child2 = child;
}

void eg_paned_set_position(EgPaned *paned, int position) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_position(GTK_PANED(paned->base.native), position);
}

int eg_paned_get_position(EgPaned *paned) {
    if (paned == NULL || paned->base.native == NULL) return 0;
    return gtk_paned_get_position(GTK_PANED(paned->base.native));
}

void eg_paned_set_start_child_resize(EgPaned *paned, bool resize) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_resize_start_child(GTK_PANED(paned->base.native), resize);
}

void eg_paned_set_end_child_resize(EgPaned *paned, bool resize) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_resize_end_child(GTK_PANED(paned->base.native), resize);
}

void eg_paned_set_start_child_shrink(EgPaned *paned, bool shrink) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_shrink_start_child(GTK_PANED(paned->base.native), shrink);
}

void eg_paned_set_end_child_shrink(EgPaned *paned, bool shrink) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_shrink_end_child(GTK_PANED(paned->base.native), shrink);
}

void eg_paned_set_wide_handle(EgPaned *paned, bool wide) {
    if (paned == NULL || paned->base.native == NULL) return;
    gtk_paned_set_wide_handle(GTK_PANED(paned->base.native), wide);
}

EgWidget *eg_paned_as_widget(EgPaned *paned) {
    return (EgWidget *)paned;
}

void *eg_paned_get_native(EgPaned *paned) {
    if (paned == NULL) return NULL;
    return paned->base.native;
}
