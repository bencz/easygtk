/**
 * EasyGTK - PopoverMenu
 */

#include <gtk/gtk.h>
#include "internal/internal.h"
#include <easygtk/popover_menu.h>
#include <string.h>

/* Dados de ação do menu */
typedef struct {
    EgMenuActionCallback callback;
    void *user_data;
    char *action_name;
} MenuActionData;

/* Funções da vtable */
static void popover_menu_destroy(EgWidget *widget);
static void *popover_menu_get_native(EgWidget *widget);
static void popover_menu_set_visible(EgWidget *widget, bool visible);
static bool popover_menu_get_visible(EgWidget *widget);
static void popover_menu_set_sensitive(EgWidget *widget, bool sensitive);
static bool popover_menu_get_sensitive(EgWidget *widget);

const EgWidgetVTable eg_popover_menu_vtable = {
    .type = EG_WIDGET_TYPE_POPOVER_MENU,
    .type_name = "EgPopoverMenu",
    .destroy = popover_menu_destroy,
    .get_native = popover_menu_get_native,
    .set_visible = popover_menu_set_visible,
    .get_visible = popover_menu_get_visible,
    .set_sensitive = popover_menu_set_sensitive,
    .get_sensitive = popover_menu_get_sensitive
};

static void popover_menu_destroy(EgWidget *widget) {
    EgPopoverMenu *menu = (EgPopoverMenu *)widget;
    if (menu == NULL) return;

    if (menu->menu) {
        g_object_unref(menu->menu);
    }
    if (menu->action_group) {
        g_object_unref(menu->action_group);
    }
    eg_free(menu);
}

static void *popover_menu_get_native(EgWidget *widget) {
    if (widget == NULL) return NULL;
    return widget->native;
}

static void popover_menu_set_visible(EgWidget *widget, bool visible) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_visible(widget->native, visible);
}

static bool popover_menu_get_visible(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_visible(widget->native);
}

static void popover_menu_set_sensitive(EgWidget *widget, bool sensitive) {
    if (widget == NULL || widget->native == NULL) return;
    gtk_widget_set_sensitive(widget->native, sensitive);
}

static bool popover_menu_get_sensitive(EgWidget *widget) {
    if (widget == NULL || widget->native == NULL) return false;
    return gtk_widget_get_sensitive(widget->native);
}

/* Callback para ações */
static void action_callback(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    (void)action;
    (void)parameter;
    MenuActionData *data = (MenuActionData *)user_data;
    if (data && data->callback) {
        data->callback(data->action_name, data->user_data);
    }
}

/* Libera dados da ação - assinatura compatível com GClosureNotify */
static void action_data_free(gpointer data, GClosure *closure) {
    (void)closure;
    MenuActionData *action_data = (MenuActionData *)data;
    if (action_data) {
        eg_free(action_data->action_name);
        eg_free(action_data);
    }
}

EgPopoverMenu *eg_popover_menu_new(void) {
    EgPopoverMenu *menu = EG_ALLOC(EgPopoverMenu);
    if (menu == NULL) return NULL;

    /* Cria o GMenu */
    menu->menu = g_menu_new();
    if (menu->menu == NULL) {
        eg_free(menu);
        return NULL;
    }

    /* Cria o grupo de ações */
    menu->action_group = g_simple_action_group_new();

    /* Cria o popover a partir do menu */
    GtkWidget *popover = gtk_popover_menu_new_from_model(G_MENU_MODEL(menu->menu));
    if (popover == NULL) {
        g_object_unref(menu->menu);
        g_object_unref(menu->action_group);
        eg_free(menu);
        return NULL;
    }

    eg_widget_init(&menu->base, EG_WIDGET_TYPE_POPOVER_MENU, popover, &eg_popover_menu_vtable);

    /* Insere o grupo de ações no popover */
    gtk_widget_insert_action_group(popover, "menu", G_ACTION_GROUP(menu->action_group));

    return menu;
}

void eg_popover_menu_free(EgPopoverMenu *menu) {
    popover_menu_destroy((EgWidget *)menu);
}

