#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <direct.h>

// Compilação necessária para funcionar
// gcc -o main main.c `pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango`
// Sem terminal: gcc -o main.exe main.c -mwindows `pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango`
// =====================================================================================================

// Ponteiros globais
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// Main window
GtkWidget *window; 
GtkBuilder *builder;
GtkStack *main_stack;

// Frame 1

// Frame 2
GtkEntry *fr2_inp_user;
GtkEntry *fr2_inp_pass;

// Frame 4
GtkStack *fr4_stack;

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Registro de funções referentes a tela
// ********************************************************************************************************
void registerSignals(GtkBuilder *builder);
void switchPage(GtkButton *btn, gpointer user_data);
void clean_elements(GtkEntry **input_vec, GtkLabel **label_vec);
void labeltextModifier(GtkLabel *label, const gchar *text);
static void set_cursor_window(GtkWidget *widget, gpointer data);

// ********************************************************************************************************


int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.utf8");
    gtk_init(&argc, &argv); // Init gtk

    // Inicialização dos ponteiros GTK
    GtkCssProvider *css_provider;
    // Iniciando interface XML para C
    builder = gtk_builder_new_from_file("../assets/ui_files/T_Dragons.glade");
    gtk_builder_connect_signals(builder, NULL);

    // Inicialização de objetos principais da interface
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "../assets/img_files/T_dragons.ico", NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Fecha a tela
    
    GdkGeometry hints_window;
    hints_window.max_width = 1000;  
    hints_window.max_height = 600; 
    gtk_window_set_geometry_hints(GTK_WINDOW(window), NULL, &hints_window, GDK_HINT_MAX_SIZE);

    // Carregar e aplicar o arquivo CSS
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "../assets/css/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Stack principal
    main_stack = GTK_STACK(gtk_builder_get_object(builder, "main_stack"));

    // Frame 1 Inicial
    // Imagem background
    GtkWidget *fr1_image = GTK_WIDGET(gtk_builder_get_object(builder, "fr1_img"));
    gtk_image_set_from_file(GTK_IMAGE(fr1_image), "../assets/img_files/background_start.png");

    // Frame 2 Login
    // Imagem background
    GtkWidget *fr2_image = GTK_WIDGET(gtk_builder_get_object(builder, "fr2_img"));
    gtk_image_set_from_file(GTK_IMAGE(fr2_image), "../assets/img_files/background_login.png");

    // Input entry
    fr2_inp_user = GTK_ENTRY(gtk_builder_get_object(builder, "fr2_inp_name"));
    fr2_inp_pass = GTK_ENTRY(gtk_builder_get_object(builder, "fr2_inp_pass"));

    // Frame 3 Cadastro
    // Imagem background
    GtkWidget *fr3_image = GTK_WIDGET(gtk_builder_get_object(builder, "fr3_img"));
    gtk_image_set_from_file(GTK_IMAGE(fr3_image), "../assets/img_files/background_login.png");

    // Frame 4 Recuperação
    // Imagem background
    GtkWidget *fr4_image = GTK_WIDGET(gtk_builder_get_object(builder, "fr4_img"));
    gtk_image_set_from_file(GTK_IMAGE(fr4_image), "../assets/img_files/background_login.png");

    // Stack
    fr4_stack = GTK_STACK(gtk_builder_get_object(builder, "fr4_stack"));

    // Registrando sinais de callback para botões executarem funções
    registerSignals(builder);

    g_signal_connect(window, "map", G_CALLBACK(set_cursor_window), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}

