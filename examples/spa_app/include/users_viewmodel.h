/**
 * SPA App - Users ViewModel
 *
 * MVVM ViewModel for user management.
 * Contains properties and commands for the Users page.
 */

#ifndef SPA_USERS_VIEWMODEL_H
#define SPA_USERS_VIEWMODEL_H

#include <easygtk/easygtk.h>
#include "database.h"

/* Forward declaration */
typedef struct UsersViewModel UsersViewModel;

/**
 * Creates the Users ViewModel.
 * Contains properties for form fields and commands for actions.
 *
 * @return Pointer to UsersViewModel or NULL
 */
UsersViewModel *users_view_model_new(void);

/**
 * Frees the Users ViewModel.
 */
void users_view_model_free(UsersViewModel *vm);

/**
 * Gets the base EgViewModel for binding.
 */
EgViewModel *users_view_model_get_base(UsersViewModel *vm);

/**
 * Refreshes the user list from database.
 * Updates the users_list property.
 */
void users_view_model_refresh(UsersViewModel *vm);

/**
 * Sets the search filter text.
 */
void users_view_model_set_search(UsersViewModel *vm, const char *search);

/**
 * Gets the User at specified row index.
 * Caller must free the returned User with db_user_free().
 *
 * @param vm ViewModel
 * @param row Row index
 * @return User struct or NULL
 */
User *users_view_model_get_user_at_row(UsersViewModel *vm, int row);

/**
 * Gets number of users in current list.
 */
int users_view_model_get_count(UsersViewModel *vm);

/**
 * Sets the currently selected row.
 */
void users_view_model_set_selected_row(UsersViewModel *vm, int row);

/**
 * Gets the currently selected row.
 */
int users_view_model_get_selected_row(UsersViewModel *vm);

/**
 * Opens the add user dialog (sets dialog mode).
 */
void users_view_model_begin_add(UsersViewModel *vm);

/**
 * Opens the edit dialog for selected user.
 */
void users_view_model_begin_edit(UsersViewModel *vm);

/**
 * Cancels the current dialog operation.
 */
void users_view_model_cancel_edit(UsersViewModel *vm);

/**
 * Saves the current user (add or update).
 * Uses values from dialog properties.
 *
 * @return true if save succeeded
 */
bool users_view_model_save(UsersViewModel *vm);

/**
 * Deletes the selected user.
 *
 * @return true if delete succeeded
 */
bool users_view_model_delete_selected(UsersViewModel *vm);

/**
 * Checks if current selected user can be deleted.
 * (admin user cannot be deleted)
 */
bool users_view_model_can_delete(UsersViewModel *vm);

/**
 * Checks if currently editing (vs adding).
 */
bool users_view_model_is_editing(UsersViewModel *vm);

/**
 * Gets the dialog title based on mode.
 */
const char *users_view_model_get_dialog_title(UsersViewModel *vm);

/* Property names (for binding) */
#define USERS_VM_PROP_SEARCH_TEXT      "search_text"
#define USERS_VM_PROP_SELECTED_ROW     "selected_row"
#define USERS_VM_PROP_SELECTED_USER_ID "selected_user_id"
#define USERS_VM_PROP_USER_COUNT       "user_count"

/* Dialog form properties */
#define USERS_VM_PROP_DIALOG_USERNAME  "dialog_username"
#define USERS_VM_PROP_DIALOG_EMAIL     "dialog_email"
#define USERS_VM_PROP_DIALOG_PASSWORD  "dialog_password"
#define USERS_VM_PROP_DIALOG_ACTIVE    "dialog_active"
#define USERS_VM_PROP_DIALOG_VISIBLE   "dialog_visible"
#define USERS_VM_PROP_DIALOG_IS_EDIT   "dialog_is_edit"

/* Command names */
#define USERS_VM_CMD_ADD               "cmd_add_user"
#define USERS_VM_CMD_EDIT              "cmd_edit_user"
#define USERS_VM_CMD_DELETE            "cmd_delete_user"
#define USERS_VM_CMD_SAVE              "cmd_save_user"
#define USERS_VM_CMD_CANCEL            "cmd_cancel"
#define USERS_VM_CMD_REFRESH           "cmd_refresh"

#endif /* SPA_USERS_VIEWMODEL_H */
