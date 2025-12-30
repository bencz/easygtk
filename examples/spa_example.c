/**
 * EasyGTK - SPA (Single Page Application) Example
 *
 * Demonstra um aplicativo estilo SPA com:
 * - Navegação entre páginas (Home, Products, Profile)
 * - ViewModel global compartilhado
 * - Data binding declarativo
 * - Computed properties
 * - Model para dados de produtos
 * - Commands para navegação e ações
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

/* ============================================
 * Models - Dados de Domínio
 * ============================================ */

/* Produto */
typedef struct {
    int id;
    char name[64];
    double price;
    int quantity;
} Product;

/* ============================================
 * ViewModel Global
 * ============================================ */

static EgViewModel *app_vm = NULL;

/* Pages */
typedef enum {
    PAGE_HOME = 0,
    PAGE_PRODUCTS,
    PAGE_PROFILE
} AppPage;

/* Produtos de exemplo */
static Product products[] = {
    {1, "Laptop", 999.99, 5},
    {2, "Mouse", 29.99, 15},
    {3, "Keyboard", 79.99, 10},
    {4, "Monitor", 299.99, 7}
};
static const int product_count = 4;

/* ============================================
 * Computed Property: Total de produtos
 * ============================================ */

static void compute_total_products(EgProperty *computed, void *user_data) {
    (void)user_data;

    /* Calcula total baseado nos produtos disponíveis */
    int total = 0;
    for (int i = 0; i < product_count; i++) {
        total += products[i].quantity;
    }

    eg_property_set_int(computed, total);
}

/* ============================================
 * Computed Property: Valor total do estoque
 * ============================================ */

static void compute_total_value(EgProperty *computed, void *user_data) {
    (void)user_data;

    /* Calcula valor total do estoque */
    double total = 0.0;
    for (int i = 0; i < product_count; i++) {
        total += products[i].price * products[i].quantity;
    }

    eg_property_set_double(computed, total);
}

/* ============================================
 * Commands - Navegação
 * ============================================ */

static EgStack *page_stack = NULL;

static void cmd_navigate_home(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    eg_view_model_set_int(app_vm, "current_page", PAGE_HOME);
    eg_stack_set_visible_child_name(page_stack, "home");
}

static void cmd_navigate_products(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    eg_view_model_set_int(app_vm, "current_page", PAGE_PRODUCTS);
    eg_stack_set_visible_child_name(page_stack, "products");
}

static void cmd_navigate_profile(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;
    eg_view_model_set_int(app_vm, "current_page", PAGE_PROFILE);
    eg_stack_set_visible_child_name(page_stack, "profile");
}

/* ============================================
 * Command - Refresh Products
 * ============================================ */

static void cmd_refresh_products(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;

    /* Simula atualização de dados */
    for (int i = 0; i < product_count; i++) {
        products[i].quantity += 1;
    }

    /* Força recálculo das computed properties de forma segura */
    EgProperty *total_products = eg_view_model_get_property(app_vm, "total_products");
    EgProperty *total_value = eg_view_model_get_property(app_vm, "total_value");

    if (total_products && total_value) {
        /* Calcula novos valores */
        int total = 0;
        double value = 0.0;
        for (int i = 0; i < product_count; i++) {
            total += products[i].quantity;
            value += products[i].price * products[i].quantity;
        }

        /* Atualiza as properties (isso dispara os signals automaticamente) */
        eg_property_set_int(total_products, total);
        eg_property_set_double(total_value, value);
    }

    eg_log_info("SPA", "Products refreshed! Total items: %d",
                total_products ? eg_property_get_int(total_products) : 0);
}

/* ============================================
 * Command - Save Profile
 * ============================================ */

static void cmd_save_profile(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;

    const char *username = eg_view_model_get_string(app_vm, "username");
    const char *email = eg_view_model_get_string(app_vm, "email");
    bool notifications = eg_view_model_get_bool(app_vm, "notifications_enabled");

    char message[256];
    snprintf(message, sizeof(message),
        "Profile Saved!\nUsername: %s\nEmail: %s\nNotifications: %s",
        username ? username : "",
        email ? email : "",
        notifications ? "Enabled" : "Disabled"
    );

    eg_log_info("SPA", "%s", message);
}

static bool cmd_save_profile_can_execute(EgCommand *cmd, void *param, void *user_data) {
    (void)cmd; (void)param; (void)user_data;

    /* Só pode salvar se username não estiver vazio */
    const char *username = eg_view_model_get_string(app_vm, "username");
    return username != NULL && username[0] != '\0';
}

