/**
 * EasyGTK - Aplicação
 */

#include <gtk/gtk.h>
#include "internal/internal.h"

/* Callback interno para ativação */
static void app_activate_callback(GtkApplication *gtk_app, gpointer user_data) {
    (void)gtk_app;
    EgApp *app = (EgApp *)user_data;
    if (app != NULL && app->on_activate != NULL) {
        app->on_activate(NULL, app->activate_data);
    }
}

EgApp *eg_app_new(const char *app_id) {
    EgApp *app = EG_ALLOC(EgApp);
    if (app == NULL) {
        return NULL;
    }
    
    app->native = gtk_application_new(app_id, G_APPLICATION_DEFAULT_FLAGS);
    if (app->native == NULL) {
        eg_free(app);
        return NULL;
    }
    
    app->on_activate = NULL;
    app->activate_data = NULL;
    app->argc = 0;
    app->argv = NULL;
    
    return app;
}

void eg_app_free(EgApp *app) {
    if (app == NULL) return;
    
    if (app->native != NULL) {
        g_object_unref(app->native);
    }
    
    eg_free(app);
}

int eg_app_run(EgApp *app) {
    if (app == NULL || app->native == NULL) {
        return -1;
    }
    
    /* Conecta o callback de ativação se definido */
    if (app->on_activate != NULL) {
        g_signal_connect(app->native, "activate", G_CALLBACK(app_activate_callback), app);
    }
    
    return g_application_run(G_APPLICATION(app->native), app->argc, app->argv);
}

void eg_app_quit(EgApp *app) {
    if (app == NULL || app->native == NULL) return;
    g_application_quit(G_APPLICATION(app->native));
}

void eg_app_on_activate(EgApp *app, EgCallback callback, void *user_data) {
    if (app == NULL) return;
    
    app->on_activate = callback;
    app->activate_data = user_data;
}

void *eg_app_get_native(EgApp *app) {
    if (app == NULL) return NULL;
    return app->native;
}
