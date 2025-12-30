/**
 * EasyGTK - MenuButton
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/menu_button.h>
#include <easygtk/popover_menu.h>

/* Funções da vtable */
static void menu_button_destroy(EgWidget *widget);
static void *menu_button_get_native(EgWidget *widget);
static void menu_button_set_visible(EgWidget *widget, bool visible);
static bool menu_button_get_visible(EgWidget *widget);
static void menu_button_set_sensitive(EgWidget *widget, bool sensitive);
static bool menu_button_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_menu_button_vtable = {
    .type = EG_WIDGET_TYPE_MENU_BUTTON,
    .type_name = "EgMenuButton",
    .destroy = menu_button_destroy,
    .get_native = menu_button_get_native,
    .set_visible = menu_button_set_visible,
    .get_visible = menu_button_get_visible,
    .set_sensitive = menu_button_set_sensitive,
    .get_sensitive = menu_button_get_sensitive
};

static void menu_button_destroy(EgWidget *widget) {
    EgMenuButton *button = (EgMenuButton *)widget;
    if (button == NULL) return;
    eg_free(button);
}

static void *menu_button_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void menu_button_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool menu_button_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void menu_button_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool menu_button_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

EgMenuButton *eg_menu_button_new(void) {
    EgMenuButton *button = EG_ALLOC(EgMenuButton);
    if (button == NULL) return NULL;

    GtkWidget *gtk_button = gtk_menu_button_new();
    if (gtk_button == NULL) {
        eg_free(button);
        return NULL;
    }

    eg_widget_init(&button->base, EG_WIDGET_TYPE_MENU_BUTTON, gtk_button, &eg_menu_button_vtable);

    return button;
}

EgMenuButton *eg_menu_button_new_with_label(const char *label) {
    EgMenuButton *button = eg_menu_button_new();
    if (button == NULL) return NULL;

    if (label) {
        gtk_menu_button_set_label(GTK_MENU_BUTTON(button->base.native), label);
    }

    return button;
}

EgMenuButton *eg_menu_button_new_with_icon(const char *icon_name) {
    EgMenuButton *button = eg_menu_button_new();
    if (button == NULL) return NULL;

    if (icon_name) {
        gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(button->base.native), icon_name);
    }

    return button;
}

void eg_menu_button_free(EgMenuButton *button) {
    menu_button_destroy((EgWidget *)button);
}

void eg_menu_button_set_popover(EgMenuButton *button, EgPopoverMenu *menu) {
    if (button == NULL || button->base.native == NULL) return;

    if (menu == NULL) {
        gtk_menu_button_set_popover(GTK_MENU_BUTTON(button->base.native), NULL);
    } else {
        GtkWidget *popover = (GtkWidget *)eg_popover_menu_get_native(menu);
        gtk_menu_button_set_popover(GTK_MENU_BUTTON(button->base.native), popover);
    }
}

EgPopoverMenu *eg_menu_button_get_popover(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return NULL;

    /* Nota: Não podemos recuperar o EgPopoverMenu original do GTK
     * O usuário deve manter sua própria referência */
    return NULL;
}

void eg_menu_button_set_label(EgMenuButton *button, const char *label) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_menu_button_set_label(GTK_MENU_BUTTON(button->base.native), label);
}

const char *eg_menu_button_get_label(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return NULL;
    return gtk_menu_button_get_label(GTK_MENU_BUTTON(button->base.native));
}

void eg_menu_button_set_icon_name(EgMenuButton *button, const char *icon_name) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(button->base.native), icon_name);
}

const char *eg_menu_button_get_icon_name(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return NULL;
    return gtk_menu_button_get_icon_name(GTK_MENU_BUTTON(button->base.native));
}

void eg_menu_button_set_direction(EgMenuButton *button, int direction) {
    if (button == NULL || button->base.native == NULL) return;

    GtkArrowType arrow_type;
    switch (direction) {
        case 0: arrow_type = GTK_ARROW_UP; break;
        case 1: arrow_type = GTK_ARROW_DOWN; break;
        case 2: arrow_type = GTK_ARROW_LEFT; break;
        case 3: arrow_type = GTK_ARROW_RIGHT; break;
        case 4: arrow_type = GTK_ARROW_NONE; break;
        default: arrow_type = GTK_ARROW_DOWN; break;
    }

    gtk_menu_button_set_direction(GTK_MENU_BUTTON(button->base.native), arrow_type);
}

void eg_menu_button_set_has_frame(EgMenuButton *button, bool has_frame) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_menu_button_set_has_frame(GTK_MENU_BUTTON(button->base.native), has_frame);
}

bool eg_menu_button_get_has_frame(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return false;
    return gtk_menu_button_get_has_frame(GTK_MENU_BUTTON(button->base.native));
}

void eg_menu_button_popup(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_menu_button_popup(GTK_MENU_BUTTON(button->base.native));
}

void eg_menu_button_popdown(EgMenuButton *button) {
    if (button == NULL || button->base.native == NULL) return;
    gtk_menu_button_popdown(GTK_MENU_BUTTON(button->base.native));
}

EgWidget *eg_menu_button_as_widget(EgMenuButton *button) {
    return (EgWidget *)button;
}

void *eg_menu_button_get_native(EgMenuButton *button) {
    if (button == NULL) return NULL;
    return button->base.native;
}
