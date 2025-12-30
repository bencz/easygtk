/**
 * SPA App - Users Management Page
 * Full CRUD operations for users
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Page widgets */
static EgBox *users_list_box = NULL;
static EgEntry *search_entry = NULL;

/* Edit dialog widgets (reused) */
static EgWindow *edit_dialog = NULL;
static EgEntry *edit_username_entry = NULL;
static EgEntry *edit_email_entry = NULL;
static EgEntry *edit_password_entry = NULL;
static EgCheckButton *edit_active_check = NULL;
static int editing_user_id = -1; /* -1 = creating new, >0 = editing existing */

/* Forward declarations */
static void refresh_users_list(void);
static void show_user_dialog(User *user);
static void on_add_user_clicked(EgWidget *widget, void *user_data);
static void on_edit_user_clicked(EgWidget *widget, void *user_data);
static void on_delete_user_clicked(EgWidget *widget, void *user_data);
static void on_save_user_clicked(EgWidget *widget, void *user_data);
static void on_cancel_dialog_clicked(EgWidget *widget, void *user_data);
static void on_search_changed(EgWidget *widget, void *user_data);

/* Create a user list item row */
static EgWidget *create_user_row(User *user) {
    EgBox *row = eg_box_new_horizontal(12);
    eg_widget_add_css_class(eg_box_as_widget(row), "list-item");
    eg_widget_set_hexpand(eg_box_as_widget(row), true);

    /* User info */
    EgBox *info_box = eg_box_new_vertical(4);
    eg_widget_set_hexpand(eg_box_as_widget(info_box), true);

    char name_markup[256];
    snprintf(name_markup, sizeof(name_markup), "<b>%s</b>", user->username);
    EgLabel *name_label = eg_label_new("");
    eg_label_set_markup(name_label, name_markup);
    eg_label_set_xalign(name_label, 0.0f);
    eg_box_append(info_box, eg_label_as_widget(name_label));

    EgLabel *email_label = eg_label_new(user->email);
    eg_widget_add_css_class(eg_label_as_widget(email_label), "text-muted");
    eg_label_set_xalign(email_label, 0.0f);
    eg_box_append(info_box, eg_label_as_widget(email_label));

    eg_box_append(row, eg_box_as_widget(info_box));

    /* Status badge */
    EgLabel *status_label = eg_label_new(user->is_active ? "Active" : "Inactive");
    eg_widget_add_css_class(eg_label_as_widget(status_label),
        user->is_active ? "badge-success" : "badge-danger");
    eg_widget_set_valign(eg_label_as_widget(status_label), EG_ALIGN_CENTER);
    eg_box_append(row, eg_label_as_widget(status_label));

    /* Action buttons */
    EgBox *actions_box = eg_box_new_horizontal(8);
    eg_widget_set_valign(eg_box_as_widget(actions_box), EG_ALIGN_CENTER);

    EgButton *edit_btn = eg_button_new("Edit");
    eg_widget_add_css_class(eg_button_as_widget(edit_btn), "btn-small");
    /* Store user_id in user_data - allocate memory for it */
    int *user_id = malloc(sizeof(int));
    *user_id = user->id;
    eg_button_on_click(edit_btn, on_edit_user_clicked, user_id);
    eg_box_append(actions_box, eg_button_as_widget(edit_btn));

    /* Don't allow deleting admin user */
    if (strcmp(user->username, "admin") != 0) {
        EgButton *delete_btn = eg_button_new("Delete");
        eg_widget_add_css_class(eg_button_as_widget(delete_btn), "btn-small");
        eg_widget_add_css_class(eg_button_as_widget(delete_btn), "btn-danger");
        int *del_user_id = malloc(sizeof(int));
        *del_user_id = user->id;
        eg_button_on_click(delete_btn, on_delete_user_clicked, del_user_id);
        eg_box_append(actions_box, eg_button_as_widget(delete_btn));
    }

    eg_box_append(row, eg_box_as_widget(actions_box));

    return eg_box_as_widget(row);
}