void eg_popover_menu_add_item(EgPopoverMenu *menu, const char *label,
                               const char *action_name, EgMenuActionCallback callback,
                               void *user_data) {
    if (menu == NULL || label == NULL || action_name == NULL) return;

    /* Cria os dados da ação */
    MenuActionData *data = EG_ALLOC(MenuActionData);
    if (data == NULL) return;

    data->callback = callback;
    data->user_data = user_data;
    data->action_name = eg_strdup(action_name);

    /* Cria a ação */
    GSimpleAction *action = g_simple_action_new(action_name, NULL);
    g_signal_connect_data(action, "activate", G_CALLBACK(action_callback),
                          data, (GClosureNotify)action_data_free, 0);

    /* Adiciona ao grupo */
    g_action_map_add_action(G_ACTION_MAP(menu->action_group), G_ACTION(action));
    g_object_unref(action);

    /* Cria o item de menu com a ação prefixada */
    char *detailed_action = g_strdup_printf("menu.%s", action_name);
    g_menu_append(menu->menu, label, detailed_action);
    g_free(detailed_action);
}

void eg_popover_menu_add_item_with_icon(EgPopoverMenu *menu, const char *label,
                                         const char *icon_name, const char *action_name,
                                         EgMenuActionCallback callback, void *user_data) {
    if (menu == NULL || label == NULL || action_name == NULL) return;

    /* Cria os dados da ação */
    MenuActionData *data = EG_ALLOC(MenuActionData);
    if (data == NULL) return;

    data->callback = callback;
    data->user_data = user_data;
    data->action_name = eg_strdup(action_name);

    /* Cria a ação */
    GSimpleAction *action = g_simple_action_new(action_name, NULL);
    g_signal_connect_data(action, "activate", G_CALLBACK(action_callback),
                          data, (GClosureNotify)action_data_free, 0);

    /* Adiciona ao grupo */
    g_action_map_add_action(G_ACTION_MAP(menu->action_group), G_ACTION(action));
    g_object_unref(action);

    /* Cria o item de menu com ícone */
    char *detailed_action = g_strdup_printf("menu.%s", action_name);
    GMenuItem *item = g_menu_item_new(label, detailed_action);

    if (icon_name) {
        GIcon *icon = g_themed_icon_new(icon_name);
        g_menu_item_set_icon(item, icon);
        g_object_unref(icon);
    }

    g_menu_append_item(menu->menu, item);
    g_object_unref(item);
    g_free(detailed_action);
}

void eg_popover_menu_add_separator(EgPopoverMenu *menu) {
    if (menu == NULL) return;

    /* No GMenu, separadores são implementados como seções vazias */
    GMenu *section = g_menu_new();
    g_menu_append_section(menu->menu, NULL, G_MENU_MODEL(section));
    g_object_unref(section);
}

void eg_popover_menu_add_section(EgPopoverMenu *menu, const char *title) {
    if (menu == NULL) return;

    /* Cria uma nova seção com título opcional */
    GMenu *section = g_menu_new();
    g_menu_append_section(menu->menu, title, G_MENU_MODEL(section));
    g_object_unref(section);
}

void eg_popover_menu_set_item_enabled(EgPopoverMenu *menu, const char *action_name,
                                       bool enabled) {
    if (menu == NULL || action_name == NULL) return;

    GAction *action = g_action_map_lookup_action(G_ACTION_MAP(menu->action_group), action_name);
    if (action && G_IS_SIMPLE_ACTION(action)) {
        g_simple_action_set_enabled(G_SIMPLE_ACTION(action), enabled);
    }
}

void eg_popover_menu_popup(EgPopoverMenu *menu) {
    if (menu == NULL || menu->base.native == NULL) return;
    gtk_popover_popup(GTK_POPOVER(menu->base.native));
}

void eg_popover_menu_popdown(EgPopoverMenu *menu) {
    if (menu == NULL || menu->base.native == NULL) return;
    gtk_popover_popdown(GTK_POPOVER(menu->base.native));
}

EgWidget *eg_popover_menu_as_widget(EgPopoverMenu *menu) {
    return (EgWidget *)menu;
}

void *eg_popover_menu_get_native(EgPopoverMenu *menu) {
    if (menu == NULL) return NULL;
    return menu->base.native;
}
