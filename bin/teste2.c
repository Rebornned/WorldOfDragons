#include <gtk/gtk.h>
#include <cairo.h>

// Função de desenho que simula desfoque desenhando o texto múltiplas vezes
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    const char *text = "Dragões";
    int x = 50, y = 100;
    double blur_radius = 3.0; // Define o raio do desfoque

    // Definir a fonte e o tamanho
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 40.0);

    // Simular desfoque desenhando o texto várias vezes com deslocamentos
    cairo_set_source_rgba(cr, 0, 0, 0, 0.3); // Cor preta semi-transparente para o desfoque
    for (double dx = -blur_radius; dx <= blur_radius; dx += 1.0) {
        for (double dy = -blur_radius; dy <= blur_radius; dy += 1.0) {
            cairo_move_to(cr, x + dx, y + dy);
            cairo_show_text(cr, text);
        }
    }

    // Desenhar o texto principal por cima
    cairo_set_source_rgb(cr, 1, 1, 1); // Cor branca para o texto principal
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, text);

    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    // Criar a janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Label com Desfoque");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Criar o GtkDrawingArea
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Conectar o handler de desenho
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
