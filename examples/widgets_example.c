/**
 * EasyGTK - Exemplo de Widgets
 * 
 * Demonstra CheckButton, Switch e ProgressBar.
 */

#include <easygtk/easygtk.h>
#include <stdio.h>

static EgProgressBar *progress_bar = NULL;
static EgLabel *status_label = NULL;
static EgTimerId timer_id = 0;
static double progress = 0.0;

/* Callback do timer para animar a progress bar */
static bool on_timer_tick(void *user_data) {
    (void)user_data;
    
    progress += 0.02;
    if (progress > 1.0) {
        progress = 0.0;
    }
    
    eg_progress_bar_set_fraction(progress_bar, progress);
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%.0f%%", progress * 100);
    eg_progress_bar_set_text(progress_bar, buf);
    
    return true; /* Continua o timer */
}

/* Callback do CheckButton */
static void on_check_toggled(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgCheckButton *cb = (EgCheckButton *)widget;
    bool active = eg_check_button_get_active(cb);
    
    const char *label = eg_check_button_get_label(cb);
    printf("CheckButton '%s': %s\n", label, active ? "marcado" : "desmarcado");
}

/* Callback do Switch - controla o timer */
static void on_switch_changed(EgWidget *widget, void *user_data) {
    (void)user_data;
    EgSwitch *sw = (EgSwitch *)widget;
    bool active = eg_switch_get_active(sw);
    
    if (active) {
        if (timer_id == 0) {
            timer_id = eg_timeout_add(50, on_timer_tick, NULL);
            eg_label_set_text(status_label, "Progresso: Rodando");
        }
    } else {
        if (timer_id != 0) {
            eg_source_remove(timer_id);
            timer_id = 0;
            eg_label_set_text(status_label, "Progresso: Pausado");
        }
    }
}

static void on_activate(EgWidget *widget, void *user_data) {
    (void)widget;
    EgApp *app = (EgApp *)user_data;
    
    /* Janela principal */
    EgWindow *window = eg_window_new(app, "Widgets Demo", 400, 350);
    eg_window_set_resizable(window, false);
    
    /* Box principal */
    EgBox *main_box = eg_box_new_vertical(15);
    eg_widget_set_margin(eg_box_as_widget(main_box), 20);
    
    /* Título */
    EgLabel *title = eg_label_new(NULL);
    eg_label_set_markup(title, "<span size='x-large' weight='bold'>Novos Widgets</span>");
    eg_widget_set_halign(eg_label_as_widget(title), EG_ALIGN_CENTER);
    
    /* === Seção CheckButtons === */
    EgLabel *cb_title = eg_label_new("CheckButtons:");
    eg_widget_set_halign(eg_label_as_widget(cb_title), EG_ALIGN_START);
    
    EgBox *cb_box = eg_box_new_vertical(5);
    eg_widget_set_margin(eg_box_as_widget(cb_box), 10);
    
    EgCheckButton *cb1 = eg_check_button_new("Opção 1");
    eg_check_button_on_toggled(cb1, on_check_toggled, NULL);
    eg_widget_set_tooltip(eg_check_button_as_widget(cb1), "Primeira opção");
    
    EgCheckButton *cb2 = eg_check_button_new("Opção 2");
    eg_check_button_on_toggled(cb2, on_check_toggled, NULL);
    eg_widget_set_tooltip(eg_check_button_as_widget(cb2), "Segunda opção");
    
    EgCheckButton *cb3 = eg_check_button_new("Opção 3 (desabilitada)");
    eg_widget_set_sensitive(eg_check_button_as_widget(cb3), false);
    
    eg_box_append(cb_box, eg_check_button_as_widget(cb1));
    eg_box_append(cb_box, eg_check_button_as_widget(cb2));
    eg_box_append(cb_box, eg_check_button_as_widget(cb3));
    
    /* === Seção Switch === */
    EgBox *switch_box = eg_box_new_horizontal(10);
    eg_widget_set_halign(eg_box_as_widget(switch_box), EG_ALIGN_START);
    
    EgLabel *switch_label = eg_label_new("Animar Progress Bar:");
    EgSwitch *sw = eg_switch_new();
    eg_switch_on_state_changed(sw, on_switch_changed, NULL);
    eg_widget_set_tooltip(eg_switch_as_widget(sw), "Liga/desliga a animação");
    
    eg_box_append(switch_box, eg_label_as_widget(switch_label));
    eg_box_append(switch_box, eg_switch_as_widget(sw));
    
    /* === Seção ProgressBar === */
    EgLabel *pb_title = eg_label_new("ProgressBar:");
    eg_widget_set_halign(eg_label_as_widget(pb_title), EG_ALIGN_START);
    
    progress_bar = eg_progress_bar_new();
    eg_progress_bar_set_show_text(progress_bar, true);
    eg_progress_bar_set_text(progress_bar, "0%");
    eg_widget_set_hexpand(eg_progress_bar_as_widget(progress_bar), true);
    
    status_label = eg_label_new("Progresso: Pausado");
    eg_widget_set_halign(eg_label_as_widget(status_label), EG_ALIGN_START);
    eg_widget_add_css_class(eg_label_as_widget(status_label), "dim-label");
    
    /* === Seção Radio Buttons === */
    EgLabel *radio_title = eg_label_new("Radio Buttons (grupo):");
    eg_widget_set_halign(eg_label_as_widget(radio_title), EG_ALIGN_START);
    
    EgBox *radio_box = eg_box_new_horizontal(15);
    
    EgCheckButton *radio1 = eg_check_button_new("Pequeno");
    EgCheckButton *radio2 = eg_check_button_new("Médio");
    EgCheckButton *radio3 = eg_check_button_new("Grande");
    
    /* Agrupa como radio buttons */
    eg_check_button_set_group(radio2, radio1);
    eg_check_button_set_group(radio3, radio1);
    eg_check_button_set_active(radio2, true); /* Seleciona o do meio */
    
    eg_box_append(radio_box, eg_check_button_as_widget(radio1));
    eg_box_append(radio_box, eg_check_button_as_widget(radio2));
    eg_box_append(radio_box, eg_check_button_as_widget(radio3));
    
    /* Monta a UI */
    eg_box_append(main_box, eg_label_as_widget(title));
    eg_box_append(main_box, eg_label_as_widget(cb_title));
    eg_box_append(main_box, eg_box_as_widget(cb_box));
    eg_box_append(main_box, eg_box_as_widget(switch_box));
    eg_box_append(main_box, eg_label_as_widget(pb_title));
    eg_box_append(main_box, eg_progress_bar_as_widget(progress_bar));
    eg_box_append(main_box, eg_label_as_widget(status_label));
    eg_box_append(main_box, eg_label_as_widget(radio_title));
    eg_box_append(main_box, eg_box_as_widget(radio_box));
    
    eg_window_set_child(window, eg_box_as_widget(main_box));
    eg_window_show(window);
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    
    EgApp *app = eg_app_new("com.example.widgets");
    if (app == NULL) return 1;
    
    eg_app_on_activate(app, on_activate, app);
    
    int status = eg_app_run(app);
    
    if (timer_id != 0) {
        eg_source_remove(timer_id);
    }
    
    eg_app_free(app);
    
    return status;
}