/* ============================================
 * UI - Home Page
 * ============================================ */

/* Callback para formatar valor como moeda */
static void on_total_value_changed(EgProperty *prop, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    char buf[64];
    double val = eg_property_get_double(prop);
    snprintf(buf, sizeof(buf), "<span size='x-large' weight='bold'>$%.2f</span>", val);
    eg_label_set_markup(label, buf);
}

static EgWidget *create_home_page(void) {
    EgBox *page = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(page), 40);
    eg_widget_set_halign(eg_box_as_widget(page), EG_ALIGN_CENTER);
    eg_widget_set_valign(eg_box_as_widget(page), EG_ALIGN_CENTER);

    /* Título */
    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='xx-large' weight='bold'>Welcome to SPA Demo</span>");

    /* Subtítulo */
    EgLabel *subtitle = eg_label_new("Built with EasyGTK MVVM");
    eg_widget_add_css_class(eg_label_as_widget(subtitle), "dim-label");

    /* Estatísticas */
    EgBox *stats_box = eg_box_new_horizontal(30);
    eg_widget_set_halign(eg_box_as_widget(stats_box), EG_ALIGN_CENTER);

    /* Total Products (computed property) */
    EgBox *stat1 = eg_box_new_vertical(5);
    EgLabel *stat1_label = eg_label_new("Total Products");
    eg_widget_add_css_class(eg_label_as_widget(stat1_label), "dim-label");

    EgLabel *stat1_value = eg_label_new("0");
    eg_label_set_markup(stat1_value, "<span size='x-large' weight='bold'>0</span>");
    eg_bind_label_text(stat1_value, app_vm, "total_products");

    eg_box_append(stat1, eg_label_as_widget(stat1_label));
    eg_box_append(stat1, eg_label_as_widget(stat1_value));

    /* Total Value (computed property) */
    EgBox *stat2 = eg_box_new_vertical(5);
    EgLabel *stat2_label = eg_label_new("Inventory Value");
    eg_widget_add_css_class(eg_label_as_widget(stat2_label), "dim-label");

    EgLabel *stat2_value = eg_label_new("$0.00");
    eg_label_set_markup(stat2_value, "<span size='x-large' weight='bold'>$0.00</span>");

    /* Formata valor como moeda */
    EgProperty *total_value_prop = eg_view_model_get_property(app_vm, "total_value");
    eg_property_on_changed(total_value_prop, on_total_value_changed, stat2_value);
    /* Atualiza valor inicial */
    on_total_value_changed(total_value_prop, stat2_value);

    eg_box_append(stat2, eg_label_as_widget(stat2_label));
    eg_box_append(stat2, eg_label_as_widget(stat2_value));

    eg_box_append(stats_box, eg_box_as_widget(stat1));
    eg_box_append(stats_box, eg_box_as_widget(stat2));

    /* Botões de navegação */
    EgBox *nav_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(nav_box), EG_ALIGN_CENTER);

    EgButton *btn_products = eg_button_new("View Products");
    eg_widget_add_css_class(eg_button_as_widget(btn_products), "suggested-action");
    eg_bind_button_command(btn_products, app_vm, "navigate_products");

    EgButton *btn_profile = eg_button_new("Edit Profile");
    eg_bind_button_command(btn_profile, app_vm, "navigate_profile");

    eg_box_append(nav_box, eg_button_as_widget(btn_products));
    eg_box_append(nav_box, eg_button_as_widget(btn_profile));

    /* Montagem */
    eg_box_append(page, eg_label_as_widget(title));
    eg_box_append(page, eg_label_as_widget(subtitle));
    eg_box_append(page, eg_box_as_widget(stats_box));
    eg_box_append(page, eg_box_as_widget(nav_box));

    return eg_box_as_widget(page);
}

/* ============================================
 * UI - Products Page
 * ============================================ */

