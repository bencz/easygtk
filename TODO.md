# EasyGTK - TODO

---

## Arquitetura Avançada (Futuro)

### Sistema Reativo / Data Binding
- [x] Observable properties (eg_property_new, eg_property_bind)
- [x] Binding bidirecional entre widgets e modelo
- [x] Notificação automática de mudanças
- [ ] Computed properties (derivadas de outras)

### Padrão MVVM/MVC
- [x] EgViewModel base com vtable
- [ ] EgModel para dados
- [x] Sistema de commands (ICommand pattern)
- [ ] Binding declarativo widget <-> viewmodel

### Sistema de Signals Avançado (Lock-free)
- [ ] Signal queue lock-free (atomic operations)
- [ ] Async callbacks com continuations
- [ ] eg_async_run(task, callback) - executa em thread, callback no main
- [ ] eg_await pattern para operações assíncronas
- [ ] Cancellation tokens

### Event Loop Customizado
- [ ] Integração com io_uring (Linux) para I/O async
- [ ] Task scheduler cooperativo
- [ ] Futures/Promises em C
- [ ] eg_task_new(), eg_task_then(), eg_task_await()

### Exemplo de uso futuro (conceitual):
```c
// ViewModel com propriedades observáveis
EgViewModel *vm = eg_view_model_new();
eg_view_model_add_property(vm, "counter", EG_TYPE_INT, 0);
eg_view_model_add_property(vm, "label_text", EG_TYPE_STRING, "Count: 0");

// Computed property
eg_view_model_add_computed(vm, "label_text", compute_label, "counter");

// Binding automático
eg_bind(entry, "text", vm, "username", EG_BIND_TWO_WAY);
eg_bind(label, "text", vm, "label_text", EG_BIND_ONE_WAY);

// Async sem bloquear UI
eg_async_run(fetch_data_from_api, on_data_received, user_data);
```

---

## Core / Infraestrutura

### Gerenciamento de Memória
- [x] Sistema de ref-counting opcional para widgets (eg_widget_ref, eg_widget_unref)
- [x] Pool de alocação (EgMemoryPool) - **estrutura pronta, falta integrar aos widgets**
- [ ] Destruição automática de filhos ao destruir container
- [ ] Integrar EgMemoryPool na alocação de widgets pequenos (EgLabel, EgButton, etc.)

### Sistema de Eventos
- [x] Suporte a múltiplos callbacks por evento (EgSignal)
- [x] Desconectar callbacks (retornar handler ID)
- [x] Eventos de teclado (key-press, key-release)
- [x] Eventos de mouse (motion, enter, leave, scroll, click)
- [x] Eventos de foco (focus-in, focus-out)
- [x] Drag and drop - **parcialmente implementado**
  - [x] eg_widget_set_drag_source (básico)
  - [x] eg_widget_set_drop_target (básico)
  - [ ] eg_widget_on_drag_begin - buscar DragSourceData existente
  - [ ] eg_widget_on_drag_end - buscar DragSourceData existente
  - [ ] eg_widget_remove_drag_source - remover controllers
  - [ ] eg_widget_remove_drop_target - remover controllers

### Utilitários Core
- [x] Timers (eg_timeout_add, eg_idle_add)
- [x] Clipboard (copiar/colar)
- [x] Sistema de CSS (eg_css_load_from_file, eg_css_load_from_string)
- [x] Logging/debug helpers (eg_log_debug, eg_log_info, eg_log_warning, eg_log_error)
- [x] Conversão de tipos helper (eg_widget_cast_to_button, eg_widget_is_type, etc.)

### Propriedades Genéricas de Widget
- [x] Tooltip (eg_widget_set_tooltip, eg_widget_set_tooltip_markup)
- [x] Cursor customizado (eg_widget_set_cursor)
- [x] Opacity (eg_widget_set_opacity, eg_widget_get_opacity)
- [x] Nome/ID para lookup (eg_widget_set_name, eg_widget_get_name)

### Data Binding (property.c)
- [x] Binding básico entre properties
- [ ] Transformação de valores no binding (transform callback)
- [ ] Transformação reversa no binding bidirecional

---

## Widgets de Input

### CheckButton
- [x] eg_check_button_new(label)
- [x] eg_check_button_get_active / set_active
- [x] eg_check_button_on_toggled
- [x] eg_check_button_set_group (radio button behavior)

### Switch
- [x] eg_switch_new()
- [x] eg_switch_get_active / set_active
- [x] eg_switch_on_state_changed

