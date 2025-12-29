/**
 * EasyGTK - Sistema de Eventos
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/events.h>

/* ============================================
 * Estruturas de dados para callbacks
 * ============================================ */

typedef struct {
    EgKeyCallback callback;
    void *user_data;
    EgWidget *widget;
    bool is_press;
} KeyCallbackData;

typedef struct {
    EgMotionCallback callback;
    void *user_data;
    EgWidget *widget;
} MotionCallbackData;

typedef struct {
    EgCrossingCallback callback;
    void *user_data;
    EgWidget *widget;
} CrossingCallbackData;

typedef struct {
    EgScrollCallback callback;
    void *user_data;
    EgWidget *widget;
} ScrollCallbackData;

typedef struct {
    EgButtonCallback callback;
    void *user_data;
    EgWidget *widget;
} ButtonCallbackData;

typedef struct {
    EgFocusCallback callback;
    void *user_data;
    EgWidget *widget;
} FocusCallbackData;

/* ============================================
 * Funções auxiliares
 * ============================================ */

static EgModifierType gdk_to_eg_modifiers(GdkModifierType state) {
    EgModifierType mods = EG_MODIFIER_NONE;
    if (state & GDK_SHIFT_MASK) mods |= EG_MODIFIER_SHIFT;
    if (state & GDK_CONTROL_MASK) mods |= EG_MODIFIER_CTRL;
    if (state & GDK_ALT_MASK) mods |= EG_MODIFIER_ALT;
    if (state & GDK_SUPER_MASK) mods |= EG_MODIFIER_SUPER;
    return mods;
}

/* ============================================
 * Callbacks GTK para eventos de teclado
 * ============================================ */

