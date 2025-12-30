# EasyGTK - TODO

---

## Arquitetura Avançada (Futuro)

### Sistema Reativo / Data Binding
- [x] Observable properties (eg_property_new, eg_property_bind)
- [x] Binding bidirecional entre widgets e modelo
- [x] Notificação automática de mudanças
- [x] Computed properties (derivadas de outras) - eg_property_set_computed

### Padrão MVVM/MVC
- [x] EgViewModel base com vtable
- [x] EgModel para dados com validação
- [x] Sistema de commands (ICommand pattern)
- [x] Binding declarativo widget <-> viewmodel
  - [x] eg_bind_entry_text (two-way)
  - [x] eg_bind_label_text (one-way, suporta int/double/bool/string)
  - [x] eg_bind_check_button_active (two-way)
  - [x] eg_bind_switch_active (two-way)
  - [x] eg_bind_spin_button_value (two-way)
  - [x] eg_bind_scale_value (two-way)
  - [x] eg_bind_widget_visible (one-way)
  - [x] eg_bind_widget_sensitive (one-way)
  - [x] eg_bind_button_command (command binding)

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

### Exemplo de uso (implementado em spa_example.c):
```c
// ViewModel com propriedades observáveis
EgViewModel *vm = eg_view_model_new();
eg_view_model_add_property(vm, eg_property_new_int("counter", 0));
eg_view_model_add_property(vm, eg_property_new_string("username", ""));

// Computed property
EgProperty *total = eg_property_new_int("total", 0);
EgProperty *deps[] = {counter_prop};
eg_property_set_computed(total, compute_total, deps, 1, NULL);

// Binding declarativo automático
eg_bind_entry_text(entry, vm, "username");         // Two-way
eg_bind_label_text(label, vm, "counter");          // One-way
eg_bind_button_command(button, vm, "increment");   // Command binding

// Model para dados
EgModel *user_model = eg_model_new();
eg_model_add_property(user_model, eg_property_new_string("name", ""));
eg_model_add_property(user_model, eg_property_new_string("email", ""));
if (eg_model_validate(user_model)) {
    // Salvar...
}
```

**Status**: Sistema MVVM completo e funcional! Veja `examples/spa_example.c` e `examples/SPA_README.md`.

---

## Core / Infraestrutura

### Gerenciamento de Memória
- [x] Sistema de ref-counting opcional para widgets (eg_widget_ref, eg_widget_unref)
- [x] Pool de alocação (EgMemoryPool) - **estrutura pronta, falta integrar aos widgets**
- [x] Destruição automática de filhos ao destruir container
- [ ] Integrar EgMemoryPool na alocação de widgets pequenos (EgLabel, EgButton, etc.)

#### Propósito do EgMemoryPool

O `EgMemoryPool` é um **alocador de memória otimizado** para objetos de tamanho fixo. Benefícios:

1. **Reduz fragmentação de memória** - Aloca blocos contíguos
2. **Melhora performance** - Evita chamadas frequentes a malloc/free
3. **Cache-friendly** - Objetos próximos na memória

**Onde será usado:**
- Alocação de widgets pequenos (EgLabel, EgButton) que são criados/destruídos frequentemente
- Alocação de handlers de signal (EgSignalHandler)
- Alocação de bindings de property (EgBinding)
- Alocação de callbacks de eventos

**Exemplo de integração futura:**
```c
/* Pool global para widgets pequenos */
static EgMemoryPool *g_widget_pool = NULL;

void eg_init(void) {
    g_widget_pool = eg_pool_new(sizeof(EgButton), 64); /* 64 botões pré-alocados */
}

EgButton *eg_button_new(const char *label) {
    EgButton *btn = eg_pool_alloc(g_widget_pool);  /* Usa pool ao invés de malloc */
    /* ... */
}

void eg_button_free(EgButton *btn) {
    eg_pool_release(g_widget_pool, btn);  /* Devolve ao pool ao invés de free */
}
```

### Sistema de Eventos
- [x] Suporte a múltiplos callbacks por evento (EgSignal)
- [x] Desconectar callbacks (retornar handler ID)
- [x] Eventos de teclado (key-press, key-release)
- [x] Eventos de mouse (motion, enter, leave, scroll, click)
- [x] Eventos de foco (focus-in, focus-out)
- [x] Drag and drop
  - [x] eg_widget_set_drag_source (básico)
  - [x] eg_widget_set_drop_target (básico)
  - [x] eg_widget_on_drag_begin - buscar DragSourceData existente
  - [x] eg_widget_on_drag_end - buscar DragSourceData existente
  - [x] eg_widget_remove_drag_source - remover controllers
  - [x] eg_widget_remove_drop_target - remover controllers

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
- [x] Transformação de valores no binding (transform callback)
- [x] Transformação reversa no binding bidirecional

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
- [x] eg_spin_button_new(min, max, step)
- [x] eg_spin_button_get_value / set_value
- [x] eg_spin_button_on_value_changed

