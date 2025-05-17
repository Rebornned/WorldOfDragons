#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>




// Callback do botão
void on_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    shakeScreen(1000, window, 3000, 10); // 1000 ms, intensidade 10
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    srand(time(NULL)); // Inicializa aleatório

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo de Terremoto");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *button = gtk_button_new_with_label("Tremer Janela!");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);

    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), container);
    gtk_box_pack_start(GTK_BOX(container), button, TRUE, TRUE, 10);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
