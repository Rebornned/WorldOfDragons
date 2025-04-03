#include <gtk/gtk.h>

GtkWidget *debuff_container;

void remover_debuff(GtkWidget *debuff) {
    gtk_widget_destroy(debuff); // Remove o widget da interface
}

void adicionar_debuff(GtkWidget *parent) {
    GtkWidget *debuff = gtk_button_new_with_label("🔥"); // Apenas um exemplo, pode ser uma imagem
    gtk_box_pack_start(GTK_BOX(parent), debuff, FALSE, FALSE, 5);
    gtk_widget_show(debuff);
    
    // Simula a remoção do debuff após um tempo
    g_timeout_add(3000, (GSourceFunc) remover_debuff, debuff);
}

void ativar_turno(GtkWidget *widget, gpointer data) {
    adicionar_debuff(debuff_container);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Container para os debuffs
    debuff_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(debuff_container), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), debuff_container, FALSE, FALSE, 0);

    // Botão para simular turno e adicionar debuff
    GtkWidget *btn = gtk_button_new_with_label("Novo Turno");
    g_signal_connect(btn, "clicked", G_CALLBACK(ativar_turno), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), btn, FALSE, FALSE, 5);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
