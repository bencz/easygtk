/**
 * SPA App - Users Management Page (MVVM)
 *
 * View layer using MVVM pattern.
 * All logic is in UsersViewModel - this file only handles UI.
 *
 * Note: Dialog uses manual data transfer instead of binding because
 * bindings persist after widget destruction, causing crashes.
 */

#include "../include/pages.h"
#include "../include/app_state.h"
#include "../include/users_viewmodel.h"
#include <easygtk/column_view.h>
#include <stdio.h>
#include <string.h>

/* ViewModel instance */
static UsersViewModel *users_vm = NULL;

/* View widgets */
static EgColumnView *users_table = NULL;
static EgEntry *search_entry = NULL;
static EgWindow *edit_dialog = NULL;

/* Dialog widgets (no binding - manual sync) */
static EgEntry *dialog_username_entry = NULL;
static EgEntry *dialog_email_entry = NULL;
static EgEntry *dialog_password_entry = NULL;
static EgCheckButton *dialog_active_check = NULL;

/* Forward declarations */
static void refresh_table_from_viewmodel(void);
static void show_edit_dialog(void);
static void close_edit_dialog(void);

/* Callback: property changed - refresh table */
static void on_user_count_changed(EgProperty *prop, void *user_data) {
    (void)prop;
    (void)user_data;
    refresh_table_from_viewmodel();
}

/* Callback: dialog visibility changed */
static void on_dialog_visible_changed(EgProperty *prop, void *user_data) {
    (void)user_data;
    bool visible = eg_property_get_bool(prop);
    if (visible) {
        show_edit_dialog();
    } else {
        close_edit_dialog();
    }
}

/* Refresh the ColumnView table from ViewModel data */
static void refresh_table_from_viewmodel(void) {
    if (!users_table || !users_vm) return;

    eg_column_view_clear(users_table);

    int count = users_view_model_get_count(users_vm);
    for (int i = 0; i < count; i++) {
        User *user = users_view_model_get_user_at_row(users_vm, i);
        if (user) {
            char id_str[16];
            snprintf(id_str, sizeof(id_str), "%d", user->id);
            const char *status = user->is_active ? "Active" : "Inactive";

            eg_column_view_append_rowv(users_table, id_str, user->username,
                                        user->email, status, NULL);
            db_user_free(user);
        }
    }
}

/* Dialog Save button callback - sync widget values to ViewModel then save */
static void on_dialog_save_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    if (!users_vm || !dialog_username_entry) return;

    EgViewModel *vm = users_view_model_get_base(users_vm);

    /* Sync widget values to ViewModel properties */
    eg_view_model_set_string(vm, USERS_VM_PROP_DIALOG_USERNAME,
                              eg_entry_get_text(dialog_username_entry));
    eg_view_model_set_string(vm, USERS_VM_PROP_DIALOG_EMAIL,
                              eg_entry_get_text(dialog_email_entry));
    eg_view_model_set_string(vm, USERS_VM_PROP_DIALOG_PASSWORD,
                              eg_entry_get_text(dialog_password_entry));
    eg_view_model_set_bool(vm, USERS_VM_PROP_DIALOG_ACTIVE,
                            eg_check_button_get_active(dialog_active_check));

    /* Execute save command */
    bool success = users_view_model_save(users_vm);

    if (success) {
        eg_dialog_info(g_app_state->main_window,
            users_view_model_is_editing(users_vm) ? "User Updated" : "User Created",
            users_view_model_is_editing(users_vm) ?
                "User information was updated." : "New user was created successfully.");
    } else {
        eg_dialog_error(g_app_state->main_window, "Error",
            users_view_model_is_editing(users_vm) ?
                "Failed to update user." : "Failed to create user. Username may already exist.");
    }
}

/* Dialog Cancel button callback */
static void on_dialog_cancel_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    users_view_model_cancel_edit(users_vm);
}

