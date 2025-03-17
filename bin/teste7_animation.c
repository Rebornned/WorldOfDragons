#include <gtk/gtk.h>
#include <cairo.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 1000

static GdkPixbuf *frames[MAX_FRAMES];
static int current_frame = 0;
static int total_frames = 0;
static guint animation_timer = 0;

static gboolean on_timeout(gpointer user_data) {
    if (total_frames > 0) {
        current_frame = (current_frame + 1) % total_frames;
        gtk_widget_queue_draw(GTK_WIDGET(user_data)); // Atualiza a área de desenho
    }
    return TRUE;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    if (total_frames == 0) {
        return FALSE;
    }

    GdkPixbuf *current_frame_pixbuf = frames[current_frame];
    if (current_frame_pixbuf == NULL) {
        g_print("Erro: Frame %d está NULL!\n", current_frame);
        return FALSE;
    }

    // Renderiza a imagem do frame atual
    gdk_cairo_set_source_pixbuf(cr, current_frame_pixbuf, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

static void load_frames_from_directory(const gchar *directory_path, gint expected_frames) {
    total_frames = 0;  // Reset total_frames antes de carregar novos
    for (int i = 0; i < expected_frames; i++) {
        gchar filename[256];
        snprintf(filename, sizeof(filename), "%s/frame (%d).png", directory_path, i + 1);
        g_print("Tentando carregar: %s\n", filename);
        
        frames[total_frames] = gdk_pixbuf_new_from_file(filename, NULL);
        if (frames[total_frames] != NULL) {
            g_print("Frame %d carregado com sucesso.\n", total_frames);
            total_frames++;  // Incrementa apenas se o frame for carregado corretamente
        } else {
            g_printerr("Erro ao carregar frame %d: %s\n", i + 1, filename);
        }
    }
}

static GtkWidget *create_animation_area(const gchar *directory_path) {
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 800, 600); // Defina o tamanho desejado

    // Carregar frames do diretório
    load_frames_from_directory(directory_path, 109);

    // Iniciar o temporizador para animar os frames, somente se frames foram carregados
    if (total_frames > 0) {
        animation_timer = g_timeout_add(16, (GSourceFunc)on_timeout, drawing_area); // 30 FPS
    } else {
        g_printerr("Nenhum frame foi carregado. Verifique o diretório fornecido.\n");
    }

    // Conectar o sinal de desenho
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);

    return drawing_area;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        g_printerr("Uso: %s <diretório com frames PNG>\n", argv[0]);
        return -1;
    }

    gtk_init(&argc, &argv);

    // Criar a janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Animação PNG");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Criar a área de animação e adicionar à janela
    GtkWidget *animation_area = create_animation_area(argv[1]);
    gtk_container_add(GTK_CONTAINER(window), animation_area);

    // Conectar o sinal de fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Exibir a janela
    gtk_widget_show_all(window);

    // Rodar a aplicação GTK
    gtk_main();

    return 0;
}
