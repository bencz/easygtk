/**
 * SPA App - Users ViewModel Implementation
 *
 * MVVM ViewModel for user management.
 * All database operations are encapsulated here.
 */

#include "../include/users_viewmodel.h"
#include "../include/app_state.h"
#include <stdlib.h>
#include <string.h>

/* Internal structure */
struct UsersViewModel {
    EgViewModel *base;

    /* Cached user list */
    User **users;
    int user_count;
    int *row_to_user_id;  /* Maps row index to user ID */

    /* State */
    int editing_user_id;  /* -1 = adding new, >0 = editing existing */
    char search_filter[256];
};

/* Forward declarations for commands */
static void cmd_add_execute(EgCommand *cmd, void *param, void *user_data);
static void cmd_edit_execute(EgCommand *cmd, void *param, void *user_data);
static void cmd_delete_execute(EgCommand *cmd, void *param, void *user_data);
static bool cmd_delete_can_execute(EgCommand *cmd, void *param, void *user_data);
static void cmd_save_execute(EgCommand *cmd, void *param, void *user_data);
static void cmd_cancel_execute(EgCommand *cmd, void *param, void *user_data);
static void cmd_refresh_execute(EgCommand *cmd, void *param, void *user_data);
static bool cmd_has_selection(EgCommand *cmd, void *param, void *user_data);

/* Helper to free cached users */
static void free_cached_users(UsersViewModel *vm) {
    if (vm->users) {
        db_user_list_free(vm->users, vm->user_count);
        vm->users = NULL;
    }
    if (vm->row_to_user_id) {
        free(vm->row_to_user_id);
        vm->row_to_user_id = NULL;
    }
    vm->user_count = 0;
}

UsersViewModel *users_view_model_new(void) {
    UsersViewModel *vm = (UsersViewModel *)calloc(1, sizeof(UsersViewModel));
    if (!vm) return NULL;

    vm->base = eg_view_model_new();
    if (!vm->base) {
        free(vm);
        return NULL;
    }

    /* Initialize state */
    vm->users = NULL;
    vm->user_count = 0;
    vm->row_to_user_id = NULL;
    vm->editing_user_id = -1;
    vm->search_filter[0] = '\0';

    /* Add properties */
    eg_view_model_add_property(vm->base, eg_property_new_string(USERS_VM_PROP_SEARCH_TEXT, ""));
    eg_view_model_add_property(vm->base, eg_property_new_int(USERS_VM_PROP_SELECTED_ROW, -1));
    eg_view_model_add_property(vm->base, eg_property_new_int(USERS_VM_PROP_SELECTED_USER_ID, -1));
    eg_view_model_add_property(vm->base, eg_property_new_int(USERS_VM_PROP_USER_COUNT, 0));

    /* Dialog form properties */
    eg_view_model_add_property(vm->base, eg_property_new_string(USERS_VM_PROP_DIALOG_USERNAME, ""));
    eg_view_model_add_property(vm->base, eg_property_new_string(USERS_VM_PROP_DIALOG_EMAIL, ""));
    eg_view_model_add_property(vm->base, eg_property_new_string(USERS_VM_PROP_DIALOG_PASSWORD, ""));
    eg_view_model_add_property(vm->base, eg_property_new_bool(USERS_VM_PROP_DIALOG_ACTIVE, true));
    eg_view_model_add_property(vm->base, eg_property_new_bool(USERS_VM_PROP_DIALOG_VISIBLE, false));
    eg_view_model_add_property(vm->base, eg_property_new_bool(USERS_VM_PROP_DIALOG_IS_EDIT, false));

    /* Add commands */
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_ADD, cmd_add_execute, NULL, vm));
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_EDIT, cmd_edit_execute, cmd_has_selection, vm));
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_DELETE, cmd_delete_execute, cmd_delete_can_execute, vm));
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_SAVE, cmd_save_execute, NULL, vm));
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_CANCEL, cmd_cancel_execute, NULL, vm));
    eg_view_model_add_command(vm->base, eg_command_new(USERS_VM_CMD_REFRESH, cmd_refresh_execute, NULL, vm));

    return vm;
}

void users_view_model_free(UsersViewModel *vm) {
    if (!vm) return;

    free_cached_users(vm);

    if (vm->base) {
        eg_view_model_free(vm->base);
    }

    free(vm);
}

EgViewModel *users_view_model_get_base(UsersViewModel *vm) {
    return vm ? vm->base : NULL;
}