/* Refresh users list from database */
static void refresh_users_list(void) {
    if (!users_list_box) return;

    eg_box_clear(users_list_box);

    int count = 0;
    User **users = db_user_list_all(&count);

    /* Filter by search if text entered */
    const char *search_text = search_entry ? eg_entry_get_text(search_entry) : "";

    for (int i = 0; i < count; i++) {
        /* Simple search filter */
        if (search_text && search_text[0] != '\0') {
            if (strstr(users[i]->username, search_text) == NULL &&
                strstr(users[i]->email, search_text) == NULL) {
                continue;
            }
        }

        EgWidget *row = create_user_row(users[i]);
        eg_box_append(users_list_box, row);
    }

    db_user_list_free(users, count);

    /* Update stats in dashboard */
    eg_view_model_set_int(g_app_state->view_model, "total_users", db_stats_total_users());
}

/* Show add/edit user dialog */
static void show_user_dialog(User *user) {
    /* Create dialog window using app reference */
    edit_dialog = eg_window_new(g_app_state->app, user ? "Edit User" : "Add User", 400, 300);
    if (!edit_dialog) {
        fprintf(stderr, "Failed to create dialog window\n");
        return;
    }
    eg_window_set_resizable(edit_dialog, false);

    EgBox *content = eg_box_new_vertical(16);
    eg_widget_set_margin(eg_box_as_widget(content), 24);

    /* Title */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, user ? "<b>Edit User</b>" : "<b>Add New User</b>");
    eg_widget_add_css_class(eg_label_as_widget(title), "title-3");
    eg_box_append(content, eg_label_as_widget(title));

    /* Form grid */
    EgGrid *form = eg_grid_new();
    eg_grid_set_row_spacing(form, 12);
    eg_grid_set_column_spacing(form, 12);

    /* Username */
    EgLabel *username_label = eg_label_new("Username:");
    eg_label_set_xalign(username_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(username_label), 0, 0, 1, 1);

    edit_username_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(edit_username_entry), true);
    if (user) {
        eg_entry_set_text(edit_username_entry, user->username);
        eg_widget_set_sensitive(eg_entry_as_widget(edit_username_entry), false);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_username_entry), 1, 0, 1, 1);

    /* Email */
    EgLabel *email_label = eg_label_new("Email:");
    eg_label_set_xalign(email_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(email_label), 0, 1, 1, 1);

    edit_email_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(edit_email_entry), true);
    if (user) {
        eg_entry_set_text(edit_email_entry, user->email);
    }
    eg_grid_attach(form, eg_entry_as_widget(edit_email_entry), 1, 1, 1, 1);

    /* Password (only for new users or if changing) */
    EgLabel *password_label = eg_label_new(user ? "New Password:" : "Password:");
    eg_label_set_xalign(password_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(password_label), 0, 2, 1, 1);

    edit_password_entry = eg_entry_new_with_placeholder(user ? "(leave empty to keep)" : "Enter password");
    eg_entry_set_visibility(edit_password_entry, false);
    eg_widget_set_hexpand(eg_entry_as_widget(edit_password_entry), true);
    eg_grid_attach(form, eg_entry_as_widget(edit_password_entry), 1, 2, 1, 1);

    /* Active checkbox */
    edit_active_check = eg_check_button_new("User is active");
    eg_check_button_set_active(edit_active_check, user ? user->is_active : true);
    eg_grid_attach(form, eg_check_button_as_widget(edit_active_check), 1, 3, 1, 1);

    eg_box_append(content, eg_grid_as_widget(form));

    /* Buttons */
    EgBox *buttons = eg_box_new_horizontal(12);
    eg_widget_set_halign(eg_box_as_widget(buttons), EG_ALIGN_END);
    eg_widget_set_margin(eg_box_as_widget(buttons), 8);

    EgButton *cancel_btn = eg_button_new("Cancel");
    eg_button_on_click(cancel_btn, on_cancel_dialog_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(cancel_btn));

    EgButton *save_btn = eg_button_new(user ? "Save Changes" : "Create User");
    eg_widget_add_css_class(eg_button_as_widget(save_btn), "btn-primary");
    eg_button_on_click(save_btn, on_save_user_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(save_btn));

    eg_box_append(content, eg_box_as_widget(buttons));

    /* Store editing user id */
    editing_user_id = user ? user->id : -1;

    eg_window_set_child(edit_dialog, eg_box_as_widget(content));
    eg_window_show(edit_dialog);
}

/* Callbacks */
static void on_add_user_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    show_user_dialog(NULL);
}

