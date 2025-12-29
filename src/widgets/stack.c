/**
 * EasyGTK - Stack
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/stack.h>

/* Funções da vtable - Stack */
static void stack_destroy(EgWidget *widget);
static void *stack_get_native(EgWidget *widget);
static void stack_set_visible(EgWidget *widget, bool visible);
static bool stack_get_visible(EgWidget *widget);
static void stack_set_sensitive(EgWidget *widget, bool sensitive);
static bool stack_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_stack_vtable = {
    .type = EG_WIDGET_TYPE_STACK,
    .type_name = "EgStack",
    .destroy = stack_destroy,
    .get_native = stack_get_native,
    .set_visible = stack_set_visible,
    .get_visible = stack_get_visible,
    .set_sensitive = stack_set_sensitive,
    .get_sensitive = stack_get_sensitive
};

/* Funções da vtable - StackSwitcher */
static void stack_switcher_destroy(EgWidget *widget);
static void *stack_switcher_get_native(EgWidget *widget);

const EgWidgetVTable eg_stack_switcher_vtable = {
    .type = EG_WIDGET_TYPE_STACK_SWITCHER,
    .type_name = "EgStackSwitcher",
    .destroy = stack_switcher_destroy,
    .get_native = stack_switcher_get_native,
    .set_visible = stack_set_visible,
    .get_visible = stack_get_visible,
    .set_sensitive = stack_set_sensitive,
    .get_sensitive = stack_get_sensitive
};

static void stack_destroy(EgWidget *widget) {
    EgStack *stack = (EgStack *)widget;
    if (stack == NULL) return;
    eg_free(stack);
}

static void *stack_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void stack_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool stack_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void stack_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool stack_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

static void stack_switcher_destroy(EgWidget *widget) {
    EgStackSwitcher *switcher = (EgStackSwitcher *)widget;
    if (switcher == NULL) return;
    eg_free(switcher);
}

static void *stack_switcher_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

/* Converte EgStackTransition para GtkStackTransitionType */
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

EgStack *eg_stack_new(void) {
    EgStack *stack = EG_ALLOC(EgStack);
    if (stack == NULL) return NULL;
    
    GtkWidget *gtk_stack = gtk_stack_new();
    if (gtk_stack == NULL) {
        eg_free(stack);
        return NULL;
    }
    
    eg_widget_init(&stack->base, EG_WIDGET_TYPE_STACK, gtk_stack, &eg_stack_vtable);
    return stack;
}

void eg_stack_free(EgStack *stack) {
    stack_destroy((EgWidget *)stack);
}

void eg_stack_add_named(EgStack *stack, EgWidget *child, const char *name) {
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_stack_add_named(GTK_STACK(stack->base.native), child->native, name);
}

void eg_stack_add_titled(EgStack *stack, EgWidget *child, const char *name, const char *title) {
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_stack_add_titled(GTK_STACK(stack->base.native), child->native, name, title);
}

void eg_stack_remove(EgStack *stack, EgWidget *child) {
    if (stack == NULL || stack->base.native == NULL) return;
    if (child == NULL || child->native == NULL) return;
    gtk_stack_remove(GTK_STACK(stack->base.native), child->native);
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
    (void)stack;
    /* Nota: Retornar o EgWidget requer mapeamento reverso que não temos */
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
 * StackSwitcher
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
    stack_switcher_destroy((EgWidget *)switcher);
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
