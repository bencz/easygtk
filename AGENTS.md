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

---

## Arquitetura Interna (VTables)

### Conceito

A biblioteca usa **VTables (Virtual Tables)** para implementar polimorfismo em C. Isso permite:
- Evitar switches/ifs para determinar tipo de widget
- Cada widget define suas próprias implementações de funções
- Extensibilidade: novos widgets podem ser adicionados sem modificar código existente
- O tipo do widget é determinado pela vtable, não por um campo enum

### Estrutura Base (EgWidget)

Todo widget herda de `EgWidget` que contém um ponteiro para sua vtable:

```c
/* src/internal/internal.h */
struct EgWidget {
    EgWidgetType type;              /* Tipo (redundante, vtable->type é preferido) */
    GtkWidget *native;              /* Widget GTK nativo */
    const EgWidgetVTable *vtable;   /* Ponteiro para vtable do tipo */
    void *user_data;
    EgDestroyNotify user_data_destroy;
    uint32_t ref_count;             /* Contagem de referências */
    bool is_floating;               /* Estado floating para ref-counting */
};
```

### VTable Base

```c
/* src/internal/vtable.h */
typedef struct EgWidgetVTable {
    EgWidgetType type;              /* Identificador do tipo */
    const char *type_name;          /* Nome legível ("EgButton", "EgLabel", etc.) */
    
    /* Funções virtuais */
    void (*destroy)(EgWidget *widget);
    void *(*get_native)(EgWidget *widget);
    void (*set_visible)(EgWidget *widget, bool visible);
    bool (*get_visible)(EgWidget *widget);
    void (*set_sensitive)(EgWidget *widget, bool sensitive);
    bool (*get_sensitive)(EgWidget *widget);
} EgWidgetVTable;

/* VTable estendida para containers */
typedef struct EgContainerVTable {
    EgWidgetVTable base;
    void (*add_child)(EgWidget *container, EgWidget *child);
    void (*remove_child)(EgWidget *container, EgWidget *child);
} EgContainerVTable;
```

### Como Cada Widget Define Sua VTable

Cada widget define uma vtable estática com suas implementações:

```c
/* Exemplo: src/widgets/button.c */
static void button_destroy(EgWidget *widget);
static void *button_get_native(EgWidget *widget);
/* ... outras funções ... */

const EgWidgetVTable eg_button_vtable = {
    .type = EG_WIDGET_TYPE_BUTTON,
    .type_name = "EgButton",
    .destroy = button_destroy,
    .get_native = button_get_native,
    .set_visible = button_set_visible,
    .get_visible = button_get_visible,
    .set_sensitive = button_set_sensitive,
    .get_sensitive = button_get_sensitive
};
```

### Uso da VTable

**Para obter o nome do tipo** (sem switch):
```c
const char *name = widget->vtable->type_name;  /* "EgButton", "EgLabel", etc. */
```

**Para verificar tipo** (sem switch):
```c
if (widget->vtable->type == EG_WIDGET_TYPE_BUTTON) { ... }
/* Ou usando helper: */
if (eg_widget_is_type(widget, EG_WIDGET_TYPE_BUTTON)) { ... }
```

**Para chamar função virtual**:
```c
widget->vtable->destroy(widget);  /* Chama destrutor correto */
```

### Criando Novo Widget

Para criar um novo tipo de widget:

1. **Definir estrutura** em `internal.h`:
```c
struct EgMyWidget {
    EgWidget base;  /* DEVE ser primeiro campo */
    /* campos específicos */
};
```

2. **Definir vtable** no arquivo .c:
```c
const EgWidgetVTable eg_my_widget_vtable = {
    .type = EG_WIDGET_TYPE_MY_WIDGET,
    .type_name = "EgMyWidget",
    .destroy = my_widget_destroy,
    /* ... */
};
```

3. **Inicializar widget** com `eg_widget_init`:
```c
EgMyWidget *eg_my_widget_new(void) {
    EgMyWidget *w = EG_ALLOC(EgMyWidget);
    GtkWidget *native = gtk_..._new();
    eg_widget_init(&w->base, EG_WIDGET_TYPE_MY_WIDGET, native, &eg_my_widget_vtable);
    return w;
}
```

### Vantagens da Arquitetura

