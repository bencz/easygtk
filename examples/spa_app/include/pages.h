/**
 * SPA App - Pages
 * All page creation functions
 */

#ifndef SPA_PAGES_H
#define SPA_PAGES_H

#include <easygtk/easygtk.h>

/* Page creation functions */
EgWidget *page_login_create(void);
EgWidget *page_dashboard_create(void);
EgWidget *page_users_create(void);
EgWidget *page_products_create(void);
EgWidget *page_settings_create(void);

/* Page update functions (called when navigating to page) */
void page_dashboard_update(void);
void page_users_update(void);
void page_products_update(void);
void page_settings_update(void);

#endif /* SPA_PAGES_H */