static gboolean key_event_callback(GtkEventControllerKey *controller,
                                   guint keyval,
                                   guint keycode,
                                   GdkModifierType state,
                                   gpointer user_data) {
    (void)controller;
    KeyCallbackData *data = (KeyCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return FALSE;
    
    EgKeyEvent event = {
        .keyval = keyval,
        .keycode = keycode,
        .modifiers = gdk_to_eg_modifiers(state),
        .is_press = data->is_press
    };
    
    return data->callback(data->widget, &event, data->user_data);
}

static void key_released_callback(GtkEventControllerKey *controller,
                                  guint keyval,
                                  guint keycode,
                                  GdkModifierType state,
                                  gpointer user_data) {
    (void)controller;
    KeyCallbackData *data = (KeyCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    EgKeyEvent event = {
        .keyval = keyval,
        .keycode = keycode,
        .modifiers = gdk_to_eg_modifiers(state),
        .is_press = false
    };
    
    data->callback(data->widget, &event, data->user_data);
}

/* ============================================
 * Callbacks GTK para eventos de mouse
 * ============================================ */

static void motion_callback(GtkEventControllerMotion *controller,
                           double x, double y,
                           gpointer user_data) {
    (void)controller;
    MotionCallbackData *data = (MotionCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    EgMouseEvent event = {
        .x = x,
        .y = y,
        .modifiers = EG_MODIFIER_NONE
    };
    
    data->callback(data->widget, &event, data->user_data);
}

static void enter_callback(GtkEventControllerMotion *controller,
                          double x, double y,
                          gpointer user_data) {
    (void)controller;
    CrossingCallbackData *data = (CrossingCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    data->callback(data->widget, x, y, data->user_data);
}

static void leave_callback(GtkEventControllerMotion *controller,
                          gpointer user_data) {
    (void)controller;
    CrossingCallbackData *data = (CrossingCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    data->callback(data->widget, 0, 0, data->user_data);
}

static gboolean scroll_callback(GtkEventControllerScroll *controller,
                               double dx, double dy,
                               gpointer user_data) {
    (void)controller;
    ScrollCallbackData *data = (ScrollCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return FALSE;
    
    EgScrollEvent event = {
        .x = 0,
        .y = 0,
        .delta_x = dx,
        .delta_y = dy,
        .modifiers = EG_MODIFIER_NONE
    };
    
    data->callback(data->widget, &event, data->user_data);
    return FALSE;
}

static void click_pressed_callback(GtkGestureClick *gesture,
                                   int n_press,
                                   double x, double y,
                                   gpointer user_data) {
    ButtonCallbackData *data = (ButtonCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    guint button = gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(gesture));
    GdkModifierType state = gtk_event_controller_get_current_event_state(
        GTK_EVENT_CONTROLLER(gesture));
    
    EgButtonEvent event = {
        .x = x,
        .y = y,
        .button = button,
        .n_press = (uint32_t)n_press,
        .modifiers = gdk_to_eg_modifiers(state)
    };
    
    data->callback(data->widget, &event, data->user_data);
}

/* ============================================
 * Callbacks GTK para eventos de foco
 * ============================================ */

static void focus_enter_callback(GtkEventControllerFocus *controller,
                                gpointer user_data) {
    (void)controller;
    FocusCallbackData *data = (FocusCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    data->callback(data->widget, data->user_data);
}

static void focus_leave_callback(GtkEventControllerFocus *controller,
                                gpointer user_data) {
    (void)controller;
    FocusCallbackData *data = (FocusCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;
    
    data->callback(data->widget, data->user_data);
}

/* ============================================
 * Implementação das funções públicas
 * ============================================ */

void eg_widget_on_key_press(EgWidget *widget, EgKeyCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    KeyCallbackData *data = EG_ALLOC(KeyCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    data->is_press = true;
    
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_event_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_key_release(EgWidget *widget, EgKeyCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    KeyCallbackData *data = EG_ALLOC(KeyCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    data->is_press = false;
    
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-released", G_CALLBACK(key_released_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_motion(EgWidget *widget, EgMotionCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    MotionCallbackData *data = EG_ALLOC(MotionCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "motion", G_CALLBACK(motion_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_enter(EgWidget *widget, EgCrossingCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    CrossingCallbackData *data = EG_ALLOC(CrossingCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "enter", G_CALLBACK(enter_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_leave(EgWidget *widget, EgCrossingCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    CrossingCallbackData *data = EG_ALLOC(CrossingCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "leave", G_CALLBACK(leave_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_scroll(EgWidget *widget, EgScrollCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    ScrollCallbackData *data = EG_ALLOC(ScrollCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_scroll_new(
        GTK_EVENT_CONTROLLER_SCROLL_BOTH_AXES);
    g_signal_connect(controller, "scroll", G_CALLBACK(scroll_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_click(EgWidget *widget, EgButtonCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    ButtonCallbackData *data = EG_ALLOC(ButtonCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkGesture *gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), 0); /* Todos os botões */
    g_signal_connect(gesture, "pressed", G_CALLBACK(click_pressed_callback), data);
    gtk_widget_add_controller(widget->native, GTK_EVENT_CONTROLLER(gesture));
}

void eg_widget_on_focus_in(EgWidget *widget, EgFocusCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    FocusCallbackData *data = EG_ALLOC(FocusCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_focus_new();
    g_signal_connect(controller, "enter", G_CALLBACK(focus_enter_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_on_focus_out(EgWidget *widget, EgFocusCallback callback, void *user_data) {
    if (widget == NULL || widget->native == NULL || callback == NULL) return;
    
    FocusCallbackData *data = EG_ALLOC(FocusCallbackData);
    if (data == NULL) return;
    
    data->callback = callback;
    data->user_data = user_data;
    data->widget = widget;
    
    GtkEventController *controller = gtk_event_controller_focus_new();
    g_signal_connect(controller, "leave", G_CALLBACK(focus_leave_callback), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_widget_set_focusable(EgWidget *widget, bool focusable) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_focusable(widget->native, focusable);
}

bool eg_widget_get_focusable(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_focusable(widget->native);
}

bool eg_widget_grab_focus(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_grab_focus(widget->native);
}

bool eg_widget_has_focus(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_has_focus(widget->native);
}
