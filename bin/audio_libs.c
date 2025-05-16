#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "dlibs.h"

#define MUSICS_AVAILABLE 8 // Quantidade de músicas dispóniveis

// Sounds
audioAssets audioPointer;
gint currentSound;
gint soundsPlayed[10];
musicsBeastiary musicsBackground;

typedef struct {
    gint index;
    audioAssets *assets;
    gint loop;
} playAudioData;

gint loadAudio(gchar *path, gchar *name, gchar *type, gint indexSound, audioAssets *audioAssets, gint volume);
gboolean timedPlayMusicByIndex(gpointer data);
gboolean timedPlaySoundByIndex(gpointer data);

// Inicializa o sistema de áudio
void initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        g_print("Erro ao inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        g_print("Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }
    // Preenchendo a lista de músicas dispóniveis
    for(int i=0; i<MUSICS_AVAILABLE; i++) {
        musicsBackground.musicsAvailable[i] = i;
    }
    // Embaralha as músicas para ordem aleatória
    musicsBackground.currentMusic = 0;
    musicsBackground.inBattle = FALSE;
    shuffle(musicsBackground.musicsAvailable, MUSICS_AVAILABLE);

    // Preenche as rotas de canais iniciais dos efeitos sonoros
    for(int i=0; i<10; i++)
        soundsPlayed[i] = -1;

    // Loading de áudios e músicas
    // Efeitos sonoros
    loadAudio("../assets/sounds/common/click.wav", "click", "sound", 0, &audioPointer, 75);
    loadAudio("../assets/sounds/common/level_up_dragon.wav", "level_up_dragon", "sound", 1, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/dracarys_breath.wav", "dracarys_breath", "sound", 2, &audioPointer, 50);
    loadAudio("../assets/sounds/common/dragon_change.wav", "dragon_change", "sound", 3, &audioPointer, 25);
    loadAudio("../assets/sounds/common/menu_change.wav", "menu_change", "sound", 4, &audioPointer, 25);
    loadAudio("../assets/sounds/battle/bite_crunch.wav", "bite_crunch", "sound", 5, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/scratch_claw.wav", "scratch_claw", "sound", 6, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/victory.wav", "victory", "sound", 7, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/defeat.wav", "defeat", "sound", 8, &audioPointer, 100);
    loadAudio("../assets/sounds/common/exp_reward.wav", "exp_reward", "sound", 9, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/battle_start.wav", "battle_start", "sound", 10, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/meterbar_click.wav", "meterbar_click", "sound", 11, &audioPointer, 50);
    loadAudio("../assets/sounds/battle/debuff_finish.wav", "debuff_finish", "sound", 12, &audioPointer, 50);
    loadAudio("../assets/sounds/battle/terrified_status.wav", "terrified_status", "sound", 13, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/burning_status.wav", "burning_status", "sound", 14, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/broken_status.wav", "broken_status", "sound", 15, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/bleeding_status.wav", "bleeding_status", "sound", 16, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/turn_change.wav", "turn_change", "sound", 17, &audioPointer, 100);
    loadAudio("../assets/sounds/battle/damage_hit.wav", "damage_hit", "sound", 18, &audioPointer, 100);

    // Músicas
    Mix_VolumeMusic(64); // 50% de volume da música
    //loadAudio("../assets/sounds/got_open.mp3", "got_open", "music", 0, &audioPointer, 100);
    loadAudio("../assets/sounds/beastiary/after_the_storm.mp3", "after_the_storm", "music", 0, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/fields_of_ard_skellige.mp3", "fields_of_ard_skellige", "music", 1, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/hearts_of_stone.mp3", "hearts_of_stone", "music", 2, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/kaer_morhen.mp3", "kaer_morhen", "music", 3, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/lady_of_the_lake.mp3", "lady_of_the_lake", "music", 4, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/searching_for_cecilia.mp3", "searching_for_cecilia", "music", 5, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/syanna.mp3", "syanna", "music", 6, &audioPointer, 50);
    loadAudio("../assets/sounds/beastiary/kingdom_dance.mp3", "kingdom_dance", "music", 7, &audioPointer, 50);

    //loadAudio("../assets/sounds/beastiary/reign_of_targaryen.mp3", "reign_of_targaryen", "music", 1, &audioPointer, 50);

}

gint loadAudio(gchar *path, gchar *name, gchar *type, gint indexSound, audioAssets *audioAssets, gint volume) {
    // Carregamento de áudio e música
    if(strcmp(type, "music") == 0) {
        audioAssets->musics[indexSound].music = Mix_LoadMUS(path);
        strcpy(audioAssets->musics[indexSound].name, name);
        if(!audioAssets->musics[indexSound].music) {
            fprintf(stderr, "Erro ao carregar música de fundo: %s\n", Mix_GetError());
            return -1;
        }
        g_print("Música: %s | Carregada com sucesso!!!\n", audioAssets->musics[indexSound].name);
        return 0;
    }
    if(strcmp(type, "sound") == 0) {
        gint soundVolume = 128 * (volume/100.0);
        audioAssets->sounds[indexSound].sound = Mix_LoadWAV(path);
        Mix_VolumeChunk(audioAssets->sounds[indexSound].sound, soundVolume);
        strcpy(audioAssets->sounds[indexSound].name, name);
        if(!audioAssets->sounds[indexSound].sound) {
            fprintf(stderr, "Erro ao carregar efeito sonoro: %s\n", Mix_GetError());
            return -1;
        }
        g_print("Efeito: %s | Carregado com sucesso!!!\n", audioAssets->sounds[indexSound].name);
        return 0;
    }
}

// Callback quando a música termina
void on_music_finished() {
    if(!musicsBackground.inBattle && !musicsBackground.isFinished) {
        musicsBackground.currentMusic++;
        if(musicsBackground.currentMusic == MUSICS_AVAILABLE) {
            shuffle(musicsBackground.musicsAvailable, MUSICS_AVAILABLE);
            musicsBackground.currentMusic = 0;
        }
        playMusicByIndex(0, musicsBackground.musicsAvailable[musicsBackground.currentMusic], &audioPointer, 0);
        g_print("Tocando música dos backgrounds\n");
    }
    g_print("Música terminou. Você pode carregar a próxima aqui.\n");
    musicsBackground.isFinished = FALSE;
}

// =============================================================================================
// Parte de músicas 
// *********************************************************************************************
// Toca música de fundo através do seu index
void playMusicByIndex(gint timeout, gint index, audioAssets *assets, gint loop) {
    if (!assets->musics[index].music) {
        g_print("Erro ao carregar música: %d | ERROR: %s\n", index, Mix_GetError());
        return;
    }
    if(timeout == 0) {
        g_print("%s -> Tocando agora...\n", assets->musics[index].name);
        Mix_HaltMusic();
        Mix_PlayMusic(assets->musics[index].music, loop);
        Mix_HookMusicFinished(on_music_finished);  // Signal que aciona ao finalizar a música
    }
    else {
        playAudioData *data = g_malloc(sizeof(playAudioData));
        data->index = index;
        data->assets = assets;
        data->loop = loop;
        g_timeout_add(timeout, timedPlayMusicByIndex, data);
    }
}

gboolean timedPlayMusicByIndex(gpointer data) {
    playAudioData *values = (playAudioData *) data;
    playMusicByIndex(0, values->index, values->assets, values->loop);
    g_free(values);
    return FALSE;
}

// Toca música de fundo através de seu nome
void playMusicByName(gint timeout, gchar *name, audioAssets *assets, gint loop) {
    gint index = -1;
    for(int i=0; i < 50; i++) {
        g_print("Musica verificada: %s\n", assets->musics[i].name);
        if(assets->musics[i].name && strcmp(assets->musics[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        g_print("A música: %s, Não foi encontrada.\n", name);
        return;
    }
    
    playMusicByIndex(timeout, index, assets, loop);
}

// Para a música atual
void stopCurrentMusic() {
    musicsBackground.isFinished = TRUE;
    Mix_HaltMusic();
}
//==========================================================================================
// Parte de efeitos sonoros
// *****************************************************************************************
// Toca o som de acordo com index
void playSoundByIndex(gint timeout, gint index, audioAssets *assets, gint loop) {
    if (!assets->sounds[index].sound) {
        g_print("Erro ao carregar Efeito: %d | ERROR: %s\n", index, Mix_GetError());
        return;
    }
    if(timeout == 0) {
        //g_print("%s -> Tocando agora...\n", assets->sounds[index].name);
        currentSound = Mix_PlayChannel(-1, assets->sounds[index].sound, loop); // -1 = qualquer canal livre
        for(int i=0; i<10; i++)
            if(soundsPlayed[i] == -1) {
                soundsPlayed[i] = currentSound;
                break;
            }
    }
    else {
        playAudioData *data = g_malloc(sizeof(playAudioData));
        data->index = index;
        data->assets = assets;
        data->loop = loop;
        g_timeout_add(timeout, timedPlaySoundByIndex, data);
    }
}

gboolean timedPlaySoundByIndex(gpointer data) {
    playAudioData *values = (playAudioData *) data;
    playSoundByIndex(0, values->index, values->assets, values->loop);
    g_free(values);
    return FALSE;
}

// Toca o som de acordo com o nome
void playSoundByName(gint timeout, gchar *name, audioAssets *assets, gint loop) {
    gint index = -1;
    for(int i=0; i < 50; i++) {
        //g_print("Efeito verificada: %s\n", assets->sounds[i].name);
        if(assets->sounds[i].name && strcmp(assets->sounds[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        g_print("O efeito: %s, Não foi encontrada.\n", name);
        return;
    }
    
    playSoundByIndex(timeout, index, assets, loop);
}

// Para todos os sons e reseta o vetor de canais de áudio
void stopAllSounds() {
    for(int i=0; i<10; i++) {
        if(soundsPlayed[i] != -1)
            Mix_HaltChannel(i);
        soundsPlayed[i] = -1;
    }
}

void cleanupAudio() {
    stopCurrentMusic();
    stopAllSounds();

    for(int i=0; i<50; i++) {
        if(audioPointer.musics[i].music)
            Mix_FreeMusic(audioPointer.musics[i].music);
        if(audioPointer.sounds[i].sound)
            Mix_FreeChunk(audioPointer.sounds[i].sound);
    }
    Mix_CloseAudio();
    SDL_Quit();
}
/*
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    initAudio();

    // Cria a janela GTK
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Controle de Áudio");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

                // Toca som por cima
    playMusicByName(2000, "got_open", &audioPointer, 0);
    playSoundByName(3000, "click", &audioPointer, 0);
    //playMusicByIndex(2000, 0, &audioPointer, 0);
    //SDL_Delay(5000); // Espera 5s (substitua por lógica GTK)

    //stop_sound();    // Opcional: parar efeito antes de terminar
    //stop_music();    // Parar música

    // Define o comportamento de fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    cleanupAudio();

    return 0;
}

// Função WinMain para compatibilidade com -mwindows no Windows
#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif*/