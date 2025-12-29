/**
 * EasyGTK - Drag and Drop
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/dnd.h>

/* Estrutura para dados do drag source */
typedef struct {
    EgDragPrepareCallback prepare_callback;
    EgDragBeginCallback begin_callback;
    EgDragEndCallback end_callback;
    void *prepare_data;
    void *begin_data;
    void *end_data;
    EgWidget *widget;
} DragSourceData;

/* Estrutura para dados do drop target */
typedef struct {
    EgDropCallback drop_callback;
    void *user_data;
    EgWidget *widget;
} DropTargetData;

/* Converte EgDragAction para GdkDragAction */
static GdkDragAction eg_to_gdk_drag_action(EgDragAction actions) {
    GdkDragAction gdk_actions = 0;
    if (actions & EG_DRAG_ACTION_COPY) gdk_actions |= GDK_ACTION_COPY;
    if (actions & EG_DRAG_ACTION_MOVE) gdk_actions |= GDK_ACTION_MOVE;
    if (actions & EG_DRAG_ACTION_LINK) gdk_actions |= GDK_ACTION_LINK;
    return gdk_actions;
}

/* Callback para preparar conteúdo do drag */
static GdkContentProvider *drag_prepare_callback(GtkDragSource *source,
                                                  double x, double y,
                                                  gpointer user_data) {
    (void)source; (void)x; (void)y;
    DragSourceData *data = (DragSourceData *)user_data;
    
    if (data == NULL || data->prepare_callback == NULL) {
        return NULL;
    }
    
    const char *text = data->prepare_callback(data->widget, data->prepare_data);
    if (text == NULL) {
        return NULL;
    }
    
    return gdk_content_provider_new_typed(G_TYPE_STRING, text);
}

/* Callback para início do drag */
static void drag_begin_callback(GtkDragSource *source, GdkDrag *drag, gpointer user_data) {
    (void)source; (void)drag;
    DragSourceData *data = (DragSourceData *)user_data;
    
    if (data != NULL && data->begin_callback != NULL) {
        data->begin_callback(data->widget, data->begin_data);
    }
}

/* Callback para fim do drag */
static void drag_end_callback(GtkDragSource *source, GdkDrag *drag,
                              gboolean delete_data, gpointer user_data) {
    (void)source; (void)drag; (void)delete_data;
    DragSourceData *data = (DragSourceData *)user_data;
    
    if (data != NULL && data->end_callback != NULL) {
        data->end_callback(data->widget, TRUE, data->end_data);
    }
}

/* Callback GTK para drop */
static gboolean gtk_drop_handler(GtkDropTarget *target, const GValue *value,
                                 double x, double y, gpointer user_data) {
    (void)target;
    DropTargetData *data = (DropTargetData *)user_data;
    
    if (data == NULL || data->drop_callback == NULL) {
        return FALSE;
    }
    
    if (G_VALUE_HOLDS_STRING(value)) {
        const char *text = g_value_get_string(value);
        data->drop_callback(data->widget, text, x, y, data->user_data);
        return TRUE;
    }
    
    return FALSE;
}

void eg_widget_set_drag_source(EgWidget *widget, EgDragAction actions,
                                EgDragPrepareCallback prepare_callback, void *user_data) {
    if (widget == NULL || widget->native == NULL) return;
    
    DragSourceData *data = EG_ALLOC(DragSourceData);
    if (data == NULL) return;
    
    data->prepare_callback = prepare_callback;
    data->begin_callback = NULL;
    data->end_callback = NULL;
    data->prepare_data = user_data;
    data->begin_data = NULL;
    data->end_data = NULL;
    data->widget = widget;
    
    GtkDragSource *source = gtk_drag_source_new();
    gtk_drag_source_set_actions(source, eg_to_gdk_drag_action(actions));
    
    g_signal_connect(source, "prepare", G_CALLBACK(drag_prepare_callback), data);
    g_signal_connect(source, "drag-begin", G_CALLBACK(drag_begin_callback), data);
    g_signal_connect(source, "drag-end", G_CALLBACK(drag_end_callback), data);
    
    gtk_widget_add_controller(widget->native, GTK_EVENT_CONTROLLER(source));
}

void eg_widget_on_drag_begin(EgWidget *widget, EgDragBeginCallback callback, void *user_data) {
    (void)widget; (void)callback; (void)user_data;
    /* TODO: Implementar busca do DragSourceData existente */
}

void eg_widget_on_drag_end(EgWidget *widget, EgDragEndCallback callback, void *user_data) {
    (void)widget; (void)callback; (void)user_data;
    /* TODO: Implementar busca do DragSourceData existente */
}

void eg_widget_set_drop_target(EgWidget *widget, EgDragAction actions,
                                EgDropCallback drop_callback, void *user_data) {
    if (widget == NULL || widget->native == NULL) return;
    
    DropTargetData *data = EG_ALLOC(DropTargetData);
    if (data == NULL) return;
    
    data->drop_callback = drop_callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkDropTarget *target = gtk_drop_target_new(G_TYPE_STRING, eg_to_gdk_drag_action(actions));
    
    g_signal_connect(target, "drop", G_CALLBACK(gtk_drop_handler), data);
    
    gtk_widget_add_controller(widget->native, GTK_EVENT_CONTROLLER(target));
}

void eg_widget_remove_drag_source(EgWidget *widget) {
    (void)widget;
    /* TODO: Implementar remoção de controllers */
}

void eg_widget_remove_drop_target(EgWidget *widget) {
    (void)widget;
    /* TODO: Implementar remoção de controllers */
}
