/**
 * EasyGTK - TextView
 */

#include <gtk/gtk.h>
#include <stdlib.h>
#include "internal/internal.h"
#include <easygtk/text_view.h>

/* Funções da vtable */
static void text_view_destroy(EgWidget *widget);
static void *text_view_get_native(EgWidget *widget);
static void text_view_set_visible(EgWidget *widget, bool visible);
static bool text_view_get_visible(EgWidget *widget);
static void text_view_set_sensitive(EgWidget *widget, bool sensitive);
static bool text_view_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_text_view_vtable = {
    .type = EG_WIDGET_TYPE_TEXT_VIEW,
    .type_name = "EgTextView",
    .destroy = text_view_destroy,
    .get_native = text_view_get_native,
    .set_visible = text_view_set_visible,
    .get_visible = text_view_get_visible,
    .set_sensitive = text_view_set_sensitive,
    .get_sensitive = text_view_get_sensitive
};

static void text_view_destroy(EgWidget *widget) {
    EgTextView *tv = (EgTextView *)widget;
    if (tv == NULL) return;
    eg_free(tv);
}

static void *text_view_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void text_view_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool text_view_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void text_view_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool text_view_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback GTK para changed */
static void gtk_buffer_changed_callback(GtkTextBuffer *buffer, gpointer user_data) {
    (void)buffer;
    EgTextView *tv = (EgTextView *)user_data;
    if (tv != NULL && tv->on_changed != NULL) {
        tv->on_changed((EgWidget *)tv, tv->changed_data);
    }
}

EgTextView *eg_text_view_new(void) {
    EgTextView *tv = EG_ALLOC(EgTextView);
    if (tv == NULL) return NULL;
    
    GtkWidget *gtk_tv = gtk_text_view_new();
    if (gtk_tv == NULL) {
        eg_free(tv);
        return NULL;
    }
    
    eg_widget_init(&tv->base, EG_WIDGET_TYPE_TEXT_VIEW, gtk_tv, &eg_text_view_vtable);
    tv->buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_tv));
    tv->on_changed = NULL;
    tv->changed_data = NULL;
    
    return tv;
}

void eg_text_view_free(EgTextView *text_view) {
    text_view_destroy((EgWidget *)text_view);
}

void eg_text_view_set_text(EgTextView *text_view, const char *text) {
    if (text_view == NULL || text_view->buffer == NULL) return;
    gtk_text_buffer_set_text(text_view->buffer, text ? text : "", -1);
}

char *eg_text_view_get_text(EgTextView *text_view) {
    if (text_view == NULL || text_view->buffer == NULL) return NULL;
    
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(text_view->buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(text_view->buffer, &start, &end, FALSE);
    
    /* Retorna cópia que pode ser liberada com free() */
    char *result = eg_strdup(text);
    g_free(text);
    return result;
}

void eg_text_view_set_editable(EgTextView *text_view, bool editable) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view->base.native), editable);
}

bool eg_text_view_get_editable(EgTextView *text_view) {
    if (text_view == NULL || text_view->base.native == NULL) return false;
    return gtk_text_view_get_editable(GTK_TEXT_VIEW(text_view->base.native));
}

void eg_text_view_set_cursor_visible(EgTextView *text_view, bool visible) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view->base.native), visible);
}

void eg_text_view_set_wrap_mode(EgTextView *text_view, int mode) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    GtkWrapMode wrap = GTK_WRAP_NONE;
    switch (mode) {
        case 0: wrap = GTK_WRAP_NONE; break;
        case 1: wrap = GTK_WRAP_CHAR; break;
        case 2: wrap = GTK_WRAP_WORD; break;
        case 3: wrap = GTK_WRAP_WORD_CHAR; break;
    }
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view->base.native), wrap);
}

void eg_text_view_set_justification(EgTextView *text_view, int justification) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    GtkJustification just = GTK_JUSTIFY_LEFT;
    switch (justification) {
        case 0: just = GTK_JUSTIFY_LEFT; break;
        case 1: just = GTK_JUSTIFY_RIGHT; break;
        case 2: just = GTK_JUSTIFY_CENTER; break;
        case 3: just = GTK_JUSTIFY_FILL; break;
    }
    gtk_text_view_set_justification(GTK_TEXT_VIEW(text_view->base.native), just);
}

void eg_text_view_set_left_margin(EgTextView *text_view, int margin) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view->base.native), margin);
}

void eg_text_view_set_right_margin(EgTextView *text_view, int margin) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(text_view->base.native), margin);
}

void eg_text_view_set_top_margin(EgTextView *text_view, int margin) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(text_view->base.native), margin);
}

void eg_text_view_set_bottom_margin(EgTextView *text_view, int margin) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(text_view->base.native), margin);
}

void eg_text_view_set_accepts_tab(EgTextView *text_view, bool accepts_tab) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(text_view->base.native), accepts_tab);
}

void eg_text_view_set_monospace(EgTextView *text_view, bool monospace) {
    if (text_view == NULL || text_view->base.native == NULL) return;
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view->base.native), monospace);
}

void eg_text_view_on_changed(EgTextView *text_view, EgCallback callback, void *user_data) {
    if (text_view == NULL || text_view->buffer == NULL) return;
    
    text_view->on_changed = callback;
    text_view->changed_data = user_data;
    
    if (callback != NULL) {
        g_signal_connect(text_view->buffer, "changed",
                         G_CALLBACK(gtk_buffer_changed_callback), text_view);
    }
}

EgWidget *eg_text_view_as_widget(EgTextView *text_view) {
    return (EgWidget *)text_view;
}

void *eg_text_view_get_native(EgTextView *text_view) {
    if (text_view == NULL) return NULL;
    return text_view->base.native;
}
