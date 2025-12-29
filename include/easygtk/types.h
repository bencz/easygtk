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
    EG_WIDGET_TYPE_SCROLLED_WINDOW
} EgWidgetType;

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