void switchPage(GtkButton *btn, gpointer user_data) {
    // Obtém o nome do botão clicado
    const gchar *button_name = gtk_widget_get_name(GTK_WIDGET(btn));

    // Frame 2 Elementos para limpar
    GtkLabel *fr2_label_elements[] = {NULL};
    GtkEntry *fr2_input_elements[] = {fr2_inp_user, fr2_inp_pass, NULL};

    // Frame 3 Elementos para limpar
    GtkLabel *fr3_label_elements[] = {NULL};
    GtkEntry *fr3_input_elements[] = {NULL};

    // Altera a página visível da GtkStack com base no nome do botão
    // Frame 1
    if (g_strcmp0(button_name, "fr1_btn_start") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "login_page");
    }

    // Frame 2
    if (g_strcmp0(button_name, "fr2_btn_recover") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "recover_page");
        clean_elements(fr2_input_elements, fr2_label_elements);
    }

    if (g_strcmp0(button_name, "fr2_btn_create") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "account_page");
        clean_elements(fr2_input_elements, fr2_label_elements);
    }

    // Frame 3

    if (g_strcmp0(button_name, "fr3_btn_back") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "login_page");
    }

    // Frame 4

    if (g_strcmp0(button_name, "fr4_btn_back") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "login_page");
        gtk_stack_set_visible_child_name(fr4_stack, "fr4_send");
    }

    if (g_strcmp0(button_name, "fr4_btn_send") == 0) {
        gtk_stack_set_visible_child_name(fr4_stack, "fr4_code");
    }

    if (g_strcmp0(button_name, "fr4_btn_advance") == 0) {
        gtk_stack_set_visible_child_name(fr4_stack, "fr4_change");
    }

    // Retira o foco de todos os elementos
    // Cria um widget invisível para receber o foco temporário
    GtkWidget *dummy = GTK_WIDGET(gtk_builder_get_object(builder, "fr2_dummy"));
    gtk_widget_grab_focus(dummy); // Define o foco no dummy
}

void registerSignals(GtkBuilder *builder) {
    // Registrar sinais de callback
    // Passa a main_stack como user_data

    // Frame 1 Botões
    GObject *fr1_btn_iniciar = gtk_builder_get_object(builder, "fr1_btn_start");
    g_signal_connect(fr1_btn_iniciar, "clicked", G_CALLBACK(switchPage), main_stack);

    // Frame 2 Botões
    GObject *fr2_btn_create = gtk_builder_get_object(builder, "fr2_btn_create");
    g_signal_connect(fr2_btn_create, "clicked", G_CALLBACK(switchPage), main_stack);

    GObject *fr2_btn_recover = gtk_builder_get_object(builder, "fr2_btn_recover");
    g_signal_connect(fr2_btn_recover, "clicked", G_CALLBACK(switchPage), main_stack);

    // Frame 3 Botões
    GObject *fr3_btn_back = gtk_builder_get_object(builder, "fr3_btn_back");
    g_signal_connect(fr3_btn_back, "clicked", G_CALLBACK(switchPage), main_stack);

    // Frame 4 Botoões
    GObject *fr4_btn_back = gtk_builder_get_object(builder, "fr4_btn_back");
    g_signal_connect(fr4_btn_back, "clicked", G_CALLBACK(switchPage), main_stack);

    GObject *fr4_btn_send = gtk_builder_get_object(builder, "fr4_btn_send");
    g_signal_connect(fr4_btn_send, "clicked", G_CALLBACK(switchPage), main_stack);

    GObject *fr4_btn_advance = gtk_builder_get_object(builder, "fr4_btn_advance");
    g_signal_connect(fr4_btn_advance, "clicked", G_CALLBACK(switchPage), main_stack);



    return;
}

static void set_cursor_window(GtkWidget *widget, gpointer data) {
    // Carregando uma imagem do cursor customizado
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("../assets/img_files/cursor2.png", NULL);
    GdkCursor *cursor = gdk_cursor_new_from_pixbuf(gdk_display_get_default(), pixbuf, 0, 0);

    // Definindo o cursor para a janela
    gdk_window_set_cursor(gtk_widget_get_window(window), cursor);
}

void clean_elements(GtkEntry **input_vec, GtkLabel **label_vec) {
    int inp_count = 0, label_count = 0;

    // Limpando todos os inputs da tela
    while (input_vec[inp_count] != NULL) {
        gtk_entry_set_text(input_vec[inp_count], "");
        inp_count++;
    }
    
    // Limpando as labels da tela
    while (label_vec[label_count] != NULL) {
        labeltextModifier(label_vec[label_count], "");
        label_vec++;
    }
    
}

void labeltextModifier(GtkLabel *label, const gchar *text) {
    gchar *valid_utf8_text = g_utf8_make_valid(text, -1);
    gtk_label_set_text(label, valid_utf8_text);
    g_free(valid_utf8_text);
}