/* Show the add/edit dialog */
static void show_edit_dialog(void) {
    if (edit_dialog) return; /* Already open */

    EgViewModel *vm = users_view_model_get_base(users_vm);
    bool is_edit = eg_view_model_get_bool(vm, USERS_VM_PROP_DIALOG_IS_EDIT);

    edit_dialog = eg_window_new(g_app_state->app,
                                 is_edit ? "Edit User" : "Add User",
                                 400, 300);
    if (!edit_dialog) return;

    eg_window_set_resizable(edit_dialog, false);
    eg_window_set_transient_for(edit_dialog, g_app_state->main_window);
    eg_window_set_modal(edit_dialog, true);
    eg_window_set_destroy_on_close(edit_dialog, true);

    EgBox *content = eg_box_new_vertical(16);
    eg_widget_set_margin(eg_box_as_widget(content), 24);

    /* Title */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, is_edit ? "<b>Edit User</b>" : "<b>Add New User</b>");
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

    dialog_username_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(dialog_username_entry), true);
    /* Manual sync from ViewModel (no binding) */
    const char *username = eg_view_model_get_string(vm, USERS_VM_PROP_DIALOG_USERNAME);
    if (username) eg_entry_set_text(dialog_username_entry, username);
    if (is_edit) {
        eg_widget_set_sensitive(eg_entry_as_widget(dialog_username_entry), false);
    }
    eg_grid_attach(form, eg_entry_as_widget(dialog_username_entry), 1, 0, 1, 1);

    /* Email */
    EgLabel *email_label = eg_label_new("Email:");
    eg_label_set_xalign(email_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(email_label), 0, 1, 1, 1);

    dialog_email_entry = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(dialog_email_entry), true);
    const char *email = eg_view_model_get_string(vm, USERS_VM_PROP_DIALOG_EMAIL);
    if (email) eg_entry_set_text(dialog_email_entry, email);
    eg_grid_attach(form, eg_entry_as_widget(dialog_email_entry), 1, 1, 1, 1);

    /* Password */
    EgLabel *password_label = eg_label_new(is_edit ? "New Password:" : "Password:");
    eg_label_set_xalign(password_label, 1.0f);
    eg_grid_attach(form, eg_label_as_widget(password_label), 0, 2, 1, 1);

    dialog_password_entry = eg_entry_new_with_placeholder(
        is_edit ? "(leave empty to keep)" : "Enter password");
    eg_entry_set_visibility(dialog_password_entry, false);
    eg_widget_set_hexpand(eg_entry_as_widget(dialog_password_entry), true);
    eg_grid_attach(form, eg_entry_as_widget(dialog_password_entry), 1, 2, 1, 1);

    /* Active checkbox */
    dialog_active_check = eg_check_button_new("User is active");
    eg_check_button_set_active(dialog_active_check,
                                eg_view_model_get_bool(vm, USERS_VM_PROP_DIALOG_ACTIVE));
    eg_grid_attach(form, eg_check_button_as_widget(dialog_active_check), 1, 3, 1, 1);

    eg_box_append(content, eg_grid_as_widget(form));

    /* Buttons */
    EgBox *buttons = eg_box_new_horizontal(12);
    eg_widget_set_halign(eg_box_as_widget(buttons), EG_ALIGN_END);
    eg_widget_set_margin(eg_box_as_widget(buttons), 8);

    EgButton *cancel_btn = eg_button_new("Cancel");
    eg_button_on_click(cancel_btn, on_dialog_cancel_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(cancel_btn));

    EgButton *save_btn = eg_button_new(is_edit ? "Save Changes" : "Create User");
    eg_widget_add_css_class(eg_button_as_widget(save_btn), "btn-primary");
    eg_button_on_click(save_btn, on_dialog_save_clicked, NULL);
    eg_box_append(buttons, eg_button_as_widget(save_btn));

    eg_box_append(content, eg_box_as_widget(buttons));

    eg_window_set_child(edit_dialog, eg_box_as_widget(content));
    eg_window_show(edit_dialog);
}

/* Close the edit dialog */
static void close_edit_dialog(void) {
    if (edit_dialog) {
        eg_window_close(edit_dialog);
        edit_dialog = NULL;
        dialog_username_entry = NULL;
        dialog_email_entry = NULL;
        dialog_password_entry = NULL;
        dialog_active_check = NULL;
    }
}

/* Callback: search text changed */
static void on_search_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgEntry *entry = (EgEntry *)widget;
    const char *text = eg_entry_get_text(entry);
    users_view_model_set_search(users_vm, text);
}

/* Callback: table selection changed - update ViewModel */
static void on_table_selection_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgColumnView *cv = (EgColumnView *)widget;
    int selected = eg_column_view_get_selected_row(cv);
    users_view_model_set_selected_row(users_vm, selected);
}

/* Callback: row double-clicked - begin edit */
static void on_row_activated(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    users_view_model_begin_edit(users_vm);
}

/* Callback: Add button clicked */
static void on_add_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;
    EgCommand *cmd = eg_view_model_get_command(users_view_model_get_base(users_vm), USERS_VM_CMD_ADD);
    if (cmd) eg_command_execute(cmd, NULL);
}

/* Callback: Edit button clicked */
static void on_edit_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    int selected = users_view_model_get_selected_row(users_vm);
    if (selected < 0) {
        eg_dialog_warning(g_app_state->main_window, "No Selection",
            "Please select a user from the table first.");
        return;
    }

    EgCommand *cmd = eg_view_model_get_command(users_view_model_get_base(users_vm), USERS_VM_CMD_EDIT);
    if (cmd) eg_command_execute(cmd, NULL);
}

/* Delete confirmation callback */
static void on_delete_confirm(EgDialogResponse response, void *user_data) {
    (void)user_data;
    if (response == EG_RESPONSE_YES) {
        EgCommand *cmd = eg_view_model_get_command(users_view_model_get_base(users_vm), USERS_VM_CMD_DELETE);
        if (cmd) eg_command_execute(cmd, NULL);
    }
}