void users_view_model_refresh(UsersViewModel *vm) {
    if (!vm) return;

    /* Free old cached data */
    free_cached_users(vm);

    /* Get all users from database */
    int total_count = 0;
    User **all_users = db_user_list_all(&total_count);

    /* Apply search filter */
    const char *filter = vm->search_filter;
    bool has_filter = filter && filter[0] != '\0';

    /* Count matching users */
    int match_count = 0;
    for (int i = 0; i < total_count; i++) {
        if (has_filter) {
            if (strstr(all_users[i]->username, filter) == NULL &&
                strstr(all_users[i]->email, filter) == NULL) {
                continue;
            }
        }
        match_count++;
    }

    /* Build filtered list */
    if (match_count > 0) {
        vm->users = (User **)malloc(match_count * sizeof(User *));
        vm->row_to_user_id = (int *)malloc(match_count * sizeof(int));

        int idx = 0;
        for (int i = 0; i < total_count; i++) {
            if (has_filter) {
                if (strstr(all_users[i]->username, filter) == NULL &&
                    strstr(all_users[i]->email, filter) == NULL) {
                    continue;
                }
            }
            /* Copy user to our list */
            vm->users[idx] = all_users[i];
            vm->row_to_user_id[idx] = all_users[i]->id;
            all_users[i] = NULL; /* Prevent double-free */
            idx++;
        }
        vm->user_count = match_count;
    }

    /* Free remaining users that didn't match filter */
    for (int i = 0; i < total_count; i++) {
        if (all_users[i]) {
            db_user_free(all_users[i]);
        }
    }
    free(all_users);

    /* Update properties */
    eg_view_model_set_int(vm->base, USERS_VM_PROP_USER_COUNT, vm->user_count);
    eg_view_model_set_int(vm->base, USERS_VM_PROP_SELECTED_ROW, -1);
    eg_view_model_set_int(vm->base, USERS_VM_PROP_SELECTED_USER_ID, -1);

    /* Update global stats */
    if (g_app_state && g_app_state->view_model) {
        eg_view_model_set_int(g_app_state->view_model, "total_users", db_stats_total_users());
    }

    /* Notify commands that can_execute may have changed */
    EgCommand *edit_cmd = eg_view_model_get_command(vm->base, USERS_VM_CMD_EDIT);
    EgCommand *delete_cmd = eg_view_model_get_command(vm->base, USERS_VM_CMD_DELETE);
    if (edit_cmd) eg_command_raise_can_execute_changed(edit_cmd);
    if (delete_cmd) eg_command_raise_can_execute_changed(delete_cmd);
}

void users_view_model_set_search(UsersViewModel *vm, const char *search) {
    if (!vm) return;

    if (search) {
        strncpy(vm->search_filter, search, sizeof(vm->search_filter) - 1);
        vm->search_filter[sizeof(vm->search_filter) - 1] = '\0';
    } else {
        vm->search_filter[0] = '\0';
    }

    eg_view_model_set_string(vm->base, USERS_VM_PROP_SEARCH_TEXT, vm->search_filter);
    users_view_model_refresh(vm);
}

User *users_view_model_get_user_at_row(UsersViewModel *vm, int row) {
    if (!vm || row < 0 || row >= vm->user_count) return NULL;

    /* Return a copy - caller must free */
    return db_user_get_by_id(vm->row_to_user_id[row]);
}

int users_view_model_get_count(UsersViewModel *vm) {
    return vm ? vm->user_count : 0;
}

void users_view_model_set_selected_row(UsersViewModel *vm, int row) {
    if (!vm) return;

    eg_view_model_set_int(vm->base, USERS_VM_PROP_SELECTED_ROW, row);

    int user_id = -1;
    if (row >= 0 && row < vm->user_count) {
        user_id = vm->row_to_user_id[row];
    }
    eg_view_model_set_int(vm->base, USERS_VM_PROP_SELECTED_USER_ID, user_id);

    /* Update global state */
    if (g_app_state && g_app_state->view_model) {
        eg_view_model_set_int(g_app_state->view_model, "selected_user_id", user_id);
    }

    /* Notify commands */
    EgCommand *edit_cmd = eg_view_model_get_command(vm->base, USERS_VM_CMD_EDIT);
    EgCommand *delete_cmd = eg_view_model_get_command(vm->base, USERS_VM_CMD_DELETE);
    if (edit_cmd) eg_command_raise_can_execute_changed(edit_cmd);
    if (delete_cmd) eg_command_raise_can_execute_changed(delete_cmd);
}

int users_view_model_get_selected_row(UsersViewModel *vm) {
    return vm ? eg_view_model_get_int(vm->base, USERS_VM_PROP_SELECTED_ROW) : -1;
}

