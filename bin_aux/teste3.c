#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

static Mix_Music *current_music = NULL;
static Mix_Chunk *current_sound = NULL;
static int current_channel = -1;

// Inicializa o sistema de áudio
void init_audio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }
}

// Callback quando a música termina
void on_music_finished() {
    printf("Música terminou. Você pode carregar a próxima aqui.\n");
    // Ex: tocar nova música aleatória
}

// Toca música de fundo (em loop infinito por padrão)
void play_music(const char *filepath, int loop) {
    if (current_music != NULL) {
        Mix_FreeMusic(current_music);
    }

    current_music = Mix_LoadMUS(filepath);
    if (!current_music) {
        fprintf(stderr, "Erro ao carregar música: %s\n", Mix_GetError());
        return;
    }

    Mix_PlayMusic(current_music, loop);
    Mix_HookMusicFinished(on_music_finished);  // Detecta fim da música
}

// Para a música atual
void stop_music() {
    Mix_HaltMusic();
}

// Toca um som (efeito curto)
void play_sound(const char *filepath) {
    if (current_sound != NULL) {
        Mix_FreeChunk(current_sound);
    }

    current_sound = Mix_LoadWAV(filepath);
    if (!current_sound) {
        fprintf(stderr, "Erro ao carregar som: %s\n", Mix_GetError());
        return;
    }

    current_channel = Mix_PlayChannel(-1, current_sound, 0); // -1 = qualquer canal livre
    if (current_channel == -1) {
        fprintf(stderr, "Erro ao tocar som: %s\n", Mix_GetError());
    }
}

// Para o som atual
void stop_sound() {
    if (current_channel != -1) {
        Mix_HaltChannel(current_channel);
        current_channel = -1;
    }
}

// Libera tudo ao final do programa
void cleanup_audio() {
    stop_music();
    stop_sound();

    if (current_music) Mix_FreeMusic(current_music);
    if (current_sound) Mix_FreeChunk(current_sound);

    Mix_CloseAudio();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    init_audio();

    play_music("../assets/sounds/got_open.mp3", -1);          // Música em loop infinito
    play_sound("../assets/sounds/common/click.mp3");              // Toca som por cima

    SDL_Delay(15000); // Espera 5s (substitua por lógica GTK)

    //stop_sound();    // Opcional: parar efeito antes de terminar
    //stop_music();    // Parar música

    //cleanup_audio();
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