/**
 * EasyGTK - Sistema de Signals
 */

#include <string.h>
#include "internal/internal.h"
#include <easygtk/signal.h>

/* Estrutura de um handler */
typedef struct EgSignalHandler {
    EgHandlerId id;
    EgSignalCallback callback;
    void *user_data;
    struct EgSignalHandler *next;
} EgSignalHandler;

/* Estrutura do signal */
struct EgSignal {
    char *name;
    EgSignalHandler *handlers;
    EgHandlerId next_id;
    bool blocked;
    size_t handler_count;
};

EgSignal *eg_signal_new(const char *name) {
    EgSignal *signal = EG_ALLOC(EgSignal);
    if (signal == NULL) return NULL;
    
    signal->name = eg_strdup(name);
    signal->handlers = NULL;
    signal->next_id = 1;
    signal->blocked = false;
    signal->handler_count = 0;
    
    return signal;
}

void eg_signal_free(EgSignal *signal) {
    if (signal == NULL) return;
    
    /* Libera todos os handlers */
    EgSignalHandler *handler = signal->handlers;
    while (handler != NULL) {
        EgSignalHandler *next = handler->next;
        eg_free(handler);
        handler = next;
    }
    
    eg_free(signal->name);
    eg_free(signal);
}

EgHandlerId eg_signal_connect(EgSignal *signal, EgSignalCallback callback, void *user_data) {
    if (signal == NULL || callback == NULL) return 0;
    
    EgSignalHandler *handler = EG_ALLOC(EgSignalHandler);
    if (handler == NULL) return 0;
    
    handler->id = signal->next_id++;
    handler->callback = callback;
    handler->user_data = user_data;
    handler->next = NULL;
    
    /* Adiciona ao final da lista */
    if (signal->handlers == NULL) {
        signal->handlers = handler;
    } else {
        EgSignalHandler *last = signal->handlers;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = handler;
    }
    
    signal->handler_count++;
    return handler->id;
}

bool eg_signal_disconnect(EgSignal *signal, EgHandlerId handler_id) {
    if (signal == NULL || handler_id == 0) return false;
    
    EgSignalHandler *prev = NULL;
    EgSignalHandler *current = signal->handlers;
    
    while (current != NULL) {
        if (current->id == handler_id) {
            if (prev == NULL) {
                signal->handlers = current->next;
            } else {
                prev->next = current->next;
            }
            eg_free(current);
            signal->handler_count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;
}

void eg_signal_disconnect_all(EgSignal *signal) {
    if (signal == NULL) return;
    
    EgSignalHandler *handler = signal->handlers;
    while (handler != NULL) {
        EgSignalHandler *next = handler->next;
        eg_free(handler);
        handler = next;
    }
    
    signal->handlers = NULL;
    signal->handler_count = 0;
}

void eg_signal_emit(EgSignal *signal, void *sender, void *event_data) {
    if (signal == NULL || signal->blocked) return;
    
    EgSignalHandler *handler = signal->handlers;
    while (handler != NULL) {
        /* Guarda next antes de chamar callback (callback pode desconectar) */
        EgSignalHandler *next = handler->next;
        handler->callback(sender, event_data, handler->user_data);
        handler = next;
    }
}

size_t eg_signal_get_handler_count(EgSignal *signal) {
    if (signal == NULL) return 0;
    return signal->handler_count;
}

void eg_signal_block(EgSignal *signal) {
    if (signal == NULL) return;
    signal->blocked = true;
}

void eg_signal_unblock(EgSignal *signal) {
    if (signal == NULL) return;
    signal->blocked = false;
}