1. **Sem switches gigantes** - Cada tipo sabe como se comportar
2. **Type-safe casting** - `eg_widget_cast_to_button()` usa vtable->type
3. **Extensível** - Novos widgets não modificam código existente
4. **Performance** - Chamada de função via ponteiro é O(1)
5. **Debug fácil** - `vtable->type_name` sempre disponível

---

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

### SpinButton (EgSpinButton)

Campo numérico com botões de incremento/decremento.

```c
EgSpinButton *eg_spin_button_new(double min, double max, double step);
EgSpinButton *eg_spin_button_new_int(int min, int max, int step);
void eg_spin_button_free(EgSpinButton *spin_button);

void eg_spin_button_set_value(EgSpinButton *spin_button, double value);
double eg_spin_button_get_value(EgSpinButton *spin_button);
int eg_spin_button_get_value_as_int(EgSpinButton *spin_button);

void eg_spin_button_set_range(EgSpinButton *spin_button, double min, double max);
void eg_spin_button_set_increments(EgSpinButton *spin_button, double step, double page);
void eg_spin_button_set_digits(EgSpinButton *spin_button, unsigned int digits);
void eg_spin_button_set_wrap(EgSpinButton *spin_button, bool wrap);

void eg_spin_button_on_value_changed(EgSpinButton *spin_button, EgCallback callback, void *user_data);

EgWidget *eg_spin_button_as_widget(EgSpinButton *spin_button);
```

**Exemplo:**
```c
EgSpinButton *spin = eg_spin_button_new_int(0, 100, 1);
eg_spin_button_set_value(spin, 50);

static void on_value_changed(EgWidget *widget, void *user_data) {
    EgSpinButton *s = (EgSpinButton *)widget;
    printf("Valor: %d\n", eg_spin_button_get_value_as_int(s));
}
eg_spin_button_on_value_changed(spin, on_value_changed, NULL);
```

---

### Scale (EgScale)

Slider para seleção de valores numéricos.

```c
EgScale *eg_scale_new(EgOrientation orientation, double min, double max, double step);
EgScale *eg_scale_new_horizontal(double min, double max, double step);
EgScale *eg_scale_new_vertical(double min, double max, double step);
void eg_scale_free(EgScale *scale);

void eg_scale_set_value(EgScale *scale, double value);
double eg_scale_get_value(EgScale *scale);

void eg_scale_set_range(EgScale *scale, double min, double max);
void eg_scale_set_draw_value(EgScale *scale, bool draw_value);
void eg_scale_set_digits(EgScale *scale, int digits);
void eg_scale_add_mark(EgScale *scale, double value, int position, const char *markup);
void eg_scale_clear_marks(EgScale *scale);

void eg_scale_on_value_changed(EgScale *scale, EgCallback callback, void *user_data);

EgWidget *eg_scale_as_widget(EgScale *scale);
```

**Exemplo:**
```c
EgScale *scale = eg_scale_new_horizontal(0.0, 100.0, 1.0);
eg_scale_set_value(scale, 50.0);
eg_scale_set_draw_value(scale, true);
eg_scale_add_mark(scale, 0, 0, "Min");
eg_scale_add_mark(scale, 100, 0, "Max");
```

---

### TextView (EgTextView)

Campo de texto multi-linha.

```c
EgTextView *eg_text_view_new(void);
void eg_text_view_free(EgTextView *text_view);

void eg_text_view_set_text(EgTextView *text_view, const char *text);
char *eg_text_view_get_text(EgTextView *text_view);  /* Deve ser liberado com free() */

void eg_text_view_set_editable(EgTextView *text_view, bool editable);
bool eg_text_view_get_editable(EgTextView *text_view);

void eg_text_view_set_wrap_mode(EgTextView *text_view, int mode);
```
`mode`: 0=none, 1=char, 2=word, 3=word_char

```c
void eg_text_view_set_monospace(EgTextView *text_view, bool monospace);
void eg_text_view_on_changed(EgTextView *text_view, EgCallback callback, void *user_data);

EgWidget *eg_text_view_as_widget(EgTextView *text_view);
```