static void on_edit_user_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    int user_id = *(int *)user_data;
    User *user = db_user_get_by_id(user_id);
    if (user) {
        show_user_dialog(user);
        db_user_free(user);
    }
}

static void on_delete_confirm(EgDialogResponse response, void *user_data) {
    if (response == EG_RESPONSE_YES) {
        int user_id = *(int *)user_data;
        if (db_user_delete(user_id)) {
            refresh_users_list();
        }
    }
    free(user_data);
}

static void on_delete_user_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    int *user_id = (int *)user_data;

    /* Need to pass user_id to callback, allocate new copy */
    int *id_copy = malloc(sizeof(int));
    *id_copy = *user_id;

    eg_dialog_question(NULL, "Confirm Delete",
        "Are you sure you want to delete this user?",
        on_delete_confirm, id_copy);
}

static void on_save_user_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    const char *username = eg_entry_get_text(edit_username_entry);
    const char *email = eg_entry_get_text(edit_email_entry);
    const char *password = eg_entry_get_text(edit_password_entry);
    bool is_active = eg_check_button_get_active(edit_active_check);

    bool success = false;

    if (editing_user_id < 0) {
        /* Creating new user */
        if (username[0] != '\0' && email[0] != '\0' && password[0] != '\0') {
            success = db_user_create(username, email, password);
        }
    } else {
        /* Updating existing user */
        if (email[0] != '\0') {
            success = db_user_update(editing_user_id, email, is_active);
            /* Handle password change if provided */
            if (success && password[0] != '\0') {
                db_user_change_password(editing_user_id, password);
            }
        }
    }

    if (success) {
        eg_window_close(edit_dialog);
        edit_dialog = NULL;
        refresh_users_list();

        /* Show success message */
        eg_dialog_info(g_app_state->main_window,
            editing_user_id < 0 ? "User Created" : "User Updated",
            editing_user_id < 0 ? "New user was created successfully." : "User information was updated.");
    } else {
        /* Show error message */
        eg_dialog_error(g_app_state->main_window, "Error",
            editing_user_id < 0 ? "Failed to create user. Username may already exist." : "Failed to update user.");
    }
}

static void on_cancel_dialog_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    if (edit_dialog) {
        eg_window_close(edit_dialog);
        edit_dialog = NULL;
    }
}

static void on_search_changed(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    refresh_users_list();
}

/* Public functions */
void page_users_update(void) {
    refresh_users_list();
}

EgWidget *page_users_create(void) {
    EgBox *page = eg_box_new_vertical(16);
    eg_widget_add_css_class(eg_box_as_widget(page), "page-content");
    eg_widget_set_margin(eg_box_as_widget(page), 16);

    /* Header with title and add button */
    EgBox *header = eg_box_new_horizontal(12);
    eg_widget_set_hexpand(eg_box_as_widget(header), true);

    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='large' weight='bold'>Users Management</span>");
    eg_widget_set_hexpand(eg_label_as_widget(title), true);
    eg_label_set_xalign(title, 0.0f);
    eg_box_append(header, eg_label_as_widget(title));

    EgButton *add_btn = eg_button_new("+ Add User");
    eg_widget_add_css_class(eg_button_as_widget(add_btn), "btn-primary");
    eg_button_on_click(add_btn, on_add_user_clicked, NULL);
    eg_box_append(header, eg_button_as_widget(add_btn));

    eg_box_append(page, eg_box_as_widget(header));

    /* Search bar */
    EgBox *search_box = eg_box_new_horizontal(8);

    EgLabel *search_label = eg_label_new("Search:");
    eg_box_append(search_box, eg_label_as_widget(search_label));

    search_entry = eg_entry_new_with_placeholder("Search users...");
    eg_widget_set_hexpand(eg_entry_as_widget(search_entry), true);
    eg_entry_on_changed(search_entry, on_search_changed, NULL);
    eg_box_append(search_box, eg_entry_as_widget(search_entry));

    eg_box_append(page, eg_box_as_widget(search_box));

    /* Users list in scrolled window */
    users_list_box = eg_box_new_vertical(8);

    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(users_list_box));
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_scrolled_window_set_min_content_size(scroll, -1, 300);

    eg_box_append(page, eg_scrolled_window_as_widget(scroll));

    return eg_box_as_widget(page);
}