/* Callback: Delete button clicked */
static void on_delete_clicked(EgWidget *widget, void *user_data) {
    (void)widget;
    (void)user_data;

    int selected = users_view_model_get_selected_row(users_vm);
    if (selected < 0) {
        eg_dialog_warning(g_app_state->main_window, "No Selection",
            "Please select a user from the table first.");
        return;
    }

    if (!users_view_model_can_delete(users_vm)) {
        eg_dialog_error(g_app_state->main_window, "Cannot Delete",
            "The admin user cannot be deleted.");
        return;
    }

    eg_dialog_question(g_app_state->main_window, "Confirm Delete",
        "Are you sure you want to delete this user?",
        on_delete_confirm, NULL);
}

/* Public: update page */
void page_users_update(void) {
    if (users_vm) {
        users_view_model_refresh(users_vm);
    }
}

/* Public: create page */
EgWidget *page_users_create(void) {
    /* Create ViewModel */
    users_vm = users_view_model_new();
    if (!users_vm) {
        fprintf(stderr, "Failed to create UsersViewModel\n");
        return NULL;
    }

    EgViewModel *vm = users_view_model_get_base(users_vm);

    /* Subscribe to property changes */
    EgProperty *count_prop = eg_view_model_get_property(vm, USERS_VM_PROP_USER_COUNT);
    if (count_prop) {
        eg_property_on_changed(count_prop, on_user_count_changed, NULL);
    }

    EgProperty *dialog_visible_prop = eg_view_model_get_property(vm, USERS_VM_PROP_DIALOG_VISIBLE);
    if (dialog_visible_prop) {
        eg_property_on_changed(dialog_visible_prop, on_dialog_visible_changed, NULL);
    }

    /* Build View */
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
    eg_button_on_click(add_btn, on_add_clicked, NULL);
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

    /* Hint label */
    EgLabel *hint = eg_label_new("Tip: Click on column headers to sort. Double-click a row to edit.");
    eg_widget_add_css_class(eg_label_as_widget(hint), "dim-label");
    eg_label_set_xalign(hint, 0.0f);
    eg_box_append(page, eg_label_as_widget(hint));

    /* Create ColumnView table */
    users_table = eg_column_view_new(EG_SELECTION_SINGLE);

    /* Add columns */
    int id_col = eg_column_view_add_column(users_table, "ID", 0);
    int name_col = eg_column_view_add_column(users_table, "Username", 1);
    int email_col = eg_column_view_add_column(users_table, "Email", 2);
    int status_col = eg_column_view_add_column(users_table, "Status", 3);

    /* Make columns sortable */
    eg_column_view_set_column_sortable(users_table, id_col, true);
    eg_column_view_set_column_sortable(users_table, name_col, true);
    eg_column_view_set_column_sortable(users_table, email_col, true);
    eg_column_view_set_column_sortable(users_table, status_col, true);

    /* Set all columns to expand */
    eg_column_view_set_column_expand(users_table, id_col, true);
    eg_column_view_set_column_expand(users_table, name_col, true);
    eg_column_view_set_column_expand(users_table, email_col, true);
    eg_column_view_set_column_expand(users_table, status_col, true);

    /* Show row separators only */
    eg_column_view_set_show_row_separators(users_table, true);
    eg_column_view_set_show_column_separators(users_table, false);

    /* Set callbacks */
    eg_column_view_on_selection_changed(users_table, on_table_selection_changed, NULL);
    eg_column_view_on_activate(users_table, on_row_activated, NULL);

    /* Put table in scrolled window */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_column_view_as_widget(users_table));
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);
    eg_scrolled_window_set_min_content_size(scroll, -1, 300);

    eg_box_append(page, eg_scrolled_window_as_widget(scroll));

    /* Action buttons */
    EgBox *action_box = eg_box_new_horizontal(12);
    eg_widget_set_halign(eg_box_as_widget(action_box), EG_ALIGN_END);

    EgButton *edit_btn = eg_button_new("Edit Selected");
    eg_widget_add_css_class(eg_button_as_widget(edit_btn), "btn-primary");
    eg_button_on_click(edit_btn, on_edit_clicked, NULL);
    eg_box_append(action_box, eg_button_as_widget(edit_btn));

    EgButton *delete_btn = eg_button_new("Delete Selected");
    eg_widget_add_css_class(eg_button_as_widget(delete_btn), "btn-danger");
    eg_button_on_click(delete_btn, on_delete_clicked, NULL);
    eg_box_append(action_box, eg_button_as_widget(delete_btn));

    eg_box_append(page, eg_box_as_widget(action_box));

    /* Add selected_user_id property to global ViewModel */
    if (!eg_view_model_get_property(g_app_state->view_model, "selected_user_id")) {
        eg_view_model_add_property(g_app_state->view_model,
                                    eg_property_new_int("selected_user_id", -1));
    }

    return eg_box_as_widget(page);
}