### Scale (Slider)
- [x] eg_scale_new(orientation, min, max, step)
- [x] eg_scale_get_value / set_value
- [x] eg_scale_on_value_changed

### ProgressBar
- [x] eg_progress_bar_new()
- [x] eg_progress_bar_set_fraction
- [x] eg_progress_bar_set_text
- [x] eg_progress_bar_pulse (indeterminate)
- [x] eg_progress_bar_set_show_text, set_inverted

### TextView (Multi-line)
- [x] eg_text_view_new()
- [x] eg_text_view_get_text / set_text
- [x] eg_text_view_set_editable
- [x] eg_text_view_on_changed

---

## Containers

### ScrolledWindow
- [x] eg_scrolled_window_new()
- [x] eg_scrolled_window_set_child
- [x] eg_scrolled_window_set_policy
- [x] eg_scrolled_window_set_min/max_content_size

### Stack
- [x] eg_stack_new()
- [x] eg_stack_add_named(child, name)
- [x] eg_stack_set_visible_child_name

### StackSwitcher
- [x] eg_stack_switcher_new()
- [x] eg_stack_switcher_set_stack

### Notebook (Tabs)
- [x] eg_notebook_new()
- [x] eg_notebook_append_page(child, label)
- [x] eg_notebook_set_current_page
- [x] eg_notebook_on_page_changed

### Paned (Split view)
- [x] eg_paned_new(orientation)
- [x] eg_paned_set_start_child / set_end_child
- [x] eg_paned_set_position

### Frame
- [x] eg_frame_new(label)
- [x] eg_frame_set_child

### Expander
- [x] eg_expander_new(label)
- [x] eg_expander_set_child
- [x] eg_expander_set_expanded

---

## Diálogos

### MessageDialog
- [x] eg_dialog_info(parent, title, message)
- [x] eg_dialog_warning(...)
- [x] eg_dialog_error(...)
- [x] eg_dialog_question(..., callback)
- [x] eg_dialog_confirm(..., callback)

### FileChooser
- [x] eg_file_chooser_open(parent, title, callback)
- [x] eg_file_chooser_save(parent, title, callback)
- [x] eg_file_chooser_select_folder(...)
- [x] Filtros de arquivo (EgFileFilter)
- [x] eg_file_chooser_open_multiple
- [x] eg_file_chooser_open_with_filters

### ColorChooser
- [x] eg_color_chooser_dialog(parent, callback)
- [x] eg_color_chooser_dialog_with_alpha
- [x] eg_color_from_rgb, eg_color_from_rgba, eg_color_from_string
- [x] eg_color_to_string

### AboutDialog
- [x] eg_about_dialog_new()
- [x] eg_about_dialog_set_program_name, version, copyright, comments
- [x] eg_about_dialog_set_license, website, authors, artists
- [x] eg_about_dialog_show

---

## Widgets de Exibição

### Image
- [x] eg_image_new(), eg_image_new_from_file(path)
- [x] eg_image_new_from_icon(icon_name)
- [x] eg_image_new_from_resource
- [x] eg_image_set_pixel_size, eg_image_clear
- [x] eg_image_set_from_file, eg_image_set_from_icon

### Picture
- [x] eg_picture_new(), eg_picture_new_from_file(path)
- [x] eg_picture_new_from_resource
- [x] eg_picture_set_content_fit (EgContentFit)
- [x] eg_picture_set_can_shrink
- [x] eg_picture_set_alternative_text

### Separator
- [x] eg_separator_new(orientation)
- [x] eg_separator_new_horizontal, eg_separator_new_vertical

### Spinner
- [x] eg_spinner_new()
- [x] eg_spinner_start / stop
- [x] eg_spinner_set_spinning / get_spinning

### LevelBar
- [x] eg_level_bar_new(), eg_level_bar_new_for_interval
- [x] eg_level_bar_set_value / get_value
- [x] eg_level_bar_set_min_value / set_max_value
- [x] eg_level_bar_set_mode (continuous/discrete)
- [x] eg_level_bar_set_inverted
- [x] eg_level_bar_add_offset_value / remove_offset_value

---

## Menus e Ações

### HeaderBar
- [x] eg_header_bar_new()
- [x] eg_header_bar_set_title_widget
- [x] eg_header_bar_pack_start / pack_end
- [x] eg_header_bar_remove
- [x] eg_header_bar_set_show_title_buttons
- [x] eg_header_bar_set_decoration_layout

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
2. **Média**: ~~ScrolledWindow~~, ~~ComboBox~~, ~~ProgressBar~~, ~~FileChooser~~, ~~ColorChooser~~
3. **Baixa**: Menus, ListView, ~~Notebook~~, ~~HeaderBar~~, ~~Image~~, ~~Spinner~~, ~~LevelBar~~
