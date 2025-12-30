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
- **Gerenciamento de Memória**: Sistema simplificado de ciclo de vida dos widgets

## Estrutura do Projeto

```
easygtk/
├── include/easygtk/    # Headers públicos
│   ├── easygtk.h       # Header principal
│   ├── types.h         # Tipos e enums
│   ├── signal.h        # Sistema de signals
│   ├── property.h      # Properties observáveis e computed properties
│   ├── model.h         # Model (camada de dados)
│   ├── viewmodel.h     # ViewModel + Commands
│   ├── binding.h       # Data binding declarativo
│   ├── timer.h         # Timers e idle callbacks
│   ├── css.h           # Carregamento de CSS
│   └── *.h             # Widgets (window, button, label, etc.)
├── src/
│   ├── internal/       # Headers internos e vtables
│   ├── core/           # Core da biblioteca
│   └── widgets/        # Implementação dos widgets
└── examples/           # Exemplos de uso
```

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
  - `eg_bind_entry_text()` - Two-way binding para Entry
  - `eg_bind_label_text()` - One-way binding para Label (auto-converte tipos)
  - `eg_bind_check_button_active()` - Two-way para CheckButton
  - `eg_bind_switch_active()` - Two-way para Switch
  - `eg_bind_spin_button_value()` - Two-way para SpinButton
  - `eg_bind_scale_value()` - Two-way para Scale
  - `eg_bind_widget_visible()` - One-way para visibilidade
  - `eg_bind_widget_sensitive()` - One-way para habilitado/desabilitado
  - `eg_bind_button_command()` - Liga botão a command com auto-enable/disable

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
