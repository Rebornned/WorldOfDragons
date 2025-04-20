#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

// Função para inicializar SDL2
void init_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Inicializa o SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL Mixer: %s\n", Mix_GetError());
        exit(1);
    }
}

// Função para tocar a música de fundo
void play_background_music(const char *filename) {
    Mix_Music *music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Erro ao carregar a música: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayMusic(music, -1);  // -1 significa loop infinito
}

// Função para parar a música de fundo
void stop_background_music() {
    Mix_HaltMusic();  // Para a música atual
}

// Função chamada quando o botão "Mudar Música" é clicado
void on_change_music(GtkWidget *widget, gpointer data) {
    const char *new_music = (const char *)data;
    stop_background_music();       // Para a música atual
    play_background_music(new_music); // Toca a nova música
}

int main(int argc, char *argv[]) {
    // Inicializa GTK
    gtk_init(&argc, &argv);

    // Inicializa SDL2 áudio
    init_audio();

    // Cria uma janela GTK
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Trocar Música de Fundo");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Cria um botão para mudar a música
    GtkWidget *button = gtk_button_new_with_label("Mudar Música");
    const char *new_music_file = "../assets/sounds/got_open.wav"; // Altere para o caminho do seu arquivo
    g_signal_connect(button, "clicked", G_CALLBACK(on_change_music), (gpointer)new_music_file);

    // Adiciona o botão à janela
    gtk_container_add(GTK_CONTAINER(window), button);

    // Sinal para fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Exibe todos os widgets
    gtk_widget_show_all(window);

    // Toca a música de fundo inicial (opcional)
    play_background_music(".../assets/sounds/got_open.wav"); // Altere para o seu arquivo inicial

    // Loop principal do GTK
    gtk_main();

    // Limpa recursos do SDL
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
