#include <gtk/gtk.h>

typedef struct {
    int vida;
    int ataque;
    int defesa;
    int velocidade;
} Dragao;

Dragao dragao1 = {100, 20, 10, 15};
Dragao dragao2 = {100, 15, 15, 10};

GtkWidget *vida1_label;
GtkWidget *vida2_label;

void atualizar_labels() {
    char buffer[32];
    sprintf(buffer, "Vida: %d", dragao1.vida);
    gtk_label_set_text(GTK_LABEL(vida1_label), buffer);
    sprintf(buffer, "Vida: %d", dragao2.vida);
    gtk_label_set_text(GTK_LABEL(vida2_label), buffer);
}

void atacar(Dragao *atacante, Dragao *defensor) {
    int dano = atacante->ataque - defensor->defesa;
    if (dano > 0) {
        defensor->vida -= dano;
    }
}

void on_atacar_clicked(GtkButton *button, gpointer data) {
    atacar(&dragao1, &dragao2);
    if (dragao2.vida > 0) {
        atacar(&dragao2, &dragao1);
    }
    atualizar_labels();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Combate de Dragões");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *label1 = gtk_label_new("Dragão 1");
    gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);

    vida1_label = gtk_label_new("Vida: 100");
    gtk_grid_attach(GTK_GRID(grid), vida1_label, 0, 1, 1, 1);

    GtkWidget *label2 = gtk_label_new("Dragão 2");
    gtk_grid_attach(GTK_GRID(grid), label2, 1, 0, 1, 1);

    vida2_label = gtk_label_new("Vida: 100");
    gtk_grid_attach(GTK_GRID(grid), vida2_label, 1, 1, 1, 1);

    GtkWidget *button = gtk_button_new_with_label("Atacar");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 2, 1);

    g_signal_connect(button, "clicked", G_CALLBACK(on_atacar_clicked), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
