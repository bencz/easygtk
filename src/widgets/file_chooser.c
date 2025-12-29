/**
 * EasyGTK - FileChooser Dialog
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/file_chooser.h>
#include <easygtk/window.h>

/* Estrutura interna do filtro */
struct EgFileFilter {
    GtkFileFilter *native;
};

/* Dados para callback assíncrono */
typedef struct {
    EgFileChooserCallback callback;
    void *user_data;
} FileChooserData;

typedef struct {
    EgFileChooserMultiCallback callback;
    void *user_data;
} FileChooserMultiData;

EgFileFilter *eg_file_filter_new(const char *name) {
    EgFileFilter *filter = EG_ALLOC(EgFileFilter);
    if (filter == NULL) return NULL;
    
    filter->native = gtk_file_filter_new();
    if (filter->native == NULL) {
        eg_free(filter);
        return NULL;
    }
    
    if (name != NULL) {
        gtk_file_filter_set_name(filter->native, name);
    }
    
    return filter;
}

void eg_file_filter_free(EgFileFilter *filter) {
    if (filter == NULL) return;
    eg_free(filter);
}

void eg_file_filter_add_mime_type(EgFileFilter *filter, const char *mime_type) {
    if (filter == NULL || filter->native == NULL || mime_type == NULL) return;
    gtk_file_filter_add_mime_type(filter->native, mime_type);
}

void eg_file_filter_add_pattern(EgFileFilter *filter, const char *pattern) {
    if (filter == NULL || filter->native == NULL || pattern == NULL) return;
    gtk_file_filter_add_pattern(filter->native, pattern);
}

void eg_file_filter_add_suffix(EgFileFilter *filter, const char *suffix) {
    if (filter == NULL || filter->native == NULL || suffix == NULL) return;
    gtk_file_filter_add_suffix(filter->native, suffix);
}

/* Callback para GtkFileDialog open */
static void file_dialog_open_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    FileChooserData *data = (FileChooserData *)user_data;
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    
    GError *error = NULL;
    GFile *file = gtk_file_dialog_open_finish(dialog, result, &error);
    
    if (file != NULL) {
        char *path = g_file_get_path(file);
        if (data->callback != NULL) {
            data->callback(path, data->user_data);
        }
        g_free(path);
        g_object_unref(file);
    } else {
        if (data->callback != NULL) {
            data->callback(NULL, data->user_data);
        }
        if (error != NULL) {
            g_error_free(error);
        }
    }
    
    eg_free(data);
}

/* Callback para GtkFileDialog open_multiple */
static void file_dialog_open_multiple_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    FileChooserMultiData *data = (FileChooserMultiData *)user_data;
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    
    GError *error = NULL;
    GListModel *files = gtk_file_dialog_open_multiple_finish(dialog, result, &error);
    
    if (files != NULL) {
        guint n_items = g_list_model_get_n_items(files);
        char **paths = EG_ALLOC_ARRAY(char *, n_items + 1);
        
        if (paths != NULL) {
            for (guint i = 0; i < n_items; i++) {
                GFile *file = G_FILE(g_list_model_get_item(files, i));
                paths[i] = g_file_get_path(file);
                g_object_unref(file);
            }
            paths[n_items] = NULL;
            
            if (data->callback != NULL) {
                data->callback(paths, (int)n_items, data->user_data);
            }
            
            for (guint i = 0; i < n_items; i++) {
                g_free(paths[i]);
            }
            eg_free(paths);
        }
        
        g_object_unref(files);
    } else {
        if (data->callback != NULL) {
            data->callback(NULL, 0, data->user_data);
        }
        if (error != NULL) {
            g_error_free(error);
        }
    }
    
    eg_free(data);
}

/* Callback para GtkFileDialog save */
static void file_dialog_save_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    FileChooserData *data = (FileChooserData *)user_data;
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    
    GError *error = NULL;
    GFile *file = gtk_file_dialog_save_finish(dialog, result, &error);
    
    if (file != NULL) {
        char *path = g_file_get_path(file);
        if (data->callback != NULL) {
            data->callback(path, data->user_data);
        }
        g_free(path);
        g_object_unref(file);
    } else {
        if (data->callback != NULL) {
            data->callback(NULL, data->user_data);
        }
        if (error != NULL) {
            g_error_free(error);
        }
    }
    
    eg_free(data);
}

