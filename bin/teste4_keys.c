#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

gint arrowVector[12];
gint arrowVectorIndex = 0;
gint timer_total = 1000;
gboolean active_timer = FALSE, arrowSucess = FALSE;
GtkWidget *newlabel;

gboolean timer_reduces(gpointer data);
void random_vector();

int random_choice(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Função callback que será chamada quando uma tecla for pressionada
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer label) {
    gchar keyval_name[32];
    gint pressed_number;
    // Obtém o nome da tecla pressionada
    gdk_keyval_name(event->keyval);
    snprintf(keyval_name, sizeof(keyval_name), "%s", gdk_keyval_name(event->keyval));

    if(strcmp(keyval_name, "Left") == 0 || strcmp(keyval_name, "a") == 0) pressed_number = 0;
    else if(strcmp(keyval_name, "Up") == 0 || strcmp(keyval_name, "w") == 0) pressed_number = 1;
    else if(strcmp(keyval_name, "Right") == 0 || strcmp(keyval_name, "d") == 0) pressed_number = 2;
    else if(strcmp(keyval_name, "Down") == 0 || strcmp(keyval_name, "s") == 0) pressed_number = 3;

    // Atualiza o texto do label
    gtk_label_set_text(GTK_LABEL(label), keyval_name);
    g_print("Tecla pressionada %s, number:%d \n", keyval_name, pressed_number);

    if(arrowVector[arrowVectorIndex] == pressed_number && active_timer == TRUE)
        arrowVectorIndex++;
    else if(arrowVector[arrowVectorIndex] != pressed_number && active_timer == TRUE) {
        g_print("Pressionou errado\n");
        timer_total -= 200;
    }   
    if(arrowVectorIndex == 12 ) {
        arrowSucess = TRUE;
        g_print("FINISHED IN %.2f seconds\n", (1000-timer_total)*0.01);
        timer_total = 0;
    }

    if(strcmp(keyval_name, "Return") == 0 && active_timer == FALSE) {
        random_vector();
        timer_total = 1000;
        active_timer = TRUE;
        g_timeout_add(1, timer_reduces, GINT_TO_POINTER(timer_total));
    }
    else if(strcmp(keyval_name, "Return") == 0 && active_timer == TRUE) {
        timer_total = 0;
    }

    return FALSE;  // Retorna FALSE para continuar o processamento do evento
}

gboolean timer_reduces(gpointer data) {
    gchar countText[50];
    timer_total--;
    snprintf(countText, sizeof(countText), "%.2f seconds", timer_total*0.01);
    gtk_label_set_text(GTK_LABEL(newlabel), countText);
    if(timer_total > 0)
        return TRUE;
    
    if(arrowSucess)
        g_print("SUCESSO!!!\n");
    else if(!arrowSucess)
        g_print("FAILED\n");

    gtk_label_set_text(GTK_LABEL(newlabel), "finished");
    arrowSucess = FALSE;
    active_timer = FALSE;
    arrowVectorIndex = 0;
    return FALSE;
}

void random_vector() {
    gchar letters[] = "AWDS";
    g_print("\nNovo vetor de setas: ");
    for(int i=0; i<12; i++) {
        arrowVector[i] = random_choice(0, 3);
        g_print("%c ", letters[arrowVector[i]]);
    }
    g_print("\n");

}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *boxContainer;
    srand(time(NULL));
    // Inicializa o GTK
    gtk_init(&argc, &argv);
    

    // Cria uma nova janela
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Captura de Tecla");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Conecta o evento "destroy" para fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria um label para exibir a tecla pressionada
    boxContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    label = gtk_label_new("Pressione qualquer tecla...");
    newlabel = gtk_label_new("contador: ");

    // Adiciona o label à janela

    gtk_box_pack_start(GTK_BOX(boxContainer), label, TRUE, TRUE, 0); // Expansível
    gtk_box_pack_start(GTK_BOX(boxContainer), newlabel, TRUE, TRUE, 0); // Expansível

    gtk_container_add(GTK_CONTAINER(window), boxContainer);



    // Conecta o evento de pressionar teclas à função de callback
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), label);

    // Exibe a janela e seus componentes
    gtk_widget_show_all(window);

    // Inicia o loop principal do GTK
    gtk_main();

    return 0;
}
