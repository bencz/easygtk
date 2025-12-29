# EasyGTK

Uma biblioteca C99 para simplificar o desenvolvimento de interfaces gráficas com GTK4.

## Características

- **API Simples**: Sem macros públicas, apenas funções claras e diretas
- **Componentização**: Sistema baseado em vtables para extensibilidade
- **C99**: Compatível com o padrão C99
- **MVVM/Data Binding**: Properties observáveis e ViewModel integrado
- **Gerenciamento de Memória**: Sistema simplificado de ciclo de vida dos widgets

## Estrutura do Projeto

```
easygtk/
├── include/easygtk/    # Headers públicos
│   ├── easygtk.h       # Header principal
│   ├── types.h         # Tipos e enums
│   ├── signal.h        # Sistema de signals
│   ├── property.h      # Properties observáveis
│   ├── viewmodel.h     # MVVM (ViewModel + Commands)
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

## Exemplo MVVM

```c
#include <easygtk/easygtk.h>

static EgViewModel *vm = NULL;

void on_counter_changed(EgProperty *prop, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    char buf[64];
    snprintf(buf, sizeof(buf), "Count: %d", eg_property_get_int(prop));
    eg_label_set_text(label, buf);
}

void cmd_increment(EgCommand *cmd, void *param, void *data) {
    int val = eg_view_model_get_int(vm, "counter");
    eg_view_model_set_int(vm, "counter", val + 1);
}

void on_activate(EgWidget *widget, void *user_data) {
    EgApp *app = (EgApp *)user_data;
    
    // Criar ViewModel
    vm = eg_view_model_new();
    EgProperty *counter = eg_property_new_int("counter", 0);
    eg_view_model_add_property(vm, counter);
    
    // Criar Command
    EgCommand *cmd = eg_command_new("increment", cmd_increment, NULL, NULL);
    eg_view_model_add_command(vm, cmd);
    
    // UI
    EgWindow *window = eg_window_new(app, "MVVM Demo", 300, 200);
    EgBox *box = eg_box_new_vertical(10);
    
    EgLabel *label = eg_label_new("Count: 0");
    eg_property_on_changed(counter, on_counter_changed, label);
    
    EgButton *button = eg_button_new("Increment");
    eg_button_on_click(button, (EgCallback)cmd_increment, NULL);
    
    eg_box_append(box, eg_label_as_widget(label));
    eg_box_append(box, eg_button_as_widget(button));
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

- **EgWindow**: Janela principal
- **EgButton**: Botão
- **EgLabel**: Texto estático
- **EgEntry**: Campo de entrada de texto
- **EgBox**: Container linear (horizontal/vertical)
- **EgGrid**: Container em grade
- **EgCheckButton**: Checkbox (suporta agrupamento como radio buttons)
- **EgSwitch**: Toggle switch liga/desliga
- **EgProgressBar**: Barra de progresso (determinada e indeterminada)
- **EgComboBox**: Dropdown de seleção
- **EgScrolledWindow**: Container com barras de rolagem

## Diálogos

- **eg_dialog_info()**: Diálogo de informação
- **eg_dialog_warning()**: Diálogo de aviso
- **eg_dialog_error()**: Diálogo de erro
- **eg_dialog_question()**: Diálogo Sim/Não com callback
- **eg_dialog_confirm()**: Diálogo OK/Cancelar com callback

## Core Features

- **EgSignal**: Sistema de eventos com múltiplos listeners
- **EgProperty**: Properties observáveis com data binding
- **EgViewModel**: Base para padrão MVVM
- **EgCommand**: Commands para ações (ICommand pattern)
- **Timers**: `eg_timeout_add()`, `eg_idle_add()`
- **CSS**: `eg_css_load_from_file()`, `eg_css_load_from_string()`
- **Tooltip**: `eg_widget_set_tooltip()`, `eg_widget_set_tooltip_markup()`
- **Eventos**: Teclado, mouse (motion, enter, leave, scroll, click), foco
- **Clipboard**: `eg_clipboard_set_text()`, `eg_clipboard_get_text()`
- **Cursor**: `eg_widget_set_cursor()`
- **Opacity**: `eg_widget_set_opacity()`, `eg_widget_get_opacity()`
- **Nome/ID**: `eg_widget_set_name()`, `eg_widget_get_name()`
- **Drag and Drop**: `eg_widget_set_drag_source()`, `eg_widget_set_drop_target()`
- **Logging**: `eg_log_debug()`, `eg_log_info()`, `eg_log_warning()`, `eg_log_error()`
- **Type Casting**: `eg_widget_cast_to_*()`, `eg_widget_is_type()`, `eg_widget_is_container()`
- **Ref-counting**: `eg_widget_ref()`, `eg_widget_unref()`

## Exemplos

Após compilar, os exemplos estarão em `build/`:

```bash
./build/hello_world      # Exemplo básico
./build/form_example     # Formulário com grid
./build/counter_example  # Contador simples
./build/mvvm_example     # Demonstração MVVM
./build/widgets_example  # CheckButton, Switch, ProgressBar
./build/dialogs_example  # Diálogos, ComboBox, ScrolledWindow
./build/events_example   # Eventos de teclado, mouse, foco, clipboard
```

## Licença

MIT License
