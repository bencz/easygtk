/**
 * EasyGTK - VTables
 *
 * Definições de vtables para componentização.
 * Permite extensibilidade e polimorfismo em C.
 */

#ifndef EASYGTK_VTABLE_H
#define EASYGTK_VTABLE_H

#include <easygtk/types.h>
#include <easygtk/property.h>
#include <easygtk/binding.h>
#include <easygtk/events.h>

/* Forward declarations */
struct EgWidget;
struct EgCommand;
struct EgValidatorChain;
struct EgValidationResult;

/* ============================================
 * Binding VTable
 * ============================================ */

/**
 * Descreve as capacidades de binding de um widget.
 */
typedef struct EgBindingCapabilities {
    EgPropertyType primary_type;        /* Tipo primário (STRING, INT, BOOL, DOUBLE) */
    EgBindingMode default_mode;         /* Modo padrão (TWO_WAY ou ONE_WAY) */
    bool supports_command;              /* Se suporta binding de command */
} EgBindingCapabilities;

/**
 * Contexto passado para funções de binding na vtable.
 */
typedef struct EgBindingContext {
    EgProperty *property;               /* Property a ser bindada */
    EgBindingMode mode;                 /* Modo de binding solicitado */
} EgBindingContext;

/* ============================================
 * Event VTable
 * ============================================ */

/**
 * Capacidades de eventos que um widget suporta.
 */
typedef struct EgEventCapabilities {
    bool supports_key;                  /* Eventos de teclado */
    bool supports_mouse;                /* Eventos de mouse (motion, click) */
    bool supports_focus;                /* Eventos de foco */
    bool supports_scroll;               /* Eventos de scroll */
} EgEventCapabilities;

/**
 * VTable para tratamento de eventos.
 * Widgets podem customizar como eventos são conectados.
 */
typedef struct EgEventVTable {
    const EgEventCapabilities *caps;

    /* Conecta evento de teclado (press=true para keypress, false para release) */
    void (*connect_key)(struct EgWidget *widget, EgKeyCallback cb, void *ud, bool press);

    /* Conecta evento de movimento do mouse */
    void (*connect_motion)(struct EgWidget *widget, EgMotionCallback cb, void *ud);

    /* Conecta evento de clique */
    void (*connect_click)(struct EgWidget *widget, EgButtonCallback cb, void *ud);

    /* Conecta evento de foco (enter=true para focus_in, false para focus_out) */
    void (*connect_focus)(struct EgWidget *widget, EgFocusCallback cb, void *ud, bool enter);

    /* Conecta evento de scroll */
    void (*connect_scroll)(struct EgWidget *widget, EgScrollCallback cb, void *ud);

    /* Conecta evento de enter/leave (enter=true para enter, false para leave) */
    void (*connect_crossing)(struct EgWidget *widget, EgCrossingCallback cb, void *ud, bool enter);
} EgEventVTable;

/* ============================================
 * Validation VTable
 * ============================================ */

/**
 * Valor genérico para validação.
 */
typedef struct EgValue {
    EgPropertyType type;
    union {
        const char *string_val;
        int int_val;
        double double_val;
        bool bool_val;
    };
} EgValue;

/**
 * Capacidades de validação de um widget.
 */
typedef struct EgValidationCapabilities {
    EgPropertyType value_type;          /* Tipo de valor que o widget produz */
    bool supports_inline_error;         /* Mostra erro inline (tooltip/label) */
    bool supports_error_style;          /* Suporta CSS class de erro */
} EgValidationCapabilities;

/**
 * VTable para validação de widgets.
 */
typedef struct EgValidationVTable {
    const EgValidationCapabilities *caps;

    /* Obtém o valor atual do widget para validação */
    EgValue (*get_value)(struct EgWidget *widget);

    /* Mostra estado de erro no widget */
    void (*show_error)(struct EgWidget *widget, const char *message);

    /* Remove estado de erro do widget */
    void (*clear_error)(struct EgWidget *widget);
} EgValidationVTable;

/* ============================================
 * Container VTable Capabilities
 * ============================================ */

/**
 * Capacidades do container.
 */
typedef struct EgContainerCapabilities {
    bool supports_multiple;             /* true = múltiplos filhos (Box, Grid) */
    bool supports_named;                /* true = filhos nomeados (Stack, Notebook) */
    bool supports_positioned;           /* true = posição (Grid: row/col) */
    size_t max_children;                /* 0 = ilimitado, 1 = single (Frame), 2 = dual (Paned) */
} EgContainerCapabilities;

/* ============================================
 * Widget VTable Base
 * ============================================ */

/**
 * VTable base para todos os widgets.
 */