**Exemplo:**
```c
EgTextView *tv = eg_text_view_new();
eg_text_view_set_text(tv, "Texto\nmulti-linha");
eg_text_view_set_wrap_mode(tv, 2); /* word wrap */

/* Coloque dentro de ScrolledWindow para scroll */
EgScrolledWindow *scroll = eg_scrolled_window_new();
eg_scrolled_window_set_child(scroll, eg_text_view_as_widget(tv));
```

---

### Stack (EgStack)

Container que mostra apenas um filho por vez, com transições animadas.

```c
EgStack *eg_stack_new(void);
void eg_stack_free(EgStack *stack);

void eg_stack_add_named(EgStack *stack, EgWidget *child, const char *name);
void eg_stack_add_titled(EgStack *stack, EgWidget *child, const char *name, const char *title);
void eg_stack_remove(EgStack *stack, EgWidget *child);

void eg_stack_set_visible_child_name(EgStack *stack, const char *name);
const char *eg_stack_get_visible_child_name(EgStack *stack);

void eg_stack_set_transition_type(EgStack *stack, EgStackTransition transition);
void eg_stack_set_transition_duration(EgStack *stack, unsigned int duration);

EgWidget *eg_stack_as_widget(EgStack *stack);
```

Tipos de transição: `EG_STACK_TRANSITION_NONE`, `EG_STACK_TRANSITION_CROSSFADE`, `EG_STACK_TRANSITION_SLIDE_LEFT`, `EG_STACK_TRANSITION_SLIDE_RIGHT`, etc.

### StackSwitcher (EgStackSwitcher)

Botões para alternar páginas do Stack.

```c
EgStackSwitcher *eg_stack_switcher_new(void);
void eg_stack_switcher_free(EgStackSwitcher *switcher);
void eg_stack_switcher_set_stack(EgStackSwitcher *switcher, EgStack *stack);
EgWidget *eg_stack_switcher_as_widget(EgStackSwitcher *switcher);
```

**Exemplo:**
```c
EgStack *stack = eg_stack_new();
eg_stack_set_transition_type(stack, EG_STACK_TRANSITION_SLIDE_LEFT_RIGHT);

eg_stack_add_titled(stack, page1, "page1", "Página 1");
eg_stack_add_titled(stack, page2, "page2", "Página 2");

EgStackSwitcher *switcher = eg_stack_switcher_new();
eg_stack_switcher_set_stack(switcher, stack);
```

---

### Notebook (EgNotebook)

Container com abas (tabs).

```c
EgNotebook *eg_notebook_new(void);
void eg_notebook_free(EgNotebook *notebook);

int eg_notebook_append_page(EgNotebook *notebook, EgWidget *child, const char *label);
int eg_notebook_prepend_page(EgNotebook *notebook, EgWidget *child, const char *label);
int eg_notebook_insert_page(EgNotebook *notebook, EgWidget *child, const char *label, int position);
void eg_notebook_remove_page(EgNotebook *notebook, int page_num);

void eg_notebook_set_current_page(EgNotebook *notebook, int page_num);
int eg_notebook_get_current_page(EgNotebook *notebook);
int eg_notebook_get_n_pages(EgNotebook *notebook);

void eg_notebook_set_tab_pos(EgNotebook *notebook, int position);
```
`position`: 0=left, 1=right, 2=top, 3=bottom

```c
void eg_notebook_set_scrollable(EgNotebook *notebook, bool scrollable);
void eg_notebook_on_page_changed(EgNotebook *notebook, EgCallback callback, void *user_data);

EgWidget *eg_notebook_as_widget(EgNotebook *notebook);
```

**Exemplo:**
```c
EgNotebook *notebook = eg_notebook_new();
eg_notebook_append_page(notebook, content1, "Aba 1");
eg_notebook_append_page(notebook, content2, "Aba 2");
eg_notebook_set_current_page(notebook, 0);
```

---

### Paned (EgPaned)

Container dividido em dois painéis redimensionáveis.

```c
EgPaned *eg_paned_new(EgOrientation orientation);
EgPaned *eg_paned_new_horizontal(void);
EgPaned *eg_paned_new_vertical(void);
void eg_paned_free(EgPaned *paned);

void eg_paned_set_start_child(EgPaned *paned, EgWidget *child);
void eg_paned_set_end_child(EgPaned *paned, EgWidget *child);

void eg_paned_set_position(EgPaned *paned, int position);
int eg_paned_get_position(EgPaned *paned);

void eg_paned_set_wide_handle(EgPaned *paned, bool wide);

EgWidget *eg_paned_as_widget(EgPaned *paned);
```

