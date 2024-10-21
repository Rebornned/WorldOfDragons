#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>

typedef struct {
    GtkWindow *window;
    gint original_x;
    gint original_y;
    gint iterations;
    gint max_iterations;
} TremorData;

gboolean tremor_callback(gpointer data) {
    TremorData *tremor = (TremorData *)data;

    if (tremor->iterations >= tremor->max_iterations) {
        // Restaurar posição original
        gtk_window_move(tremor->window, tremor->original_x, tremor->original_y);
        return FALSE; // Parar o timer
    }

    // Gerar deslocamentos aleatórios
    gint offset_x = (rand() % 11) - 5; // Deslocamento entre -5 e 5
    gint offset_y = (rand() % 11) - 5;

    // Aplicar deslocamento
    gtk_window_move(tremor->window, tremor->original_x + offset_x, tremor->original_y + offset_y);

    tremor->iterations++;

    return TRUE; // Continuar o timer
}

void iniciar_tremor(GtkWindow *window) {
    TremorData *tremor = g_malloc(sizeof(TremorData));
    tremor->window = window;
    gtk_window_get_position(window, &tremor->original_x, &tremor->original_y);
    tremor->iterations = 0;
    tremor->max_iterations = 20; // Ajuste conforme necessário

    srand(time(NULL));

    // Iniciar timer a cada 50 ms
    g_timeout_add(50, tremor_callback, tremor);
}

// Handler para eventos de pressionamento de tecla
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    g_print("Tecla pressionada: %s\n", gdk_keyval_name(event->keyval));
    // Adicione aqui o processamento adicional conforme necessário
    return FALSE; // Retorna FALSE para permitir que outros handlers processem o evento

}


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulação de Tremor GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Por exemplo, iniciar o tremor após 1 segundo
    g_timeout_add_seconds(1, (GSourceFunc)iniciar_tremor, window);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
