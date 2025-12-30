/**
 * SPA App - Application State
 * Global state management with MVVM
 */

#ifndef SPA_APP_STATE_H
#define SPA_APP_STATE_H

#include <easygtk/easygtk.h>
#include "database.h"

/* Page identifiers */
typedef enum {
    PAGE_LOGIN = 0,
    PAGE_DASHBOARD,
    PAGE_USERS,
    PAGE_PRODUCTS,
    PAGE_SETTINGS,
    PAGE_COUNT
} AppPage;

/* Global application state */
typedef struct {
    EgViewModel *view_model;
    EgModel *current_user_model;
    User *logged_user;
    AppPage current_page;

    /* Widgets */
    EgStack *main_stack;
    EgStack *content_stack;
    EgButton *nav_buttons[PAGE_COUNT];

    /* Flags */
    bool is_authenticated;
} AppState;

/* Global state instance */
extern AppState *g_app_state;

/* State management */
AppState *app_state_new(void);
void app_state_free(AppState *state);

/* Authentication */
bool app_state_login(const char *username, const char *password);
void app_state_logout(void);
bool app_state_is_authenticated(void);

/* Navigation */
void app_state_navigate_to(AppPage page);
AppPage app_state_get_current_page(void);

/* Current user */
User *app_state_get_current_user(void);
const char *app_state_get_username(void);

#endif /* SPA_APP_STATE_H */
