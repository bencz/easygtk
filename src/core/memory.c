/**
 * EasyGTK - Gerenciamento de memória
 */

#include <stdlib.h>
#include <string.h>
#include "internal/internal.h"

void *eg_alloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    void *ptr = calloc(1, size);
    return ptr;
}

void *eg_realloc(void *ptr, size_t size) {
    if (size == 0) {
        eg_free(ptr);
        return NULL;
    }
    
    return realloc(ptr, size);
}

void eg_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

char *eg_strdup(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    size_t len = strlen(str) + 1;
    char *copy = eg_alloc(len);
    if (copy != NULL) {
        memcpy(copy, str, len);
    }
    return copy;
}

EgCallbackData *eg_callback_data_new(EgCallback callback, void *user_data, EgWidget *widget) {
    EgCallbackData *data = EG_ALLOC(EgCallbackData);
    if (data != NULL) {
        data->callback = callback;
        data->user_data = user_data;
        data->widget = widget;
    }
    return data;
}

void eg_callback_data_free(EgCallbackData *data) {
    eg_free(data);
}