static EgWidget *create_products_page(void) {
    EgBox *page = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(page), 40);

    /* Cabeçalho */
    EgBox *header = eg_box_new_horizontal(10);

    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Products</span>");
    eg_widget_set_hexpand(eg_label_as_widget(title), true);

    EgButton *btn_refresh = eg_button_new("Refresh");
    eg_bind_button_command(btn_refresh, app_vm, "refresh_products");

    EgButton *btn_back = eg_button_new("← Back");
    eg_bind_button_command(btn_back, app_vm, "navigate_home");

    eg_box_append(header, eg_label_as_widget(title));
    eg_box_append(header, eg_button_as_widget(btn_refresh));
    eg_box_append(header, eg_button_as_widget(btn_back));

    /* Lista de produtos */
    EgBox *products_list = eg_box_new_vertical(10);

    for (int i = 0; i < product_count; i++) {
        EgFrame *product_frame = eg_frame_new(NULL);

        EgBox *product_box = eg_box_new_horizontal(20);
        eg_widget_set_margin(eg_box_as_widget(product_box), 15);

        /* Nome e preço */
        EgBox *info_box = eg_box_new_vertical(5);
        eg_widget_set_hexpand(eg_box_as_widget(info_box), true);

        char name_markup[128];
        snprintf(name_markup, sizeof(name_markup),
            "<span weight='bold'>%s</span>", products[i].name);
        EgLabel *name_label = eg_label_new("");
        eg_label_set_markup(name_label, name_markup);
        eg_widget_set_halign(eg_label_as_widget(name_label), EG_ALIGN_START);

        char price_text[64];
        snprintf(price_text, sizeof(price_text), "$%.2f", products[i].price);
        EgLabel *price_label = eg_label_new(price_text);
        eg_widget_set_halign(eg_label_as_widget(price_label), EG_ALIGN_START);
        eg_widget_add_css_class(eg_label_as_widget(price_label), "dim-label");

        eg_box_append(info_box, eg_label_as_widget(name_label));
        eg_box_append(info_box, eg_label_as_widget(price_label));

        /* Quantidade */
        char qty_text[64];
        snprintf(qty_text, sizeof(qty_text), "Stock: %d", products[i].quantity);
        EgLabel *qty_label = eg_label_new(qty_text);

        eg_box_append(product_box, eg_box_as_widget(info_box));
        eg_box_append(product_box, eg_label_as_widget(qty_label));

        eg_frame_set_child(product_frame, eg_box_as_widget(product_box));
        eg_box_append(products_list, eg_frame_as_widget(product_frame));
    }

    /* ScrolledWindow para lista */
    EgScrolledWindow *scroll = eg_scrolled_window_new();
    eg_scrolled_window_set_child(scroll, eg_box_as_widget(products_list));
    eg_scrolled_window_set_policy(scroll, EG_SCROLL_NEVER, EG_SCROLL_AUTOMATIC);
    eg_widget_set_vexpand(eg_scrolled_window_as_widget(scroll), true);

    /* Montagem */
    eg_box_append(page, eg_box_as_widget(header));
    eg_box_append(page, eg_scrolled_window_as_widget(scroll));

    return eg_box_as_widget(page);
}

/* ============================================
 * UI - Profile Page
 * ============================================ */

static EgWidget *create_profile_page(void) {
    EgBox *page = eg_box_new_vertical(20);
    eg_widget_set_margin(eg_box_as_widget(page), 40);
    eg_widget_set_halign(eg_box_as_widget(page), EG_ALIGN_CENTER);

    /* Cabeçalho */
    EgBox *header = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(header), EG_ALIGN_CENTER);

    EgLabel *title = eg_label_new("");
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Profile Settings</span>");

    eg_box_append(header, eg_label_as_widget(title));

    /* Formulário */
    EgBox *form = eg_box_new_vertical(15);
    eg_widget_set_size_request(eg_box_as_widget(form), 400, -1);

    /* Username */
    EgLabel *username_label = eg_label_new("Username:");
    eg_widget_set_halign(eg_label_as_widget(username_label), EG_ALIGN_START);

    EgEntry *username_entry = eg_entry_new();
    eg_entry_set_placeholder(username_entry, "Enter your username");
    eg_bind_entry_text(username_entry, app_vm, "username");

    /* Email */
    EgLabel *email_label = eg_label_new("Email:");
    eg_widget_set_halign(eg_label_as_widget(email_label), EG_ALIGN_START);

    EgEntry *email_entry = eg_entry_new();
    eg_entry_set_placeholder(email_entry, "your@email.com");
    eg_bind_entry_text(email_entry, app_vm, "email");

    /* Notifications */
    EgCheckButton *notifications_check = eg_check_button_new("Enable notifications");
    eg_bind_check_button_active(notifications_check, app_vm, "notifications_enabled");

    /* Botões */
    EgBox *button_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(button_box), EG_ALIGN_CENTER);

    EgButton *btn_save = eg_button_new("Save Changes");
    eg_widget_add_css_class(eg_button_as_widget(btn_save), "suggested-action");
    eg_bind_button_command(btn_save, app_vm, "save_profile");

    EgButton *btn_cancel = eg_button_new("Cancel");
    eg_bind_button_command(btn_cancel, app_vm, "navigate_home");

    eg_box_append(button_box, eg_button_as_widget(btn_save));
    eg_box_append(button_box, eg_button_as_widget(btn_cancel));

    /* Montagem do formulário */
    eg_box_append(form, eg_label_as_widget(username_label));
    eg_box_append(form, eg_entry_as_widget(username_entry));
    eg_box_append(form, eg_label_as_widget(email_label));
    eg_box_append(form, eg_entry_as_widget(email_entry));
    eg_box_append(form, eg_check_button_as_widget(notifications_check));
    eg_box_append(form, eg_box_as_widget(button_box));

    /* Montagem final */
    eg_box_append(page, eg_box_as_widget(header));
    eg_box_append(page, eg_box_as_widget(form));

    return eg_box_as_widget(page);
}

