#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_FRAMES 109  // Número total de frames
#define FRAME_DELAY 16  // Aproximadamente 60 FPS

GLuint textures[NUM_FRAMES] = {0};
int current_frame = 0;
GtkGLArea *gl_area;
GLuint shader_program, vao, vbo;

// **Shader Vertex**
const char *vertex_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec2 pos;\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "out vec2 TexCoord;\n"
    "void main() {\n"
    "   gl_Position = vec4(pos, 0.0, 1.0);\n"
    "   TexCoord = texCoord;\n"
    "}\n";

// **Shader Fragment**
const char *fragment_shader_src = "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D texture1;\n"
    "void main() {\n"
    "   FragColor = texture(texture1, TexCoord);\n"
    "}\n";

// **Carrega textura de arquivo PNG**
GLuint load_texture_from_file(const char *filename) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    if (!pixbuf) {
        g_printerr("Erro ao carregar imagem: %s\n", filename);
        return 0;
    }

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);
    int has_alpha = gdk_pixbuf_get_has_alpha(pixbuf);
    const guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = has_alpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    g_object_unref(pixbuf);
    return texture;
}

// **Carrega todas as texturas dos frames**
void load_all_textures() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        char filename[150];
        snprintf(filename, sizeof(filename), "../assets/img_files/animations/animation_battle_opening/render/frame (%d).png", i + 1);
        textures[i] = load_texture_from_file(filename);

        if (textures[i] == 0) {
            g_printerr("Falha ao carregar frame %d\n", i + 1);
        }
    }
}

// **Compila um shader OpenGL**
GLuint compile_shader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        g_printerr("Erro ao compilar shader: %s\n", log);
        exit(1);
    }
    return shader;
}

// **Cria o pipeline de renderização OpenGL**
void setup_opengl_pipeline() {
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {
        //  Posição    |  Coordenada UV
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// **Renderiza o frame atual**
static gboolean on_render(GtkGLArea *area, GdkGLContext *context) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindTexture(GL_TEXTURE_2D, textures[current_frame]);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    return TRUE;
}

// **Atualiza frame e chama render**
static gboolean update_frame(gpointer user_data) {
    current_frame = (current_frame + 1) % NUM_FRAMES;
    gtk_gl_area_queue_render(GTK_GL_AREA(gl_area));
    return G_SOURCE_CONTINUE;
}

// **Inicializa OpenGL**
static void on_realize(GtkGLArea *area) {
    gtk_gl_area_make_current(area);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setup_opengl_pipeline();
    load_all_textures();
}

// **Cria interface GTK**
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Animação OpenGL no GTK3");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    gl_area = GTK_GL_AREA(gtk_gl_area_new());
    g_signal_connect(gl_area, "render", G_CALLBACK(on_render), NULL);
    g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(gl_area));
    gtk_widget_show_all(window);

    g_timeout_add(FRAME_DELAY, update_frame, NULL);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.exemplo.openglgtk3", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