/* Callback para GtkFileDialog select_folder */
static void file_dialog_select_folder_callback(GObject *source, GAsyncResult *result, gpointer user_data) {
    FileChooserData *data = (FileChooserData *)user_data;
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    
    GError *error = NULL;
    GFile *file = gtk_file_dialog_select_folder_finish(dialog, result, &error);
    
    if (file != NULL) {
        char *path = g_file_get_path(file);
        if (data->callback != NULL) {
            data->callback(path, data->user_data);
        }
        g_free(path);
        g_object_unref(file);
    } else {
        if (data->callback != NULL) {
            data->callback(NULL, data->user_data);
        }
        if (error != NULL) {
            g_error_free(error);
        }
    }
    
    eg_free(data);
}

void eg_file_chooser_open(EgWindow *parent, const char *title,
                          EgFileChooserCallback callback, void *user_data) {
    eg_file_chooser_open_with_filters(parent, title, NULL, callback, user_data);
}

void eg_file_chooser_open_with_filters(EgWindow *parent, const char *title,
                                        EgFileFilter **filters,
                                        EgFileChooserCallback callback, void *user_data) {
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    if (title != NULL) {
        gtk_file_dialog_set_title(dialog, title);
    }
    
    if (filters != NULL) {
        GListStore *filter_list = g_list_store_new(GTK_TYPE_FILE_FILTER);
        for (int i = 0; filters[i] != NULL; i++) {
            g_list_store_append(filter_list, filters[i]->native);
        }
        gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filter_list));
        g_object_unref(filter_list);
    }
    
    FileChooserData *data = EG_ALLOC(FileChooserData);
    if (data == NULL) {
        g_object_unref(dialog);
        return;
    }
    data->callback = callback;
    data->user_data = user_data;
    
    GtkWindow *gtk_parent = parent ? GTK_WINDOW(eg_window_get_native(parent)) : NULL;
    gtk_file_dialog_open(dialog, gtk_parent, NULL, file_dialog_open_callback, data);
    
    g_object_unref(dialog);
}

void eg_file_chooser_open_multiple(EgWindow *parent, const char *title,
                                    EgFileChooserMultiCallback callback, void *user_data) {
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    if (title != NULL) {
        gtk_file_dialog_set_title(dialog, title);
    }
    
    FileChooserMultiData *data = EG_ALLOC(FileChooserMultiData);
    if (data == NULL) {
        g_object_unref(dialog);
        return;
    }
    data->callback = callback;
    data->user_data = user_data;
    
    GtkWindow *gtk_parent = parent ? GTK_WINDOW(eg_window_get_native(parent)) : NULL;
    gtk_file_dialog_open_multiple(dialog, gtk_parent, NULL, file_dialog_open_multiple_callback, data);
    
    g_object_unref(dialog);
}

void eg_file_chooser_save(EgWindow *parent, const char *title,
                          const char *initial_name,
                          EgFileChooserCallback callback, void *user_data) {
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    if (title != NULL) {
        gtk_file_dialog_set_title(dialog, title);
    }
    
    if (initial_name != NULL) {
        gtk_file_dialog_set_initial_name(dialog, initial_name);
    }
    
    FileChooserData *data = EG_ALLOC(FileChooserData);
    if (data == NULL) {
        g_object_unref(dialog);
        return;
    }
    data->callback = callback;
    data->user_data = user_data;
    
    GtkWindow *gtk_parent = parent ? GTK_WINDOW(eg_window_get_native(parent)) : NULL;
    gtk_file_dialog_save(dialog, gtk_parent, NULL, file_dialog_save_callback, data);
    
    g_object_unref(dialog);
}

void eg_file_chooser_select_folder(EgWindow *parent, const char *title,
                                    EgFileChooserCallback callback, void *user_data) {
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    if (title != NULL) {
        gtk_file_dialog_set_title(dialog, title);
    }
    
    FileChooserData *data = EG_ALLOC(FileChooserData);
    if (data == NULL) {
        g_object_unref(dialog);
        return;
    }
    data->callback = callback;
    data->user_data = user_data;
    
    GtkWindow *gtk_parent = parent ? GTK_WINDOW(eg_window_get_native(parent)) : NULL;
    gtk_file_dialog_select_folder(dialog, gtk_parent, NULL, file_dialog_select_folder_callback, data);
    
    g_object_unref(dialog);
}
