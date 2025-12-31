# EasyGTK SPA Example

Complete example of a **Single Page Application** using EasyGTK with MVVM pattern.

## Features

### Complete MVVM Architecture
- **Global ViewModel**: Shared state between all pages
- **Declarative Data Binding**: Automatic binding between UI and data
- **Computed Properties**: Automatically calculated properties
- **Commands**: Reusable actions with can_execute validation

### Implemented Features

#### 1. Home Page
- Displays automatically calculated statistics:
  - Total products in stock (computed property)
  - Total inventory value (computed property)
- Navigation to other pages
- Reactive updates when data changes

#### 2. Products Page
- Product list with information
- **Refresh** button that updates quantities
- Demonstrates reactive update of computed properties
- ScrolledWindow for long lists

#### 3. Profile Page
- Form with two-way data binding:
  - Username (Entry)
  - Email (Entry)
  - Notifications (CheckButton)
- **Save** button with validation:
  - Disabled when username is empty
  - Automatic state update
- Command binding for actions

## Demonstrated Concepts

### 1. Computed Properties
```c
/* Total products calculated automatically */
static void compute_total_products(EgProperty *computed, void *user_data) {
    int total = 0;
    for (int i = 0; i < product_count; i++) {
        total += products[i].quantity;
    }
    eg_property_set_int(computed, total);
}
```

### 2. Data Binding Declarativo
```c
/* Two-way binding: Entry <-> Property */
eg_bind(eg_entry_as_widget(username_entry), app_vm, "username");

/* One-way binding: Label <- Property */
eg_bind(eg_label_as_widget(stat1_value), app_vm, "total_products");

/* Command binding: Button -> Command */
eg_bind_cmd(eg_button_as_widget(btn_save), app_vm, "save_profile");
```

### 3. Commands with Validation
```c
/* Command with dynamic can_execute */
static bool cmd_save_profile_can_execute(EgCommand *cmd, void *param, void *user_data) {
    const char *username = eg_view_model_get_string(app_vm, "username");
    return username != NULL && username[0] != '\0';
}

/* Observe changes to update can_execute */
eg_property_on_changed(username_prop, on_username_changed_for_save, save_cmd);
```

### 4. Navigation between Pages
```c
/* Stack for pages */
EgStack *page_stack = eg_stack_new();
eg_stack_add_named(page_stack, create_home_page(), "home");
eg_stack_add_named(page_stack, create_products_page(), "products");
eg_stack_add_named(page_stack, create_profile_page(), "profile");

/* Navigation via command */
static void cmd_navigate_products(EgCommand *cmd, void *param, void *user_data) {
    eg_stack_set_visible_child_name(page_stack, "products");
}
```

## How to Run

```bash
# Build
cmake -B build -S .
cmake --build build -j4

# Run
./build/spa_example
```

## MVVM Pattern Benefits

1. **Separation of Concerns**: UI separated from logic
2. **Testability**: ViewModel can be tested without UI
3. **Reactivity**: Changes propagate automatically
4. **Reusability**: Commands and bindings are declarative
5. **Maintainability**: Organized and decoupled code

## Next Steps

Possible improvements:
- Add data persistence
- Implement navigation with history
- Add more complex validations
- Implement ListView for dynamic lists
- Add transition animations