**Exemplo:**
```c
EgPaned *paned = eg_paned_new_horizontal();
eg_paned_set_start_child(paned, left_panel);
eg_paned_set_end_child(paned, right_panel);
eg_paned_set_position(paned, 200); /* divisor em 200px */
eg_paned_set_wide_handle(paned, true);
```

---

### Frame (EgFrame)

Container com borda e título opcional.

```c
EgFrame *eg_frame_new(const char *label);
void eg_frame_free(EgFrame *frame);

void eg_frame_set_child(EgFrame *frame, EgWidget *child);
void eg_frame_set_label(EgFrame *frame, const char *label);
const char *eg_frame_get_label(EgFrame *frame);
void eg_frame_set_label_align(EgFrame *frame, float xalign);

EgWidget *eg_frame_as_widget(EgFrame *frame);
```

**Exemplo:**
```c
EgFrame *frame = eg_frame_new("Configurações");
eg_frame_set_child(frame, settings_box);
```

---

### Expander (EgExpander)

Container expansível/recolhível com título.

```c
EgExpander *eg_expander_new(const char *label);
EgExpander *eg_expander_new_with_markup(const char *markup);
void eg_expander_free(EgExpander *expander);

void eg_expander_set_child(EgExpander *expander, EgWidget *child);
void eg_expander_set_expanded(EgExpander *expander, bool expanded);
bool eg_expander_get_expanded(EgExpander *expander);

void eg_expander_set_label(EgExpander *expander, const char *label);

EgWidget *eg_expander_as_widget(EgExpander *expander);
```

**Exemplo:**
```c
EgExpander *expander = eg_expander_new("Opções avançadas");
eg_expander_set_child(expander, options_box);
eg_expander_set_expanded(expander, false); /* começa recolhido */
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

void eg_widget_set_cursor(EgWidget *widget, const char *cursor_name);
```
Cursores: `"pointer"`, `"crosshair"`, `"text"`, `"wait"`, `"help"`, `"grab"`, `"not-allowed"`, `NULL` para padrão.

```c
void eg_widget_set_opacity(EgWidget *widget, double opacity);
double eg_widget_get_opacity(EgWidget *widget);
```
`opacity` entre 0.0 (transparente) e 1.0 (opaco).

```c
void eg_widget_set_name(EgWidget *widget, const char *name);
const char *eg_widget_get_name(EgWidget *widget);
```
Nome/ID para identificação do widget.

```c
void *eg_widget_get_native(EgWidget *widget);
void eg_widget_free(EgWidget *widget);
```

---

## Sistema de Eventos

Eventos de teclado, mouse e foco usando GtkEventController.

### Tipos de Eventos

```c
typedef enum EgModifierType {
    EG_MODIFIER_NONE = 0,
    EG_MODIFIER_SHIFT = 1 << 0,
    EG_MODIFIER_CTRL = 1 << 1,
    EG_MODIFIER_ALT = 1 << 2,
    EG_MODIFIER_SUPER = 1 << 3
} EgModifierType;

typedef struct EgKeyEvent {
    uint32_t keyval;           /* Código da tecla */
    uint32_t keycode;          /* Código físico */
    EgModifierType modifiers;
    bool is_press;
} EgKeyEvent;

typedef struct EgMouseEvent {
    double x, y;
    EgModifierType modifiers;
} EgMouseEvent;

typedef struct EgScrollEvent {
    double x, y;
    double delta_x, delta_y;
    EgModifierType modifiers;
} EgScrollEvent;

typedef struct EgButtonEvent {
    double x, y;
    uint32_t button;           /* 1=esquerdo, 2=meio, 3=direito */
    uint32_t n_press;          /* 1=simples, 2=duplo */
    EgModifierType modifiers;
} EgButtonEvent;
```

### Callbacks

```c
typedef bool (*EgKeyCallback)(EgWidget *widget, EgKeyEvent *event, void *user_data);
typedef void (*EgMotionCallback)(EgWidget *widget, EgMouseEvent *event, void *user_data);
typedef void (*EgScrollCallback)(EgWidget *widget, EgScrollEvent *event, void *user_data);
typedef bool (*EgButtonCallback)(EgWidget *widget, EgButtonEvent *event, void *user_data);
typedef void (*EgFocusCallback)(EgWidget *widget, void *user_data);
typedef void (*EgCrossingCallback)(EgWidget *widget, double x, double y, void *user_data);
```

