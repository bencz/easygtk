# EasyGTK

A C99 library to simplify GUI development with GTK4.

## Features

- **Simple API**: No public macros, just clear and straightforward functions
- **Componentization**: VTable-based system for extensibility
- **C99**: Compatible with the C99 standard
- **Complete MVVM**: ViewModel, Model, Observable Properties and Declarative Data Binding
- **Computed Properties**: Automatically calculated properties
- **Command Pattern**: Commands with can_execute validation
- **Reactivity**: Automatic bidirectional binding system
- **Declarative Validation**: Validation system with automatic visual feedback
- **Generic Container API**: Polymorphic manipulation of any container via vtable
- **Memory Management**: Simplified widget lifecycle management system

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Dependencies

- GTK4 (`gtk4-devel` on Fedora, `libgtk-4-dev` on Debian/Ubuntu)
- CMake 3.10+
- C99 compiler

## Basic Example

```c
#include <easygtk/easygtk.h>

void on_button_click(EgWidget *widget, void *user_data) {
    eg_label_set_text((EgLabel *)user_data, "Button clicked!");
}

void on_activate(EgWidget *widget, void *user_data) {
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Hello EasyGTK", 400, 300);
    EgBox *box = eg_box_new_vertical(10);
    
    EgLabel *label = eg_label_new("Hello, World!");
    EgButton *button = eg_button_new("Click here");
    
    eg_button_on_click(button, on_button_click, label);
    
    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_button_as_widget(button));
    
    eg_window_set_child(window, eg_box_as_widget(box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    EgApp *app = eg_app_new("com.example.hello");
    eg_app_on_activate(app, on_activate, app);
    return eg_app_run(app);
}
```

## MVVM Example with Data Binding

```c
#include <easygtk/easygtk.h>

static EgViewModel *vm = NULL;

static void cmd_increment(EgCommand *cmd, void *param, void *data) {
    int val = eg_view_model_get_int(vm, "counter");
    eg_view_model_set_int(vm, "counter", val + 1);
}

static bool cmd_reset_can_execute(EgCommand *cmd, void *param, void *data) {
    return eg_view_model_get_int(vm, "counter") > 0;
}

static void cmd_reset(EgCommand *cmd, void *param, void *data) {
    eg_view_model_set_int(vm, "counter", 0);
}

void on_activate(EgWidget *widget, void *user_data) {
    EgApp *app = (EgApp *)user_data;

    // Create ViewModel
    vm = eg_view_model_new();
    eg_view_model_add_property(vm, eg_property_new_int("counter", 0));

    // Create Commands
    eg_view_model_add_command(vm,
        eg_command_new("increment", cmd_increment, NULL, NULL));
    eg_view_model_add_command(vm,
        eg_command_new("reset", cmd_reset, cmd_reset_can_execute, NULL));

    // UI with Declarative Data Binding
    EgWindow *window = eg_window_new(app, "MVVM Demo", 300, 200);
    EgBox *box = eg_box_new_vertical(10);

    // Label automatically bound to counter
    EgLabel *label = eg_label_new("0");
    eg_bind_label_text(label, vm, "counter");  // One-way binding

    // Buttons bound to commands
    EgButton *btn_inc = eg_button_new("Increment");
    eg_bind_button_command(btn_inc, vm, "increment");  // Command binding

    EgButton *btn_reset = eg_button_new("Reset");
    eg_bind_button_command(btn_reset, vm, "reset");  // Auto-disable when counter = 0

    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_button_as_widget(btn_inc));
    eg_box_append(box, eg_button_as_widget(btn_reset));
    eg_window_set_child(window, eg_box_as_widget(box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    EgApp *app = eg_app_new("com.example.mvvm");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_view_model_free(vm);
    eg_app_free(app);
    return status;
}
```

## Available Widgets

### Basic
- **EgWindow**: Main window
- **EgButton**: Button
- **EgLabel**: Static text
- **EgEntry**: Text input field

### Input
- **EgCheckButton**: Checkbox (supports grouping as radio buttons)
- **EgSwitch**: On/off toggle switch
- **EgProgressBar**: Progress bar (determinate and indeterminate)
- **EgComboBox**: Selection dropdown
- **EgSpinButton**: Numeric field with increment/decrement
- **EgScale**: Slider for value selection
- **EgTextView**: Multi-line text field

### Containers
- **EgBox**: Linear container (horizontal/vertical)
- **EgGrid**: Grid container
- **EgScrolledWindow**: Container with scrollbars
- **EgStack**: Container that shows one child at a time (with transitions)
- **EgStackSwitcher**: Buttons to switch Stack pages
- **EgNotebook**: Container with tabs
- **EgPaned**: Container split into two resizable panels
- **EgFrame**: Container with border and title
- **EgExpander**: Expandable/collapsible container

### Menus
- **EgPopoverMenu**: Popup menu with items, icons and separators
- **EgMenuButton**: Button that opens a popover menu

### Lists and Tables
- **EgListView**: List of items with single/multiple selection
- **EgColumnView**: Table with multiple columns

## Dialogs

- **eg_dialog_info()**: Information dialog
- **eg_dialog_warning()**: Warning dialog
- **eg_dialog_error()**: Error dialog
- **eg_dialog_question()**: Yes/No dialog with callback
- **eg_dialog_confirm()**: OK/Cancel dialog with callback

