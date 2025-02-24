#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

// Variáveis globais
static GstElement *pipeline = NULL;

// Função para parar e liberar o vídeo
void stop_video(GtkWidget *drawing_area) {
    if (pipeline) {
        // Parar a reprodução
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
        pipeline = NULL;
    }

    // Redesenhar o GtkDrawingArea para limpar
    gtk_widget_queue_draw(drawing_area);
}

// Callback para desenhar o GtkDrawingArea transparente
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Defina a cor transparente
    cairo_set_source_rgba(cr, 0, 0, 0, 0); // RGB + Alpha
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    return FALSE;
}

// Exemplo de uso
void play_video(GtkWidget *drawing_area) {
    // Cria a pipeline
    pipeline = gst_parse_launch(
        "playbin uri=file:///C:/Users/Amage/Desktop/Programacao/GKT_C/WorldOfDragons/assets/img_files/animations/animation_battle_opening/render.webm video-sink=gtksink", NULL);

    if (!pipeline) {
        g_printerr("Falha ao criar a pipeline.\n");
        return;
    }

    // Obtém a janela do DrawingArea e conecta ao GStreamer
    GdkWindow *window = gtk_widget_get_window(drawing_area);
    if (!window) {
        g_printerr("Falha ao obter a janela do DrawingArea.\n");
        return;
    }

    GstVideoOverlay *overlay = GST_VIDEO_OVERLAY(pipeline);
    gst_video_overlay_set_window_handle(overlay, GDK_WINDOW_XID(window));

    // Inicia o vídeo
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Conecta o evento de desenhar para restaurar a transparência
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);

    // Botão para iniciar o vídeo
    GtkWidget *button_start = gtk_button_new_with_label("Play Video");
    g_signal_connect_swapped(button_start, "clicked", G_CALLBACK(play_video), drawing_area);

    // Botão para parar o vídeo e restaurar a transparência
    GtkWidget *button_stop = gtk_button_new_with_label("Stop Video");
    g_signal_connect_swapped(button_stop, "clicked", G_CALLBACK(stop_video), drawing_area);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), drawing_area, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), button_start, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), button_stop, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
