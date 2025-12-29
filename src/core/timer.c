/**
 * EasyGTK - Timers
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/timer.h>

/* Estrutura para dados do timer */
typedef struct {
    EgTimerCallback callback;
    void *user_data;
    bool once;
} TimerData;

/* Callback wrapper para GLib */
static gboolean timer_callback_wrapper(gpointer user_data) {
    TimerData *data = (TimerData *)user_data;
    if (data == NULL || data->callback == NULL) {
        return G_SOURCE_REMOVE;
    }
    
    bool continue_timer = data->callback(data->user_data);
    
    if (!continue_timer || data->once) {
        eg_free(data);
        return G_SOURCE_REMOVE;
    }
    
    return G_SOURCE_CONTINUE;
}

/* Callback para timers "once" - sempre remove após executar */
static gboolean timer_once_callback_wrapper(gpointer user_data) {
    TimerData *data = (TimerData *)user_data;
    if (data != NULL && data->callback != NULL) {
        data->callback(data->user_data);
    }
    eg_free(data);
    return G_SOURCE_REMOVE;
}

EgTimerId eg_timeout_add(uint32_t interval_ms, EgTimerCallback callback, void *user_data) {
    if (callback == NULL) return 0;
    
    TimerData *data = EG_ALLOC(TimerData);
    if (data == NULL) return 0;
    
    data->callback = callback;
    data->user_data = user_data;
    data->once = false;
    
    return (EgTimerId)g_timeout_add(interval_ms, timer_callback_wrapper, data);
}

EgTimerId eg_timeout_add_once(uint32_t interval_ms, EgTimerCallback callback, void *user_data) {
    if (callback == NULL) return 0;
    
    TimerData *data = EG_ALLOC(TimerData);
    if (data == NULL) return 0;
    
    data->callback = callback;
    data->user_data = user_data;
    data->once = true;
    
    return (EgTimerId)g_timeout_add(interval_ms, timer_once_callback_wrapper, data);
}

EgTimerId eg_idle_add(EgTimerCallback callback, void *user_data) {
    if (callback == NULL) return 0;
    
    TimerData *data = EG_ALLOC(TimerData);
    if (data == NULL) return 0;
    
    data->callback = callback;
    data->user_data = user_data;
    data->once = false;
    
    return (EgTimerId)g_idle_add(timer_callback_wrapper, data);
}

EgTimerId eg_idle_add_once(EgTimerCallback callback, void *user_data) {
    if (callback == NULL) return 0;
    
    TimerData *data = EG_ALLOC(TimerData);
    if (data == NULL) return 0;
    
    data->callback = callback;
    data->user_data = user_data;
    data->once = true;
    
    return (EgTimerId)g_idle_add(timer_once_callback_wrapper, data);
}

bool eg_source_remove(EgTimerId timer_id) {
    if (timer_id == 0) return false;
    return g_source_remove((guint)timer_id) == TRUE;
}