### ComboBox (Dropdown)
- [x] eg_combo_box_new()
- [x] eg_combo_box_append(text)
- [x] eg_combo_box_get_active / set_active
- [x] eg_combo_box_on_changed
- [x] eg_combo_box_get_active_text

### SpinButton
- [ ] eg_spin_button_new(min, max, step)
- [ ] eg_spin_button_get_value / set_value
- [ ] eg_spin_button_on_value_changed

### Scale (Slider)
- [ ] eg_scale_new(orientation, min, max, step)
- [ ] eg_scale_get_value / set_value
- [ ] eg_scale_on_value_changed

### ProgressBar
- [x] eg_progress_bar_new()
- [x] eg_progress_bar_set_fraction
- [x] eg_progress_bar_set_text
- [x] eg_progress_bar_pulse (indeterminate)
- [x] eg_progress_bar_set_show_text, set_inverted

### TextView (Multi-line)
- [ ] eg_text_view_new()
- [ ] eg_text_view_get_text / set_text
- [ ] eg_text_view_set_editable
- [ ] eg_text_view_on_changed

---

## Containers

### ScrolledWindow
- [x] eg_scrolled_window_new()
- [x] eg_scrolled_window_set_child
- [x] eg_scrolled_window_set_policy
- [x] eg_scrolled_window_set_min/max_content_size

### Stack
- [ ] eg_stack_new()
- [ ] eg_stack_add_named(child, name)
- [ ] eg_stack_set_visible_child_name

### StackSwitcher
- [ ] eg_stack_switcher_new()
- [ ] eg_stack_switcher_set_stack

### Notebook (Tabs)
- [ ] eg_notebook_new()
- [ ] eg_notebook_append_page(child, label)
- [ ] eg_notebook_set_current_page
- [ ] eg_notebook_on_page_changed

### Paned (Split view)
- [ ] eg_paned_new(orientation)
- [ ] eg_paned_set_start_child / set_end_child
- [ ] eg_paned_set_position

### Frame
- [ ] eg_frame_new(label)
- [ ] eg_frame_set_child

### Expander
- [ ] eg_expander_new(label)
- [ ] eg_expander_set_child
- [ ] eg_expander_set_expanded

---

## Diálogos

### MessageDialog
- [x] eg_dialog_info(parent, title, message)
- [x] eg_dialog_warning(...)
- [x] eg_dialog_error(...)
- [x] eg_dialog_question(..., callback)
- [x] eg_dialog_confirm(..., callback)

### FileChooser
- [ ] eg_file_chooser_open(parent, title, callback)
- [ ] eg_file_chooser_save(parent, title, callback)
- [ ] eg_file_chooser_select_folder(...)
- [ ] Filtros de arquivo

### ColorChooser
- [ ] eg_color_chooser_dialog(parent, callback)

### AboutDialog
- [ ] eg_about_dialog_new()
- [ ] eg_about_dialog_set_program_name, version, etc.

---

## Widgets de Exibição

### Image
- [ ] eg_image_new_from_file(path)
- [ ] eg_image_new_from_icon(icon_name)
- [ ] eg_image_set_pixel_size

### Picture
- [ ] eg_picture_new_from_file(path)
- [ ] eg_picture_set_content_fit

### Separator
- [ ] eg_separator_new(orientation)

### Spinner
- [ ] eg_spinner_new()
- [ ] eg_spinner_start / stop

### LevelBar
- [ ] eg_level_bar_new()
- [ ] eg_level_bar_set_value

---

## Menus e Ações

### HeaderBar
- [ ] eg_header_bar_new()
- [ ] eg_header_bar_set_title
- [ ] eg_header_bar_pack_start / pack_end

### PopoverMenu
- [ ] eg_popover_menu_new()
- [ ] eg_popover_menu_add_item

### MenuButton
- [ ] eg_menu_button_new()
- [ ] eg_menu_button_set_popover

---

## ListView / TreeView (Avançado)

### ListView
- [ ] eg_list_view_new()
- [ ] Modelo de dados simples
- [ ] Seleção simples/múltipla
- [ ] on_selection_changed

### ColumnView (Tabela)
- [ ] eg_column_view_new()
- [ ] eg_column_view_add_column

---

## Prioridades Sugeridas

1. **Alta**: ~~Timers~~, ~~CSS~~, ~~CheckButton~~, ~~Switch~~, ~~MessageDialog~~
2. **Média**: ~~ScrolledWindow~~, ~~ComboBox~~, ~~ProgressBar~~, FileChooser
3. **Baixa**: Menus, ListView, Notebook, HeaderBar
