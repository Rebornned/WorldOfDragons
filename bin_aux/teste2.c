#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

// Função que será chamada ao apertar o botão
void play_sound(GtkWidget *widget, gpointer data) {
    Mix_Chunk *sound = (Mix_Chunk *)data;
    Mix_PlayChannel(-1, sound, 0);  // Toca o som no próximo canal livre (-1)
}

// Função para inicializar SDL2
void init_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Inicializa o SDL mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL Mixer: %s\n", Mix_GetError());
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // Inicializa GTK
    gtk_init(&argc, &argv);

    // Inicializa SDL2 áudio
    init_audio();

    // Carrega o arquivo de som
    Mix_Chunk *sound = Mix_LoadWAV("../assets/sounds/got_open.wav");
    if (!sound) {
        printf("Erro ao carregar o som: %s\n", Mix_GetError());
        return 1;
    }

    // Cria uma janela GTK
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Exemplo de Som");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    
    // Cria um botão
    GtkWidget *button = gtk_button_new_with_label("Tocar Som");
    g_signal_connect(button, "clicked", G_CALLBACK(play_sound), sound);

    // Adiciona o botão à janela
    gtk_container_add(GTK_CONTAINER(window), button);

    // Sinal para fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Exibe todos os widgets
    gtk_widget_show_all(window);

    // Loop principal do GTK
    gtk_main();

    // Limpa recursos do SDL
    Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}

// Função WinMain para compatibilidade com -mwindows no Windows
#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif