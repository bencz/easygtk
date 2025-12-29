/**
 * EasyGTK - VTables
 * 
 * Definições de vtables para componentização.
 * Permite extensibilidade e polimorfismo em C.
 */

#ifndef EASYGTK_VTABLE_H
#define EASYGTK_VTABLE_H

#include <easygtk/types.h>

/* Forward declarations */
struct EgWidget;

/* VTable base para todos os widgets */
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
    
} EgWidgetVTable;

/* VTable para containers */
typedef struct EgContainerVTable {
    EgWidgetVTable base;
    
    /* Adiciona filho */
    void (*add_child)(struct EgWidget *container, struct EgWidget *child);
    
    /* Remove filho */
    void (*remove_child)(struct EgWidget *container, struct EgWidget *child);
    
} EgContainerVTable;

/* Declarações das vtables globais */
extern const EgWidgetVTable eg_widget_vtable;
extern const EgWidgetVTable eg_window_vtable;
extern const EgWidgetVTable eg_button_vtable;
extern const EgWidgetVTable eg_label_vtable;
extern const EgWidgetVTable eg_entry_vtable;
extern const EgContainerVTable eg_box_vtable;
extern const EgContainerVTable eg_grid_vtable;

#endif /* EASYGTK_VTABLE_H */
