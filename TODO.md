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

### MVVM Avançado - A Implementar

#### Refatoração: Binding via VTable (Prioridade Alta)
Atualmente temos funções específicas por widget (`eg_bind_entry_text`, `eg_bind_label_text`, etc.).
Isso não escala bem e dificulta extensibilidade.

**Proposta**: Usar a vtable dos widgets para declarar capacidades de binding.

```c
/* Cada widget declara na sua vtable como fazer binding */
typedef struct EgWidgetVTable {
    /* ... campos existentes ... */

    /* Binding capabilities */
    bool (*bind_value)(EgWidget *widget, EgProperty *prop, EgBindingMode mode);
    bool (*bind_command)(EgWidget *widget, EgCommand *cmd);
    EgPropertyType supported_binding_type;  /* STRING, INT, BOOL, etc. */
} EgWidgetVTable;

/* API genérica e limpa */
eg_bind(widget, vm, "property_name");           /* Usa vtable para descobrir como bindar */
eg_bind_command(widget, vm, "command_name");    /* Genérico para qualquer widget */

/* Exemplos de uso - mesma função para todos os widgets */
eg_bind(entry, vm, "username");      /* Entry sabe que é two-way string */
eg_bind(label, vm, "counter");       /* Label sabe que é one-way, formata int */
eg_bind(check, vm, "is_active");     /* CheckButton sabe que é two-way bool */
eg_bind(scale, vm, "volume");        /* Scale sabe que é two-way double */
```

**Benefícios:**
- Uma única função `eg_bind()` ao invés de N funções específicas
- Novos widgets automaticamente suportam binding se implementarem vtable
- Widgets customizados do usuário podem ter binding sem modificar o core
- Código mais limpo e consistente

**Implementação:**
- [x] Adicionar campos de binding na EgWidgetVTable
- [x] Implementar `eg_bind(widget, vm, property_name)` genérico
- [x] Migrar widgets existentes para usar vtable
- [x] Deprecar funções `eg_bind_<widget>_*` antigas
- [ ] Documentar como criar widgets com binding customizado

---

#### Outras Melhorias MVVM
- [x] Validação reativa com feedback visual automático
  - [x] Entry fica com classe CSS "error" automaticamente
  - [x] SpinButton fica com classe CSS "error" automaticamente
  - [x] Tooltip mostra mensagem de erro
  - [x] Sistema de validação declarativo via EgValidatorChain
  - [x] Validadores built-in: required, min/max length, email, pattern, range
  - [x] Validadores customizados via callback
  - [x] Validação automática on_focus_out
  - [x] Validação em lote (eg_validate_all, eg_validate_first_invalid)
  - [ ] eg_bind_validation(widget, vm, prop, error_label) - binding de erro a label
- [ ] CSS class binding
  - [ ] eg_bind_css_class(widget, vm, "class-name", "bool_property")
- [ ] Converters reutilizáveis
  - [ ] Registro global de converters nomeados
  - [ ] eg_converter_register("currency", format_currency, parse_currency)
  - [ ] eg_bind_with_converter(widget, vm, prop, "currency")
- [ ] Bindings condicionais sem callbacks manuais
  - [ ] eg_bind_visible_when(widget, vm, "condition_property")
  - [ ] eg_bind_sensitive_when(widget, vm, "condition_property")
- [ ] ViewModel scopes (parent/child)
  - [ ] eg_view_model_new_child(parent_vm)
  - [ ] Propagação controlada de properties
  - [ ] Herança de commands
- [ ] Namespacing de properties
  - [ ] Suporte a "user.name", "settings.theme" no binding
  - [ ] Composição de ViewModels sem explosão de nomes

### Sistema de Signals Avançado (Lock-free)
- [ ] Signal queue lock-free (atomic operations)
- [ ] Async callbacks com continuations
- [ ] eg_async_run(task, callback) - executa em thread, callback no main
- [ ] eg_await pattern para operações assíncronas
- [ ] Cancellation tokens integrados ao ViewModel lifecycle

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

