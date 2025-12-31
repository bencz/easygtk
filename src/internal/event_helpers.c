/**
 * EasyGTK - Event Helpers
 *
 * Funções auxiliares para conexão de eventos via GtkEventController.
 * Usadas como implementação default quando widget->vtable->events == NULL.
 */

#include <gtk/gtk.h>
#include "internal.h"

/* ============================================
 * Estrutura unificada de callback
 * ============================================ */

typedef struct EgEventCallbackData {
    void *callback;         /* Ponteiro para função de callback */
    void *user_data;        /* Dados do usuário */
    EgWidget *widget;       /* Widget associado */
    bool flag;              /* Flag auxiliar (press/release, enter/leave) */
} EgEventCallbackData;

static EgEventCallbackData *event_data_new(void *cb, void *ud, EgWidget *w, bool flag) {
    EgEventCallbackData *data = EG_ALLOC(EgEventCallbackData);
    if (data == NULL) return NULL;
    data->callback = cb;
    data->user_data = ud;
    data->widget = w;
    data->flag = flag;
    return data;
}

/* ============================================
 * Conversão de modificadores
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
 * Callbacks GTK internos
 * ============================================ */

static gboolean on_key_pressed(GtkEventControllerKey *controller,
                               guint keyval, guint keycode,
                               GdkModifierType state, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return FALSE;

    EgKeyEvent event = {
        .keyval = keyval,
        .keycode = keycode,
        .modifiers = gdk_to_eg_modifiers(state),
        .is_press = data->flag
    };

    EgKeyCallback cb = (EgKeyCallback)data->callback;
    return cb(data->widget, &event, data->user_data);
}

static void on_key_released(GtkEventControllerKey *controller,
                            guint keyval, guint keycode,
                            GdkModifierType state, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgKeyEvent event = {
        .keyval = keyval,
        .keycode = keycode,
        .modifiers = gdk_to_eg_modifiers(state),
        .is_press = false
    };

    EgKeyCallback cb = (EgKeyCallback)data->callback;
    cb(data->widget, &event, data->user_data);
}

static void on_motion(GtkEventControllerMotion *controller,
                      double x, double y, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgMouseEvent event = {
        .x = x,
        .y = y,
        .modifiers = EG_MODIFIER_NONE
    };

    EgMotionCallback cb = (EgMotionCallback)data->callback;
    cb(data->widget, &event, data->user_data);
}

static void on_click_pressed(GtkGestureClick *gesture,
                             int n_press, double x, double y,
                             gpointer user_data) {
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
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

    EgButtonCallback cb = (EgButtonCallback)data->callback;
    cb(data->widget, &event, data->user_data);
}

static void on_focus_enter(GtkEventControllerFocus *controller, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgFocusCallback cb = (EgFocusCallback)data->callback;
    cb(data->widget, data->user_data);
}

static void on_focus_leave(GtkEventControllerFocus *controller, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgFocusCallback cb = (EgFocusCallback)data->callback;
    cb(data->widget, data->user_data);
}

static gboolean on_scroll(GtkEventControllerScroll *controller,
                          double dx, double dy, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return FALSE;

    EgScrollEvent event = {
        .x = 0,
        .y = 0,
        .delta_x = dx,
        .delta_y = dy,
        .modifiers = EG_MODIFIER_NONE
    };

    EgScrollCallback cb = (EgScrollCallback)data->callback;
    cb(data->widget, &event, data->user_data);
    return FALSE;
}

static void on_enter(GtkEventControllerMotion *controller,
                     double x, double y, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgCrossingCallback cb = (EgCrossingCallback)data->callback;
    cb(data->widget, x, y, data->user_data);
}

static void on_leave(GtkEventControllerMotion *controller, gpointer user_data) {
    (void)controller;
    EgEventCallbackData *data = (EgEventCallbackData *)user_data;
    if (data == NULL || data->callback == NULL) return;

    EgCrossingCallback cb = (EgCrossingCallback)data->callback;
    cb(data->widget, 0, 0, data->user_data);
}

/* ============================================
 * Implementações Default Públicas
 * ============================================ */

void eg_event_connect_key_default(EgWidget *widget, EgKeyCallback cb, void *ud, bool press) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, press);
    if (data == NULL) return;

    GtkEventController *controller = gtk_event_controller_key_new();
    if (press) {
        g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_pressed), data);
    } else {
        g_signal_connect(controller, "key-released", G_CALLBACK(on_key_released), data);
    }
    gtk_widget_add_controller(widget->native, controller);
}

void eg_event_connect_motion_default(EgWidget *widget, EgMotionCallback cb, void *ud) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, false);
    if (data == NULL) return;

    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "motion", G_CALLBACK(on_motion), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_event_connect_click_default(EgWidget *widget, EgButtonCallback cb, void *ud) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, false);
    if (data == NULL) return;

    GtkGesture *gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture), 0);
    g_signal_connect(gesture, "pressed", G_CALLBACK(on_click_pressed), data);
    gtk_widget_add_controller(widget->native, GTK_EVENT_CONTROLLER(gesture));
}

void eg_event_connect_focus_default(EgWidget *widget, EgFocusCallback cb, void *ud, bool enter) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, enter);
    if (data == NULL) return;

    GtkEventController *controller = gtk_event_controller_focus_new();
    if (enter) {
        g_signal_connect(controller, "enter", G_CALLBACK(on_focus_enter), data);
    } else {
        g_signal_connect(controller, "leave", G_CALLBACK(on_focus_leave), data);
    }
    gtk_widget_add_controller(widget->native, controller);
}

void eg_event_connect_scroll_default(EgWidget *widget, EgScrollCallback cb, void *ud) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, false);
    if (data == NULL) return;

    GtkEventController *controller = gtk_event_controller_scroll_new(
        GTK_EVENT_CONTROLLER_SCROLL_BOTH_AXES);
    g_signal_connect(controller, "scroll", G_CALLBACK(on_scroll), data);
    gtk_widget_add_controller(widget->native, controller);
}

void eg_event_connect_crossing_default(EgWidget *widget, EgCrossingCallback cb, void *ud, bool enter) {
    if (widget == NULL || widget->native == NULL || cb == NULL) return;

    EgEventCallbackData *data = event_data_new(cb, ud, widget, enter);
    if (data == NULL) return;

    GtkEventController *controller = gtk_event_controller_motion_new();
    if (enter) {
        g_signal_connect(controller, "enter", G_CALLBACK(on_enter), data);
    } else {
        g_signal_connect(controller, "leave", G_CALLBACK(on_leave), data);
    }
    gtk_widget_add_controller(widget->native, controller);
}

/* ============================================
 * VTable Default de Eventos
 * ============================================ */

static const EgEventCapabilities default_event_caps = {
    .supports_key = true,
    .supports_mouse = true,
    .supports_focus = true,
    .supports_scroll = true
};

const EgEventVTable eg_event_vtable_default = {
    .caps = &default_event_caps,
    .connect_key = eg_event_connect_key_default,
    .connect_motion = eg_event_connect_motion_default,
    .connect_click = eg_event_connect_click_default,
    .connect_focus = eg_event_connect_focus_default,
    .connect_scroll = eg_event_connect_scroll_default,
    .connect_crossing = eg_event_connect_crossing_default
};