## Core Features

### MVVM/Reactivity System
- **EgProperty**: Observable properties with automatic notification
- **Computed Properties**: `eg_property_set_computed()` - automatically calculated properties
- **EgModel**: Data layer with validation
- **EgViewModel**: Application state management
- **EgCommand**: Commands with `can_execute` validation (ICommand pattern)
- **Declarative Data Binding**:
  - `eg_bind()` - Generic binding via vtable (auto-detects mode)
  - `eg_bind_cmd()` - Generic command binding
  - `eg_bind_widget_visible()` - One-way for visibility
  - `eg_bind_widget_sensitive()` - One-way for enabled/disabled

### Validation System
- **EgValidatorChain**: Chain of validators executed in sequence
- **Built-in Validators**:
  - `eg_validator_add_required()` - Required field
  - `eg_validator_add_min_length()` / `eg_validator_add_max_length()` - String length
  - `eg_validator_add_email()` - Email format
  - `eg_validator_add_pattern()` - Regular expression
  - `eg_validator_add_range_int()` / `eg_validator_add_range_double()` - Numeric range
  - `eg_validator_add_custom()` - Custom validator via callback
- **Widget Integration**:
  - `eg_widget_set_validators()` - Associates validators with a widget
  - `eg_widget_validate()` - Validates and shows visual error (CSS + tooltip)
  - `eg_widget_validate_on_focus_out()` - Automatic validation on focus loss
  - `eg_validate_all()` - Validates multiple widgets at once

### Generic Container API
- **Polymorphism via VTable**: Operates on any container generically
- `eg_widget_is_container()` - Checks if widget is a container
- `eg_container_add()` / `eg_container_remove()` - Adds/removes children
- `eg_container_add_named()` - Adds named child (Stack, Notebook)
- `eg_container_get_child_count()` / `eg_container_get_child_at()` - Iterates children
- `eg_container_clear()` - Removes all children
- `eg_container_supports_multiple()` / `eg_container_supports_named()` - Checks capabilities

### Event System
- **EgSignal**: Event system with multiple listeners
- **Events**: Keyboard, mouse (motion, enter, leave, scroll, click), focus
- **Clipboard**: `eg_clipboard_set_text()`, `eg_clipboard_get_text()`
- **Drag and Drop**: `eg_widget_set_drag_source()`, `eg_widget_set_drop_target()`

### Utilities
- **Timers**: `eg_timeout_add()`, `eg_idle_add()`
- **CSS**: `eg_css_load_from_file()`, `eg_css_load_from_string()`
- **Tooltip**: `eg_widget_set_tooltip()`, `eg_widget_set_tooltip_markup()`
- **Cursor**: `eg_widget_set_cursor()`
- **Opacity**: `eg_widget_set_opacity()`, `eg_widget_get_opacity()`
- **Name/ID**: `eg_widget_set_name()`, `eg_widget_get_name()`
- **Logging**: `eg_log_debug()`, `eg_log_info()`, `eg_log_warning()`, `eg_log_error()`
- **Type Casting**: `eg_widget_cast_to_*()`, `eg_widget_is_type()`, `eg_widget_is_container()`
- **Ref-counting**: `eg_widget_ref()`, `eg_widget_unref()`

### Window Controls (Custom Titlebar)
- **HeaderBar**: `eg_header_bar_new()`, `eg_header_bar_set_title_widget()`, `eg_header_bar_pack_start/end()`
- **Custom Titlebar**: `eg_window_set_titlebar()` - Use custom HeaderBar instead of system decorations
- **Window Control**: `eg_window_minimize()`, `eg_window_maximize()`, `eg_window_toggle_maximize()`
- **Fullscreen**: `eg_window_fullscreen()`, `eg_window_unfullscreen()`, `eg_window_is_fullscreen()`
- **Decoration**: `eg_window_set_decorated()` - Enable/disable system window decorations

## Examples

After building, examples will be in `build/`:

```bash
./build/hello_world           # Basic example
./build/form_example          # Form with grid
./build/counter_example       # Simple counter
./build/mvvm_example          # MVVM demonstration
./build/spa_example           # Complete SPA with MVVM, binding and computed properties
./build/widgets_example       # CheckButton, Switch, ProgressBar
./build/dialogs_example       # Dialogs, ComboBox, ScrolledWindow
./build/events_example        # Keyboard, mouse, focus, clipboard events
./build/dnd_example           # Drag and Drop
./build/input_widgets_example # SpinButton, Scale, TextView, Frame
./build/containers_example    # Stack, Notebook, Paned, Expander
./build/menu_example          # PopoverMenu, MenuButton
./build/menubar_example       # Traditional menu bar (File, Edit, View, Help)
./build/listview_example      # ListView and ColumnView (table)
./build/dashboard_example     # Dashboard with charts and metrics
./build/validation_example    # Form validation system
./build/container_api_example # Generic container API via vtable
```

**Highlight**: The `spa_example` demonstrates a complete SPA-style (Single Page Application) application with:
- Navigation between multiple pages (Home, Products, Profile)
- Declarative data binding (two-way and one-way)
- Computed properties (automatically calculated statistics)
- Commands with can_execute validation
- Reactive forms

See `examples/SPA_README.md` for more details.

## License

MIT License