void users_view_model_begin_add(UsersViewModel *vm) {
    if (!vm) return;

    vm->editing_user_id = -1;

    /* Clear dialog form */
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_USERNAME, "");
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_EMAIL, "");
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_PASSWORD, "");
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_ACTIVE, true);
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_IS_EDIT, false);
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_VISIBLE, true);
}

void users_view_model_begin_edit(UsersViewModel *vm) {
    if (!vm) return;

    int row = users_view_model_get_selected_row(vm);
    if (row < 0 || row >= vm->user_count) return;

    User *user = users_view_model_get_user_at_row(vm, row);
    if (!user) return;

    vm->editing_user_id = user->id;

    /* Populate dialog form */
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_USERNAME, user->username);
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_EMAIL, user->email);
    eg_view_model_set_string(vm->base, USERS_VM_PROP_DIALOG_PASSWORD, "");
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_ACTIVE, user->is_active);
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_IS_EDIT, true);
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_VISIBLE, true);

    db_user_free(user);
}

void users_view_model_cancel_edit(UsersViewModel *vm) {
    if (!vm) return;

    vm->editing_user_id = -1;
    eg_view_model_set_bool(vm->base, USERS_VM_PROP_DIALOG_VISIBLE, false);
}

bool users_view_model_save(UsersViewModel *vm) {
    if (!vm) return false;

    const char *username = eg_view_model_get_string(vm->base, USERS_VM_PROP_DIALOG_USERNAME);
    const char *email = eg_view_model_get_string(vm->base, USERS_VM_PROP_DIALOG_EMAIL);
    const char *password = eg_view_model_get_string(vm->base, USERS_VM_PROP_DIALOG_PASSWORD);
    bool is_active = eg_view_model_get_bool(vm->base, USERS_VM_PROP_DIALOG_ACTIVE);

    bool success = false;

    if (vm->editing_user_id < 0) {
        /* Creating new user */
        if (username && username[0] != '\0' &&
            email && email[0] != '\0' &&
            password && password[0] != '\0') {
            success = db_user_create(username, email, password);
        }
    } else {
        /* Updating existing user */
        if (email && email[0] != '\0') {
            success = db_user_update(vm->editing_user_id, email, is_active);
            if (success && password && password[0] != '\0') {
                db_user_change_password(vm->editing_user_id, password);
            }
        }
    }

    if (success) {
        users_view_model_cancel_edit(vm);
        users_view_model_refresh(vm);
    }

    return success;
}

bool users_view_model_delete_selected(UsersViewModel *vm) {
    if (!vm) return false;

    int row = users_view_model_get_selected_row(vm);
    if (row < 0 || row >= vm->user_count) return false;

    int user_id = vm->row_to_user_id[row];

    /* Cannot delete admin */
    User *user = db_user_get_by_id(user_id);
    if (user && strcmp(user->username, "admin") == 0) {
        db_user_free(user);
        return false;
    }
    if (user) db_user_free(user);

    bool success = db_user_delete(user_id);
    if (success) {
        users_view_model_refresh(vm);
    }

    return success;
}

bool users_view_model_can_delete(UsersViewModel *vm) {
    if (!vm) return false;

    int row = users_view_model_get_selected_row(vm);
    if (row < 0 || row >= vm->user_count) return false;

    int user_id = vm->row_to_user_id[row];
    User *user = db_user_get_by_id(user_id);
    if (!user) return false;

    bool can_delete = strcmp(user->username, "admin") != 0;
    db_user_free(user);

    return can_delete;
}

bool users_view_model_is_editing(UsersViewModel *vm) {
    return vm && vm->editing_user_id >= 0;
}

const char *users_view_model_get_dialog_title(UsersViewModel *vm) {
    if (!vm) return "User";
    return vm->editing_user_id >= 0 ? "Edit User" : "Add User";
}

/* Command implementations */
static void cmd_add_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_begin_add(vm);
}

static void cmd_edit_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_begin_edit(vm);
}

static void cmd_delete_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_delete_selected(vm);
}

static bool cmd_delete_can_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    return users_view_model_can_delete(vm);
}

static void cmd_save_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_save(vm);
}

static void cmd_cancel_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_cancel_edit(vm);
}

static void cmd_refresh_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    users_view_model_refresh(vm);
}

static bool cmd_has_selection(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd;
    (void)param;
    UsersViewModel *vm = (UsersViewModel *)user_data;
    return users_view_model_get_selected_row(vm) >= 0;
}
