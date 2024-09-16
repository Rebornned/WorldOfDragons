#include <gtk/gtk.h>

// Função de callback para restaurar a opacidade do botão
gboolean restore_opacity(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data); // Converte o gpointer para GtkWidget*
    gtk_widget_set_opacity(button, 1.0); // Restaura a opacidade
    return G_SOURCE_REMOVE; // Remove o timeout
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Botão clicado!\n");
    gtk_widget_set_opacity(widget, 0.7); // Altera a opacidade para dar um efeito de clique
    
    // Adiciona o timeout para restaurar a opacidade após 100 ms
    g_timeout_add(100, restore_opacity, widget);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Cria uma nova janela
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Botão com Imagem");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    // Cria um botão
    GtkWidget *button = gtk_button_new();

    // Carrega uma imagem e a define no botão
    GtkWidget *image = gtk_image_new_from_file("../assets/img_files/advance.png");
    gtk_button_set_image(GTK_BUTTON(button), image);
    gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE); // Mostra sempre a imagem

    // Conecta o sinal de clique do botão
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // Adiciona o botão à janela
    gtk_container_add(GTK_CONTAINER(window), button);

    // Conectar o sinal de destruir para fechar a aplicação
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Exibir todos os widgets
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