// Binding declarativo automático (nova API genérica)
eg_bind(eg_entry_as_widget(entry), vm, "username");   // Two-way (auto-detectado)
eg_bind(eg_label_as_widget(label), vm, "counter");    // One-way (auto-detectado)
eg_bind_cmd(eg_button_as_widget(button), vm, "increment"); // Command binding

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

### Debug e DX (Developer Experience) - A Implementar
- [ ] Modo debug para binding e MVVM
  - [ ] Logs opcionais mostrando property changes e bindings disparados
  - [ ] eg_debug_enable_binding_logs(bool)
- [ ] Introspection leve
  - [ ] eg_view_model_dump(vm) - dump de todas properties e valores
  - [ ] eg_widget_dump_tree(widget) - dump da hierarquia de widgets
- [ ] Assertions amigáveis
  - [ ] Mensagens claras para property inexistente, tipo incompatível, widget inválido

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

### SearchEntry - A Implementar
- [ ] eg_search_entry_new()
- [ ] eg_search_entry_get_text / set_text
- [ ] eg_search_entry_on_search_changed (debounced)
- [ ] eg_search_entry_on_activate (Enter pressionado)
- [ ] eg_search_entry_set_placeholder
- [ ] Ícone de busca e botão de limpar integrados

### PasswordEntry - A Implementar
- [ ] eg_password_entry_new()
- [ ] eg_password_entry_get_text / set_text
- [ ] eg_password_entry_set_show_peek_icon (toggle visibilidade)
- [ ] Integração com validação MVVM

### DatePicker - A Implementar
- [ ] eg_date_picker_new()
- [ ] eg_date_picker_get_date / set_date
- [ ] eg_date_picker_on_date_changed
- [ ] eg_date_picker_set_format
- [ ] Calendário popup para seleção

### TimePicker - A Implementar
- [ ] eg_time_picker_new()
- [ ] eg_time_picker_get_time / set_time
- [ ] eg_time_picker_on_time_changed
- [ ] eg_time_picker_set_24h_format

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

### API Genérica de Containers (via VTable)
- [x] EgContainerVTable com capacidades
- [x] eg_widget_is_container() - detecção via vtable
- [x] eg_container_supports_multiple() - múltiplos filhos
- [x] eg_container_supports_named() - filhos nomeados
- [x] eg_container_max_children() - limite de filhos
- [x] eg_container_add() - adicionar filho genérico
- [x] eg_container_remove() - remover filho genérico
- [x] eg_container_add_named() - adicionar com nome
- [x] eg_container_get_child_count() - contar filhos
- [x] eg_container_get_child_at() - obter por índice
- [x] eg_container_clear() - remover todos
- [x] Containers com suporte: Box, Grid, Stack, Notebook, Frame, Paned, ScrolledWindow, Expander

### Overlay - A Implementar
- [ ] eg_overlay_new()
- [ ] eg_overlay_set_child (widget principal)
- [ ] eg_overlay_add_overlay (widget sobreposto)
- [ ] eg_overlay_remove_overlay
- [ ] Suporte a posicionamento (halign, valign, margin)
- [ ] Útil para loading states, badges, notificações

### Revealer - A Implementar
- [ ] eg_revealer_new()
- [ ] eg_revealer_set_child
- [ ] eg_revealer_set_reveal_child / get_reveal_child
- [ ] eg_revealer_set_transition_type (slide, crossfade, etc.)
- [ ] eg_revealer_set_transition_duration
- [ ] Binding MVVM para reveal state

### FlowBox - A Implementar
- [ ] eg_flow_box_new()
- [ ] eg_flow_box_append / insert / remove
- [ ] eg_flow_box_set_selection_mode
- [ ] eg_flow_box_set_max_children_per_line
- [ ] eg_flow_box_set_column_spacing / row_spacing
- [ ] on_child_activated
- [ ] Útil para galerias, grids responsivos, cards

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

## Feedback e Notificações - A Implementar

### Toast / Snackbar
- [ ] eg_toast_new(message)
- [ ] eg_toast_set_timeout(ms)
- [ ] eg_toast_set_button(label, callback) - ação opcional (ex: "Undo")
- [ ] eg_toast_show(parent_window)
- [ ] eg_toast_dismiss()
- [ ] Empilhamento automático de múltiplos toasts
- [ ] Posicionamento configurável (bottom, top)

