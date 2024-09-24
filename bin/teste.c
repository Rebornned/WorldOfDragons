#include <gtk/gtk.h>

typedef struct {
    GtkWidget *label;
    int start_width;
    int target_width;
    int current_width;
    int steps;
} LevelBarData;

gboolean increase_width(gpointer data) {
    LevelBarData *bar_data = (LevelBarData *)data;

    if (bar_data->current_width >= bar_data->target_width) {
        // Alcançou o tamanho desejado, parar o timer
        return FALSE;
    }

    // Incrementa a largura atual
    bar_data->current_width += (bar_data->target_width - bar_data->start_width) / bar_data->steps;

    if (bar_data->current_width > bar_data->target_width) {
        bar_data->current_width = bar_data->target_width;
    }

    // Ajusta a largura da label
    char css[100];
    snprintf(css, sizeof(css), "width: %dpx;", bar_data->current_width);

    // Cria um estilo CSS dinamicamente
    GtkStyleContext *context = gtk_widget_get_style_context(bar_data->label);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Continue a animação
    return TRUE;
}

void start_animation(GtkWidget *label, int start, int target, int duration_ms, int steps) {
    // Define os dados da barra
    LevelBarData *data = g_new(LevelBarData, 1);
    data->label = label;
    data->start_width = start;
    data->target_width = target;
    data->current_width = start;
    data->steps = steps;

    // Calcula o intervalo entre cada passo
    int interval = duration_ms / steps;

    // Inicia o timer para a animação
    g_timeout_add(interval, increase_width, data);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria uma label que será usada como barra de progresso
    GtkWidget *label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_widget_show_all(window);

    // Começar animação de 40% (160px) para 70% (280px) em 200ms com 30 steps
    start_animation(label, 160, 280, 200, 30);

    gtk_main();
    return 0;
}
