#include <gtk/gtk.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#define FRAME_COUNT 109  // Número de imagens da animação
#define FRAME_DELAY 16  // ~60 FPS (1000 ms / 60 = 16 ms)

GtkWidget *gl_area;
GtkWidget *start_button;
GLuint textures[FRAME_COUNT];
int current_frame = 0;
gboolean animation_running = FALSE;

// Carrega uma imagem e converte para textura OpenGL
GLuint load_texture_from_pixbuf(const char *filename) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    if (!pixbuf) {
        g_printerr("Erro ao carregar imagem: %s\n", filename);
        return 0;
    }

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    g_object_unref(pixbuf);  // Libera a memória da imagem carregada
    return texture;
}

// Callback de renderização
static gboolean on_render(GtkGLArea *area, GdkGLContext *context) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[current_frame]);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-1, -1);
        glTexCoord2f(1, 1); glVertex2f(1, -1);
        glTexCoord2f(1, 0); glVertex2f(1, 1);
        glTexCoord2f(0, 0); glVertex2f(-1, 1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    return TRUE;
}

// Atualiza o frame da animação
static gboolean update_frame(gpointer user_data) {
    if (animation_running) {
        current_frame = (current_frame + 1) % FRAME_COUNT;
        gtk_widget_queue_draw(gl_area);  // Solicita a renderização
    }
    return G_SOURCE_CONTINUE;
}

// Inicia a animação quando o botão é clicado
static void start_animation(GtkButton *button, gpointer user_data) {
    animation_running = TRUE;  // Inicia a animação
}

// Inicializa OpenGL e carrega as imagens
static void on_realize(GtkGLArea *area, gpointer user_data) {
    gtk_gl_area_make_current(area);
    if (glewInit() != GLEW_OK) {
        g_printerr("Erro ao inicializar GLEW\n");
        return;
    }

    glEnable(GL_TEXTURE_2D);

    // Carregar os frames da animação
    for (int i = 1; i <= FRAME_COUNT; i++) {
        char filename[150];
        sprintf(filename, "../assets/img_files/animations/animation_battle_opening/render/frame (%d).png", i);
        textures[i] = load_texture_from_pixbuf(filename);
    }

    // Iniciar o loop de animação
    g_timeout_add(FRAME_DELAY, update_frame, NULL);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Animação OpenGL");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Criação da área OpenGL
    gl_area = gtk_gl_area_new();
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(on_render), NULL);

    // Criação do botão de iniciar animação
    start_button = gtk_button_new_with_label("Iniciar Animação");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_animation), NULL);

    // Organizando os widgets
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), gl_area, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), start_button, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
