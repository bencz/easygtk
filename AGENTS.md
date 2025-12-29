# EasyGTK - API Reference for LLM Agents

Este documento contém a referência completa da API EasyGTK para uso por agentes LLM. A biblioteca simplifica o desenvolvimento de interfaces GTK4 em C99.

---

## Índice

1. [Visão Geral](#visão-geral)
2. [Tipos Básicos](#tipos-básicos)
3. [Aplicação (EgApp)](#aplicação-egapp)
4. [Janela (EgWindow)](#janela-egwindow)
5. [Widgets Básicos](#widgets-básicos)
   - [Button](#button-egbutton)
   - [Label](#label-eglabel)
   - [Entry](#entry-egentry)
   - [CheckButton](#checkbutton-egcheckbutton)
   - [Switch](#switch-egswitch)
   - [ProgressBar](#progressbar-egprogressbar)
   - [ComboBox](#combobox-egcombobox)
6. [Containers](#containers)
   - [Box](#box-egbox)
   - [Grid](#grid-eggrid)
   - [ScrolledWindow](#scrolledwindow-egscrolledwindow)
7. [Diálogos](#diálogos)
8. [Sistema de Signals](#sistema-de-signals)
9. [Properties Observáveis](#properties-observáveis)
10. [ViewModel (MVVM)](#viewmodel-mvvm)
11. [Timers](#timers)
12. [CSS](#css)
13. [Funções Genéricas de Widget](#funções-genéricas-de-widget)
14. [Padrões de Uso](#padrões-de-uso)
15. [Exemplos Completos](#exemplos-completos)

---

## Visão Geral

**EasyGTK** é uma biblioteca C99 que encapsula GTK4 com uma API simplificada:

- **Sem macros públicas** - API baseada apenas em funções
- **Tipos opacos** - Implementação escondida do usuário
- **VTables** - Polimorfismo em C para extensibilidade
- **MVVM** - Suporte a properties observáveis e data binding

### Estrutura de Projeto

```
easygtk/
├── include/easygtk/    # Headers públicos
│   ├── easygtk.h       # Header principal (inclui todos)
│   ├── types.h         # Tipos e enums
│   ├── app.h           # Aplicação
│   ├── window.h        # Janela
│   ├── button.h        # Botão
│   ├── label.h         # Label
│   ├── entry.h         # Campo de texto
│   ├── box.h           # Container linear
│   ├── grid.h          # Container em grade
│   ├── check_button.h  # Checkbox
│   ├── switch.h        # Toggle switch
│   ├── progress_bar.h  # Barra de progresso
│   ├── combo_box.h     # Dropdown
│   ├── scrolled_window.h # Container com scroll
│   ├── dialog.h        # Diálogos
│   ├── signal.h        # Sistema de signals
│   ├── property.h      # Properties observáveis
│   ├── viewmodel.h     # MVVM
│   ├── timer.h         # Timers
│   └── css.h           # CSS
├── src/
│   ├── internal/       # Headers internos
│   ├── core/           # Implementações core
│   └── widgets/        # Implementações de widgets
└── examples/           # Exemplos de uso
```

### Compilação

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Uso Básico

```c
#include <easygtk/easygtk.h>

static void on_activate(EgWidget *widget, void *user_data) {
    EgApp *app = (EgApp *)user_data;
    EgWindow *window = eg_window_new(app, "Título", 400, 300);
    // ... criar widgets ...
    eg_window_show(window);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.app");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_app_free(app);
    return status;
}
```

---

## Tipos Básicos

### Enumerações

```c
typedef enum EgOrientation {
    EG_ORIENTATION_HORIZONTAL = 0,
    EG_ORIENTATION_VERTICAL = 1
} EgOrientation;

typedef enum EgAlign {
    EG_ALIGN_FILL = 0,
    EG_ALIGN_START = 1,
    EG_ALIGN_END = 2,
    EG_ALIGN_CENTER = 3
} EgAlign;

typedef enum EgWidgetType {
    EG_WIDGET_TYPE_BASE = 0,
    EG_WIDGET_TYPE_WINDOW,
    EG_WIDGET_TYPE_BUTTON,
    EG_WIDGET_TYPE_LABEL,
    EG_WIDGET_TYPE_ENTRY,
    EG_WIDGET_TYPE_BOX,
    EG_WIDGET_TYPE_GRID,
    EG_WIDGET_TYPE_CHECK_BUTTON,
    EG_WIDGET_TYPE_SWITCH,
    EG_WIDGET_TYPE_PROGRESS_BAR,
    EG_WIDGET_TYPE_COMBO_BOX,
    EG_WIDGET_TYPE_SCROLLED_WINDOW
} EgWidgetType;
```

### Callbacks

```c
typedef void (*EgCallback)(EgWidget *widget, void *user_data);
typedef void (*EgDestroyNotify)(void *data);
```

### Estruturas

```c
typedef struct EgMargins {
    int top;
    int bottom;
    int start;
    int end;
} EgMargins;

typedef struct EgSize {
    int width;
    int height;
} EgSize;
```

---

## Aplicação (EgApp)

Gerencia o ciclo de vida da aplicação GTK.

### Funções

```c
EgApp *eg_app_new(const char *app_id);
```
Cria nova aplicação. `app_id` deve ser no formato reverse-DNS (ex: "com.example.app").

```c
void eg_app_free(EgApp *app);
```
Libera recursos da aplicação.

```c
int eg_app_run(EgApp *app);
```
Executa o loop principal. Retorna código de saída.

```c
void eg_app_quit(EgApp *app);
```
Encerra a aplicação.

```c
void eg_app_on_activate(EgApp *app, EgCallback callback, void *user_data);
```
Define callback chamado quando a aplicação é ativada. **Obrigatório** - é onde você cria a UI.

```c
void *eg_app_get_native(EgApp *app);
```
Retorna ponteiro para `GtkApplication` nativo.

### Exemplo

```c
static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Minha App", 800, 600);
    eg_window_show(window);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.myapp");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_app_free(app);
    return status;
}
```

---

## Janela (EgWindow)

Janela principal da aplicação.

### Funções

```c
EgWindow *eg_window_new(EgApp *app, const char *title, int width, int height);
```
Cria nova janela associada à aplicação.

```c
void eg_window_free(EgWindow *window);
```
Libera recursos da janela.

```c
void eg_window_show(EgWindow *window);
void eg_window_hide(EgWindow *window);
```
Mostra/esconde a janela.

```c
void eg_window_set_title(EgWindow *window, const char *title);
const char *eg_window_get_title(EgWindow *window);
```
Define/obtém título da janela.

```c
void eg_window_set_default_size(EgWindow *window, int width, int height);
void eg_window_set_resizable(EgWindow *window, bool resizable);
```
Configura tamanho e redimensionamento.

```c
void eg_window_set_child(EgWindow *window, EgWidget *child);
```
Define o widget filho (conteúdo) da janela.

```c
void eg_window_on_close(EgWindow *window, EgCallback callback, void *user_data);
void eg_window_on_destroy(EgWindow *window, EgCallback callback, void *user_data);
```
Define callbacks para eventos de fechamento/destruição.

```c
EgWidget *eg_window_as_widget(EgWindow *window);
void *eg_window_get_native(EgWindow *window);
```
Conversões de tipo.

---

## Widgets Básicos

### Button (EgButton)

Botão clicável.

```c
EgButton *eg_button_new(const char *label);
EgButton *eg_button_new_with_icon(const char *icon_name);
void eg_button_free(EgButton *button);

void eg_button_set_label(EgButton *button, const char *label);
const char *eg_button_get_label(EgButton *button);

void eg_button_on_click(EgButton *button, EgCallback callback, void *user_data);

void eg_button_set_sensitive(EgButton *button, bool sensitive);
bool eg_button_get_sensitive(EgButton *button);

EgWidget *eg_button_as_widget(EgButton *button);
void *eg_button_get_native(EgButton *button);
```

**Exemplo:**
```c
static void on_click(EgWidget *widget, void *user_data) {
    (void)widget;
    printf("Botão clicado!\n");
}

EgButton *btn = eg_button_new("Clique aqui");
eg_button_on_click(btn, on_click, NULL);
eg_widget_add_css_class(eg_button_as_widget(btn), "suggested-action");
```

---

### Label (EgLabel)

Texto estático.

```c
EgLabel *eg_label_new(const char *text);
void eg_label_free(EgLabel *label);

void eg_label_set_text(EgLabel *label, const char *text);
const char *eg_label_get_text(EgLabel *label);

void eg_label_set_markup(EgLabel *label, const char *markup);
```
`markup` usa formato Pango: `<span size='large' weight='bold'>Texto</span>`

```c
void eg_label_set_xalign(EgLabel *label, float xalign);
void eg_label_set_yalign(EgLabel *label, float yalign);
```
Alinhamento (0.0 = início, 0.5 = centro, 1.0 = fim).

```c
void eg_label_set_wrap(EgLabel *label, bool wrap);
void eg_label_set_selectable(EgLabel *label, bool selectable);

EgWidget *eg_label_as_widget(EgLabel *label);
void *eg_label_get_native(EgLabel *label);
```

**Exemplo:**
```c
EgLabel *title = eg_label_new(NULL);
eg_label_set_markup(title, "<span size='xx-large' weight='bold'>Título</span>");
eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
```

---

### Entry (EgEntry)

Campo de entrada de texto.

```c
EgEntry *eg_entry_new(void);
EgEntry *eg_entry_new_with_placeholder(const char *placeholder);
void eg_entry_free(EgEntry *entry);

void eg_entry_set_text(EgEntry *entry, const char *text);
const char *eg_entry_get_text(EgEntry *entry);

void eg_entry_set_placeholder(EgEntry *entry, const char *placeholder);
void eg_entry_set_max_length(EgEntry *entry, int max_length);
void eg_entry_set_visibility(EgEntry *entry, bool visible);
```
`visibility=false` para campos de senha.

```c
void eg_entry_on_changed(EgEntry *entry, EgCallback callback, void *user_data);
void eg_entry_on_activate(EgEntry *entry, EgCallback callback, void *user_data);
```
`on_activate` é chamado quando Enter é pressionado.

```c
EgWidget *eg_entry_as_widget(EgEntry *entry);
void *eg_entry_get_native(EgEntry *entry);
```

**Exemplo:**
```c
EgEntry *password = eg_entry_new_with_placeholder("Senha");
eg_entry_set_visibility(password, false);
eg_entry_set_max_length(password, 32);

static void on_submit(EgWidget *widget, void *user_data) {
    EgEntry *entry = (EgEntry *)user_data;
    printf("Texto: %s\n", eg_entry_get_text(entry));
}
eg_entry_on_activate(password, on_submit, password);
```

---

### CheckButton (EgCheckButton)

Checkbox. Pode ser agrupado como radio buttons.

```c
EgCheckButton *eg_check_button_new(const char *label);
EgCheckButton *eg_check_button_new_with_markup(const char *markup);
void eg_check_button_free(EgCheckButton *check_button);

void eg_check_button_set_active(EgCheckButton *check_button, bool active);
bool eg_check_button_get_active(EgCheckButton *check_button);

void eg_check_button_set_label(EgCheckButton *check_button, const char *label);
const char *eg_check_button_get_label(EgCheckButton *check_button);

void eg_check_button_on_toggled(EgCheckButton *check_button, EgCallback callback, void *user_data);

void eg_check_button_set_group(EgCheckButton *check_button, EgCheckButton *group);
```
`set_group` transforma checkboxes em radio buttons (mutuamente exclusivos).

```c
EgWidget *eg_check_button_as_widget(EgCheckButton *check_button);
void *eg_check_button_get_native(EgCheckButton *check_button);
```

**Exemplo - Checkboxes:**
```c
EgCheckButton *cb1 = eg_check_button_new("Opção 1");
EgCheckButton *cb2 = eg_check_button_new("Opção 2");
eg_check_button_set_active(cb1, true);
```

**Exemplo - Radio Buttons:**
```c
EgCheckButton *radio1 = eg_check_button_new("Pequeno");
EgCheckButton *radio2 = eg_check_button_new("Médio");
EgCheckButton *radio3 = eg_check_button_new("Grande");

eg_check_button_set_group(radio2, radio1);
eg_check_button_set_group(radio3, radio1);
eg_check_button_set_active(radio2, true);
```

---

### Switch (EgSwitch)

Toggle switch liga/desliga.

```c
EgSwitch *eg_switch_new(void);
void eg_switch_free(EgSwitch *sw);

void eg_switch_set_active(EgSwitch *sw, bool active);
bool eg_switch_get_active(EgSwitch *sw);

void eg_switch_on_state_changed(EgSwitch *sw, EgCallback callback, void *user_data);

EgWidget *eg_switch_as_widget(EgSwitch *sw);
void *eg_switch_get_native(EgSwitch *sw);
```

**Exemplo:**
```c
static void on_switch_changed(EgWidget *widget, void *user_data) {
    EgSwitch *sw = (EgSwitch *)widget;
    printf("Switch: %s\n", eg_switch_get_active(sw) ? "ON" : "OFF");
}

EgSwitch *sw = eg_switch_new();
eg_switch_on_state_changed(sw, on_switch_changed, NULL);
```

---

### ProgressBar (EgProgressBar)

Barra de progresso.

```c
EgProgressBar *eg_progress_bar_new(void);
void eg_progress_bar_free(EgProgressBar *progress_bar);

void eg_progress_bar_set_fraction(EgProgressBar *progress_bar, double fraction);
double eg_progress_bar_get_fraction(EgProgressBar *progress_bar);
```
`fraction` é valor entre 0.0 e 1.0.

```c
void eg_progress_bar_set_text(EgProgressBar *progress_bar, const char *text);
const char *eg_progress_bar_get_text(EgProgressBar *progress_bar);
void eg_progress_bar_set_show_text(EgProgressBar *progress_bar, bool show_text);

void eg_progress_bar_pulse(EgProgressBar *progress_bar);
void eg_progress_bar_set_pulse_step(EgProgressBar *progress_bar, double fraction);
```
`pulse()` para modo indeterminado (animação de "carregando").

```c
void eg_progress_bar_set_inverted(EgProgressBar *progress_bar, bool inverted);

EgWidget *eg_progress_bar_as_widget(EgProgressBar *progress_bar);
void *eg_progress_bar_get_native(EgProgressBar *progress_bar);
```

**Exemplo - Progresso determinado:**
```c
EgProgressBar *pb = eg_progress_bar_new();
eg_progress_bar_set_show_text(pb, true);
eg_progress_bar_set_fraction(pb, 0.75);
eg_progress_bar_set_text(pb, "75%");
```

**Exemplo - Progresso indeterminado (com timer):**
```c
static bool on_pulse(void *user_data) {
    EgProgressBar *pb = (EgProgressBar *)user_data;
    eg_progress_bar_pulse(pb);
    return true; // continua o timer
}

EgProgressBar *pb = eg_progress_bar_new();
eg_timeout_add(100, on_pulse, pb);
```

---

### ComboBox (EgComboBox)

Dropdown de seleção.

```c
EgComboBox *eg_combo_box_new(void);
EgComboBox *eg_combo_box_new_with_items(const char *const *items);
```
`items` é array NULL-terminated: `{"Item 1", "Item 2", NULL}`

```c
void eg_combo_box_free(EgComboBox *combo_box);

void eg_combo_box_append(EgComboBox *combo_box, const char *text);
void eg_combo_box_clear(EgComboBox *combo_box);

void eg_combo_box_set_active(EgComboBox *combo_box, int index);
int eg_combo_box_get_active(EgComboBox *combo_box);
```
`index` é 0-based. -1 significa nenhum selecionado.

```c
const char *eg_combo_box_get_active_text(EgComboBox *combo_box);

void eg_combo_box_on_changed(EgComboBox *combo_box, EgCallback callback, void *user_data);

EgWidget *eg_combo_box_as_widget(EgComboBox *combo_box);
void *eg_combo_box_get_native(EgComboBox *combo_box);
```

**Exemplo:**
```c
const char *items[] = { "Opção A", "Opção B", "Opção C", NULL };
EgComboBox *combo = eg_combo_box_new_with_items(items);
eg_combo_box_set_active(combo, 0);

static void on_changed(EgWidget *widget, void *user_data) {
    EgComboBox *cb = (EgComboBox *)widget;
    printf("Selecionado: %s\n", eg_combo_box_get_active_text(cb));
}
eg_combo_box_on_changed(combo, on_changed, NULL);
```

---

## Containers

### Box (EgBox)

Container linear (horizontal ou vertical).

```c
EgBox *eg_box_new(EgOrientation orientation, int spacing);
EgBox *eg_box_new_horizontal(int spacing);
EgBox *eg_box_new_vertical(int spacing);
void eg_box_free(EgBox *box);

void eg_box_append(EgBox *box, EgWidget *child);
void eg_box_prepend(EgBox *box, EgWidget *child);
void eg_box_remove(EgBox *box, EgWidget *child);

void eg_box_set_spacing(EgBox *box, int spacing);
int eg_box_get_spacing(EgBox *box);

void eg_box_set_homogeneous(EgBox *box, bool homogeneous);
bool eg_box_get_homogeneous(EgBox *box);

EgWidget *eg_box_as_widget(EgBox *box);
void *eg_box_get_native(EgBox *box);
```

**Exemplo:**
```c
EgBox *vbox = eg_box_new_vertical(10);
eg_widget_set_margin(eg_box_as_widget(vbox), 20);

EgLabel *label = eg_label_new("Texto");
EgButton *button = eg_button_new("Botão");

eg_box_append(vbox, eg_label_as_widget(label));
eg_box_append(vbox, eg_button_as_widget(button));

eg_window_set_child(window, eg_box_as_widget(vbox));
```

---

### Grid (EgGrid)

Container em grade.

```c
EgGrid *eg_grid_new(void);
void eg_grid_free(EgGrid *grid);

void eg_grid_attach(EgGrid *grid, EgWidget *child, int column, int row, int width, int height);
```
`width` e `height` são o span (quantas células o widget ocupa).

```c
void eg_grid_remove(EgGrid *grid, EgWidget *child);

void eg_grid_set_row_spacing(EgGrid *grid, int spacing);
void eg_grid_set_column_spacing(EgGrid *grid, int spacing);

void eg_grid_set_row_homogeneous(EgGrid *grid, bool homogeneous);
void eg_grid_set_column_homogeneous(EgGrid *grid, bool homogeneous);

EgWidget *eg_grid_as_widget(EgGrid *grid);
void *eg_grid_get_native(EgGrid *grid);
```

**Exemplo:**
```c
EgGrid *grid = eg_grid_new();
eg_grid_set_row_spacing(grid, 10);
eg_grid_set_column_spacing(grid, 10);

EgLabel *lbl_name = eg_label_new("Nome:");
EgEntry *entry_name = eg_entry_new();
EgLabel *lbl_email = eg_label_new("Email:");
EgEntry *entry_email = eg_entry_new();

eg_grid_attach(grid, eg_label_as_widget(lbl_name), 0, 0, 1, 1);
eg_grid_attach(grid, eg_entry_as_widget(entry_name), 1, 0, 1, 1);
eg_grid_attach(grid, eg_label_as_widget(lbl_email), 0, 1, 1, 1);
eg_grid_attach(grid, eg_entry_as_widget(entry_email), 1, 1, 1, 1);
```

---

### ScrolledWindow (EgScrolledWindow)

Container com barras de rolagem.

```c
EgScrolledWindow *eg_scrolled_window_new(void);
void eg_scrolled_window_free(EgScrolledWindow *scrolled_window);

void eg_scrolled_window_set_child(EgScrolledWindow *scrolled_window, EgWidget *child);

void eg_scrolled_window_set_policy(EgScrolledWindow *scrolled_window, 
                                    EgScrollPolicy hscroll, EgScrollPolicy vscroll);
```

```c
typedef enum EgScrollPolicy {
    EG_SCROLL_AUTOMATIC = 0,  // Mostra quando necessário
    EG_SCROLL_ALWAYS = 1,     // Sempre mostra
    EG_SCROLL_NEVER = 2       // Nunca mostra
} EgScrollPolicy;
```

```c
void eg_scrolled_window_set_min_content_size(EgScrolledWindow *scrolled_window, 
                                              int width, int height);
void eg_scrolled_window_set_max_content_size(EgScrolledWindow *scrolled_window, 
                                              int width, int height);

void eg_scrolled_window_set_overlay_scrolling(EgScrolledWindow *scrolled_window, bool overlay);

EgWidget *eg_scrolled_window_as_widget(EgScrolledWindow *scrolled_window);
void *eg_scrolled_window_get_native(EgScrolledWindow *scrolled_window);
```

**Exemplo:**
```c
EgScrolledWindow *scroll = eg_scrolled_window_new();
eg_scrolled_window_set_policy(scroll, EG_SCROLL_NEVER, EG_SCROLL_AUTOMATIC);
eg_scrolled_window_set_min_content_size(scroll, -1, 200);
eg_scrolled_window_set_max_content_size(scroll, -1, 400);

EgBox *content = eg_box_new_vertical(5);
// ... adicionar muitos widgets ao content ...

eg_scrolled_window_set_child(scroll, eg_box_as_widget(content));
```

---

## Diálogos

Diálogos de mensagem usando `GtkAlertDialog` (GTK4).

```c
typedef enum EgDialogResponse {
    EG_RESPONSE_NONE = 0,
    EG_RESPONSE_OK = 1,
    EG_RESPONSE_CANCEL = 2,
    EG_RESPONSE_YES = 3,
    EG_RESPONSE_NO = 4,
    EG_RESPONSE_CLOSE = 5
} EgDialogResponse;

typedef void (*EgDialogCallback)(EgDialogResponse response, void *user_data);
```

### Diálogos Simples (sem callback)

```c
void eg_dialog_info(EgWindow *parent, const char *title, const char *message);
void eg_dialog_warning(EgWindow *parent, const char *title, const char *message);
void eg_dialog_error(EgWindow *parent, const char *title, const char *message);
```

### Diálogos com Callback

```c
void eg_dialog_question(EgWindow *parent, const char *title, const char *message,
                        EgDialogCallback callback, void *user_data);
```
Mostra diálogo Sim/Não. Callback recebe `EG_RESPONSE_YES` ou `EG_RESPONSE_NO`.

```c
void eg_dialog_confirm(EgWindow *parent, const char *title, const char *message,
                       EgDialogCallback callback, void *user_data);
```
Mostra diálogo OK/Cancelar. Callback recebe `EG_RESPONSE_YES` (OK) ou `EG_RESPONSE_NO` (Cancelar).

**Exemplo:**
```c
static void on_question_response(EgDialogResponse response, void *user_data) {
    if (response == EG_RESPONSE_YES) {
        printf("Usuário confirmou!\n");
    } else {
        printf("Usuário cancelou.\n");
    }
}

eg_dialog_question(window, "Confirmar", "Deseja continuar?", 
                   on_question_response, NULL);
```

---

## Sistema de Signals

Sistema de eventos com múltiplos listeners.

```c
typedef void (*EgSignalCallback)(void *sender, void *event_data, void *user_data);
typedef uint32_t EgHandlerId;
```

### Funções

```c
EgSignal *eg_signal_new(const char *name);
void eg_signal_free(EgSignal *signal);

EgHandlerId eg_signal_connect(EgSignal *signal, EgSignalCallback callback, void *user_data);
bool eg_signal_disconnect(EgSignal *signal, EgHandlerId handler_id);
void eg_signal_disconnect_all(EgSignal *signal);

void eg_signal_emit(EgSignal *signal, void *sender, void *event_data);

size_t eg_signal_get_handler_count(EgSignal *signal);

void eg_signal_block(EgSignal *signal);
void eg_signal_unblock(EgSignal *signal);
```

**Exemplo:**
```c
static void on_event(void *sender, void *event_data, void *user_data) {
    printf("Evento recebido!\n");
}

EgSignal *my_signal = eg_signal_new("my-event");
EgHandlerId id = eg_signal_connect(my_signal, on_event, NULL);

// Emitir evento
eg_signal_emit(my_signal, NULL, NULL);

// Desconectar
eg_signal_disconnect(my_signal, id);

eg_signal_free(my_signal);
```

---

## Properties Observáveis

Sistema de properties com notificação de mudanças e data binding.

```c
typedef enum EgPropertyType {
    EG_PROPERTY_TYPE_INT,
    EG_PROPERTY_TYPE_DOUBLE,
    EG_PROPERTY_TYPE_BOOL,
    EG_PROPERTY_TYPE_STRING,
    EG_PROPERTY_TYPE_POINTER
} EgPropertyType;

typedef void (*EgPropertyChangedCallback)(EgProperty *property, void *user_data);
```

### Criação

```c
EgProperty *eg_property_new_int(const char *name, int initial_value);
EgProperty *eg_property_new_double(const char *name, double initial_value);
EgProperty *eg_property_new_bool(const char *name, bool initial_value);
EgProperty *eg_property_new_string(const char *name, const char *initial_value);
EgProperty *eg_property_new_pointer(const char *name, void *initial_value);
void eg_property_free(EgProperty *property);
```

### Getters/Setters

```c
int eg_property_get_int(EgProperty *property);
void eg_property_set_int(EgProperty *property, int value);

double eg_property_get_double(EgProperty *property);
void eg_property_set_double(EgProperty *property, double value);

bool eg_property_get_bool(EgProperty *property);
void eg_property_set_bool(EgProperty *property, bool value);

const char *eg_property_get_string(EgProperty *property);
void eg_property_set_string(EgProperty *property, const char *value);

void *eg_property_get_pointer(EgProperty *property);
void eg_property_set_pointer(EgProperty *property, void *value);
```

### Metadados

```c
const char *eg_property_get_name(EgProperty *property);
EgPropertyType eg_property_get_type(EgProperty *property);
```

### Observação

```c
EgHandlerId eg_property_on_changed(EgProperty *property, EgPropertyChangedCallback callback, void *user_data);
bool eg_property_disconnect(EgProperty *property, EgHandlerId handler_id);
EgSignal *eg_property_get_signal(EgProperty *property);
```

### Data Binding

```c
typedef enum EgBindingFlags {
    EG_BINDING_ONE_WAY = 0,
    EG_BINDING_TWO_WAY = 1,
    EG_BINDING_ONE_WAY_TO_SOURCE = 2
} EgBindingFlags;

EgBinding *eg_property_bind(EgProperty *source, EgProperty *target, EgBindingFlags flags);
void eg_binding_unbind(EgBinding *binding);
```

**Exemplo:**
```c
static void on_counter_changed(EgProperty *prop, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    char buf[32];
    snprintf(buf, sizeof(buf), "Valor: %d", eg_property_get_int(prop));
    eg_label_set_text(label, buf);
}

EgProperty *counter = eg_property_new_int("counter", 0);
EgLabel *label = eg_label_new("Valor: 0");

eg_property_on_changed(counter, on_counter_changed, label);

// Quando mudar, o label é atualizado automaticamente
eg_property_set_int(counter, 42);
```

---

## ViewModel (MVVM)

Base para padrão MVVM com properties e commands.

### ViewModel

```c
EgViewModel *eg_view_model_new(void);
void eg_view_model_free(EgViewModel *vm);

bool eg_view_model_add_property(EgViewModel *vm, EgProperty *property);
EgProperty *eg_view_model_get_property(EgViewModel *vm, const char *name);

// Atalhos para int/string/bool
bool eg_view_model_set_int(EgViewModel *vm, const char *name, int value);
int eg_view_model_get_int(EgViewModel *vm, const char *name);

bool eg_view_model_set_string(EgViewModel *vm, const char *name, const char *value);
const char *eg_view_model_get_string(EgViewModel *vm, const char *name);

bool eg_view_model_set_bool(EgViewModel *vm, const char *name, bool value);
bool eg_view_model_get_bool(EgViewModel *vm, const char *name);
```

### Commands

```c
typedef void (*EgCommandExecute)(EgCommand *command, void *parameter, void *user_data);
typedef bool (*EgCommandCanExecute)(EgCommand *command, void *parameter, void *user_data);

EgCommand *eg_command_new(const char *name, EgCommandExecute execute, 
                          EgCommandCanExecute can_execute, void *user_data);
void eg_command_free(EgCommand *command);

void eg_command_execute(EgCommand *command, void *parameter);
bool eg_command_can_execute(EgCommand *command, void *parameter);

void eg_command_raise_can_execute_changed(EgCommand *command);
EgSignal *eg_command_get_can_execute_changed_signal(EgCommand *command);

bool eg_view_model_add_command(EgViewModel *vm, EgCommand *command);
EgCommand *eg_view_model_get_command(EgViewModel *vm, const char *name);
```

**Exemplo MVVM Completo:**
```c
static EgViewModel *vm = NULL;

// Command execute
static void cmd_increment(EgCommand *cmd, void *param, void *data) {
    (void)cmd; (void)param; (void)data;
    int val = eg_view_model_get_int(vm, "counter");
    eg_view_model_set_int(vm, "counter", val + 1);
}

// Callback quando property muda
static void on_counter_changed(EgProperty *prop, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    char buf[32];
    snprintf(buf, sizeof(buf), "Count: %d", eg_property_get_int(prop));
    eg_label_set_text(label, buf);
}

// Wrapper para callback de botão
static void on_btn_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    cmd_increment(NULL, NULL, NULL);
}

void setup_mvvm(EgLabel *label, EgButton *button) {
    vm = eg_view_model_new();
    
    // Adicionar property
    EgProperty *counter = eg_property_new_int("counter", 0);
    eg_view_model_add_property(vm, counter);
    
    // Adicionar command
    EgCommand *cmd = eg_command_new("increment", cmd_increment, NULL, NULL);
    eg_view_model_add_command(vm, cmd);
    
    // Conectar property ao label
    eg_property_on_changed(counter, on_counter_changed, label);
    
    // Conectar botão ao command
    eg_button_on_click(button, on_btn_click, NULL);
}
```

---

## Timers

Timers e callbacks idle.

```c
typedef uint32_t EgTimerId;
typedef bool (*EgTimerCallback)(void *user_data);
```
Callback retorna `true` para continuar, `false` para parar.

```c
EgTimerId eg_timeout_add(uint32_t interval_ms, EgTimerCallback callback, void *user_data);
EgTimerId eg_timeout_add_once(uint32_t interval_ms, EgTimerCallback callback, void *user_data);
```
`timeout_add` repete até callback retornar `false`. `timeout_add_once` executa apenas uma vez.

```c
EgTimerId eg_idle_add(EgTimerCallback callback, void *user_data);
EgTimerId eg_idle_add_once(EgTimerCallback callback, void *user_data);
```
Executa quando o loop de eventos está ocioso.

```c
bool eg_source_remove(EgTimerId timer_id);
```
Remove timer/idle pelo ID.

**Exemplo:**
```c
static bool on_tick(void *user_data) {
    static int count = 0;
    printf("Tick %d\n", ++count);
    return count < 10; // Para após 10 ticks
}

EgTimerId timer = eg_timeout_add(1000, on_tick, NULL); // A cada 1 segundo

// Para cancelar manualmente:
// eg_source_remove(timer);
```

---

## CSS

Carregamento e aplicação de estilos CSS.

```c
EgCssProvider *eg_css_load_from_string(const char *css_string);
EgCssProvider *eg_css_load_from_file(const char *file_path);
void eg_css_provider_free(EgCssProvider *provider);
```
CSS é aplicado globalmente a todos os widgets.

```c
bool eg_widget_set_css(EgWidget *widget, const char *css_string);
```
Aplica CSS específico a um widget.

**Exemplo:**
```c
// CSS global
const char *css = 
    ".my-button { background: #3498db; color: white; }"
    ".my-button:hover { background: #2980b9; }";
EgCssProvider *provider = eg_css_load_from_string(css);

// Aplicar classe ao widget
EgButton *btn = eg_button_new("Estilizado");
eg_widget_add_css_class(eg_button_as_widget(btn), "my-button");

// CSS inline em widget específico
eg_widget_set_css(eg_label_as_widget(label), "label { font-size: 20px; }");
```

---

## Funções Genéricas de Widget

Funções que funcionam com qualquer widget.

```c
EgWidgetType eg_widget_get_type(EgWidget *widget);

void eg_widget_set_visible(EgWidget *widget, bool visible);
bool eg_widget_get_visible(EgWidget *widget);

void eg_widget_set_sensitive(EgWidget *widget, bool sensitive);
bool eg_widget_get_sensitive(EgWidget *widget);
```
`sensitive=false` desabilita o widget.

```c
void eg_widget_set_margins(EgWidget *widget, EgMargins margins);
void eg_widget_set_margin(EgWidget *widget, int margin);
```
`set_margin` aplica mesma margem em todos os lados.

```c
void eg_widget_set_halign(EgWidget *widget, EgAlign align);
void eg_widget_set_valign(EgWidget *widget, EgAlign align);

void eg_widget_set_hexpand(EgWidget *widget, bool expand);
void eg_widget_set_vexpand(EgWidget *widget, bool expand);

void eg_widget_set_size_request(EgWidget *widget, int width, int height);
```
`-1` para tamanho natural.

```c
void eg_widget_add_css_class(EgWidget *widget, const char *class_name);
void eg_widget_remove_css_class(EgWidget *widget, const char *class_name);
```
Classes GTK úteis: `"suggested-action"`, `"destructive-action"`, `"dim-label"`, `"title-1"`, `"title-2"`, etc.

```c
void eg_widget_set_tooltip(EgWidget *widget, const char *tooltip);
void eg_widget_set_tooltip_markup(EgWidget *widget, const char *markup);

void *eg_widget_get_native(EgWidget *widget);
void eg_widget_free(EgWidget *widget);
```

---

## Padrões de Uso

### Padrão Básico de Aplicação

```c
#include <easygtk/easygtk.h>

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    // 1. Criar janela
    EgWindow *window = eg_window_new(app, "Título", 400, 300);
    
    // 2. Criar container principal
    EgBox *main_box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    // 3. Criar widgets
    EgLabel *label = eg_label_new("Hello World");
    EgButton *button = eg_button_new("Click Me");
    
    // 4. Adicionar widgets ao container
    eg_box_append(main_box, eg_label_as_widget(label));
    eg_box_append(main_box, eg_button_as_widget(button));
    
    // 5. Definir container como filho da janela
    eg_window_set_child(window, eg_box_as_widget(main_box));
    
    // 6. Mostrar janela
    eg_window_show(window);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.app");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_app_free(app);
    return status;
}
```

### Padrão de Callback

```c
// Callback deve ter assinatura: void (*)(EgWidget*, void*)
static void on_button_click(EgWidget *widget, void *user_data) {
    (void)widget; // Silencia warning se não usar
    EgLabel *label = (EgLabel *)user_data;
    eg_label_set_text(label, "Clicado!");
}

// Conectar
eg_button_on_click(button, on_button_click, label);
```

### Padrão MVVM

```c
// 1. Criar ViewModel global ou em struct
static EgViewModel *vm = NULL;

// 2. Definir callbacks de command
static void cmd_action(EgCommand *cmd, void *param, void *data) {
    (void)cmd; (void)param; (void)data;
    // Modificar estado via ViewModel
    eg_view_model_set_int(vm, "value", eg_view_model_get_int(vm, "value") + 1);
}

// 3. Definir callback de property changed
static void on_value_changed(EgProperty *prop, void *user_data) {
    EgLabel *label = (EgLabel *)user_data;
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", eg_property_get_int(prop));
    eg_label_set_text(label, buf);
}

// 4. Wrapper para conectar botão ao command
static void on_btn_click(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    cmd_action(NULL, NULL, NULL);
}

// 5. Setup
void setup(EgLabel *label, EgButton *button) {
    vm = eg_view_model_new();
    
    EgProperty *prop = eg_property_new_int("value", 0);
    eg_view_model_add_property(vm, prop);
    
    eg_property_on_changed(prop, on_value_changed, label);
    eg_button_on_click(button, on_btn_click, NULL);
}
```

---

## Exemplos Completos

### Hello World

```c
#include <easygtk/easygtk.h>

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Hello EasyGTK", 300, 200);
    
    EgBox *box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box), 20);
    eg_widget_set_valign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    
    EgLabel *label = eg_label_new("Hello, World!");
    eg_widget_set_halign(eg_label_as_widget(label), EG_ALIGN_CENTER);
    
    eg_box_append(box, eg_label_as_widget(label));
    eg_window_set_child(window, eg_box_as_widget(box));
    eg_window_show(window);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.hello");
    eg_app_on_activate(app, on_activate, app);
    return eg_app_run(app);
}
```

### Formulário

```c
#include <easygtk/easygtk.h>
#include <stdio.h>

static EgEntry *entry_name = NULL;
static EgEntry *entry_email = NULL;

static void on_submit(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    printf("Nome: %s\n", eg_entry_get_text(entry_name));
    printf("Email: %s\n", eg_entry_get_text(entry_email));
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Formulário", 350, 200);
    
    EgGrid *grid = eg_grid_new();
    eg_grid_set_row_spacing(grid, 10);
    eg_grid_set_column_spacing(grid, 10);
    eg_widget_set_margin(eg_grid_as_widget(grid), 20);
    
    EgLabel *lbl_name = eg_label_new("Nome:");
    entry_name = eg_entry_new();
    eg_widget_set_hexpand(eg_entry_as_widget(entry_name), true);
    
    EgLabel *lbl_email = eg_label_new("Email:");
    entry_email = eg_entry_new_with_placeholder("exemplo@email.com");
    eg_widget_set_hexpand(eg_entry_as_widget(entry_email), true);
    
    EgButton *btn_submit = eg_button_new("Enviar");
    eg_button_on_click(btn_submit, on_submit, NULL);
    eg_widget_add_css_class(eg_button_as_widget(btn_submit), "suggested-action");
    
    eg_grid_attach(grid, eg_label_as_widget(lbl_name), 0, 0, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_name), 1, 0, 1, 1);
    eg_grid_attach(grid, eg_label_as_widget(lbl_email), 0, 1, 1, 1);
    eg_grid_attach(grid, eg_entry_as_widget(entry_email), 1, 1, 1, 1);
    eg_grid_attach(grid, eg_button_as_widget(btn_submit), 1, 2, 1, 1);
    
    eg_window_set_child(window, eg_grid_as_widget(grid));
    eg_window_show(window);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.form");
    eg_app_on_activate(app, on_activate, app);
    int status = eg_app_run(app);
    eg_app_free(app);
    return status;
}
```

### Contador com Timer

```c
#include <easygtk/easygtk.h>
#include <stdio.h>

static EgLabel *counter_label = NULL;
static int counter = 0;

static bool on_tick(void *user_data) {
    (void)user_data;
    counter++;
    char buf[32];
    snprintf(buf, sizeof(buf), "Contador: %d", counter);
    eg_label_set_text(counter_label, buf);
    return true; // Continua o timer
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    EgWindow *window = eg_window_new(app, "Timer", 250, 150);
    
    EgBox *box = eg_box_new_vertical(10);
    eg_widget_set_margin(eg_box_as_widget(box), 20);
    eg_widget_set_valign(eg_box_as_widget(box), EG_ALIGN_CENTER);
    
    counter_label = eg_label_new("Contador: 0");
    eg_widget_set_halign(eg_label_as_widget(counter_label), EG_ALIGN_CENTER);
    
    eg_box_append(box, eg_label_as_widget(counter_label));
    eg_window_set_child(window, eg_box_as_widget(box));
    eg_window_show(window);
    
    // Iniciar timer de 1 segundo
    eg_timeout_add(1000, on_tick, NULL);
}

int main(void) {
    EgApp *app = eg_app_new("com.example.timer");
    eg_app_on_activate(app, on_activate, app);
    return eg_app_run(app);
}
```

---

## Notas para Agentes LLM

### Convenções de Nomenclatura

- Prefixo `eg_` para todas as funções públicas
- Prefixo `Eg` para todos os tipos
- Sufixo `_new` para construtores
- Sufixo `_free` para destrutores
- Sufixo `_as_widget` para conversão para `EgWidget*`
- Sufixo `_get_native` para obter ponteiro GTK nativo
- Prefixo `on_` para callbacks de eventos

### Conversão de Tipos

Todos os widgets derivam de `EgWidget`. Para passar um widget específico onde `EgWidget*` é esperado, use a função `*_as_widget()`:

```c
EgButton *button = eg_button_new("OK");
eg_box_append(box, eg_button_as_widget(button));  // Correto
eg_box_append(box, (EgWidget *)button);           // Também funciona
```

### Gerenciamento de Memória

- Widgets são gerenciados pelo GTK quando adicionados a containers
- Chamar `*_free()` apenas em widgets não adicionados a containers
- `EgApp` deve ser liberado com `eg_app_free()` após `eg_app_run()`
- `EgViewModel` deve ser liberado manualmente
- `EgProperty` adicionada a ViewModel é liberada junto com ele

### Callbacks

- Assinatura padrão: `void (*)(EgWidget*, void*)`
- Sempre silenciar parâmetros não usados com `(void)param;`
- Para commands MVVM, criar wrapper com assinatura correta

### Classes CSS GTK Úteis

- `"suggested-action"` - Botão de ação principal (azul)
- `"destructive-action"` - Botão de ação destrutiva (vermelho)
- `"dim-label"` - Label com cor mais clara
- `"title-1"`, `"title-2"`, `"title-3"`, `"title-4"` - Títulos
- `"monospace"` - Fonte monoespaçada
- `"card"` - Estilo de cartão com borda
- `"frame"` - Borda ao redor do widget

### Markup Pango para Labels

```c
eg_label_set_markup(label, "<span size='large'>Grande</span>");
eg_label_set_markup(label, "<span weight='bold'>Negrito</span>");
eg_label_set_markup(label, "<span color='red'>Vermelho</span>");
eg_label_set_markup(label, "<span size='xx-large' weight='bold' color='#3498db'>Combinado</span>");
```

Atributos: `size`, `weight`, `style`, `color`, `background`, `underline`, `strikethrough`, `font_family`

---

*Documentação gerada para EasyGTK v1.0.0*
