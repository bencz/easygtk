/**
 * EasyGTK - Tipos públicos
 * 
 * Define os tipos opacos e estruturas públicas da biblioteca.
 * Não expõe detalhes de implementação ao usuário.
 */

#ifndef EASYGTK_TYPES_H
#define EASYGTK_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Tipos opacos - implementação escondida */
typedef struct EgApp EgApp;
typedef struct EgWidget EgWidget;
typedef struct EgWindow EgWindow;
typedef struct EgButton EgButton;
typedef struct EgLabel EgLabel;
typedef struct EgEntry EgEntry;
typedef struct EgBox EgBox;
typedef struct EgGrid EgGrid;
typedef struct EgCheckButton EgCheckButton;
typedef struct EgSwitch EgSwitch;
typedef struct EgProgressBar EgProgressBar;
typedef struct EgComboBox EgComboBox;
typedef struct EgScrolledWindow EgScrolledWindow;
typedef struct EgSpinButton EgSpinButton;
typedef struct EgScale EgScale;
typedef struct EgTextView EgTextView;
typedef struct EgStack EgStack;
typedef struct EgStackSwitcher EgStackSwitcher;
typedef struct EgNotebook EgNotebook;
typedef struct EgPaned EgPaned;
typedef struct EgFrame EgFrame;
typedef struct EgExpander EgExpander;
typedef struct EgImage EgImage;
typedef struct EgPicture EgPicture;
typedef struct EgSeparator EgSeparator;
typedef struct EgSpinner EgSpinner;
typedef struct EgLevelBar EgLevelBar;
typedef struct EgHeaderBar EgHeaderBar;
typedef struct EgPopoverMenu EgPopoverMenu;
typedef struct EgMenuButton EgMenuButton;
typedef struct EgListView EgListView;
typedef struct EgColumnView EgColumnView;
typedef struct EgListStore EgListStore;
typedef struct EgSignal EgSignal;
typedef struct EgViewModel EgViewModel;
typedef struct EgCommand EgCommand;
typedef struct EgProperty EgProperty;
typedef struct EgBinding EgBinding;
typedef struct EgCssProvider EgCssProvider;
typedef struct EgMemoryPool EgMemoryPool;
typedef struct EgModel EgModel;
typedef struct EgFileFilter EgFileFilter;
typedef struct EgAboutDialog EgAboutDialog;

/**
 * Tipo da função de comparação para ordenação.
 * Retorna < 0 se a < b, 0 se a == b, > 0 se a > b.
 */
typedef int (*EgListViewCompareFunc)(const char *a, const char *b, void *user_data);

/**
 * Tipo da função de comparação para ordenação de colunas.
 * Retorna < 0 se a < b, 0 se a == b, > 0 se a > b.
 * Os parâmetros são os valores das células na coluna sendo ordenada.
 */
typedef int (*EgColumnViewCompareFunc)(const char *a, const char *b, void *user_data);

/* Enumerações */
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
    EG_WIDGET_TYPE_SCROLLED_WINDOW,
    EG_WIDGET_TYPE_SPIN_BUTTON,
    EG_WIDGET_TYPE_SCALE,
    EG_WIDGET_TYPE_TEXT_VIEW,
    EG_WIDGET_TYPE_STACK,
    EG_WIDGET_TYPE_STACK_SWITCHER,
    EG_WIDGET_TYPE_NOTEBOOK,
    EG_WIDGET_TYPE_PANED,
    EG_WIDGET_TYPE_FRAME,
    EG_WIDGET_TYPE_EXPANDER,
    EG_WIDGET_TYPE_IMAGE,
    EG_WIDGET_TYPE_PICTURE,
    EG_WIDGET_TYPE_SEPARATOR,
    EG_WIDGET_TYPE_SPINNER,
    EG_WIDGET_TYPE_LEVEL_BAR,
    EG_WIDGET_TYPE_HEADER_BAR,
    EG_WIDGET_TYPE_POPOVER_MENU,
    EG_WIDGET_TYPE_MENU_BUTTON,
    EG_WIDGET_TYPE_LIST_VIEW,
    EG_WIDGET_TYPE_COLUMN_VIEW
} EgWidgetType;

/* Modo de seleção para ListView/ColumnView */
typedef enum EgSelectionMode {
    EG_SELECTION_NONE = 0,
    EG_SELECTION_SINGLE = 1,
    EG_SELECTION_MULTIPLE = 2
} EgSelectionMode;

/* Callbacks */
typedef void (*EgCallback)(EgWidget *widget, void *user_data);
typedef void (*EgDestroyNotify)(void *data);

/* Estrutura para margens */
typedef struct EgMargins {
    int top;
    int bottom;
    int start;
    int end;
} EgMargins;

/* Estrutura para tamanho */
typedef struct EgSize {
    int width;
    int height;
} EgSize;

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_TYPES_H */
