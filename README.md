# EasyGTK

Uma biblioteca C99 para simplificar o desenvolvimento de interfaces gráficas com GTK4.

## Características

- **API Simples**: Sem macros públicas, apenas funções claras e diretas
- **Componentização**: Sistema baseado em vtables para extensibilidade
- **C99**: Compatível com o padrão C99
- **MVVM Completo**: ViewModel, Model, Properties observáveis e Data Binding declarativo
- **Computed Properties**: Propriedades calculadas automaticamente
- **Command Pattern**: Commands com validação can_execute
- **Reatividade**: Sistema de binding bidirecional automático
- **Validação Declarativa**: Sistema de validação com feedback visual automático
- **API Genérica de Containers**: Manipulação polimórfica de qualquer container via vtable
- **Gerenciamento de Memória**: Sistema simplificado de ciclo de vida dos widgets

## Compilação

```bash
mkdir build && cd build
cmake ..
make
```

## Dependências

- GTK4 (`gtk4-devel` no Fedora, `libgtk-4-dev` no Debian/Ubuntu)
- CMake 3.10+
- Compilador C99

## Exemplo Básico

```c
#include <easygtk/easygtk.h>

void on_button_click(EgWidget *widget, void *user_data) {
    eg_label_set_text((EgLabel *)user_data, "Botão clicado!");
}

void on_activate(EgWidget *widget, void *user_data) {
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Hello EasyGTK", 400, 300);
    EgBox *box = eg_box_new_vertical(10);
    
    EgLabel *label = eg_label_new("Olá, Mundo!");
    EgButton *button = eg_button_new("Clique aqui");
    
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

## Exemplo MVVM com Data Binding

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

    // Criar ViewModel
    vm = eg_view_model_new();
    eg_view_model_add_property(vm, eg_property_new_int("counter", 0));

    // Criar Commands
    eg_view_model_add_command(vm,
        eg_command_new("increment", cmd_increment, NULL, NULL));
    eg_view_model_add_command(vm,
        eg_command_new("reset", cmd_reset, cmd_reset_can_execute, NULL));

    // UI com Data Binding Declarativo
    EgWindow *window = eg_window_new(app, "MVVM Demo", 300, 200);
    EgBox *box = eg_box_new_vertical(10);

    // Label ligado automaticamente ao counter
    EgLabel *label = eg_label_new("0");
    eg_bind_label_text(label, vm, "counter");  // One-way binding

    // Botões ligados aos commands
    EgButton *btn_inc = eg_button_new("Increment");
    eg_bind_button_command(btn_inc, vm, "increment");  // Command binding

    EgButton *btn_reset = eg_button_new("Reset");
    eg_bind_button_command(btn_reset, vm, "reset");  // Auto-disable quando counter = 0

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

## Widgets Disponíveis

### Básicos
- **EgWindow**: Janela principal
- **EgButton**: Botão
- **EgLabel**: Texto estático
- **EgEntry**: Campo de entrada de texto

### Input
- **EgCheckButton**: Checkbox (suporta agrupamento como radio buttons)
- **EgSwitch**: Toggle switch liga/desliga
- **EgProgressBar**: Barra de progresso (determinada e indeterminada)
- **EgComboBox**: Dropdown de seleção
- **EgSpinButton**: Campo numérico com incremento/decremento
- **EgScale**: Slider para seleção de valores
- **EgTextView**: Campo de texto multi-linha

### Containers
- **EgBox**: Container linear (horizontal/vertical)
- **EgGrid**: Container em grade
- **EgScrolledWindow**: Container com barras de rolagem
- **EgStack**: Container que mostra um filho por vez (com transições)
- **EgStackSwitcher**: Botões para alternar páginas do Stack
- **EgNotebook**: Container com abas (tabs)
- **EgPaned**: Container dividido em dois painéis redimensionáveis
- **EgFrame**: Container com borda e título
- **EgExpander**: Container expansível/recolhível

### Menus
- **EgPopoverMenu**: Menu popup com itens, ícones e separadores
- **EgMenuButton**: Botão que abre um menu popover

### Listas e Tabelas
- **EgListView**: Lista de itens com seleção simples/múltipla
- **EgColumnView**: Tabela com múltiplas colunas

## Diálogos

- **eg_dialog_info()**: Diálogo de informação
- **eg_dialog_warning()**: Diálogo de aviso
- **eg_dialog_error()**: Diálogo de erro
- **eg_dialog_question()**: Diálogo Sim/Não com callback
- **eg_dialog_confirm()**: Diálogo OK/Cancelar com callback

## Core Features

### Sistema MVVM/Reatividade
- **EgProperty**: Properties observáveis com notificação automática
- **Computed Properties**: `eg_property_set_computed()` - propriedades calculadas automaticamente
- **EgModel**: Camada de dados com validação
- **EgViewModel**: Gerenciamento de estado da aplicação
- **EgCommand**: Commands com validação `can_execute` (ICommand pattern)
- **Data Binding Declarativo**:
  - `eg_bind()` - Binding genérico via vtable (detecta modo automaticamente)
  - `eg_bind_cmd()` - Binding de command genérico
  - `eg_bind_widget_visible()` - One-way para visibilidade
  - `eg_bind_widget_sensitive()` - One-way para habilitado/desabilitado

### Sistema de Validação
- **EgValidatorChain**: Chain de validadores executados em sequência
- **Validadores Built-in**:
  - `eg_validator_add_required()` - Campo obrigatório
  - `eg_validator_add_min_length()` / `eg_validator_add_max_length()` - Tamanho de string
  - `eg_validator_add_email()` - Formato de email
  - `eg_validator_add_pattern()` - Expressão regular
  - `eg_validator_add_range_int()` / `eg_validator_add_range_double()` - Range numérico
  - `eg_validator_add_custom()` - Validador customizado via callback
- **Integração com Widgets**:
  - `eg_widget_set_validators()` - Associa validadores a um widget
  - `eg_widget_validate()` - Valida e mostra erro visual (CSS + tooltip)
  - `eg_widget_validate_on_focus_out()` - Validação automática ao perder foco
  - `eg_validate_all()` - Valida múltiplos widgets de uma vez

### API Genérica de Containers
- **Polimorfismo via VTable**: Opera em qualquer container de forma genérica
- `eg_widget_is_container()` - Verifica se widget é container
- `eg_container_add()` / `eg_container_remove()` - Adiciona/remove filhos
- `eg_container_add_named()` - Adiciona filho com nome (Stack, Notebook)
- `eg_container_get_child_count()` / `eg_container_get_child_at()` - Itera filhos
- `eg_container_clear()` - Remove todos os filhos
- `eg_container_supports_multiple()` / `eg_container_supports_named()` - Verifica capacidades

### Sistema de Eventos
- **EgSignal**: Sistema de eventos com múltiplos listeners
- **Eventos**: Teclado, mouse (motion, enter, leave, scroll, click), foco
- **Clipboard**: `eg_clipboard_set_text()`, `eg_clipboard_get_text()`
- **Drag and Drop**: `eg_widget_set_drag_source()`, `eg_widget_set_drop_target()`

### Utilitários
- **Timers**: `eg_timeout_add()`, `eg_idle_add()`
- **CSS**: `eg_css_load_from_file()`, `eg_css_load_from_string()`
- **Tooltip**: `eg_widget_set_tooltip()`, `eg_widget_set_tooltip_markup()`
- **Cursor**: `eg_widget_set_cursor()`
- **Opacity**: `eg_widget_set_opacity()`, `eg_widget_get_opacity()`
- **Nome/ID**: `eg_widget_set_name()`, `eg_widget_get_name()`
- **Logging**: `eg_log_debug()`, `eg_log_info()`, `eg_log_warning()`, `eg_log_error()`
- **Type Casting**: `eg_widget_cast_to_*()`, `eg_widget_is_type()`, `eg_widget_is_container()`
- **Ref-counting**: `eg_widget_ref()`, `eg_widget_unref()`

### Window Controls (Custom Titlebar)
- **HeaderBar**: `eg_header_bar_new()`, `eg_header_bar_set_title_widget()`, `eg_header_bar_pack_start/end()`
- **Custom Titlebar**: `eg_window_set_titlebar()` - Use custom HeaderBar instead of system decorations
- **Window Control**: `eg_window_minimize()`, `eg_window_maximize()`, `eg_window_toggle_maximize()`
- **Fullscreen**: `eg_window_fullscreen()`, `eg_window_unfullscreen()`, `eg_window_is_fullscreen()`
- **Decoration**: `eg_window_set_decorated()` - Enable/disable system window decorations

## Exemplos

Após compilar, os exemplos estarão em `build/`:

```bash
./build/hello_world           # Exemplo básico
./build/form_example          # Formulário com grid
./build/counter_example       # Contador simples
./build/mvvm_example          # Demonstração MVVM
./build/spa_example           # SPA completa com MVVM, binding e computed properties
./build/widgets_example       # CheckButton, Switch, ProgressBar
./build/dialogs_example       # Diálogos, ComboBox, ScrolledWindow
./build/events_example        # Eventos de teclado, mouse, foco, clipboard
./build/dnd_example           # Drag and Drop
./build/input_widgets_example # SpinButton, Scale, TextView, Frame
./build/containers_example    # Stack, Notebook, Paned, Expander
./build/menu_example          # PopoverMenu, MenuButton
./build/menubar_example       # Barra de menu tradicional (File, Edit, View, Help)
./build/listview_example      # ListView e ColumnView (tabela)
./build/dashboard_example     # Dashboard com gráficos e métricas
./build/validation_example    # Sistema de validação de formulários
./build/container_api_example # API genérica de containers via vtable
```

**Destaque**: O `spa_example` demonstra uma aplicação completa estilo SPA (Single Page Application) com:
- Navegação entre múltiplas páginas (Home, Products, Profile)
- Data binding declarativo (two-way e one-way)
- Computed properties (estatísticas calculadas automaticamente)
- Commands com validação can_execute
- Formulários reativos

Veja `examples/SPA_README.md` para mais detalhes.

## Licença

MIT License
