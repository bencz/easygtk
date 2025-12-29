/**
 * EasyGTK - Sistema de Eventos
 * 
 * Eventos de teclado, mouse e foco.
 */

#ifndef EASYGTK_EVENTS_H
#define EASYGTK_EVENTS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================
 * Tipos de Eventos
 * ============================================ */

/* Modificadores de teclado */
typedef enum EgModifierType {
    EG_MODIFIER_NONE = 0,
    EG_MODIFIER_SHIFT = 1 << 0,
    EG_MODIFIER_CTRL = 1 << 1,
    EG_MODIFIER_ALT = 1 << 2,
    EG_MODIFIER_SUPER = 1 << 3
} EgModifierType;

/* Dados de evento de teclado */
typedef struct EgKeyEvent {
    uint32_t keyval;           /* Código da tecla (GDK_KEY_*) */
    uint32_t keycode;          /* Código físico da tecla */
    EgModifierType modifiers;  /* Modificadores ativos */
    bool is_press;             /* true = press, false = release */
} EgKeyEvent;

/* Dados de evento de mouse */
typedef struct EgMouseEvent {
    double x;                  /* Posição X relativa ao widget */
    double y;                  /* Posição Y relativa ao widget */
    EgModifierType modifiers;  /* Modificadores ativos */
} EgMouseEvent;

/* Dados de evento de scroll */
typedef struct EgScrollEvent {
    double x;                  /* Posição X */
    double y;                  /* Posição Y */
    double delta_x;            /* Delta horizontal */
    double delta_y;            /* Delta vertical */
    EgModifierType modifiers;  /* Modificadores ativos */
} EgScrollEvent;

/* Dados de evento de botão do mouse */
typedef struct EgButtonEvent {
    double x;                  /* Posição X */
    double y;                  /* Posição Y */
    uint32_t button;           /* Botão (1=esquerdo, 2=meio, 3=direito) */
    uint32_t n_press;          /* Número de cliques (1=simples, 2=duplo) */
    EgModifierType modifiers;  /* Modificadores ativos */
} EgButtonEvent;

/* ============================================
 * Callbacks de Eventos
 * ============================================ */

/* Callback para eventos de teclado. Retorna true para parar propagação. */
typedef bool (*EgKeyCallback)(EgWidget *widget, EgKeyEvent *event, void *user_data);

/* Callback para eventos de movimento do mouse */
typedef void (*EgMotionCallback)(EgWidget *widget, EgMouseEvent *event, void *user_data);

/* Callback para eventos de scroll */
typedef void (*EgScrollCallback)(EgWidget *widget, EgScrollEvent *event, void *user_data);

/* Callback para eventos de botão do mouse. Retorna true para parar propagação. */
typedef bool (*EgButtonCallback)(EgWidget *widget, EgButtonEvent *event, void *user_data);

/* Callback para eventos de foco */
typedef void (*EgFocusCallback)(EgWidget *widget, void *user_data);

/* Callback para eventos enter/leave */
typedef void (*EgCrossingCallback)(EgWidget *widget, double x, double y, void *user_data);

/* ============================================
 * Funções de Eventos de Teclado
 * ============================================ */

/**
 * Define callback para evento de tecla pressionada.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_key_press(EgWidget *widget, EgKeyCallback callback, void *user_data);

/**
 * Define callback para evento de tecla liberada.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_key_release(EgWidget *widget, EgKeyCallback callback, void *user_data);

/* ============================================
 * Funções de Eventos de Mouse
 * ============================================ */

/**
 * Define callback para movimento do mouse.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_motion(EgWidget *widget, EgMotionCallback callback, void *user_data);

/**
 * Define callback para quando o mouse entra no widget.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_enter(EgWidget *widget, EgCrossingCallback callback, void *user_data);

/**
 * Define callback para quando o mouse sai do widget.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_leave(EgWidget *widget, EgCrossingCallback callback, void *user_data);

/**
 * Define callback para scroll do mouse.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_scroll(EgWidget *widget, EgScrollCallback callback, void *user_data);

/**
 * Define callback para clique do mouse.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_click(EgWidget *widget, EgButtonCallback callback, void *user_data);

/* ============================================
 * Funções de Eventos de Foco
 * ============================================ */

/**
 * Define callback para quando o widget ganha foco.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_focus_in(EgWidget *widget, EgFocusCallback callback, void *user_data);

/**
 * Define callback para quando o widget perde foco.
 * 
 * @param widget Widget que receberá o evento
 * @param callback Função de callback
 * @param user_data Dados do usuário
 */
void eg_widget_on_focus_out(EgWidget *widget, EgFocusCallback callback, void *user_data);

/**
 * Define se o widget pode receber foco.
 * 
 * @param widget Widget
 * @param focusable true se pode receber foco
 */
void eg_widget_set_focusable(EgWidget *widget, bool focusable);

/**
 * Verifica se o widget pode receber foco.
 * 
 * @param widget Widget
 * @return true se pode receber foco
 */
bool eg_widget_get_focusable(EgWidget *widget);

/**
 * Dá foco ao widget.
 * 
 * @param widget Widget
 * @return true se conseguiu dar foco
 */
bool eg_widget_grab_focus(EgWidget *widget);

/**
 * Verifica se o widget tem foco.
 * 
 * @param widget Widget
 * @return true se tem foco
 */
bool eg_widget_has_focus(EgWidget *widget);

/* ============================================
 * Constantes de Teclas Comuns
 * ============================================ */

/* Teclas especiais (valores compatíveis com GDK) */
#define EG_KEY_Return     0xff0d
#define EG_KEY_Escape     0xff1b
#define EG_KEY_Tab        0xff09
#define EG_KEY_BackSpace  0xff08
#define EG_KEY_Delete     0xffff
#define EG_KEY_Insert     0xff63
#define EG_KEY_Home       0xff50
#define EG_KEY_End        0xff57
#define EG_KEY_Page_Up    0xff55
#define EG_KEY_Page_Down  0xff56
#define EG_KEY_Left       0xff51
#define EG_KEY_Up         0xff52
#define EG_KEY_Right      0xff53
#define EG_KEY_Down       0xff54
#define EG_KEY_F1         0xffbe
#define EG_KEY_F2         0xffbf
#define EG_KEY_F3         0xffc0
#define EG_KEY_F4         0xffc1
#define EG_KEY_F5         0xffc2
#define EG_KEY_F6         0xffc3
#define EG_KEY_F7         0xffc4
#define EG_KEY_F8         0xffc5
#define EG_KEY_F9         0xffc6
#define EG_KEY_F10        0xffc7
#define EG_KEY_F11        0xffc8
#define EG_KEY_F12        0xffc9
#define EG_KEY_space      0x020

#ifdef __cplusplus
}
#endif

#endif /* EASYGTK_EVENTS_H */