### InfoBar
- [ ] eg_info_bar_new()
- [ ] eg_info_bar_set_message_type (info, warning, error, question)
- [ ] eg_info_bar_set_message(text)
- [ ] eg_info_bar_add_button(label, response_id)
- [ ] eg_info_bar_set_revealed / get_revealed
- [ ] on_response callback
- [ ] Binding MVVM para visibilidade e mensagem

### LoadingOverlay - A Implementar
- [ ] eg_loading_overlay_new()
- [ ] eg_loading_overlay_set_child (widget a sobrepor)
- [ ] eg_loading_overlay_set_loading(bool)
- [ ] eg_loading_overlay_set_message(text)
- [ ] Binding MVVM com property "is_busy"
- [ ] Spinner + overlay semi-transparente

---

## Menus e Ações

### HeaderBar
- [x] eg_header_bar_new()
- [x] eg_header_bar_set_title_widget
- [x] eg_header_bar_pack_start / pack_end
- [x] eg_header_bar_remove
- [x] eg_header_bar_set_show_title_buttons
- [x] eg_header_bar_set_decoration_layout

### Window Controls (Custom Titlebar Support)
- [x] eg_window_minimize
- [x] eg_window_maximize / eg_window_unmaximize
- [x] eg_window_toggle_maximize
- [x] eg_window_is_maximized
- [x] eg_window_fullscreen / eg_window_unfullscreen
- [x] eg_window_is_fullscreen
- [x] eg_window_set_decorated / eg_window_get_decorated
- [x] eg_window_set_transient_for
- [x] eg_window_set_modal / eg_window_get_modal
- [x] eg_window_set_destroy_on_close

### PopoverMenu
- [x] eg_popover_menu_new()
- [x] eg_popover_menu_free()
- [x] eg_popover_menu_add_item()
- [x] eg_popover_menu_add_item_with_icon()
- [x] eg_popover_menu_add_separator()
- [x] eg_popover_menu_add_section()
- [x] eg_popover_menu_set_item_enabled()
- [x] eg_popover_menu_popup / popdown

### MenuButton
- [x] eg_menu_button_new()
- [x] eg_menu_button_new_with_label()
- [x] eg_menu_button_new_with_icon()
- [x] eg_menu_button_set_popover()
- [x] eg_menu_button_set_label / get_label
- [x] eg_menu_button_set_icon_name / get_icon_name
- [x] eg_menu_button_set_direction()
- [x] eg_menu_button_set_has_frame()
- [x] eg_menu_button_popup / popdown

---

## Navegação - A Implementar

### Sidebar / NavigationView
- [ ] eg_sidebar_new()
- [ ] eg_sidebar_add_item(icon, label, page_name)
- [ ] eg_sidebar_add_separator()
- [ ] eg_sidebar_add_section(title)
- [ ] eg_sidebar_set_selected(page_name)
- [ ] eg_sidebar_on_selection_changed
- [ ] Integração com Stack para navegação SPA
- [ ] Binding MVVM para página ativa

### Breadcrumbs
- [ ] eg_breadcrumbs_new()
- [ ] eg_breadcrumbs_push(label, data)
- [ ] eg_breadcrumbs_pop()
- [ ] eg_breadcrumbs_clear()
- [ ] eg_breadcrumbs_on_item_clicked
- [ ] Útil com TreeView para navegação hierárquica

---

## ListView / ColumnView (Avançado)

### ListView
- [x] eg_list_view_new()
- [x] eg_list_view_append / insert / remove / clear
- [x] eg_list_view_get_count / get_item
- [x] Modelo de dados simples (GtkStringList)
- [x] Seleção simples/múltipla/nenhuma (EgSelectionMode)
- [x] eg_list_view_select / get_selected / get_selected_count / get_all_selected
- [x] on_selection_changed
- [x] on_activate (double-click)
- [x] eg_list_view_sort_ascending / sort_descending
- [x] eg_list_view_sort_custom (função de comparação customizada)
- [x] eg_list_view_set_auto_sort / disable_auto_sort

