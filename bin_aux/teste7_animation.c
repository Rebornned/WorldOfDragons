#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Teste HeaderBar");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);

    // Tema escuro
    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);

    // HeaderBar
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "World of Dragons");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_widget_show(header);

    gtk_window_set_titlebar(GTK_WINDOW(window), header);
    gtk_window_set_decorated(GTK_WINDOW(window), TRUE);

    // Corpo da janela
    GtkWidget *label = gtk_label_new("Conteúdo da Janela");
    gtk_container_add(GTK_CONTAINER(window), label);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