### Funções de Eventos

```c
void eg_widget_on_key_press(EgWidget *widget, EgKeyCallback callback, void *user_data);
void eg_widget_on_key_release(EgWidget *widget, EgKeyCallback callback, void *user_data);

void eg_widget_on_motion(EgWidget *widget, EgMotionCallback callback, void *user_data);
void eg_widget_on_enter(EgWidget *widget, EgCrossingCallback callback, void *user_data);
void eg_widget_on_leave(EgWidget *widget, EgCrossingCallback callback, void *user_data);
void eg_widget_on_scroll(EgWidget *widget, EgScrollCallback callback, void *user_data);
void eg_widget_on_click(EgWidget *widget, EgButtonCallback callback, void *user_data);

void eg_widget_on_focus_in(EgWidget *widget, EgFocusCallback callback, void *user_data);
void eg_widget_on_focus_out(EgWidget *widget, EgFocusCallback callback, void *user_data);

void eg_widget_set_focusable(EgWidget *widget, bool focusable);
bool eg_widget_get_focusable(EgWidget *widget);
bool eg_widget_grab_focus(EgWidget *widget);
bool eg_widget_has_focus(EgWidget *widget);
```

### Constantes de Teclas

```c
#define EG_KEY_Return     0xff0d
#define EG_KEY_Escape     0xff1b
#define EG_KEY_Tab        0xff09
#define EG_KEY_BackSpace  0xff08
#define EG_KEY_Delete     0xffff
#define EG_KEY_Left       0xff51
#define EG_KEY_Up         0xff52
#define EG_KEY_Right      0xff53
#define EG_KEY_Down       0xff54
#define EG_KEY_F1         0xffbe  /* até F12 = 0xffc9 */
#define EG_KEY_space      0x020
```

**Exemplo:**
```c
static bool on_key_press(EgWidget *widget, EgKeyEvent *event, void *user_data) {
    (void)widget; (void)user_data;
    
    if (event->keyval == EG_KEY_Escape) {
        printf("Escape pressionado!\n");
    }
    
    if ((event->modifiers & EG_MODIFIER_CTRL) && event->keyval == 's') {
        printf("Ctrl+S pressionado!\n");
        return true; /* Bloqueia propagação */
    }
    
    return false;
}

eg_widget_on_key_press(eg_window_as_widget(window), on_key_press, NULL);
```

---

## Clipboard

Sistema de área de transferência.

```c
typedef void (*EgClipboardTextCallback)(const char *text, void *user_data);

void eg_clipboard_set_text(EgWidget *widget, const char *text);
void eg_clipboard_get_text(EgWidget *widget, EgClipboardTextCallback callback, void *user_data);
void eg_clipboard_clear(EgWidget *widget);
```

**Exemplo:**
```c
/* Copiar */
eg_clipboard_set_text(eg_button_as_widget(btn), "Texto copiado");

/* Colar (assíncrono) */
static void on_paste(const char *text, void *user_data) {
    if (text != NULL) {
        printf("Colado: %s\n", text);
    }
}
eg_clipboard_get_text(eg_button_as_widget(btn), on_paste, NULL);
```

---

## Drag and Drop

Sistema de arrastar e soltar.

```c
typedef enum EgDragAction {
    EG_DRAG_ACTION_COPY = 1 << 0,
    EG_DRAG_ACTION_MOVE = 1 << 1,
    EG_DRAG_ACTION_LINK = 1 << 2
} EgDragAction;

typedef const char *(*EgDragPrepareCallback)(EgWidget *widget, void *user_data);
typedef void (*EgDropCallback)(EgWidget *widget, const char *data, double x, double y, void *user_data);

void eg_widget_set_drag_source(EgWidget *widget, EgDragAction actions,
                                EgDragPrepareCallback prepare_callback, void *user_data);
void eg_widget_set_drop_target(EgWidget *widget, EgDragAction actions,
                                EgDropCallback drop_callback, void *user_data);
```