### ColumnView (Tabela)
- [x] eg_column_view_new()
- [x] eg_column_view_add_column
- [x] eg_column_view_append_row / append_rowv
- [x] eg_column_view_set_cell / get_cell
- [x] eg_column_view_remove_row / clear
- [x] eg_column_view_get_row_count / get_column_count
- [x] Seleção simples/múltipla/nenhuma (EgSelectionMode)
- [x] eg_column_view_select_row / get_selected_row / get_selected_rows / get_selected_count
- [x] on_selection_changed
- [x] on_row_activated (double-click)
- [x] eg_column_view_sort_by_column (ordenação por coluna)
- [x] eg_column_view_sort_custom (função de comparação customizada)
- [x] eg_column_view_set_column_sortable (click-to-sort com GtkSorter)
- [x] eg_column_view_set_auto_sort_column

### Binding MVVM para Listas
- [x] eg_bind_list_view_items (binding de itens via property)
- [x] eg_bind_list_view_selection (callback de seleção para MVVM)
- [x] eg_bind_column_view_rows (binding de linhas)
- [x] eg_bind_column_view_selection (callback de seleção para MVVM)

### Melhorias Futuras para Listas
- [ ] Virtualização explícita e documentada
- [ ] Binding incremental (atualizar apenas itens afetados)
- [ ] eg_list_view_update_item(index, new_value) sem rebuild

---

## TreeView (Árvore Hierárquica) - A Implementar

### EgTreeView
- [ ] eg_tree_view_new()
- [ ] Modelo de dados hierárquico (nós com filhos)
- [ ] eg_tree_view_add_root / add_child
- [ ] eg_tree_view_expand / collapse / expand_all / collapse_all
- [ ] eg_tree_view_get_selected_path
- [ ] on_selection_changed
- [ ] on_row_expanded / on_row_collapsed
- [ ] Suporte a múltiplas colunas na árvore
- [ ] Ícones para nós (folder, file, etc.)
- [ ] Binding MVVM desde o início

---

## Form Helpers - A Implementar

### EgFormGroup
- [ ] eg_form_group_new(label)
- [ ] eg_form_group_add_field(widget, label, error_label)
- [ ] eg_form_group_set_orientation (vertical, horizontal)
- [ ] Layout automático de label + widget + erro
- [ ] Integração com validação MVVM

### Focus Management
- [ ] eg_widget_grab_focus_on_show
- [ ] eg_form_focus_next() - avança para próximo campo
- [ ] eg_entry_on_activate (Enter) para avançar foco
- [ ] eg_form_set_default_button (Enter no último campo aciona)

---

## Prioridades - Roadmap

### Fase 1: Widgets de Alta Demanda
1. SearchEntry
2. Overlay
3. Revealer
4. Toast/Snackbar
5. InfoBar

### Fase 2: Infraestrutura MVVM
1. Validação reativa com feedback visual
2. CSS class binding
3. Form helpers
4. ViewModel scopes

### Fase 3: Async
1. eg_async_run
2. Cancellation tokens
3. Property thread-safety

### Fase 4: Widgets Estruturais
1. Sidebar/NavigationView
2. TreeView
3. DatePicker/TimePicker
4. FlowBox

### Fase 5: Avançado
1. Converters reutilizáveis
2. Debug/introspection tools
3. Memory pool integration

---

## Histórico de Conclusões

- **v0.1**: Core widgets (Button, Label, Entry, Box, Grid, Window)
- **v0.2**: Timers, CSS, CheckButton, Switch, MessageDialog
- **v0.3**: ScrolledWindow, ComboBox, ProgressBar, FileChooser, ColorChooser
- **v0.4**: Menus, ListView, ColumnView, Notebook, HeaderBar, Image, Spinner, LevelBar
- **v0.5**: MVVM completo (ViewModel, Model, Property, Command, Binding declarativo)
- **v0.6**: Click-to-sort em ColumnView, window modal/transient, ListView/ColumnView bindings
- **v0.7**: Sistema de Validação (EgValidatorChain, validadores built-in, validação automática), API genérica de Containers (EgContainerVTable), refatoração de eventos via event_helpers
