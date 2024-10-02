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

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulação de Tremor GTK");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Por exemplo, iniciar o tremor após 1 segundo
    g_timeout_add_seconds(1, (GSourceFunc)iniciar_tremor, window);

    gtk_widget_show_all(window);
    gtk_main();

    while (1) {
        if (GetAsyncKeyState(VK_UP)) {
            g_print("Tecla UP pressionada!\n");
            Sleep(200);  // Pequena pausa para evitar múltiplas detecções consecutivas
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            g_print("Tecla DOWN pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_LEFT)) {
            g_print("Tecla LEFT pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            g_print("Tecla RIGHT pressionada!\n");
            Sleep(200);
        }
        if (GetAsyncKeyState(VK_ESCAPE)) {
            g_print("Saindo do jogo...\n");
            break;  // Sai do loop se a tecla ESC for pressionada
        }

        Sleep(50);  // Pequena pausa para não sobrecarregar o processador
    }

    return 0;
}