/* ============================================
 * ViewModel Setup
 * ============================================ */

/* Callback para atualizar can_execute do comando save quando username mudar */
static void on_username_changed_for_save(EgProperty *property, void *user_data) {
    (void)property;
    EgCommand *save_cmd = (EgCommand *)user_data;
    eg_command_raise_can_execute_changed(save_cmd);
}

static void setup_viewmodel(void) {
    app_vm = eg_view_model_new();

    /* Properties de estado */
    eg_view_model_add_property(app_vm, eg_property_new_int("current_page", PAGE_HOME));

    /* Profile properties */
    eg_view_model_add_property(app_vm, eg_property_new_string("username", "User"));
    eg_view_model_add_property(app_vm, eg_property_new_string("email", "user@example.com"));
    eg_view_model_add_property(app_vm, eg_property_new_bool("notifications_enabled", true));

    /* Computed properties */
    EgProperty *total_products = eg_property_new_int("total_products", 0);
    eg_view_model_add_property(app_vm, total_products);

    EgProperty *total_value = eg_property_new_double("total_value", 0.0);
    eg_view_model_add_property(app_vm, total_value);

    /* Configura computed properties (sem dependências explícitas neste caso) */
    compute_total_products(total_products, NULL);
    compute_total_value(total_value, NULL);

    /* Commands de navegação */
    eg_view_model_add_command(app_vm,
        eg_command_new("navigate_home", cmd_navigate_home, NULL, NULL));
    eg_view_model_add_command(app_vm,
        eg_command_new("navigate_products", cmd_navigate_products, NULL, NULL));
    eg_view_model_add_command(app_vm,
        eg_command_new("navigate_profile", cmd_navigate_profile, NULL, NULL));

    /* Commands de ações */
    eg_view_model_add_command(app_vm,
        eg_command_new("refresh_products", cmd_refresh_products, NULL, NULL));
    eg_view_model_add_command(app_vm,
        eg_command_new("save_profile", cmd_save_profile, cmd_save_profile_can_execute, NULL));

    /* Observa mudanças no username para atualizar can_execute do save */
    EgProperty *username_prop = eg_view_model_get_property(app_vm, "username");
    EgCommand *save_cmd = eg_view_model_get_command(app_vm, "save_profile");
    eg_property_on_changed(username_prop, on_username_changed_for_save, save_cmd);
}

/* ============================================
 * Main
 * ============================================ */

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;

    /* Setup ViewModel */
    setup_viewmodel();

    /* Janela principal */
    EgWindow *window = eg_window_new(app, "EasyGTK SPA Demo", 800, 600);

    /* Stack para páginas */
    page_stack = eg_stack_new();
    eg_stack_add_named(page_stack, create_home_page(), "home");
    eg_stack_add_named(page_stack, create_products_page(), "products");
    eg_stack_add_named(page_stack, create_profile_page(), "profile");

    eg_window_set_child(window, eg_stack_as_widget(page_stack));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    EgApp *app = eg_app_new("com.example.spa");
    if (app == NULL) return 1;

    eg_app_on_activate(app, on_activate, app);

    int status = eg_app_run(app);

    /* Cleanup */
    if (app_vm) eg_view_model_free(app_vm);
    eg_app_free(app);

    return status;
}