typedef struct EgWidgetVTable {
    /* Identificador do tipo */
    EgWidgetType type;

    /* Nome do tipo para debug */
    const char *type_name;

    /* Destrutor */
    void (*destroy)(struct EgWidget *widget);

    /* Obtém o widget GTK nativo */
    void *(*get_native)(struct EgWidget *widget);

    /* Define visibilidade */
    void (*set_visible)(struct EgWidget *widget, bool visible);

    /* Obtém visibilidade */
    bool (*get_visible)(struct EgWidget *widget);

    /* Define sensibilidade */
    void (*set_sensitive)(struct EgWidget *widget, bool sensitive);

    /* Obtém sensibilidade */
    bool (*get_sensitive)(struct EgWidget *widget);

    /* ============================================
     * Binding Support (NULL = não suporta)
     * ============================================ */

    const EgBindingCapabilities *binding_caps;
    void *(*bind_value)(struct EgWidget *widget, const EgBindingContext *ctx);
    void *(*bind_command)(struct EgWidget *widget, struct EgCommand *command);
    void (*unbind)(struct EgWidget *widget, void *binding_data);

    /* ============================================
     * Event Support (NULL = usa implementação default)
     * ============================================ */

    const EgEventVTable *events;

    /* ============================================
     * Validation Support (NULL = não suporta)
     * ============================================ */

    const EgValidationVTable *validation;

} EgWidgetVTable;

/* ============================================
 * Container VTable
 * ============================================ */

/**
 * VTable para containers.
 */
typedef struct EgContainerVTable {
    EgWidgetVTable base;

    /* Capacidades do container */
    const EgContainerCapabilities *caps;

    /* Operações básicas */
    void (*add_child)(struct EgWidget *container, struct EgWidget *child);
    void (*remove_child)(struct EgWidget *container, struct EgWidget *child);

    /* Operações com nome (Stack, Notebook) */
    void (*add_named)(struct EgWidget *container, struct EgWidget *child, const char *name);
    struct EgWidget *(*get_by_name)(struct EgWidget *container, const char *name);

    /* Iteração */
    size_t (*get_child_count)(struct EgWidget *container);
    struct EgWidget *(*get_child_at)(struct EgWidget *container, size_t index);

    /* Limpar */
    void (*clear)(struct EgWidget *container);
} EgContainerVTable;

/* ============================================
 * Declarações das VTables Globais
 * ============================================ */

/* Widgets simples */
extern const EgWidgetVTable eg_widget_vtable;
extern const EgWidgetVTable eg_window_vtable;
extern const EgWidgetVTable eg_button_vtable;
extern const EgWidgetVTable eg_label_vtable;
extern const EgWidgetVTable eg_entry_vtable;
extern const EgWidgetVTable eg_check_button_vtable;
extern const EgWidgetVTable eg_switch_vtable;
extern const EgWidgetVTable eg_spin_button_vtable;
extern const EgWidgetVTable eg_scale_vtable;

/* Containers */
extern const EgContainerVTable eg_box_vtable;
extern const EgContainerVTable eg_grid_vtable;
extern const EgContainerVTable eg_stack_vtable;
extern const EgContainerVTable eg_notebook_vtable;
extern const EgContainerVTable eg_frame_vtable;
extern const EgContainerVTable eg_paned_vtable;
extern const EgContainerVTable eg_scrolled_window_vtable;

/* Event VTable default (usada quando widget->vtable->events == NULL) */
extern const EgEventVTable eg_event_vtable_default;

/* ============================================
 * Widget Helpers (reduzem duplicação)
 * ============================================ */

/* Implementações default para vtable base */
void *eg_widget_get_native_default(struct EgWidget *widget);
void eg_widget_set_visible_default(struct EgWidget *widget, bool visible);
bool eg_widget_get_visible_default(struct EgWidget *widget);
void eg_widget_set_sensitive_default(struct EgWidget *widget, bool sensitive);
bool eg_widget_get_sensitive_default(struct EgWidget *widget);

/* Destrutor default que apenas libera memória */
void eg_widget_destroy_simple(struct EgWidget *widget);

/* ============================================
 * Event Helpers
 * ============================================ */

/* Conectores de eventos default (usam GtkEventController) */
void eg_event_connect_key_default(struct EgWidget *widget, EgKeyCallback cb, void *ud, bool press);
void eg_event_connect_motion_default(struct EgWidget *widget, EgMotionCallback cb, void *ud);
void eg_event_connect_click_default(struct EgWidget *widget, EgButtonCallback cb, void *ud);
void eg_event_connect_focus_default(struct EgWidget *widget, EgFocusCallback cb, void *ud, bool enter);
void eg_event_connect_scroll_default(struct EgWidget *widget, EgScrollCallback cb, void *ud);
void eg_event_connect_crossing_default(struct EgWidget *widget, EgCrossingCallback cb, void *ud, bool enter);

/* ============================================
 * Container Helpers
 * ============================================ */

/* Destrutor para containers com EgChildList */
void eg_container_destroy_with_children(struct EgWidget *widget);

/* Gerenciamento de EgChildList */
void eg_child_list_init(void *list);
void eg_child_list_add(void *list, struct EgWidget *child);
void eg_child_list_remove(void *list, struct EgWidget *child);
void eg_child_list_free(void *list);
size_t eg_child_list_count(void *list);
struct EgWidget *eg_child_list_get(void *list, size_t index);
void eg_child_list_clear(void *list);

#endif /* EASYGTK_VTABLE_H */
