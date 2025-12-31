/**
 * EasyGTK - Stack
 *
 * Container que mostra um filho por vez, com transições.
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/stack.h>

/* ============================================
 * Stack Container VTable
 * ============================================ */

static void stack_destroy(EgWidget *widget);
static void stack_add_child(EgWidget *container, EgWidget *child);
static void stack_remove_child(EgWidget *container, EgWidget *child);
static void stack_add_named(EgWidget *container, EgWidget *child, const char *name);
static size_t stack_get_child_count(EgWidget *container);
static EgWidget *stack_get_child_at(EgWidget *container, size_t index);
static void stack_clear(EgWidget *container);

static const EgContainerCapabilities stack_caps = {
    .supports_multiple = true,
    .supports_named = true,
    .supports_positioned = false,
    .max_children = 0  /* Ilimitado */
};

const EgContainerVTable eg_stack_vtable = {
    .base = {
        .type = EG_WIDGET_TYPE_STACK,
        .type_name = "EgStack",
        .destroy = stack_destroy,
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
    .caps = &stack_caps,
    .add_child = stack_add_child,
    .remove_child = stack_remove_child,
    .add_named = stack_add_named,
    .get_by_name = NULL,  /* TODO: implementar se necessário */
    .get_child_count = stack_get_child_count,
    .get_child_at = stack_get_child_at,
    .clear = stack_clear
};

/* ============================================
 * StackSwitcher VTable
 * ============================================ */

const EgWidgetVTable eg_stack_switcher_vtable = {
    .type = EG_WIDGET_TYPE_STACK_SWITCHER,
    .type_name = "EgStackSwitcher",
    .destroy = eg_widget_destroy_simple,
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
};

/* ============================================
 * VTable Implementations
 * ============================================ */

static void stack_destroy(EgWidget *widget) {
    EgStack *stack = (EgStack *)widget;
    if (stack == NULL) return;

    /* Destrói todos os filhos */
    for (size_t i = 0; i < stack->children.count; i++) {
        EgWidget *child = stack->children.children[i];
        if (child != NULL && child->vtable != NULL && child->vtable->destroy != NULL) {
            child->vtable->destroy(child);
        }
    }

    eg_child_list_free(&stack->children);
    eg_free(stack);
}

static void stack_add_child(EgWidget *container, EgWidget *child) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    /* Adiciona sem nome (usa nome gerado) */
    gtk_stack_add_child(GTK_STACK(stack->base.native), child->native);
    eg_child_list_add(&stack->children, child);
}

static void stack_remove_child(EgWidget *container, EgWidget *child) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_stack_remove(GTK_STACK(stack->base.native), child->native);
    eg_child_list_remove(&stack->children, child);
}

static void stack_add_named(EgWidget *container, EgWidget *child, const char *name) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_stack_add_named(GTK_STACK(stack->base.native), child->native, name);
    eg_child_list_add(&stack->children, child);
}

static size_t stack_get_child_count(EgWidget *container) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL) return 0;
    return stack->children.count;
}

static EgWidget *stack_get_child_at(EgWidget *container, size_t index) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL) return NULL;
    return eg_child_list_get(&stack->children, index);
}

static void stack_clear(EgWidget *container) {
    EgStack *stack = (EgStack *)container;
    if (stack == NULL || stack->base.native == NULL) return;

    /* Remove todos os filhos do GTK */
    GtkWidget *native = stack->base.native;
    GtkWidget *child;
    while ((child = gtk_stack_get_child_by_name(GTK_STACK(native),
            gtk_stack_get_visible_child_name(GTK_STACK(native)))) != NULL) {
        gtk_stack_remove(GTK_STACK(native), child);
    }

    /* Limpa a lista */
    eg_child_list_clear(&stack->children);
}

/* ============================================
 * Conversão de Transição
 * ============================================ */

static GtkStackTransitionType eg_to_gtk_transition(EgStackTransition transition) {
    switch (transition) {
        case EG_STACK_TRANSITION_NONE: return GTK_STACK_TRANSITION_TYPE_NONE;
        case EG_STACK_TRANSITION_CROSSFADE: return GTK_STACK_TRANSITION_TYPE_CROSSFADE;
        case EG_STACK_TRANSITION_SLIDE_RIGHT: return GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT;
        case EG_STACK_TRANSITION_SLIDE_LEFT: return GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT;
        case EG_STACK_TRANSITION_SLIDE_UP: return GTK_STACK_TRANSITION_TYPE_SLIDE_UP;
        case EG_STACK_TRANSITION_SLIDE_DOWN: return GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN;
        case EG_STACK_TRANSITION_SLIDE_LEFT_RIGHT: return GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT;
        case EG_STACK_TRANSITION_SLIDE_UP_DOWN: return GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN;
        case EG_STACK_TRANSITION_OVER_UP: return GTK_STACK_TRANSITION_TYPE_OVER_UP;
        case EG_STACK_TRANSITION_OVER_DOWN: return GTK_STACK_TRANSITION_TYPE_OVER_DOWN;
        case EG_STACK_TRANSITION_OVER_LEFT: return GTK_STACK_TRANSITION_TYPE_OVER_LEFT;
        case EG_STACK_TRANSITION_OVER_RIGHT: return GTK_STACK_TRANSITION_TYPE_OVER_RIGHT;
        case EG_STACK_TRANSITION_UNDER_UP: return GTK_STACK_TRANSITION_TYPE_UNDER_UP;
        case EG_STACK_TRANSITION_UNDER_DOWN: return GTK_STACK_TRANSITION_TYPE_UNDER_DOWN;
        case EG_STACK_TRANSITION_UNDER_LEFT: return GTK_STACK_TRANSITION_TYPE_UNDER_LEFT;
        case EG_STACK_TRANSITION_UNDER_RIGHT: return GTK_STACK_TRANSITION_TYPE_UNDER_RIGHT;
        default: return GTK_STACK_TRANSITION_TYPE_NONE;
    }
}