**Exemplo:**
```c
/* Fonte de drag */
static const char *on_drag_prepare(EgWidget *widget, void *user_data) {
    (void)widget; (void)user_data;
    return "Texto arrastado";
}
eg_widget_set_drag_source(eg_label_as_widget(label), EG_DRAG_ACTION_COPY, on_drag_prepare, NULL);

/* Destino de drop */
static void on_drop(EgWidget *widget, const char *data, double x, double y, void *user_data) {
    (void)widget; (void)x; (void)y; (void)user_data;
    printf("Recebido: %s\n", data);
}
eg_widget_set_drop_target(eg_box_as_widget(box), EG_DRAG_ACTION_COPY, on_drop, NULL);
```

---

## Logging e Debug

Sistema de logging com níveis e cores.

```c
typedef enum EgLogLevel {
    EG_LOG_LEVEL_DEBUG = 0,
    EG_LOG_LEVEL_INFO = 1,
    EG_LOG_LEVEL_WARNING = 2,
    EG_LOG_LEVEL_ERROR = 3,
    EG_LOG_LEVEL_NONE = 4
} EgLogLevel;

void eg_log_set_level(EgLogLevel level);
EgLogLevel eg_log_get_level(void);
void eg_log_set_colors(bool enabled);

void eg_log_debug(const char *domain, const char *format, ...);
void eg_log_info(const char *domain, const char *format, ...);
void eg_log_warning(const char *domain, const char *format, ...);
void eg_log_error(const char *domain, const char *format, ...);

void eg_widget_debug_print(EgWidget *widget);
```

**Exemplo:**
```c
eg_log_set_level(EG_LOG_LEVEL_DEBUG);
eg_log_debug("MyApp", "Iniciando aplicação");
eg_log_info("MyApp", "Usuário: %s", username);
eg_log_warning("MyApp", "Arquivo não encontrado");
eg_log_error("MyApp", "Falha crítica!");

eg_widget_debug_print(eg_button_as_widget(btn)); /* Imprime info do widget */
```

---

## Type Casting

Conversão segura de tipos usando vtable.

```c
EgWindow *eg_widget_cast_to_window(EgWidget *widget);
EgButton *eg_widget_cast_to_button(EgWidget *widget);
EgLabel *eg_widget_cast_to_label(EgWidget *widget);
EgEntry *eg_widget_cast_to_entry(EgWidget *widget);
EgBox *eg_widget_cast_to_box(EgWidget *widget);
EgGrid *eg_widget_cast_to_grid(EgWidget *widget);
EgCheckButton *eg_widget_cast_to_check_button(EgWidget *widget);
EgSwitch *eg_widget_cast_to_switch(EgWidget *widget);
EgProgressBar *eg_widget_cast_to_progress_bar(EgWidget *widget);
EgComboBox *eg_widget_cast_to_combo_box(EgWidget *widget);
EgScrolledWindow *eg_widget_cast_to_scrolled_window(EgWidget *widget);

bool eg_widget_is_type(EgWidget *widget, EgWidgetType type);
bool eg_widget_is_container(EgWidget *widget);
```

**Exemplo:**
```c
void process_widget(EgWidget *widget) {
    EgButton *btn = eg_widget_cast_to_button(widget);
    if (btn != NULL) {
        eg_button_set_label(btn, "Novo texto");
        return;
    }
    
    if (eg_widget_is_container(widget)) {
        printf("É um container!\n");
    }
}
```

---

## Reference Counting

Sistema opcional de contagem de referências.

```c
EgWidget *eg_widget_ref(EgWidget *widget);
void eg_widget_unref(EgWidget *widget);
uint32_t eg_widget_get_ref_count(EgWidget *widget);

void eg_widget_set_floating(EgWidget *widget, bool floating);
bool eg_widget_is_floating(EgWidget *widget);
EgWidget *eg_widget_ref_sink(EgWidget *widget);
```

**Exemplo:**
```c
EgButton *btn = eg_button_new("Test");
eg_widget_ref(eg_button_as_widget(btn));  /* ref_count = 2 */

/* ... usar o widget ... */

eg_widget_unref(eg_button_as_widget(btn)); /* ref_count = 1 */
eg_widget_unref(eg_button_as_widget(btn)); /* ref_count = 0, destruído */
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
