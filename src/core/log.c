/**
 * EasyGTK - Logging e Debug
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/log.h>

/* Estado global do sistema de log */
static EgLogLevel g_log_level = EG_LOG_LEVEL_WARNING;
static EgLogCallback g_log_callback = NULL;
static void *g_log_user_data = NULL;
static bool g_log_colors = true;

/* Cores ANSI */
#define COLOR_RESET   "\033[0m"
#define COLOR_DEBUG   "\033[36m"   /* Cyan */
#define COLOR_INFO    "\033[32m"   /* Green */
#define COLOR_WARNING "\033[33m"   /* Yellow */
#define COLOR_ERROR   "\033[31m"   /* Red */

/* Nomes dos níveis */
static const char *level_names[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "NONE"
};

/* Cores dos níveis */
static const char *level_colors[] = {
    COLOR_DEBUG,
    COLOR_INFO,
    COLOR_WARNING,
    COLOR_ERROR,
    COLOR_RESET
};

/* Handler padrão */
static void default_log_handler(EgLogLevel level, const char *domain, 
                                 const char *message, void *user_data) {
    (void)user_data;
    
    FILE *out = (level >= EG_LOG_LEVEL_WARNING) ? stderr : stdout;
    
    if (g_log_colors) {
        fprintf(out, "%s[%s]%s ", level_colors[level], level_names[level], COLOR_RESET);
    } else {
        fprintf(out, "[%s] ", level_names[level]);
    }
    
    if (domain != NULL && domain[0] != '\0') {
        fprintf(out, "(%s) ", domain);
    }
    
    fprintf(out, "%s\n", message);
    fflush(out);
}

/* Função interna de log */
static void eg_log_internal(EgLogLevel level, const char *domain, 
                            const char *format, va_list args) {
    if (level < g_log_level) return;
    
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    if (g_log_callback != NULL) {
        g_log_callback(level, domain, buffer, g_log_user_data);
    } else {
        default_log_handler(level, domain, buffer, NULL);
    }
}

void eg_log_set_level(EgLogLevel level) {
    g_log_level = level;
}

EgLogLevel eg_log_get_level(void) {
    return g_log_level;
}

void eg_log_set_handler(EgLogCallback callback, void *user_data) {
    g_log_callback = callback;
    g_log_user_data = user_data;
}

void eg_log_set_colors(bool enabled) {
    g_log_colors = enabled;
}

void eg_log_debug(const char *domain, const char *format, ...) {
    va_list args;
    va_start(args, format);
    eg_log_internal(EG_LOG_LEVEL_DEBUG, domain, format, args);
    va_end(args);
}

void eg_log_info(const char *domain, const char *format, ...) {
    va_list args;
    va_start(args, format);
    eg_log_internal(EG_LOG_LEVEL_INFO, domain, format, args);
    va_end(args);
}

void eg_log_warning(const char *domain, const char *format, ...) {
    va_list args;
    va_start(args, format);
    eg_log_internal(EG_LOG_LEVEL_WARNING, domain, format, args);
    va_end(args);
}

void eg_log_error(const char *domain, const char *format, ...) {
    va_list args;
    va_start(args, format);
    eg_log_internal(EG_LOG_LEVEL_ERROR, domain, format, args);
    va_end(args);
}

/* Obtém nome do tipo via vtable */
static const char *get_type_name_from_vtable(EgWidget *widget) {
    if (widget == NULL) return "NULL";
    if (widget->vtable != NULL && widget->vtable->type_name != NULL) {
        return widget->vtable->type_name;
    }
    return "Unknown";
}

void eg_widget_debug_print(EgWidget *widget) {
    if (widget == NULL) {
        eg_log_debug("Debug", "Widget: NULL");
        return;
    }
    
    const char *type_name = get_type_name_from_vtable(widget);
    const char *name = gtk_widget_get_name(widget->native);
    bool visible = gtk_widget_get_visible(widget->native);
    bool sensitive = gtk_widget_get_sensitive(widget->native);
    double opacity = gtk_widget_get_opacity(widget->native);
    
    int width = gtk_widget_get_width(widget->native);
    int height = gtk_widget_get_height(widget->native);
    
    eg_log_debug("Debug", "Widget Info:");
    eg_log_debug("Debug", "  Type: %s", type_name);
    eg_log_debug("Debug", "  Name: %s", name ? name : "(none)");
    eg_log_debug("Debug", "  Visible: %s", visible ? "true" : "false");
    eg_log_debug("Debug", "  Sensitive: %s", sensitive ? "true" : "false");
    eg_log_debug("Debug", "  Opacity: %.2f", opacity);
    eg_log_debug("Debug", "  Size: %dx%d", width, height);
    eg_log_debug("Debug", "  Native: %p", (void *)widget->native);
}