/* ============================================
 * API Pública - Stack
 * ============================================ */

EgStack *eg_stack_new(void) {
    EgStack *stack = EG_ALLOC(EgStack);
    if (stack == NULL) return NULL;

    GtkWidget *gtk_stack = gtk_stack_new();
    if (gtk_stack == NULL) {
        eg_free(stack);
        return NULL;
    }

    eg_widget_init(&stack->base, EG_WIDGET_TYPE_STACK, gtk_stack, &eg_stack_vtable.base);
    eg_child_list_init(&stack->children);

    return stack;
}

void eg_stack_free(EgStack *stack) {
    stack_destroy((EgWidget *)stack);
}

void eg_stack_add_named(EgStack *stack, EgWidget *child, const char *name) {
    stack_add_named((EgWidget *)stack, child, name);
}

void eg_stack_add_titled(EgStack *stack, EgWidget *child, const char *name, const char *title) {
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;

    gtk_stack_add_titled(GTK_STACK(stack->base.native), child->native, name, title);
    eg_child_list_add(&stack->children, child);
}

void eg_stack_remove(EgStack *stack, EgWidget *child) {
    stack_remove_child((EgWidget *)stack, child);
}

void eg_stack_set_visible_child_name(EgStack *stack, const char *name) {
    if (stack == NULL || stack->base.native == NULL || name == NULL) return;
    gtk_stack_set_visible_child_name(GTK_STACK(stack->base.native), name);
}

const char *eg_stack_get_visible_child_name(EgStack *stack) {
    if (stack == NULL || stack->base.native == NULL) return NULL;
    return gtk_stack_get_visible_child_name(GTK_STACK(stack->base.native));
}

void eg_stack_set_visible_child(EgStack *stack, EgWidget *child) {
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_stack_set_visible_child(GTK_STACK(stack->base.native), child->native);
}

EgWidget *eg_stack_get_visible_child(EgStack *stack) {
    if (stack == NULL || stack->base.native == NULL) return NULL;

    GtkWidget *visible = gtk_stack_get_visible_child(GTK_STACK(stack->base.native));
    if (visible == NULL) return NULL;

    /* Procura o EgWidget correspondente */
    for (size_t i = 0; i < stack->children.count; i++) {
        EgWidget *child = stack->children.children[i];
        if (child != NULL && child->native == visible) {
            return child;
        }
    }
    return NULL;
}

void eg_stack_set_transition_type(EgStack *stack, EgStackTransition transition) {
    if (stack == NULL || stack->base.native == NULL) return;
    gtk_stack_set_transition_type(GTK_STACK(stack->base.native), eg_to_gtk_transition(transition));
}

void eg_stack_set_transition_duration(EgStack *stack, unsigned int duration) {
    if (stack == NULL || stack->base.native == NULL) return;
    gtk_stack_set_transition_duration(GTK_STACK(stack->base.native), duration);
}

void eg_stack_set_hhomogeneous(EgStack *stack, bool homogeneous) {
    if (stack == NULL || stack->base.native == NULL) return;
    gtk_stack_set_hhomogeneous(GTK_STACK(stack->base.native), homogeneous);
}

void eg_stack_set_vhomogeneous(EgStack *stack, bool homogeneous) {
    if (stack == NULL || stack->base.native == NULL) return;
    gtk_stack_set_vhomogeneous(GTK_STACK(stack->base.native), homogeneous);
}

EgWidget *eg_stack_as_widget(EgStack *stack) {
    return (EgWidget *)stack;
}

void *eg_stack_get_native(EgStack *stack) {
    if (stack == NULL) return NULL;
    return stack->base.native;
}

/* ============================================
 * API Pública - StackSwitcher
 * ============================================ */

EgStackSwitcher *eg_stack_switcher_new(void) {
    EgStackSwitcher *switcher = EG_ALLOC(EgStackSwitcher);
    if (switcher == NULL) return NULL;

    GtkWidget *gtk_switcher = gtk_stack_switcher_new();
    if (gtk_switcher == NULL) {
        eg_free(switcher);
        return NULL;
    }

    eg_widget_init(&switcher->base, EG_WIDGET_TYPE_STACK_SWITCHER, gtk_switcher, &eg_stack_switcher_vtable);
    return switcher;
}

void eg_stack_switcher_free(EgStackSwitcher *switcher) {
    eg_widget_destroy_simple((EgWidget *)switcher);
}

void eg_stack_switcher_set_stack(EgStackSwitcher *switcher, EgStack *stack) {
    if (switcher == NULL || switcher->base.native == NULL) return;
    GtkStack *gtk_stack = (stack != NULL) ? GTK_STACK(stack->base.native) : NULL;
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher->base.native), gtk_stack);
}

EgWidget *eg_stack_switcher_as_widget(EgStackSwitcher *switcher) {
    return (EgWidget *)switcher;
}

void *eg_stack_switcher_get_native(EgStackSwitcher *switcher) {
    if (switcher == NULL) return NULL;
    return switcher->base.native;
}
