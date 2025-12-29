/**
 * EasyGTK - Spinner Widget
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/spinner.h>

/* Funções da vtable */
static void spinner_destroy(EgWidget *widget);
static void *spinner_get_native(EgWidget *widget);
static void spinner_set_visible(EgWidget *widget, bool visible);
static bool spinner_get_visible(EgWidget *widget);
static void spinner_set_sensitive(EgWidget *widget, bool sensitive);
static bool spinner_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_spinner_vtable = {
    .type = EG_WIDGET_TYPE_SPINNER,
    .type_name = "EgSpinner",
    .destroy = spinner_destroy,
    .get_native = spinner_get_native,
    .set_visible = spinner_set_visible,
    .get_visible = spinner_get_visible,
    .set_sensitive = spinner_set_sensitive,
    .get_sensitive = spinner_get_sensitive
};

static void spinner_destroy(EgWidget *widget) {
    EgSpinner *spinner = (EgSpinner *)widget;
    if (spinner == NULL) return;
    eg_free(spinner);
}

static void *spinner_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void spinner_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool spinner_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void spinner_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool spinner_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgSpinner *eg_spinner_new(void) {
    EgSpinner *spinner = EG_ALLOC(EgSpinner);
    if (spinner == NULL) return NULL;
    
    GtkWidget *gtk_spinner = gtk_spinner_new();
    if (gtk_spinner == NULL) {
        eg_free(spinner);
        return NULL;
    }
    
    eg_widget_init(&spinner->base, EG_WIDGET_TYPE_SPINNER, gtk_spinner, &eg_spinner_vtable);
    
    return spinner;
}

void eg_spinner_free(EgSpinner *spinner) {
    spinner_destroy((EgWidget *)spinner);
}

void eg_spinner_start(EgSpinner *spinner) {
    if (spinner == NULL || spinner->base.native == NULL) return;
    gtk_spinner_start(GTK_SPINNER(spinner->base.native));
}

void eg_spinner_stop(EgSpinner *spinner) {
    if (spinner == NULL || spinner->base.native == NULL) return;
    gtk_spinner_stop(GTK_SPINNER(spinner->base.native));
}

void eg_spinner_set_spinning(EgSpinner *spinner, bool spinning) {
    if (spinner == NULL || spinner->base.native == NULL) return;
    gtk_spinner_set_spinning(GTK_SPINNER(spinner->base.native), spinning);
}

bool eg_spinner_get_spinning(EgSpinner *spinner) {
    if (spinner == NULL || spinner->base.native == NULL) return false;
    return gtk_spinner_get_spinning(GTK_SPINNER(spinner->base.native));
}

EgWidget *eg_spinner_as_widget(EgSpinner *spinner) {
    return (EgWidget *)spinner;
}

void *eg_spinner_get_native(EgSpinner *spinner) {
    if (spinner == NULL) return NULL;
    return spinner->base.native;
}
