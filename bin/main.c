#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <glib.h>
#include <direct.h>
#include <math.h>
#include "dlibs.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

// Compilação necessária para funcionar
// cd C:/Users/Amage/Desktop/Prog/Projects/WorldOfDragons/bin/
// cd C:/Users/Amage/Desktop/Programacao/GKT_C/WorldOfDragons/bin
// gcc -o main main.c files_libs.c sorts_libs.c `pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango`
// Sem terminal: gcc -o main.exe main.c audio_libs.c animations_libs.c files_libs.c sorts_libs.c account.c player_libs.c battle_libs.c -mwindows $(pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango) -lSDL2 -lSDL2_mixer

// =====================================================================================================
// Estrutura de dados GTK
typedef struct {
    GtkWidget * widgetSingle;
    GtkWidget * widgetVector[100];
    gint intSingle;
    gfloat floatSingle;
    gint intVector[100];
    gfloat floatVector[200];
    gchar string[300];
} gtkData;

typedef struct {
    GtkWidget * widget;
    GtkFixed *fixed;
    gint totalLoops;
    gint actualPosX;
    gint actualPosY;
    gint finalPosX;
    gint finalPosY;
    gint stepX;
    gint stepY;
    gfloat stepDistance;
    gfloat actualStep;
    gint finalStep;
    gint timer;
    gboolean isActive;
    gboolean changed;
} gtkAnimationData;

typedef struct {
    GtkWidget *widget;
    GtkFixed *container;
    gfloat opacity;
    gfloat opacityDecrease;
    gint x;
    gint y;
    gint interactions;
} GtkUpAnimationData;

typedef struct {
    GtkFixed *fixed;
    GtkWidget *wdLvlUpText;
    GtkWidget *wdLevelBar;
    GtkLabel *lbExpText;
    GtkLabel *lbLvl;
    gint experience;
    gint totalLoops;
    gfloat currentStep;
    gfloat stepDistance;
} gtkLevelUpAnimationData;

typedef struct {
    GtkStack *stack;
    gchar page[128];
} gtkTimedStack;


typedef struct {
    gint *actualValue;
    gint minValue;
    gint maxValue;
    gdouble duration;        // duração da animação em segundos
    gint64 start_time;       // tempo em microssegundos
    GtkWidget *pointer;
    GtkFixed *fixed;
    gint actualY;
    gint direction;
} animMeterbarData;

typedef struct {
    gint *actualValue;
    gint minValue;
    gint maxValue;
    GtkWidget *pointer;
    GtkFixed *fixed;
    gint actualY;
    gint enemyForce;
    gint playerForce;
} animChallengeData;

// Ponteiros globais
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// Game
gchar request[300];
gboolean keyPressed;
Game *game;

// Animations
GdkPixbuf *pAnimationsFrameVector[20][500];  // Animations array

// Audio
gboolean finishedMusic;

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

// Frame 5
Dragon * pBeastVector;
Dragon * pOriginalBeastVector;
Attack * pAttackVector;
GtkStack *fr5_stack;
GtkFixed *fr5_beastiary;
GtkLabel *fr5_tittle_label;
int fr5_actual_page;
int totalBeasts;
int totalAttacks;

// Player variables
FILE *playerFile;
Player player;

// XP Bar
GtkLabel *fr5_label_lvl;
GtkLabel *fr5_exp_text;
GtkWidget *fr5_level_bar;
GtkWidget *fr5_levelup_text;

// Beastiary
GtkWidget *fr5_history_container;
GtkWidget *fr5_dragon_img;
GtkLabel *fr5_label_dragon_history;
GtkLabel *fr5_dragon_age;
GtkLabel *fr5_text_sort;
GtkLabel *fr5_btn_marker;
GtkFixed *fr5_btns_container;

// Cave
GtkStack *fr5_cave_stack;
GtkFixed *fr5_cave;
GtkWidget *fr5_cave_health_up;
GtkWidget *fr5_cave_attack_up;
GtkWidget *fr5_cave_defense_up;
GtkWidget *fr5_cave_speed_up;

// Coliseum
GtkStack *fr5_coliseum_stack;
int fr5_actual_dragon_index;


// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Registro de funções referentes a tela
// ********************************************************************************************************
void registerSignals(GtkBuilder *builder);
void switchPage(GtkButton *btn, gpointer user_data);
void clean_elements(GtkEntry **input_vec, GtkLabel **label_vec);
void labeltextModifier(GtkLabel *label, const gchar *text);
static void set_cursor_window(GtkWidget *widget, gpointer data);
void settingTimedLabelModifier(gint timeout, GtkLabel *label, gchar *text);
gboolean btn_animation_rest_opacity(gpointer data);
void btn_animation_clicked(GtkWidget *widget, gpointer data);
void set_dragon_in_beastiary(GtkButton *btn, gpointer data);
void sort_dragons_in_beastiary(GtkButton *btn, gpointer data);
void settingUpdatelvlBarAnimation(gint exp, GtkLabel *lvlTxt, GtkLabel *expTxt, GtkWidget *lvlBar, GtkFixed *fixed, GtkWidget *lvlUpTxt);
gboolean updateBarAnimation(gpointer data);
gboolean levelUpAnimation(gpointer data);
gboolean atributeUpAnimation(gpointer data);
void updateDataCave();
void updateColiseum();
void updatelvlDragon(GtkButton *btn, gpointer data);
void labelTextAnimation(GtkLabel *label, gchar *text, int timer);
void set_attack_in_cave(GtkButton *btn, gpointer data);
void retroBarAnimationStart(gint timer, GtkWidget *widget, gint actualValue, gint newValue);
void settingMoveWidgetAnimation(gint timer, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY);
void settingTimedMoveWidgetAnimation(gint timerAnimation, gint timeout, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY);
void loadAnimationFrames(gchar *path, gint totalFrames, gint animationIndex);
void registerTexturesAnimations();
void settingTimedStackChange(gint timeout, GtkStack *stack, gchar *page);
void sendRequest(GtkButton *btn, gpointer user_data);
void settingTimedImageModifier(gint timeout, GtkWidget *widget, gchar *path);
void updateDebuffAnimation(gint entityNumber, gchar *type, Debuff *debuff, gint animationType, gchar *status);
void settingTimedNumbersAnimation(gint timeout, GtkLabel *label, gint range, gint animTime);
void removeAllStyleClasses(GtkWidget *widget);
void settingMeterbarAnimation(GtkWidget *pointer, GtkFixed *fixed, gint actualY, gint minValue, gint maxValue, gint *actualValue, gdouble duration);
gboolean meterBarAnimation(gpointer data);
void settingChallengeAnimation(GtkWidget *pointer, GtkFixed *fixed, gint actualY, gint minValue, gint maxValue, gint *actualValue, gint enemyForce);
gboolean challengeAnimation(gpointer data);
gboolean timedStartChallengeGame(gpointer data);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data);
gboolean timedGtkDestroyObject(gpointer data);
gboolean timedSwitchBooleanValue(gpointer data);
gboolean timedInverseBooleanValue(gpointer data);
gboolean timedLvlBarUpdate(gpointer data);
gboolean settingNumbersAnimation(gpointer data);
gboolean timedNumbersAnimation(gpointer data);
gboolean timedImageModifier(gpointer data);
gboolean timedStackChange(gpointer data);
gboolean moveWidgetAnimation(gpointer data);
gboolean retroBarAnimationLoop(gpointer data);
gboolean turnOnButton(gpointer data);
gboolean timedSettingMoveWidgetAnimation(gpointer data);
gboolean timedLabelModifier(gpointer data);
gboolean timedLabelAnimation(gpointer data);
gboolean timedgFree(gpointer data);
gboolean updateLoaderFrame(gpointer data);
gboolean settingBattleWindow(gpointer data);
gboolean startBattle(gpointer data);

// Animações
void logStartAnimation(gchar *text, gchar *color, gint duration, gint height, gint width, gint x, gint y, gint yDirection, GtkFixed *fixed);
gboolean logAnimation(gpointer data);

// ********************************************************************************************************

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.utf8");
    gtk_init(&argc, &argv); // Init gtk
    
    srand(time(NULL)); // Inicia a semente de randomização

    initAudio(); // Inicia o sdl e carrega os áudios
    
    // Inicialização dos ponteiros GTK
    GtkCssProvider *css_provider;
    // Iniciando interface XML para C
    builder = gtk_builder_new_from_file("../assets/ui_files/T_Dragons.glade");
    if (!builder) {
        g_printerr("Erro ao carregar interface.glade\n");
        return 1;
    }
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

    // Frame 5 Principal
    // Imagem background
    GtkWidget *fr5_image = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_img"));
    gtk_image_set_from_file(GTK_IMAGE(fr5_image), "../assets/img_files/background_main.png");

    // Gtk Fixed e Gtk Stack
    fr5_stack = GTK_STACK(gtk_builder_get_object(builder, "fr5_stack"));
    fr5_beastiary = GTK_FIXED(gtk_builder_get_object(builder, "fr5_beastiary"));
    fr5_tittle_label = GTK_LABEL(gtk_builder_get_object(builder, "fr5_tittle_label"));
    
    // Barra de experiência
    fr5_actual_page = 1;
    fr5_levelup_text = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_levelup_text"));
    fr5_label_lvl = GTK_LABEL(gtk_builder_get_object(builder, "fr5_label_lvl"));
    fr5_exp_text = GTK_LABEL(gtk_builder_get_object(builder, "fr5_exp_text"));
    fr5_level_bar = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_level_bar"));

    // Bestiário
    GtkWidget *fr5_page_view1 = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_page_view1"));
    gtk_image_set_from_file(GTK_IMAGE(fr5_page_view1), "../assets/img_files/beastiary_page1.png");

    GtkButton *fr5_btn_dragon1 = GTK_BUTTON(gtk_builder_get_object(builder, "fr5_btn_dragon1"));
    fr5_history_container = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_history_container"));
    fr5_dragon_img = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_dragon_img"));
    fr5_label_dragon_history = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_history"));
    fr5_dragon_age = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_age"));
    fr5_text_sort = GTK_LABEL(gtk_builder_get_object(builder, "fr5_text_sort"));
    fr5_btn_marker = GTK_LABEL(gtk_builder_get_object(builder, "fr5_btn_marker"));
    fr5_btns_container = GTK_FIXED(gtk_builder_get_object(builder, "fr5_btns_container"));

    // Inicializar variáveis globais
    // Inicializar bestiário
    FILE * beastsFile = createBeastslistfile();
    totalBeasts = beastsLength(beastsFile);
    pBeastVector = readBeastvector(beastsFile);
    pOriginalBeastVector = readBeastvector(beastsFile);
    
    // Iniciando músicas
    playMusicByIndex(0, musicsBackground.musicsAvailable[musicsBackground.currentMusic], &audioPointer, 0);
    
    // Caverna do dragão
    GtkWidget *fr5_page_view2 = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_page_view2"));
    gtk_image_set_from_file(GTK_IMAGE(fr5_page_view2), "../assets/img_files/beastiary_page2.png");

    // Inicializar variáveis globais
    // Inicializar Caverna
    FILE * attacksFile = createAttackslistfile();
    totalAttacks = attacksLength(attacksFile);
    pAttackVector = readAttackvector(attacksFile);

    // Gtk Stack e Gtk Fixed
    fr5_cave_stack = GTK_STACK(gtk_builder_get_object(builder, "fr5_cave_stack"));
    fr5_cave = GTK_FIXED(gtk_builder_get_object(builder, "fr5_cave"));
    
    // Widgets
    fr5_cave_health_up = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_cave_health_up"));
    fr5_cave_attack_up = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_cave_attack_up"));
    fr5_cave_defense_up = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_cave_defense_up"));
    fr5_cave_speed_up = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_cave_speed_up"));

    // Colíseu
    GtkWidget *fr5_page_view3 = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_page_view3"));
    gtk_image_set_from_file(GTK_IMAGE(fr5_page_view3), "../assets/img_files/beastiary_page3.png");    
    
    // Gtk Stack
    fr5_coliseum_stack = GTK_STACK(gtk_builder_get_object(builder, "fr5_coliseum_stack"));
    fr5_actual_dragon_index = 26;


    //GtkWidget *fr6_life_bar_ent1 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_life_bar_ent1"));
    //retroBarAnimationStart(1000 ,fr6_life_bar_ent1, 2046, 1200);

    // Inicializar player
    strcpy(request, "");
    //FILE *accountsFile = createAccountslistfile();
    //g_print("Conta deletada: %d\n", delAccountinlist(accountsFile, "Rambo"));
    //newAccount(accountsFile, "Rambo", "rahs@gmail.com", "1234");
    playerFile = getAccountfile("Rambo");
    player = getPlayer(playerFile);
    settingUpdatelvlBarAnimation(0, fr5_label_lvl, fr5_exp_text, fr5_level_bar, fr5_beastiary, fr5_levelup_text);

    //initPlayer(playerFile, &player);
    //changePlayerStatus(playerFile, 0, 0, 0, 1, 27, 27, NULL);

    // Inicializar ações na tela 5
    sort_dragons_in_beastiary(fr5_btn_dragon1, NULL);
    set_dragon_in_beastiary(fr5_btn_dragon1, GINT_TO_POINTER(0));
    updateDataCave();
    updateColiseum();

    // Registra todas as animações no sistema    
    registerTexturesAnimations();

    // Conecta todos os sinais necessários a tela
    gtk_widget_realize(window);
    registerSignals(builder);
    g_signal_connect(window, "map", G_CALLBACK(set_cursor_window), NULL);

    gtk_widget_show_all(window);
    
    gtk_main();

    cleanupAudio();
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    Game *game = (Game*) data;
    gchar keyval_name[32];
    gint pressed_number;
    // Obtém o nome da tecla pressionada
    snprintf(keyval_name, sizeof(keyval_name), "%s", gdk_keyval_name(event->keyval));
    if(!keyPressed) {
      if(strcmp(keyval_name, "space") == 0 && !game->doors.mgMeterPlayed && strcmp(game->minigame->name, "meterbar") == 0) {
            game->minigame->minigameResultValue = *(game->minigame->minigameValue);
            game->doors.mgMeterPlayed = TRUE;
            *(game->minigame->minigameValue) = -1;
            playSoundByName(0, "meterbar_click", &audioPointer, 0);
        }
        if((strcmp(keyval_name, "x") == 0 || strcmp(keyval_name, "X") == 0) && !game->doors.mgChallengerPlayed && strcmp(game->minigame->name, "challenge") == 0) {     
            gint playerForce = 14 + ( 18.0 / 92.0 ) * game->battle->EntityOne.entDragon.level;
            //g_print("Força do player: %d\n", playerForce);
            *(game->minigame->minigameValue) += playerForce;
        }  
    }
    //if(strcmp(keyval_name, "c") == 0 && !game->minigame->minigamePlayed && strcmp(game->minigame->name, "challenge") == 0) {
    //    *(game->minigame->minigameValue) += 24;
    //}  

    keyPressed = TRUE;
    //g_print("Tecla pressionada %s, number:%d \n", keyval_name, pressed_number);
    
    return FALSE; 
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    keyPressed = FALSE;
    return FALSE;
}

void settingMeterbarAnimation(GtkWidget *pointer, GtkFixed *fixed, gint actualY, gint minValue, gint maxValue, gint *actualValue, gdouble duration) {
    animMeterbarData *data = g_malloc(sizeof(animMeterbarData));
    *actualValue = minValue;
    data->actualValue = actualValue;
    data->maxValue = maxValue;
    data->minValue = minValue;
    data->pointer = pointer;
    data->fixed = fixed;
    data->actualY = actualY;
    data->direction = 1;
    data->duration = duration;
    data->start_time = g_get_monotonic_time(); // tempo atual em microssegundos

    g_timeout_add(16, meterBarAnimation, data);    
}

gboolean meterBarAnimation(gpointer data) {
    animMeterbarData *animData = (animMeterbarData*) data;
    
    if(*(animData->actualValue) == -1 || game->doors.mgMeterPlayed) {
        g_print("METERBAR FINALIZADO ************************\n");
        g_free(animData);
        return FALSE;
    }

    gint64 now = g_get_monotonic_time(); // microssegundos
    gdouble elapsed = (now - animData->start_time) / 1000000.0; // segundos
    gdouble t = elapsed / animData->duration;
    if (t > 1.0) t = 1.0;
    gdouble progress = (1 - cos(t * G_PI)) / 2;

    if (progress >= 1.0) {
        // Inverte a direção
        animData->direction *= -1;
        animData->start_time = now;
        progress = 0.0;
    }

    // Interpola o valor baseado na direção
    gint range = animData->maxValue - animData->minValue;
    if (animData->direction == 1) {
        *(animData->actualValue) = animData->minValue + (gint)(range * progress);
    } else {
        *(animData->actualValue) = animData->maxValue - (gint)(range * progress);
    }

    // Move o ponteiro
    gtk_fixed_move(animData->fixed, animData->pointer, *(animData->actualValue), animData->actualY);

    return TRUE; // continua o loop
}

void settingChallengeAnimation(GtkWidget *pointer, GtkFixed *fixed, gint actualY, gint minValue, gint maxValue, gint *actualValue, gint enemyForce) {
    animChallengeData *data = g_malloc(sizeof(animChallengeData));
    *actualValue = (minValue+maxValue)/2;
    data->actualValue = actualValue;
    data->maxValue = maxValue;
    data->minValue = minValue;
    data->pointer = pointer;
    data->fixed = fixed;
    data->actualY = actualY;
    data->enemyForce = enemyForce;
    g_print("INICIANDO CHALLENGER AGORA *******************************************\n");
    g_timeout_add(16, challengeAnimation, data);    
}

gboolean challengeAnimation(gpointer data) {
    animChallengeData *animData = (animChallengeData*) data;
     
    if(game->doors.mgChallengerPlayed) {
        g_print("Challenger finalizado ************************\n");
        game->minigame->isActive = FALSE;
        *(animData->actualValue) = 1;
        g_free(animData);
        return FALSE;
    }
    if(animData) {
        if(*(animData->actualValue) < animData->minValue) { // Perde o minigame
            gtk_fixed_move(animData->fixed, animData->pointer, animData->minValue, animData->actualY);
            game->doors.mgChallengerPlayed = TRUE;
            game->minigame->minigameResultValue = -1;
            g_print("LOST minigame !_!_!__!_!_!_!_!__!_!_!_!_!\n");
            return TRUE;
        }
        else if(*(animData->actualValue) > animData->maxValue) { // Ganha o minigame
            gtk_fixed_move(animData->fixed, animData->pointer, animData->maxValue, animData->actualY);
            game->doors.mgChallengerPlayed = TRUE;
            game->minigame->minigameResultValue = 1;
            g_print("WON minigame !_!_!__!_!_!_!_!__!_!_!_!_!\n");
            return TRUE;
        }
        
        // Move o ponteiro
        gtk_fixed_move(animData->fixed, animData->pointer, *(animData->actualValue), animData->actualY);
        *(animData->actualValue) -= animData->enemyForce;
    }
    
    return TRUE; // continua o loop
}

gboolean timedStartChallengeGame(gpointer data) {
    Game *game = (Game*) data;
    GtkWidget *fr6_battle_challenge_pointer = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_battle_challenge_pointer"));
    GtkFixed *fr6_battle_challenge = GTK_FIXED(gtk_builder_get_object(builder, "fr6_battle_challenge"));
    GtkWidget *fr6_battle_challenge_animation = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_battle_challenge_animation"));

    gtk_fixed_move(fr6_battle_challenge, fr6_battle_challenge_pointer, 127, 47);
    
    // Conecta o evento de pressionar teclas à função de callback
    //game->minigame->minigamePlayed = FALSE;
    *(game->minigame->minigameValue) = 0;
    strcpy(game->minigame->name, "challenge");
    gtk_widget_realize(window);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), game);    
    g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), game);  
    g_print("Dificuldade: %d\n", game->battle->difficult); 
    settingTimedVideoPlay(fr6_battle_challenge_animation, 0, 4, "keypress", 1, game->minigame->minigameValue, FALSE); 
    settingChallengeAnimation(fr6_battle_challenge_pointer, fr6_battle_challenge, 47, 21, 237, game->minigame->minigameValue, game->battle->difficult);
    return FALSE;
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

    // Frame 5 Elementos para analisar e limpar
    GtkEntry *fr5_cave_inp_name = GTK_ENTRY(gtk_builder_get_object(builder, "fr5_cave_inp_name"));
    GtkLabel *fr5_cave_dragon_name_error = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_dragon_name_error"));

    // Altera a página visível da GtkStack com base no nome do botão
    // Frame 1
    if (g_strcmp0(button_name, "fr1_btn_start") == 0) {
        gtk_stack_set_visible_child_name(main_stack, "login_page");
    }

    // Frame 5 Main
    if (g_strcmp0(button_name, "fr5_btn_next") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        playSoundByName(0, "menu_change", &audioPointer, 0);
        gtk_stack_set_transition_type(GTK_STACK(fr5_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
        if(fr5_actual_page == 1) {
            gtk_stack_set_visible_child_name(fr5_stack, "fr5_cave");
            labeltextModifier(fr5_tittle_label, "Caverna");
            fr5_actual_page++;
        }
        else if(fr5_actual_page == 2) {
            gtk_stack_set_visible_child_name(fr5_stack, "fr5_coliseum");
            labeltextModifier(fr5_tittle_label, "Colíseu");
            updateColiseum();
            fr5_actual_page++;
        }
    }
    
    if (g_strcmp0(button_name, "fr5_btn_previous") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        playSoundByName(0, "menu_change", &audioPointer, 0);
        gtk_stack_set_transition_type(GTK_STACK(fr5_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
        if(fr5_actual_page == 3) {
            gtk_stack_set_visible_child_name(fr5_stack, "fr5_cave");
            labeltextModifier(fr5_tittle_label, "Caverna");
            fr5_actual_page--;
        }
        else if(fr5_actual_page == 2) {
            gtk_stack_set_visible_child_name(fr5_stack, "fr5_beast");
            labeltextModifier(fr5_tittle_label, "Bestiário");
            fr5_actual_page--;
        }
        
    }

    // Frame 5 Caverna
    if (g_strcmp0(button_name, "fr5_cave_btn_newdragon") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        gtk_stack_set_visible_child_name(fr5_cave_stack, "fr5_cave_confirm");
    }

    if (g_strcmp0(button_name, "fr5_cave_btn_back") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        gtk_stack_set_visible_child_name(fr5_cave_stack, "fr5_cave_newdragon");
        gtk_entry_set_text(fr5_cave_inp_name, "");
        labeltextModifier(fr5_cave_dragon_name_error, "");
    }

    if (g_strcmp0(button_name, "fr5_cave_btn_confirm") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        char dragonName[100];
        strcpy(dragonName, gtk_entry_get_text(fr5_cave_inp_name));
        if(strlen(dragonName) >= 1) {
            gtk_stack_set_visible_child_name(fr5_cave_stack, "fr5_cave_actualdragon");
            getplayerDragon(playerFile, dragonName);
            settingUpdatelvlBarAnimation(1, fr5_label_lvl, fr5_exp_text, fr5_level_bar, fr5_beastiary, fr5_levelup_text);
            //changePlayerStatus(playerFile, -1, -1, -1, -1, 1, 1, NULL);
            updateColiseum();
            updateDataCave();
        }
        else
            labeltextModifier(fr5_cave_dragon_name_error, "O nome deve possuir entre 1 e 30 caracteres.");
        
    }

    // Frame 5 Colíseu
    if (g_strcmp0(button_name, "fr5_btn_coliseum_next") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);

        if(fr5_actual_dragon_index > 0) {
            playSoundByName(0, "dragon_change", &audioPointer, 0);
            fr5_actual_dragon_index--;
            if(g_strcmp0(gtk_stack_get_visible_child_name(fr5_coliseum_stack), "0") == 0) {
                gtk_stack_set_transition_type(GTK_STACK(fr5_coliseum_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
                gtk_stack_set_visible_child_name(fr5_coliseum_stack, "1");
            }
            else {
                gtk_stack_set_transition_type(GTK_STACK(fr5_coliseum_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
                gtk_stack_set_visible_child_name(fr5_coliseum_stack, "0");
            }
            updateColiseum();
            // Atualiza a página com novo index...
        }
    }

    if (g_strcmp0(button_name, "fr5_btn_coliseum_return") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);

        if(fr5_actual_dragon_index < 26) {
            playSoundByName(0, "dragon_change", &audioPointer, 0);
            fr5_actual_dragon_index++;
            if(g_strcmp0(gtk_stack_get_visible_child_name(fr5_coliseum_stack), "1") == 0) {
                gtk_stack_set_transition_type(GTK_STACK(fr5_coliseum_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
                gtk_stack_set_visible_child_name(fr5_coliseum_stack, "0");
            }
            else {
                gtk_stack_set_transition_type(GTK_STACK(fr5_coliseum_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT);
                gtk_stack_set_visible_child_name(fr5_coliseum_stack, "1");
            }
            updateColiseum();
        }
    }

    if (g_strcmp0(button_name, "fr5_btn_battle") == 0) {
        gint dragonIndex = fr5_actual_dragon_index;
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        //g_print("Name: %s | Unlock id: %d | actual progress: %d\n", pOriginalBeastVector[dragonIndex].name, pOriginalBeastVector[dragonIndex].unlock_id, player.actualProgress);

        if(((pOriginalBeastVector[dragonIndex].unlock_id-26) * -1) <= player.actualProgress && strlen(player.dragon.name) > 0 && player.dragon.level > 0) {
            GtkStack *fr6_stack = GTK_STACK(gtk_builder_get_object(builder, "fr6_stack"));
            GtkLabel *fr6_dragon_name = GTK_LABEL(gtk_builder_get_object(builder, "fr6_dragon_name"));
            GtkLabel *fr6_enemy_name_common = GTK_LABEL(gtk_builder_get_object(builder, "fr6_enemy_name_common"));
            GtkLabel *fr6_enemy_name_rare = GTK_LABEL(gtk_builder_get_object(builder, "fr6_enemy_name_rare"));
            GtkLabel *fr6_enemy_name_epic = GTK_LABEL(gtk_builder_get_object(builder, "fr6_enemy_name_epic"));
            GtkLabel *fr6_enemy_name_legendary = GTK_LABEL(gtk_builder_get_object(builder, "fr6_enemy_name_legendary"));
            GtkImage *fr6_stared_bg = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_stared_bg"));
            GtkImage *fr6_stared_player_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_stared_player_dragon"));
            GtkImage *fr6_stared_enemy_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_stared_enemy_dragon"));
              
            gtk_stack_set_visible_child_name(main_stack, "combat_page");
            gtk_stack_set_visible_child_name(fr6_stack, "fr6_stared");
            labeltextModifier(fr6_enemy_name_legendary, "");
            labeltextModifier(fr6_enemy_name_epic, "");
            labeltextModifier(fr6_enemy_name_rare, "");
            labeltextModifier(fr6_enemy_name_common, "");

            gchar *dragonBackgroundPath = g_strdup_printf("../assets/img_files/battle/%s_bg.png", pOriginalBeastVector[dragonIndex].name);
            gchar *enemyDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_%s.png", pOriginalBeastVector[dragonIndex].name);
            gchar *playerDragonImgPath, dragonStage[50], *playerDragonName;
            gtkData *labelAnimationDataEnemy = g_malloc(sizeof(gtkData));
            
            if(player.dragon.level >= 1) {
                playerDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_baby_wyvern_1.png", pOriginalBeastVector[dragonIndex].name);
                strcpy(dragonStage, "( Bebê )");
            }
            if(player.dragon.level >= 10) {
                playerDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_juvenile_wyvern_1.png", pOriginalBeastVector[dragonIndex].name);
                strcpy(dragonStage, "( Jovem )");
            }
            if(player.dragon.level >= 50) {
                playerDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_adult_wyvern_1.png", pOriginalBeastVector[dragonIndex].name);
                strcpy(dragonStage, "( Adulto )");
            }
            if(player.dragon.level >= 80) {
                playerDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_titan_wyvern_1.png", pOriginalBeastVector[dragonIndex].name);
                strcpy(dragonStage, "( Titã )");
            }

            gtk_image_set_from_file(fr6_stared_enemy_dragon, enemyDragonImgPath);
            gtk_image_set_from_file(fr6_stared_player_dragon, playerDragonImgPath);
            gtk_image_set_from_file(fr6_stared_bg, dragonBackgroundPath);

            labelAnimationDataEnemy->intSingle = 1500;
            strcpy(labelAnimationDataEnemy->string, pOriginalBeastVector[dragonIndex].tittle);
            
            if (dragonIndex < 3) {
                labelAnimationDataEnemy->widgetSingle = GTK_WIDGET(fr6_enemy_name_legendary);
            }
            else if (dragonIndex > 2  && dragonIndex < 8) {
                labelAnimationDataEnemy->widgetSingle = GTK_WIDGET(fr6_enemy_name_epic);
            }
            else if (dragonIndex > 7  && dragonIndex < 16) {
                labelAnimationDataEnemy->widgetSingle = GTK_WIDGET(fr6_enemy_name_rare);
            }
            else if (dragonIndex > 15) {
                labelAnimationDataEnemy->widgetSingle = GTK_WIDGET(fr6_enemy_name_common);
            }

            GtkFixed *fr6_stared_fixed_animation = GTK_FIXED(gtk_builder_get_object(builder, "fr6_stared_fixed_animation"));
            GtkWidget *fr6_stared_player_dragon_wid = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_stared_player_dragon"));
            GtkWidget *fr6_stared_dragon_border2 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_stared_dragon_border2"));
            GtkWidget *fr6_stared_enemy_dragon_wid = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_stared_enemy_dragon"));
            GtkWidget *fr6_stared_dragon_border = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_stared_dragon_border"));
            
            gtk_fixed_move(fr6_stared_fixed_animation, fr6_stared_player_dragon_wid,-250, 18);
            gtk_fixed_move(fr6_stared_fixed_animation, fr6_stared_dragon_border2,-262, 4);
            settingTimedMoveWidgetAnimation(330, 3000, fr6_stared_player_dragon_wid, fr6_stared_fixed_animation, -250, 18, 80, -1);
            settingTimedMoveWidgetAnimation(330, 3000, fr6_stared_dragon_border2, fr6_stared_fixed_animation, -262, 4, 68, -1);

            playerDragonName = g_strdup_printf("%s %s", player.dragon.name, dragonStage);
            gtkData *labelAnimationDataPlayer = g_malloc(sizeof(gtkData));
            labelAnimationDataPlayer->intSingle = 1500;
            strcpy(labelAnimationDataPlayer->string, playerDragonName);
            labelAnimationDataPlayer->widgetSingle = GTK_WIDGET(fr6_dragon_name);
            labeltextModifier(GTK_LABEL(fr6_dragon_name), "");
            g_timeout_add(3330, timedLabelAnimation, labelAnimationDataPlayer);

            gtkData *labelVersusText = g_malloc(sizeof(gtkData));
            GtkWidget *fr6_label_vs_text = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_label_vs_text"));
            labelVersusText->intSingle = 750;
            strcpy(labelVersusText->string, "VS");
            labelVersusText->widgetSingle = GTK_WIDGET(fr6_label_vs_text);
            labeltextModifier(GTK_LABEL(fr6_label_vs_text), "");
            g_timeout_add(4830, timedLabelAnimation, labelVersusText);
            
            GtkFixed *fr6_stared = GTK_FIXED(gtk_builder_get_object(builder, "fr6_stared"));
            GtkWidget *fr6_video_loader = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_video_loader"));

            gtk_fixed_move(fr6_stared_fixed_animation, fr6_stared_enemy_dragon_wid, 1014, 18);
            gtk_fixed_move(fr6_stared_fixed_animation, fr6_stared_dragon_border, 1002, 4);
            settingTimedMoveWidgetAnimation(330, 5580, fr6_stared_enemy_dragon_wid, fr6_stared_fixed_animation, 1014, 18, 702, -1);
            settingTimedMoveWidgetAnimation(330, 5580, fr6_stared_dragon_border, fr6_stared_fixed_animation, 1002, 4, 688, -1);


            g_timeout_add(5910, timedLabelAnimation, labelAnimationDataEnemy);
            settingTimedVideoPlay(fr6_video_loader, 0, 109, "battle_opening", 0, NULL, FALSE);
            settingTimedVideoPlay(fr6_video_loader, 7620, 144, "battle_opening2", 0, NULL, FALSE);

            g_print("Pode batalhar\n");
            Battle *battleInstance = g_malloc(sizeof(Battle));
            strcpy(request, "");
            player = getPlayer(playerFile);
            strcpy(player.dragon.img_path, playerDragonImgPath);
            setBattleVariables(battleInstance, player.dragon, pOriginalBeastVector[dragonIndex], player, dragonIndex);
            stopCurrentMusic();
            musicsBackground.inBattle = TRUE;
            playMusicByName(0, pOriginalBeastVector[dragonIndex].name, &audioPointer, -1);
            g_timeout_add(9420, settingBattleWindow, battleInstance);
        }
        else {
            GtkFixed *fixed = GTK_FIXED(gtk_builder_get_object(builder, "fr5_coliseum"));
            logStartAnimation("Você não pode batalhar.", "color_FF0000", 1000, 44, 175, 710, 292, 10, fixed);
            g_print("Não pode batalhar\n");
        }
    }

    if (g_strcmp0(button_name, "fr5_add_experience") == 0) {
        settingUpdatelvlBarAnimation(20000, fr5_label_lvl, fr5_exp_text, fr5_level_bar, fr5_beastiary, fr5_levelup_text);
    }
    
    // Frame 7 - Resultado de batalha
    if (g_strcmp0(button_name, "fr7_btn_continue") == 0) {
        GtkWidget *btn_detail = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_coliseum_battle_label1"));
        GtkStack *fr6_stack = GTK_STACK(gtk_builder_get_object(builder, "fr6_stack"));
        playSoundByName(0, "click", &audioPointer, 0);
        settingUpdatelvlBarAnimation(0, fr5_label_lvl, fr5_exp_text, fr5_level_bar, fr5_beastiary, fr5_levelup_text);
        gtk_stack_set_visible_child_name(main_stack, "bestiary_page");
        gtk_stack_set_visible_child_name(fr6_stack, "fr6_stared");
        sort_dragons_in_beastiary(NULL, GINT_TO_POINTER(1));
        stopCurrentMusic();
        musicsBackground.inBattle = FALSE;
        musicsBackground.isFinished = FALSE;
        playMusicByIndex(0, musicsBackground.musicsAvailable[musicsBackground.currentMusic], &audioPointer, 0);
        updateColiseum();
    }   

    // Retira o foco de todos os elementos
    // Cria um widget invisível para receber o foco temporário
    GtkWidget *dummy = GTK_WIDGET(gtk_builder_get_object(builder, "fr2_dummy"));
    gtk_widget_grab_focus(dummy); 
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

    // Frame 4 Botões
    GObject *fr4_btn_back = gtk_builder_get_object(builder, "fr4_btn_back");
    g_signal_connect(fr4_btn_back, "clicked", G_CALLBACK(switchPage), main_stack);

    GObject *fr4_btn_send = gtk_builder_get_object(builder, "fr4_btn_send");
    g_signal_connect(fr4_btn_send, "clicked", G_CALLBACK(switchPage), main_stack);

    GObject *fr4_btn_advance = gtk_builder_get_object(builder, "fr4_btn_advance");
    g_signal_connect(fr4_btn_advance, "clicked", G_CALLBACK(switchPage), main_stack);

    // Frame 5 Botões

    GObject *fr5_btn_next = gtk_builder_get_object(builder, "fr5_btn_next");
    g_signal_connect(fr5_btn_next, "clicked", G_CALLBACK(switchPage), fr5_stack);

    GObject *fr5_btn_previous = gtk_builder_get_object(builder, "fr5_btn_previous");
    g_signal_connect(fr5_btn_previous, "clicked", G_CALLBACK(switchPage), fr5_stack);
    
    GObject *fr5_btn_sort = gtk_builder_get_object(builder, "fr5_btn_sort");
    g_signal_connect(fr5_btn_sort, "clicked", G_CALLBACK(sort_dragons_in_beastiary), NULL);

    GObject *fr5_add_xp = gtk_builder_get_object(builder, "fr5_add_experience");
    g_signal_connect(fr5_add_xp, "clicked", G_CALLBACK(switchPage), NULL);

    // Caverna

    GObject *fr5_cave_btn_newdragon = gtk_builder_get_object(builder, "fr5_cave_btn_newdragon");
    g_signal_connect(fr5_cave_btn_newdragon, "clicked", G_CALLBACK(switchPage), NULL);
    
    GObject *fr5_cave_btn_back = gtk_builder_get_object(builder, "fr5_cave_btn_back");
    g_signal_connect(fr5_cave_btn_back, "clicked", G_CALLBACK(switchPage), NULL);
    
    GObject *fr5_cave_btn_confirm = gtk_builder_get_object(builder, "fr5_cave_btn_confirm");
    g_signal_connect(fr5_cave_btn_confirm, "clicked", G_CALLBACK(switchPage), NULL);

    GObject *fr5_cave_btn_train = gtk_builder_get_object(builder, "fr5_cave_btn_train");
    g_signal_connect(fr5_cave_btn_train, "clicked", G_CALLBACK(updatelvlDragon), NULL);
    
    for(int j=0; j < totalAttacks; j++) {
        char actBtnName[100];
        sprintf(actBtnName, "fr5_cave_btn_attack%d", j+1);
        GObject *actual_btn = gtk_builder_get_object(builder, actBtnName);
        g_signal_connect(actual_btn, "clicked", G_CALLBACK(set_attack_in_cave), GINT_TO_POINTER(j));
    }
    
    // Coliseu
    GObject *fr5_btn_coliseum_next = gtk_builder_get_object(builder, "fr5_btn_coliseum_next");
    g_signal_connect(fr5_btn_coliseum_next, "clicked", G_CALLBACK(switchPage), NULL);

    GObject *fr5_btn_coliseum_return = gtk_builder_get_object(builder, "fr5_btn_coliseum_return");
    g_signal_connect(fr5_btn_coliseum_return, "clicked", G_CALLBACK(switchPage), NULL);

    GObject *fr5_btn_battle = gtk_builder_get_object(builder, "fr5_btn_battle");
    g_signal_connect(fr5_btn_battle, "clicked", G_CALLBACK(switchPage), NULL);


    for(int i=0; i < totalBeasts; i++) {
        gchar actBtnName[100];
        sprintf(actBtnName, "fr5_btn_dragon%d", i+1);
        GObject *actual_btn = gtk_builder_get_object(builder, actBtnName);
        g_signal_connect(actual_btn, "clicked", G_CALLBACK(set_dragon_in_beastiary), GINT_TO_POINTER(i));
    }
    // Paínel de ataque da aba de combate
    for(int i=1; i <= 5; i++) {
        gchar actBtnName[100];
        sprintf(actBtnName, "fr6_btn_attack%d", i);
        GObject *actual_btn = gtk_builder_get_object(builder, actBtnName);
        g_signal_connect(actual_btn, "clicked", G_CALLBACK(sendRequest), GINT_TO_POINTER(i));
    }

    // Frame 7 de resultado de batalha
    GObject *fr7_btn_continue = gtk_builder_get_object(builder, "fr7_btn_continue");
    g_signal_connect(fr7_btn_continue, "clicked", G_CALLBACK(switchPage), NULL);
    
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

void btn_animation_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_set_opacity(widget, 0.7); // Altera a opacidade para dar um efeito de clique
    g_timeout_add(100, btn_animation_rest_opacity, widget); // Adiciona o timeout para restaurar a opacidade após 100 ms
    playSoundByName(0, "click", &audioPointer, 0);
}

void set_dragon_in_beastiary(GtkButton *btn, gpointer data) {
    btn_animation_clicked(GTK_WIDGET(btn), NULL);
    player = getPlayer(playerFile);
    int beastIndex = GPOINTER_TO_INT(data);
    char dragonAge[200], dragonName[200];
    GtkWidget *fr5_unknown_history = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_unknown_history"));
    Dragon actualBeast = pBeastVector[beastIndex];
    int actualHeight = ceil(strlen(actualBeast.history) / 1.75) - 15;

    gtk_widget_set_size_request(fr5_history_container, 257, actualHeight);
    gtk_fixed_move(fr5_btns_container, GTK_WIDGET(fr5_btn_marker), 6, 5 + (beastIndex+1) * 56 - 56);
    
    if(((pBeastVector[beastIndex].unlock_id-27) * -1) <= player.actualProgress) {
        strcpy(dragonName, actualBeast.name);
        sprintf(dragonAge, "Idade: %s\nTamanho: %s", actualBeast.age, actualBeast.length);
        labeltextModifier(fr5_dragon_age, dragonAge);
        labeltextModifier(fr5_label_dragon_history, actualBeast.history);
        gtk_image_set_from_file(GTK_IMAGE(fr5_dragon_img), actualBeast.img_path);
        gtk_image_clear(GTK_IMAGE(fr5_unknown_history));
    }
    else {
        labeltextModifier(fr5_dragon_age, "Idade: Desconhecido\nTamanho: Desconhecido");
        labeltextModifier(fr5_label_dragon_history, "");
        strcpy(dragonName, "???");
        gtk_widget_set_size_request(fr5_history_container, 257, 310);
        gtk_image_set_from_file(GTK_IMAGE(fr5_unknown_history), "../assets/img_files/beast_unknown_history.png");
        gtk_image_set_from_file(GTK_IMAGE(fr5_dragon_img), "../assets/img_files/beast_unknown_img.png");
    }

    for(int i=0; i < 4; i++) {
        char actLbName[100];
        sprintf(actLbName, "fr5_dragon_label%d", i+1);
        GtkLabel *actual_label = GTK_LABEL(gtk_builder_get_object(builder, actLbName));
        labeltextModifier(actual_label, "");
        if(beastIndex <= 2 && i == 1)
            labeltextModifier(actual_label, dragonName);

        if(8 > beastIndex && beastIndex > 2 && i == 2) 
            labeltextModifier(actual_label, dragonName);

        if(( 16 > beastIndex && beastIndex > 7 ) && i == 3) 
            labeltextModifier(actual_label, dragonName);
        
        if(beastIndex > 15 && i == 0) 
            labeltextModifier(actual_label, dragonName);
    }
    
}

void sort_dragons_in_beastiary(GtkButton *btn, gpointer data) {
    player = getPlayer(playerFile);

    if(data == NULL) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
        int typeSort = 1;
        const gchar *sortText = gtk_label_get_text(fr5_text_sort);
        if(strcmp(sortText, "Atributo") == 0) {
            typeSort = 2;
            labeltextModifier(fr5_text_sort, "Idade");
        }
        else if(strcmp(sortText, "Idade") == 0) {
            typeSort = 3;
            labeltextModifier(fr5_text_sort, "Ataque");
        }
        else if(strcmp(sortText, "Ataque") == 0) {
            typeSort = 4;
            labeltextModifier(fr5_text_sort, "Defesa");
        }
        else if(strcmp(sortText, "Defesa") == 0) {
            typeSort = 5;
            labeltextModifier(fr5_text_sort, "Velocidade");
        }
        else if(strcmp(sortText, "Velocidade") == 0) {
            typeSort = 6;
            labeltextModifier(fr5_text_sort, "Vida");
        }
        else if(strcmp(sortText, "Vida") == 0) {
            typeSort = 1;
            labeltextModifier(fr5_text_sort, "Atributo");
        }
        pBeastVector = bubbleSort(typeSort, pBeastVector, totalBeasts);
    }
    for(int i=0; i < totalBeasts; i++) {
        char actBtnName[100];
        sprintf(actBtnName, "fr5_btn_dragon%d", i+1);
        GtkButton *actual_btn = GTK_BUTTON(gtk_builder_get_object(builder, actBtnName));
        if(((pBeastVector[i].unlock_id-26) * -1) < player.actualProgress) {
            gtk_button_set_label(GTK_BUTTON(actual_btn), pBeastVector[i].name);
            gtk_widget_set_sensitive(GTK_WIDGET(actual_btn), TRUE);
        }
        else {
            gtk_button_set_label(GTK_BUTTON(actual_btn), "???");
            gtk_widget_set_sensitive(GTK_WIDGET(actual_btn), FALSE);
        }
    }
    GtkButton *fr5_btn_dragon1 = GTK_BUTTON(gtk_builder_get_object(builder, "fr5_btn_dragon1"));
    set_dragon_in_beastiary(fr5_btn_dragon1, GINT_TO_POINTER(0));
}

void set_attack_in_cave(GtkButton *btn, gpointer data) {
    btn_animation_clicked(GTK_WIDGET(btn), NULL);
    int index = GPOINTER_TO_INT(data);
    char attackDetails[400];
    int xPosVector[] = {42, 204, 362, 526};
    int widthVector[] = {118, 118, 118, 148};
    Attack actualAttack = pAttackVector[index];
    GtkLabel *fr5_cave_attack_name = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_attack_name"));
    GtkLabel *fr5_cave_description = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_description"));
    GtkLabel *fr5_cave_attack_details = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_attack_details"));
    GtkWidget *fr5_btn_cave_marker = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_btn_cave_marker"));

    sprintf(attackDetails, "Dano: ( Ataque x %.1f )\nTaxa de acerto: %d%\nTempo de recarga: %d turno(s)",
    actualAttack.multiplicator, actualAttack.precision, actualAttack.cooldownAttack);
    gtk_fixed_move(fr5_cave, GTK_WIDGET(fr5_btn_cave_marker), xPosVector[index], 410);
    gtk_widget_set_size_request(GTK_WIDGET(fr5_btn_cave_marker), widthVector[index], 36);

    labeltextModifier(fr5_cave_attack_name, actualAttack.name);
    labeltextModifier(fr5_cave_description, actualAttack.description);
    labeltextModifier(fr5_cave_attack_details, attackDetails);
}

void updatelvlDragon(GtkButton *btn, gpointer data) {
    btn_animation_clicked(GTK_WIDGET(btn), NULL);
    gtk_widget_set_sensitive(GTK_WIDGET(btn), FALSE);
    g_timeout_add(900, turnOnButton, GTK_WIDGET(btn));
    
    if(strlen(player.dragon.name) == 0)
        return;

    if(player.trainPoints > 0) {
        Dragon preTrainDragon = player.dragon;
        char newAttribute[50];
        player.dragon = trainplayerDragon(player.dragon, 1);

        sprintf(newAttribute, "+%d      ", player.dragon.health - preTrainDragon.health);
        labeltextModifier(GTK_LABEL(fr5_cave_health_up), newAttribute);
        sprintf(newAttribute, "+%d      ", player.dragon.attack - preTrainDragon.attack);
        labeltextModifier(GTK_LABEL(fr5_cave_attack_up), newAttribute);
        sprintf(newAttribute, "+%d      ", player.dragon.defense - preTrainDragon.defense);
        labeltextModifier(GTK_LABEL(fr5_cave_defense_up), newAttribute);
        sprintf(newAttribute, "+%d      ", player.dragon.speed - preTrainDragon.speed);
        labeltextModifier(GTK_LABEL(fr5_cave_speed_up), newAttribute);
        for(int i=0; i < 11; i++) {
            g_timeout_add((800.0/10) * i, atributeUpAnimation, GINT_TO_POINTER(i));
        }
        playSoundByName(0, "level_up_dragon", &audioPointer, 0);
        changePlayerStatus(playerFile, -1, player.trainPoints-1, -1, -1, -1, -1, &player.dragon);
        player = getPlayer(playerFile);
        GtkFixed *fixed = GTK_FIXED(gtk_builder_get_object(builder, "fr5_cave_actualdragon"));
        if(player.dragon.level == 1 || player.dragon.level == 10 || player.dragon.level == 50 || player.dragon.level == 80)
            settingTimedNewWidgetAnimation(0, 61, "dragon_grow_animation", fixed, 0, 0, 295, 283);

        updateDataCave();
    }


}

void updateDataCave() {
    if(strlen(player.dragon.name) > 0) {
        char textVar[300], dragonStage[50];
        gtk_stack_set_visible_child_name(fr5_cave_stack, "fr5_cave_actualdragon");
        GtkWidget *fr5_cave_dragon_img = GTK_WIDGET(gtk_builder_get_object(builder, "fr5_cave_dragon_img"));
        GtkLabel *fr5_cave_lvl_label = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_lvl_label"));
        GtkLabel *fr5_cave_name_label = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_name_label"));
        GtkLabel *fr5_cave_points = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_points"));
        GtkLabel *fr5_cave_health = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_health"));
        GtkLabel *fr5_cave_attack = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_attack"));
        GtkLabel *fr5_cave_defense = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_defense"));
        GtkLabel *fr5_cave_speed = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_speed"));

        sprintf(textVar, "Lvl.%d", player.dragon.level);
        labeltextModifier(fr5_cave_lvl_label, textVar);
        gtk_widget_set_halign(GTK_WIDGET(fr5_cave_lvl_label), GTK_ALIGN_END);
        
        sprintf(textVar, "Pontos restantes: %d", player.trainPoints);
        labeltextModifier(fr5_cave_points, textVar);

        sprintf(textVar, "Vida: %d                        ", player.dragon.health);
        labeltextModifier(fr5_cave_health, textVar);

        sprintf(textVar, "Ataque: %d                        ", player.dragon.attack);
        labeltextModifier(fr5_cave_attack, textVar);

        sprintf(textVar, "Defesa: %d                        ", player.dragon.defense);
        labeltextModifier(fr5_cave_defense, textVar);

        sprintf(textVar, "Velocidade: %d                        ", player.dragon.speed);
        labeltextModifier(fr5_cave_speed, textVar);

        if(player.dragon.level == 0) {
            gtk_image_set_from_file(GTK_IMAGE(fr5_cave_dragon_img), "../assets/img_files/dragons/egg_wyvern_1.png");    
            strcpy(dragonStage, "Ovo");
        }        
        if(player.dragon.level >= 1) {
            gtk_image_set_from_file(GTK_IMAGE(fr5_cave_dragon_img), "../assets/img_files/dragons/baby_wyvern_1.png");    
            strcpy(dragonStage, "Bebê");
        }

        if(player.dragon.level >= 10) {
            gtk_image_set_from_file(GTK_IMAGE(fr5_cave_dragon_img), "../assets/img_files/dragons/juvenile_wyvern_1.png");    
            strcpy(dragonStage, "Jovem");
        }

        if(player.dragon.level >= 50) {
            gtk_image_set_from_file(GTK_IMAGE(fr5_cave_dragon_img), "../assets/img_files/dragons/adult_wyvern_1.png");    
            strcpy(dragonStage, "Adulto");
        }

        if(player.dragon.level >= 80) { 
            gtk_image_set_from_file(GTK_IMAGE(fr5_cave_dragon_img), "../assets/img_files/dragons/titan_wyvern_1.png");    
            strcpy(dragonStage, "Titã");
        }

        sprintf(textVar, "%s ( %s )", player.dragon.name, dragonStage);
        labeltextModifier(fr5_cave_name_label, textVar);

        //g_print("Player tem dragão.\n");
    }
    else
        g_print("Player não tem dragão.\n");
}

void updateColiseum() {
    gint dragonIndex = fr5_actual_dragon_index;
    player = getPlayer(playerFile);
    GtkLabel * fr5_dragon_name_legendary = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_name_legendary"));
    GtkLabel * fr5_dragon_name_epic = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_name_epic"));
    GtkLabel * fr5_dragon_name_rare = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_name_rare"));
    GtkLabel * fr5_dragon_name_common = GTK_LABEL(gtk_builder_get_object(builder, "fr5_dragon_name_common"));
    GtkLabel * fr5_coliseum_defeat_label = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_defeat_label"));

    GtkLabel * fr5_coliseum_difficulty_infernal = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_difficulty_infernal"));
    GtkLabel * fr5_coliseum_difficulty_hard = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_difficulty_hard"));
    GtkLabel * fr5_coliseum_difficulty_medium = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_difficulty_medium"));
    GtkLabel * fr5_coliseum_difficulty_easy = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_difficulty_easy"));
    
    GtkLabel * fr5_coliseum_rec_easy = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_rec_easy"));
    GtkLabel * fr5_coliseum_rec_medium = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_rec_medium"));
    GtkLabel * fr5_coliseum_rec_hard = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_rec_hard"));
    GtkLabel * fr5_coliseum_rec_infernal = GTK_LABEL(gtk_builder_get_object(builder, "fr5_coliseum_rec_infernal"));

    GtkImage * fr5_coliseum_bg = GTK_IMAGE(gtk_builder_get_object(builder, "fr5_coliseum_bg"));
    
    gchar * battlePathing = g_strdup_printf("../assets/img_files/dragons/battle_%s.png", pOriginalBeastVector[dragonIndex].name);
    gchar * backgroundPathing = g_strdup_printf("../assets/img_files/battle/%s_bg.png", pOriginalBeastVector[dragonIndex].name);
    gchar * widgetPathing = g_strdup_printf("fr5_coliseum_img%s", gtk_stack_get_visible_child_name(fr5_coliseum_stack));
    gchar * lvlReq = g_strdup_printf("( %d )", pOriginalBeastVector[dragonIndex].level);
    gchar *defeatDragons = g_strdup_printf("%d", player.actualProgress);

    GtkWidget *actualImage = GTK_WIDGET(gtk_builder_get_object(builder, widgetPathing));

    gtk_image_set_from_file(GTK_IMAGE(actualImage), battlePathing);
    gtk_image_set_from_file(fr5_coliseum_bg, backgroundPathing);
    labeltextModifier(fr5_coliseum_defeat_label, defeatDragons);
    
    labeltextModifier(fr5_dragon_name_legendary, "");
    labeltextModifier(fr5_dragon_name_epic, "");
    labeltextModifier(fr5_dragon_name_rare, "");
    labeltextModifier(fr5_dragon_name_common, "");

    labeltextModifier(fr5_coliseum_difficulty_infernal, "");
    labeltextModifier(fr5_coliseum_difficulty_hard, "");
    labeltextModifier(fr5_coliseum_difficulty_medium, "");
    labeltextModifier(fr5_coliseum_difficulty_easy, "");

    if (dragonIndex < 3) {
        labeltextModifier(fr5_dragon_name_legendary, pOriginalBeastVector[dragonIndex].tittle);
        labeltextModifier(fr5_coliseum_difficulty_infernal, "Infernal");
    }
    else if (dragonIndex > 2  && dragonIndex < 8) {
        labeltextModifier(fr5_dragon_name_epic, pOriginalBeastVector[dragonIndex].tittle);
        labeltextModifier(fr5_coliseum_difficulty_hard, "Difícil");
    }
    else if (dragonIndex > 7  && dragonIndex < 16) {
        labeltextModifier(fr5_dragon_name_rare, pOriginalBeastVector[dragonIndex].tittle);
        labeltextModifier(fr5_coliseum_difficulty_medium, "Média");
    }

    else if (dragonIndex > 15) {
        labeltextModifier(fr5_dragon_name_common, pOriginalBeastVector[dragonIndex].tittle);
        labeltextModifier(fr5_coliseum_difficulty_easy, "Fácil");
    }


    int levelDiff = pOriginalBeastVector[dragonIndex].level - player.dragon.level;
    
    labeltextModifier(fr5_coliseum_rec_infernal, "");
    labeltextModifier(fr5_coliseum_rec_hard, "");
    labeltextModifier(fr5_coliseum_rec_medium, "");
    labeltextModifier(fr5_coliseum_rec_easy, "");

    if(levelDiff > 14)
        labeltextModifier(fr5_coliseum_rec_infernal, lvlReq);
    else if(levelDiff < 15 && levelDiff >= 7)
        labeltextModifier(fr5_coliseum_rec_hard, lvlReq);
    else if(levelDiff < 7 && levelDiff >= 0)
        labeltextModifier(fr5_coliseum_rec_medium, lvlReq);
    else if(levelDiff < 0)
        labeltextModifier(fr5_coliseum_rec_easy, lvlReq);

    if(((pOriginalBeastVector[dragonIndex].unlock_id-26) * -1) > player.actualProgress || player.dragon.level == 0) {
        labeltextModifier(fr5_dragon_name_legendary, "");
        labeltextModifier(fr5_dragon_name_epic, "");
        labeltextModifier(fr5_dragon_name_rare, "");
        labeltextModifier(fr5_dragon_name_common, "Desconhecido");
        gtk_image_set_from_file(GTK_IMAGE(actualImage), "../assets/img_files/beast_unknown_coliseum.png");
        gtk_image_set_from_file(GTK_IMAGE(fr5_coliseum_bg), "../assets/img_files/unknown_background.png");
    }
}

void labelTextAnimation(GtkLabel *label, gchar *text, int timer) {
    int totalChars = strlen(text);
    char completeText[strlen(text)+10];
    memset(completeText, '\0', sizeof(completeText));

    for(int i=0; i < totalChars; i++) {
        gtkData *labelData = g_malloc(sizeof(gtkData)*1); 
        labelData->widgetSingle = GTK_WIDGET(label);
        strncat(completeText, &text[i], 1);
        strcpy(labelData->string, completeText);
        g_timeout_add(timer/totalChars*i, timedLabelModifier, labelData);
    }
}

void retroBarAnimationStart(gint timer, GtkWidget *widget, gint actualValue, gint newValue) {
    gint barSize;
    gtkAnimationData *barData = g_malloc(sizeof(gtkAnimationData) * 1);
    gtk_widget_get_size_request(GTK_WIDGET(widget), &barSize, NULL);
    barData->widget = widget;
    gint barDistance = (gint) ((gfloat) barSize - barSize * ((gfloat) newValue/actualValue));
    barData->totalLoops = timer/16;
    barData->stepDistance = barDistance / (gfloat) barData->totalLoops;
    barData->actualStep = 0.0;
    barData->finalPosX = barSize;
    //g_print("totalloops da animação %d | distancia de barra: %d | distancia de passo: %f | tamanho da barra pixels: %d\n", barData->totalLoops, barDistance, barData->stepDistance, barSize);
    g_timeout_add(16, retroBarAnimationLoop, barData);
}

void settingTimedLabelModifier(gint timeout, GtkLabel *label, gchar *text) {
    gtkData *data = (gtkData*) g_malloc(sizeof(gtkData));
    data->widgetSingle = GTK_WIDGET(label);
    strcpy(data->string, text);
    g_timeout_add(timeout, timedLabelModifier, data);
}

void settingTimedMoveWidgetAnimation(gint timerAnimation, gint timeout, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY) {
    gtkAnimationData *widgetAnimationData = g_malloc(sizeof(gtkAnimationData));
    widgetAnimationData->timer = timerAnimation;
    widgetAnimationData->widget = widget;
    widgetAnimationData->fixed = fixed;
    widgetAnimationData->actualPosX = actualX;
    widgetAnimationData->actualPosY = actualY;
    widgetAnimationData->finalPosX = finalPosX;
    widgetAnimationData->finalPosY = finalPosY;
    g_timeout_add(timeout, timedSettingMoveWidgetAnimation, widgetAnimationData);
}

void settingMoveWidgetAnimation(gint timer, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY) {
    gtkAnimationData *widgetData = g_malloc(sizeof(gtkAnimationData) * 1);
    widgetData->fixed = GTK_FIXED(fixed);
    widgetData->widget = GTK_WIDGET(widget);
    widgetData->finalPosX = finalPosX;
    widgetData->finalPosY = finalPosY;
    widgetData->actualPosX = actualX;
    widgetData->actualPosY = actualY;
    widgetData->totalLoops = timer / 10;
    widgetData->stepX = abs(actualX - finalPosX) / widgetData->totalLoops;
    widgetData->stepY = abs(actualY - finalPosY) / widgetData->totalLoops;
    g_timeout_add(10, moveWidgetAnimation, widgetData);
}

void settingTimedStackChange(gint timeout, GtkStack *stack, gchar *page) {
    gtkTimedStack *data = g_malloc(sizeof(gtkTimedStack));
    data->stack = stack;
    strcpy(data->page, page);
    g_timeout_add(timeout, timedStackChange, data);
}

gboolean timedStackChange(gpointer data) {
    gtkTimedStack *changeData = (gtkTimedStack*) data;
    gtk_stack_set_visible_child_name(changeData->stack, changeData->page);
    g_free(changeData);
    return FALSE;
}

gboolean settingBattleWindow(gpointer data) {
    Battle *battle = (Battle *) data;
    GtkStack *fr6_battle_stack = GTK_STACK(gtk_builder_get_object(builder, "fr6_battle_stack"));
    GtkImage *fr6_combat_bg = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_bg"));
    GtkLabel *fr6_chat_label = GTK_LABEL(gtk_builder_get_object(builder, "fr6_chat_label"));
    GtkLabel *fr6_lvl_name_ent1 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_lvl_name_ent1"));
    GtkLabel *fr6_lvl_name_ent2 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_lvl_name_ent2"));
    GtkImage *fr6_combat_player_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_player_dragon"));
    GtkImage *fr6_combat_enemy_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_enemy_dragon"));
    GtkWidget *fr6_dragon_first_border = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_dragon_first_border"));
    GtkFixed *fr6_combat = GTK_FIXED(gtk_builder_get_object(builder, "fr6_combat"));
    gchar ent1_lvl_name[100], ent2_lvl_name[100];
    gchar *battleBackgroundPath = g_strdup_printf("../assets/img_files/battle/%s_bg.png", battle->EntityTwo.entDragon.name);
    gchar *enemyDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_%s.png", battle->EntityTwo.entDragon.name);
    GtkLabel *fr6_tittle_label  = GTK_LABEL(gtk_builder_get_object(builder, "fr6_tittle_label"));
    GtkWidget *fr6_life_bar_ent1 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_life_bar_ent1"));
    GtkWidget *fr6_life_bar_ent2 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_life_bar_ent2"));
    
    gtk_image_set_from_file(fr6_combat_bg, battleBackgroundPath);
    snprintf(ent1_lvl_name, sizeof(ent1_lvl_name), "%d %s", battle->EntityOne.entDragon.level, battle->EntityOne.entDragon.name);
    snprintf(ent2_lvl_name, sizeof(ent2_lvl_name), "%d %s", battle->EntityTwo.entDragon.level, battle->EntityTwo.entDragon.name);

    labeltextModifier(fr6_lvl_name_ent1, ent1_lvl_name);
    labeltextModifier(fr6_lvl_name_ent2, ent2_lvl_name);
    gtk_image_set_from_file(fr6_combat_player_dragon, battle->EntityOne.entDragon.img_path);
    gtk_image_set_from_file(fr6_combat_enemy_dragon, enemyDragonImgPath);

    labeltextModifier(fr6_tittle_label, "Turno: 1");
    
    // Limpando debuffs e buffs
    GtkBox *fr6_enemydragon_debuff_box = GTK_BOX(gtk_builder_get_object(builder, "fr6_enemydragon_debuff_box"));
    GtkBox *fr6_playerdragon_debuff_box = GTK_BOX(gtk_builder_get_object(builder, "fr6_playerdragon_debuff_box"));

    GtkBox *debuffBoxes[] = {fr6_playerdragon_debuff_box, fr6_enemydragon_debuff_box};
    for(int i=0; i<2; i++) {
        GList *children, *iter;
        children = gtk_container_get_children(GTK_CONTAINER(debuffBoxes[i]));
        for (iter = children; iter != NULL; iter = g_list_next(iter)) {
            gtk_widget_destroy(GTK_WIDGET(iter->data));
        }
        g_list_free(children);
    }

    // Ajusta a vida atual do dragão player
    GtkStyleContext *fr6_life_bar_ent1_context = gtk_widget_get_style_context(fr6_life_bar_ent1);
    GtkLabel *fr6_life_value_ent1 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_life_value_ent1"));
    gchar *ent1ActualHealth = g_strdup_printf("%d/%d", battle->EntityOne.entDragon.health, battle->EntityOne.entDragon.health);
    labeltextModifier(fr6_life_value_ent1, ent1ActualHealth);
    gtk_widget_set_visible(GTK_WIDGET(fr6_life_bar_ent1), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(fr6_life_bar_ent1), 233, 11);
    removeAllStyleClasses(fr6_life_bar_ent1);
    gtk_style_context_add_class(fr6_life_bar_ent1_context, "fr6_lifebar_green");

    // Ajusta a vida atual do dragão inimigo
    GtkStyleContext *fr6_life_bar_ent2_context = gtk_widget_get_style_context(fr6_life_bar_ent2);
    removeAllStyleClasses(fr6_life_bar_ent2);
    gtk_style_context_add_class(fr6_life_bar_ent2_context, "fr6_lifebar_green");

    gtk_widget_set_visible(GTK_WIDGET(fr6_life_bar_ent2), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(fr6_life_bar_ent2), 233, 11);

    // Demonstrando quem joga primeiro e o texto inicial
    gchar *firstEntity, *ent1Name, *ent2Name;
    gint firstBorderX;
    gint randomPhrase = random_choice(1, 5);
    if(battle->entityTurn == 1) {
        ent1Name = g_strdup_printf("%s", battle->EntityOne.entDragon.name);
        ent2Name = g_strdup_printf("%s", battle->EntityTwo.entDragon.name);
        firstBorderX = 17;
    }
    if(battle->entityTurn == 2) {
        ent1Name = g_strdup_printf("%s", battle->EntityTwo.entDragon.name);
        ent2Name = g_strdup_printf("%s", battle->EntityOne.entDragon.name);
        firstBorderX = 663;
    }
    if(randomPhrase == 1) firstEntity = g_strdup_printf("Após a luta começar, %s sobe rápido, ocultando-se nas nuvens de %s...", ent1Name, ent2Name);
    else if(randomPhrase == 2) firstEntity = g_strdup_printf("Com um rugido feroz, %s voa alto e some do campo de visão de %s...", ent1Name, ent2Name);
    else if(randomPhrase == 3) firstEntity = g_strdup_printf("Em um giro ágil, %s desaparece entre as sombras antes que %s reaja...", ent1Name, ent2Name);
    else if(randomPhrase == 4) firstEntity = g_strdup_printf("Num salto veloz, %s se esconde no céu, deixando %s sem resposta...", ent1Name, ent2Name);
    else if(randomPhrase == 5) firstEntity = g_strdup_printf("Asas cortam o vento, %s sobe e desaparece antes que %s possa atacar...", ent1Name, ent2Name);

    labelTextAnimation(fr6_chat_label, firstEntity, 3000);
    
    GtkStack *fr6_stack = GTK_STACK(gtk_builder_get_object(builder, "fr6_stack"));
    gtk_stack_set_visible_child_name(fr6_stack, "fr6_combat");
    gtk_stack_set_visible_child_name(fr6_battle_stack, "fr6_battle_chat"); 

    gtk_fixed_move(fr6_combat, fr6_dragon_first_border, firstBorderX, 165);
    // ===========================================================================
    
    Game *game_pointer = g_malloc(sizeof(Game));
    game_pointer->battle = battle;
    game_pointer->fixed = fr6_combat;
    game_pointer->actualTurn = fr6_dragon_first_border;
    game_pointer->battleText = fr6_chat_label;
    game_pointer->pHealthText = fr6_life_value_ent1;
    game_pointer->pHealthBar = fr6_life_bar_ent1;
    game_pointer->eHealthBar = fr6_life_bar_ent2;
    game_pointer->optionsStack = fr6_battle_stack;
    game_pointer->turnsText = fr6_tittle_label;
    game_pointer->builder = builder;
    MiniGame *minigame = g_malloc(sizeof(MiniGame));
    minigame->minigameValue = (gint *) g_malloc(sizeof(gint));
    game_pointer->minigame = minigame;
    game_pointer->minigame->enemyRoars = FALSE;    
    game_pointer->minigame->isActive = FALSE;
    game_pointer->doors.mgMeterPlayed = FALSE;
    game_pointer->doors.playerPlayed = FALSE;
    game_pointer->doors.pAttackReady = FALSE;
    game_pointer->doors.eAttackReady = FALSE;
    game_pointer->doors.eFinishedAttack = FALSE;
    game_pointer->doors.enemyPlayed = FALSE;
    game_pointer->doors.mgChallengerPlayed = FALSE;
    game_pointer->doors.finishedBattle = FALSE;
    game_pointer->doors.cooldownChecked = FALSE;
    strcpy(game_pointer->minigame->pAction, "");
    game = game_pointer;
    g_timeout_add(3000, startBattle, game);

    return FALSE;
}

gboolean startBattle(gpointer data) {
    Game *game = (Game*) data;
    g_timeout_add(1000, onBattle, game);
    return FALSE;
}

gboolean onBattle(gpointer data) {
    Game *game = (Game*) data;
    GtkLabel *fr6_tittle_label = GTK_LABEL(gtk_builder_get_object(builder, "fr6_tittle_label"));
    //g_print("Informações de batalha turno %d\n", game->battle->actualTurn);
    //g_print("Nível do player: %d | Nível do inimigo: %d | Recompensa de Xp: %d\n", game->battle->EntityOne.entDragon.level, game->battle->EntityTwo.entDragon.level, game->battle->expReward);
    
    // Início do round

    // Sessão de vitória ou derrota
    GtkStack *fr7_stack = GTK_STACK(gtk_builder_get_object(builder, "fr7_stack"));
    GtkLabel *fr7_result_xp_text = GTK_LABEL(gtk_builder_get_object(builder, "fr7_result_xp_text"));
    GtkLabel *fr7_result_newbeast_legendary = GTK_LABEL(gtk_builder_get_object(builder, "fr7_result_newbeast_legendary"));
    GtkWidget *fr7_result_animation = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_result_animation"));
    GtkWidget *fr7_result_banner1 = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_result_banner1"));
    GtkWidget *fr7_result_banner2 = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_result_banner2"));
    GtkWidget *fr7_result_img = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_result_img"));
    gtkLevelUpAnimationData *animData = g_malloc(sizeof(gtkLevelUpAnimationData));
    GtkFixed *fixed = GTK_FIXED(gtk_builder_get_object(builder, "fr7_result"));
    GtkWidget *fr7_level_bar = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_level_bar"));
    GtkLabel *fr7_exp_text = GTK_LABEL(gtk_builder_get_object(builder, "fr7_exp_text"));
    GtkLabel *fr7_label_lvl = GTK_LABEL(gtk_builder_get_object(builder, "fr7_label_lvl"));
    GtkWidget *fr7_levelup_text = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_levelup_text"));
    GtkLabel *fr7_result_text2 = GTK_LABEL(gtk_builder_get_object(builder, "fr7_result_text2"));

    animData->experience = game->battle->expReward;
    animData->fixed = fixed;
    animData->lbExpText = fr7_exp_text;
    animData->wdLevelBar = fr7_level_bar;
    animData->lbLvl = fr7_label_lvl;
    animData->wdLvlUpText = fr7_levelup_text;

     // Derrota do player
     if(game->battle->EntityOne.entDragon.health <= 0) {
        settingTimedVideoPlay(fr7_result_animation, 3000, 91, "defeat", 0, NULL, FALSE);
        playSoundByName(3000, "defeat", &audioPointer, 0);
        settingTimedImageModifier(4080, fr7_result_img, "../assets/img_files/defeat.png");
        labeltextModifier(fr7_result_text2, "Não desista, continue e se torne o mais forte!");
        }
    
    // Vitória do player
    else if(game->battle->EntityTwo.entDragon.health <= 0) {
        settingTimedVideoPlay(fr7_result_animation, 3000, 91, "victory", 0, NULL, FALSE);
        playSoundByName(3000, "victory", &audioPointer, 0);

        settingTimedImageModifier(4080, fr7_result_img, "../assets/img_files/victory.png");
        //g_print("Index do dragão atual: %d | atual progresso do player %d\n", (fr5_actual_dragon_index-27)*-1, player.actualProgress);
        if((pOriginalBeastVector[fr5_actual_dragon_index].unlock_id-26) * -1 == player.actualProgress && player.actualProgress < 27) {
            GtkLabel *fr7_result_newbeast_legendary = GTK_LABEL(gtk_builder_get_object(builder, "fr7_result_newbeast_legendary"));
            labeltextModifier(fr7_result_text2, "Novo dragão adicionado ao Bestiário:                                ");
            labeltextModifier(fr7_result_newbeast_legendary, pOriginalBeastVector[fr5_actual_dragon_index].name);
            changePlayerStatus(playerFile, -1, -1, -1, -1, -1, player.actualProgress+1, NULL);
            player = getPlayer(playerFile);
        }
        else {
            labeltextModifier(fr7_result_text2, "Nenhum novo dragão foi descoberto após a batalha.");
            labeltextModifier(fr7_result_newbeast_legendary, "");
        }
        if(fr5_actual_dragon_index == 0) {
            labeltextModifier(fr7_result_text2, "Não há mais dragões, sua jornada acaba aqui.     ");
        }
    }

    // Fim de batalha
    if(game->battle->EntityTwo.entDragon.health <= 0 || game->battle->EntityOne.entDragon.health <= 0) {
        GtkWidget *fr7_btn_continue = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_btn_continue"));
        GtkWidget *fr7_btn_continue_label = GTK_WIDGET(gtk_builder_get_object(builder, "fr7_btn_continue_label"));
        game->doors.finishedBattle = TRUE;
        
        *(game->minigame->minigameValue) = -1;
        g_signal_handlers_disconnect_by_func(window, G_CALLBACK(on_key_press), game);
        g_signal_handlers_disconnect_by_func(window, G_CALLBACK(on_key_release), game);
        settingTimedLabelModifier(4000, game->turnsText, "Combate");
        settingTimedStackChange(3000, main_stack, "result_page");

        gtk_image_clear(GTK_IMAGE(fr7_result_banner1));
        gtk_image_clear(GTK_IMAGE(fr7_result_banner2));
        gtk_image_clear(GTK_IMAGE(fr7_result_img));
        settingTimedImageModifier(3750, fr7_result_banner1, "../assets/img_files/banner.png");
        settingTimedImageModifier(3750, fr7_result_banner2, "../assets/img_files/banner.png");
        labeltextModifier(fr7_result_xp_text, "");
        
        settingTimedNumbersAnimation(4600, fr7_result_xp_text, game->battle->expReward, 2);
        if(game->battle->expReward > 0) {
            playSoundByName(4600, "exp_reward", &audioPointer, 0);
        }

        gtk_widget_set_sensitive(fr7_btn_continue, FALSE);
        gtk_widget_set_opacity(fr7_btn_continue_label, 0.5);
        g_timeout_add(6700, turnOnButton, fr7_btn_continue);
        g_timeout_add(6700, btn_animation_rest_opacity, fr7_btn_continue_label);

        settingUpdatelvlBarAnimation(0, fr7_label_lvl, fr7_exp_text, fr7_level_bar, fixed, fr7_levelup_text);
        g_timeout_add(4600, timedLvlBarUpdate, animData);
        g_timeout_add(9000, timedgFree, game->battle);
        g_timeout_add(9000, timedgFree, game->minigame->minigameValue);
        g_timeout_add(9000, timedgFree, game->minigame);
        g_timeout_add(10000, timedgFree, game);
        stopCurrentMusic();
        return FALSE;
    }
    
    // Turno do player
    if(game->battle->entityTurn == 1 && !game->doors.playerPlayed && !game->doors.finishedBattle) {
        gint playerAttack = game->battle->EntityOne.entDragon.attack;
        gint totalDamage = 0;
        gint appliedDebuff = -3;
        gint duplicated = 0;
        gint dragonDifficult = game->battle->difficult;
        gint precision = 0;

        // Arrasta a barra para sinalizar quem está atacando
        gtk_fixed_move(game->fixed, game->actualTurn, 17, 165);
        
        // Verificação de cooldowns das habilidades
        if(!game->doors.cooldownChecked) {
            game->doors.cooldownChecked = TRUE;
            for(int i=0; i<4; i++) {
                gchar *object = g_strdup_printf("fr6_btn_attack%d", i+1);
                GtkWidget *fr6_btn_attack = GTK_WIDGET(gtk_builder_get_object(builder, object));
                if(game->battle->EntityOne.skillsCooldown[i] > 0) {
                    gtk_widget_set_sensitive(fr6_btn_attack, FALSE);
                    gtk_widget_set_opacity(fr6_btn_attack, 0.5);
                }
                else {
                    gtk_widget_set_sensitive(fr6_btn_attack, TRUE);
                    gtk_widget_set_opacity(fr6_btn_attack, 1.0);
                }
            }
            settingTimedStackChange(0, game->optionsStack, "fr6_battle_buttons");
        }
        // Inicia o minigame mudando o request.
        if((strcmp(request, "bite") == 0 || strcmp(request, "dracarys") == 0 || strcmp(request, "scratch") == 0) && strcmp(game->minigame->pAction, "") == 0) {
            GtkImage *fr6_battle_powerbar_img = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_battle_powerbar_img"));
            GtkWidget *fr6_battle_powerbar_pointer = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_battle_powerbar_pointer"));
            GtkFixed *fr6_battle_powerbar = GTK_FIXED(gtk_builder_get_object(builder, "fr6_battle_powerbar"));
            
            if(dragonDifficult == 4) {
                gtk_image_set_from_file(fr6_battle_powerbar_img, "../assets/img_files/meterbar_infernal.png");
                gint tempVector[4][4] = {{22,89,187,254}, {90,129,147,186}, {130,134,142,146}, {135,141,135,141}}; memcpy(game->minigame->vectorRange, tempVector, sizeof(game->minigame->vectorRange));
            }
            if(dragonDifficult == 3) {
                gtk_image_set_from_file(fr6_battle_powerbar_img, "../assets/img_files/meterbar_hard.png");
                gint tempVector[4][4] = {{22,79,197,254}, {80,109,167,196}, {110,129,147,166}, {130,146,130,146}}; 
                memcpy(game->minigame->vectorRange, tempVector, sizeof(game->minigame->vectorRange));
            }
            if(dragonDifficult == 2) {
                gtk_image_set_from_file(fr6_battle_powerbar_img, "../assets/img_files/meterbar_medium.png");
                gint tempVector[4][4] = {{22,69,206,254}, {70,99,177,205}, {100,119,157,176}, {120,156,120,156}};
                memcpy(game->minigame->vectorRange, tempVector, sizeof(game->minigame->vectorRange));
            }
            if(dragonDifficult == 1) {
                gtk_image_set_from_file(fr6_battle_powerbar_img, "../assets/img_files/meterbar_easy.png");
                gint tempVector[4][4] = {{22, 28, 248, 254}, {29, 71, 205, 247}, {72, 109, 167, 204}, {110, 166, 110, 166}}; 
                memcpy(game->minigame->vectorRange, tempVector, sizeof(game->minigame->vectorRange));
            }            
            
            gtk_stack_set_visible_child_name(game->optionsStack, "fr6_battle_powerbar");
            // Conecta o evento de pressionar teclas à função de callback
            // Seta as iniciais do minigame
            game->minigame->minigamePlayed = FALSE;
            *(game->minigame->minigameValue) = 0;
            strcpy(game->minigame->pAction, "");
            strcpy(game->minigame->name, "meterbar");

            g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), game);        
            g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), game); 
            settingMeterbarAnimation(fr6_battle_powerbar_pointer, fr6_battle_powerbar, 105, 22, 254, game->minigame->minigameValue, 0.75); 
            if(strcmp(request, "bite") == 0) strcpy(game->minigame->pAction, "bite");
            if(strcmp(request, "scratch") == 0) strcpy(game->minigame->pAction, "scratch");
            if(strcmp(request, "dracarys") == 0) strcpy(game->minigame->pAction, "dracarys");
        }
        
        // Realizando ataque após o minigame
        if(game->doors.mgMeterPlayed && *(game->minigame->minigameValue) == -1) {
            *(game->minigame->minigameValue) = 0;
            gint value = game->minigame->minigameResultValue;
            gint barResult = 0;
            g_print("Minigame finalizado dentro da função. valor obtido: %d\n", game->minigame->minigameResultValue);
            for(int i=0; i<4; i++) {
                gint start1 = game->minigame->vectorRange[i][0], end1 = game->minigame->vectorRange[i][1];
                gint start2 = game->minigame->vectorRange[i][2], end2 = game->minigame->vectorRange[i][3];
                if((start1 <= value && value <= end1) || (start2 <= value && value <= end2))
                    barResult = i;
            }
            if(barResult == 3) { // Critico - parte verde da barra
                playerAttack += playerAttack * 0.5;
                precision = 100;
                logStartAnimation("CRITICAL", "color_FF0000", 1000, 44, 175, 412, 360, 10, game->fixed);
            }
            if(barResult == 2) { // Ataque normal - parte laranja da barra
                precision = 0;
            }
            if(barResult == 1) { // Ataque fraco - parte vermelha da barra
                playerAttack -= playerAttack * 0.25;
                precision = -20;
            }
            if(barResult == 0) { // Falha - Parte preta da barra
                precision = -100;
            }
            g_print("resultado da barra: %d\n", barResult);
        } 
        
        // Ataque mordida
        if(strcmp(game->minigame->pAction, "bite") == 0 && game->doors.mgMeterPlayed) {
            strcpy(game->minigame->pAction, "");
            g_print("==================================================================\n");
            g_print("Vida atual do inimigo: %d\n", game->battle->EntityTwo.entDragon.health);
            if(precision + 90 >= 100)
                precision = 100;
            else
                precision += 90;
            g_print("Precisão atual: %d | Bite\n", precision);
            game->battle->EntityOne.skillsCooldown[0] = 4;
            game->battle->totalDamage = causeDamage(playerAttack, 1.2, precision, &game->battle->EntityTwo.entDragon);
            g_timeout_add(2000, timedInverseBooleanValue, &game->doors.pAttackReady);
            settingAttackAnimation(500, 1, 30, "bite_crunch_animation", game->fixed, 252);

            game->battle->debuffTurns = 2;
            game->battle->currentDebuffAnimation = 3;
            strcpy(game->battle->currentDebuffType, "Bleeding");
            strcpy(game->battle->currentDebuffStatus, "bleeding_status");
        }
        // Ataque Arranhão
        if(strcmp(game->minigame->pAction, "scratch") == 0 && game->doors.mgMeterPlayed) {
            strcpy(game->minigame->pAction, "");
            g_print("==================================================================\n");
            g_print("Vida atual do inimigo: %d\n", game->battle->EntityTwo.entDragon.health);
            if(precision + 100 >= 100)
                precision = 100;
            else
                precision += 100;
            g_print("Precisão atual: %d | scratch\n", precision);
            game->battle->EntityOne.skillsCooldown[1] = 0;
            game->battle->totalDamage = causeDamage(playerAttack, 1.0, precision, &game->battle->EntityTwo.entDragon);
            g_timeout_add(2500, timedInverseBooleanValue, &game->doors.pAttackReady);
            settingAttackAnimation(500, 1, 36, "scratch_claw_animation", game->fixed, 252);

            game->battle->debuffTurns = 3;
            game->battle->currentDebuffAnimation = 5;
            strcpy(game->battle->currentDebuffType, "Broken-Armor");
            strcpy(game->battle->currentDebuffStatus, "broken_status");
        }
        // Ataque Dracarys
        if(strcmp(game->minigame->pAction, "dracarys") == 0 && game->doors.mgMeterPlayed) {
            strcpy(game->minigame->pAction, "");
            g_print("==================================================================\n");
            g_print("Vida atual do inimigo: %d\n", game->battle->EntityTwo.entDragon.health);
            if(precision + 55 >= 100)
                precision = 100;
            else
                precision += 55;
            g_print("Precisão atual: %d | Dracarys\n", precision);
            game->battle->EntityOne.skillsCooldown[3] = 6;
            game->battle->totalDamage = causeDamage(playerAttack, 2.0, precision, &game->battle->EntityTwo.entDragon);
            g_timeout_add(2500, timedInverseBooleanValue, &game->doors.pAttackReady);
            settingAttackAnimation(500, 1, 39, "fire_breath_animation", game->fixed, 192);

            game->battle->debuffTurns = 2;
            game->battle->currentDebuffAnimation = 7;
            strcpy(game->battle->currentDebuffType, "Burning");
            strcpy(game->battle->currentDebuffStatus, "burning_status");
        }
        // Ataque Rugido
        if(strcmp(request, "roar") == 0 && strcmp(game->minigame->pAction, "roar") != 0) {
            strcpy(game->minigame->pAction, "roar");
            game->battle->EntityOne.skillsCooldown[2] = 6;
            g_timeout_add(3000, timedInverseBooleanValue, &game->doors.pAttackReady);
            playSoundByName(0, "dragon_start_roar", &audioPointer, 0);
            playSoundByName(2000, "dragon_end_roar", &audioPointer, 0);
            shakeScreen(0, GTK_WINDOW(window), 2500, game->battle->difficult * 2 + 6);

            game->battle->debuffTurns = 5;
            game->battle->currentDebuffAnimation = 9;
            strcpy(game->battle->currentDebuffType, "Terrified");
            strcpy(game->battle->currentDebuffStatus, "terrified_status");
        }
        // Fuga
        if(strcmp(request, "run") == 0) {
            game->battle->EntityOne.entDragon.health = 0;
            game->battle->expReward = 0;
        }
        
        // Aplica o rugido
        if(game->doors.pAttackReady && strcmp(game->minigame->pAction, "roar") == 0) {
            // Aplicação de debuff
            appliedDebuff =  applyDebuff(game->battle->currentDebuffType, game->battle->debuffTurns, &game->battle->EntityTwo, &game->battle->duplicatedDebuff);
            if(appliedDebuff >= 0 && appliedDebuff <= 4 && game->battle->duplicatedDebuff == 0) {
                updateDebuffAnimation(1, "apply", &game->battle->EntityTwo.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
            }
            if(game->battle->duplicatedDebuff == 1) {
                updateDebuffAnimation(1, "reapply", &game->battle->EntityTwo.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                g_print("Debuff reaplicado com sucesso.\n");
            }
            g_print("Debuff aplicado no slot: %d\n", appliedDebuff);
            strcpy(game->minigame->pAction, "");
            game->doors.playerPlayed = TRUE;
            g_timeout_add(3000, timedSwitchBooleanValue, game);
            g_print("==================================================================\n");
        }

        // Aplica o dano causado
        if(game->doors.pAttackReady && game->battle->totalDamage > 0) {
            gchar *damageText = g_strdup_printf("-%d", game->battle->totalDamage);
            gint beforeHealth = game->battle->EntityTwo.entDragon.health;
            GtkWidget *fr6_life_bar_ent2 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_life_bar_ent2"));
            game->battle->EntityTwo.entDragon.health -= game->battle->totalDamage;
            if(game->battle->EntityTwo.entDragon.health < 0)
                game->battle->EntityTwo.entDragon.health = 0;
            
            // Aplicação de debuff
            appliedDebuff =  applyDebuff(game->battle->currentDebuffType, game->battle->debuffTurns, &game->battle->EntityTwo, &game->battle->duplicatedDebuff);
            if(appliedDebuff >= 0 && appliedDebuff <= 4 && game->battle->duplicatedDebuff == 0) {
                updateDebuffAnimation(1, "apply", &game->battle->EntityTwo.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
            }
            if(game->battle->duplicatedDebuff == 1) {
                updateDebuffAnimation(1, "reapply", &game->battle->EntityTwo.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                g_print("Debuff reaplicado com sucesso.\n");
            }
            g_print("Debuff aplicado no slot: %d\n", appliedDebuff);
            retroBarAnimationStart(500, fr6_life_bar_ent2, beforeHealth, game->battle->EntityTwo.entDragon.health);
            logStartAnimation(damageText, "fr5_dragons_defeat", 1000, 45, 116, random_choice(667, 836), random_choice(270, 310), 30, game->fixed);
            g_print("Dano total causado: %d\n", game->battle->totalDamage);
            g_print("Vida atual do inimigo pós dano: %d\n", game->battle->EntityTwo.entDragon.health);
            for(int i=0; i < 4; i++) 
                g_print("Debuff slot[%d]: type: %s | Turns left: %d\n", i,game->battle->EntityTwo.entityDebuffs[i].type, game->battle->EntityTwo.entityDebuffs[i].turns);
            
            strcpy(game->minigame->pAction, "");
            game->doors.playerPlayed = TRUE;
            g_timeout_add(3000, timedSwitchBooleanValue, game);
            g_print("==================================================================\n");
        }
        // Erra o ataque
        if(game->doors.pAttackReady && game->battle->totalDamage == -1) {
            logStartAnimation("MISS", "fr5_dragon_name_common", 1000, 45, 116, random_choice(667, 836), random_choice(270, 310), 30, game->fixed);
            game->minigame->minigamePlayed = FALSE;
            strcpy(game->minigame->pAction, "");
            game->doors.playerPlayed = TRUE;
            g_timeout_add(3000, timedSwitchBooleanValue, game);
        }
    }
    
    // Turno do Inimigo
    if(game->battle->entityTurn == 2 && game->battle->EntityOne.entDragon.health > 0 && !game->doors.enemyPlayed && !game->doors.finishedBattle) {
        gint enemyAttack = game->battle->EntityTwo.entDragon.attack;
        gint totalDamage = 0;
        gint appliedDebuff = -3;
        gint duplicated = 0;
        gint dragonDifficult = 0;
        gint precision = 0;

        //gtk_stack_set_visible_child_name(game->optionsStack, "fr6_battle_chat");
        labeltextModifier(game->battleText, "Turno inimigo");

        // Arrasta a barra para sinalizar quem está atacando
        gtk_fixed_move(game->fixed, game->actualTurn, 663, 165);

        // Comportamento inimigo
        if(!game->doors.eAttackReady) {
            gint currentAttack = 0;
            if(game->battle->difficult == 1 && strcmp(game->minigame->eAction, "") == 0) { // Dificuldade fácil
                currentAttack = random_choice(0, 3);
                game->minigame->criticalChance = 10;
                game->minigame->attackRecharge = 1;
                while(game->battle->EntityTwo.skillsCooldown[currentAttack] != 0) {
                    currentAttack = random_choice(0, 3);
                }
            }

            if(game->battle->difficult == 2 && strcmp(game->minigame->eAction, "") == 0) { // Dificuldade Média
                currentAttack = random_choice(0, 3);
                game->minigame->attackRecharge = 0;
                game->minigame->criticalChance = 25;
                while(game->battle->EntityTwo.skillsCooldown[currentAttack] != 0) {
                    currentAttack = random_choice(0, 3);
                }
                
            }

            if(game->battle->difficult == 3 && strcmp(game->minigame->eAction, "") == 0) { // Dificuldade Difícil
                currentAttack = 0;
                game->minigame->attackRecharge = 0;
                game->minigame->criticalChance = 50;
                // Inteligencia para quebrar armadura sempre
                for(int i=0; i<4; i++) {
                    if(strcmp(game->battle->EntityOne.entityDebuffs[i].type, "Broken-Armor") == 0) { // Verifica
                        if(game->battle->EntityTwo.skillsCooldown[2] == 0) // Verifica se dracarys está disponivel
                            currentAttack = 2;
                        else
                            if(game->battle->EntityTwo.skillsCooldown[1] == 0) // Verifica se mordida está dispónivel
                                currentAttack = 1;
                    }
                }
            }

            if(game->battle->difficult == 4 && strcmp(game->minigame->eAction, "") == 0) { // Dificuldade Difícil
                currentAttack = 0;
                game->minigame->attackRecharge = -1;
                game->minigame->criticalChance = 75;
                // Inteligencia para quebrar armadura sempre
                for(int i=0; i<4; i++) {
                    if(strcmp(game->battle->EntityOne.entityDebuffs[i].type, "Broken-Armor") == 0) { // Verifica
                        if(game->battle->EntityTwo.skillsCooldown[2] == 0) // Verifica se dracarys está disponivel
                            currentAttack = 2;
                        else
                            if(game->battle->EntityTwo.skillsCooldown[1] == 0) // Verifica se mordida está dispónivel
                                currentAttack = 1;
                    }
                }
            }
            
            if(currentAttack == 0) { strcpy(game->minigame->eAction, "scratch"); }
            if(currentAttack == 1) { strcpy(game->minigame->eAction, "bite"); }
            if(currentAttack == 2) { strcpy(game->minigame->eAction, "dracarys"); }
            if(currentAttack == 3) { strcpy(game->minigame->eAction, "roar"); }
            game->doors.eAttackReady = TRUE;
        }
        //g_print("Action: %s ///////////////////\n", game->minigame->pAction);
        // Início do minigame challenge
        
        if(!game->minigame->isActive && !game->doors.mgChallengerPlayed) { // Condicional
            settingTimedStackChange(1500, game->optionsStack, "fr6_battle_challenge");
            g_timeout_add(1500, timedStartChallengeGame, game);
            game->minigame->isActive = TRUE;
        }

        // Realiza o ataque
        if(game->doors.mgChallengerPlayed && game->doors.eAttackReady) {
            gfloat attackDecrease = 0;
            gint precision = 0;
            if(game->minigame->minigameResultValue == 1) {
                precision = -5;
                game->minigame->criticalChance -= 25;
                attackDecrease = 0.25;
            }
            if(game->minigame->minigameResultValue == -1) {
                precision = 100;
                game->minigame->criticalChance += 15;
                attackDecrease = 0;
                gint randomShake = 0;
                if(game->battle->difficult >= 3  && !game->minigame->enemyRoars) {
                    randomShake = random_choice(1, 100);
                    game->minigame->enemyRoars = TRUE;
                }

                if((game->battle->difficult == 3 && randomShake >= 50) || ((game->battle->difficult == 4 && randomShake >= 25))) { // Rugido passivo
                    playSoundByName(0, "dragon_start_roar", &audioPointer, 0);
                    playSoundByName(2000, "dragon_end_roar", &audioPointer, 0);

                    shakeScreen(0, GTK_WINDOW(window), 2500, game->battle->difficult * 2 + 6);
                    
                    appliedDebuff =  applyDebuff("Terrified", 5, &game->battle->EntityOne, &game->battle->duplicatedDebuff);
                    if(appliedDebuff >= 0 && appliedDebuff <= 4 && game->battle->duplicatedDebuff == 0) {
                        updateDebuffAnimation(2, "apply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], 9, "terrified_status");
                    }
                    if(game->battle->duplicatedDebuff == 1) {
                        updateDebuffAnimation(2, "reapply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], 9, "terrified_status");
                        g_print("Debuff reaplicado com sucesso.\n");
                    }
                    g_print("Debuff aplicado no slot: %d\n", appliedDebuff);
                
                }
            }
            *(game->minigame->minigameValue) = 0;

            // Ataque mordida
            if(strcmp(game->minigame->eAction, "bite") == 0) {
                strcpy(game->minigame->eAction, "");
                g_print("==================================================================\n");
                g_print("Vida atual do Player: %d\n", game->battle->EntityOne.entDragon.health);
                if(precision + 90 >= 100)
                    precision = 100;
                else
                    precision += 90;
                g_print("Precisão atual: %d | Bite\n", precision);
                settingAttackAnimation(500, 2, 30, "bite_crunch_animation", game->fixed, 252);
                g_timeout_add(2000, timedInverseBooleanValue, &game->doors.eFinishedAttack);
                game->battle->EntityTwo.skillsCooldown[1] = 4 + game->minigame->attackRecharge;
                game->battle->totalDamage = causeDamage(enemyAttack, 1.2, precision, &game->battle->EntityOne.entDragon);
                
                game->battle->debuffTurns = 2;
                game->battle->currentDebuffAnimation = 3;
                strcpy(game->battle->currentDebuffType, "Bleeding");
                strcpy(game->battle->currentDebuffStatus, "bleeding_status");
            }
            // Ataque Arranhão
            if(strcmp(game->minigame->eAction, "scratch") == 0) {
                strcpy(game->minigame->eAction, "");
                g_print("==================================================================\n");
                g_print("Vida atual do Player: %d\n", game->battle->EntityOne.entDragon.health);
                if(precision + 100 >= 100)
                    precision = 100;
                else
                    precision += 100;
                g_print("Precisão atual: %d | scratch\n", precision);
                g_timeout_add(2000, timedInverseBooleanValue, &game->doors.eFinishedAttack);
                settingAttackAnimation(500, 2, 40, "scratch_claw_animation", game->fixed, 252);
                game->battle->EntityTwo.skillsCooldown[0] = 0;
                game->battle->totalDamage = causeDamage(enemyAttack, 1.0, precision, &game->battle->EntityOne.entDragon);
                
                game->battle->debuffTurns = 3;
                game->battle->currentDebuffAnimation = 5;
                strcpy(game->battle->currentDebuffType, "Broken-Armor");
                strcpy(game->battle->currentDebuffStatus, "broken_status");
            }
            // Ataque Dracarys
            if(strcmp(game->minigame->eAction, "dracarys") == 0) {
                strcpy(game->minigame->eAction, "");
                g_print("==================================================================\n");
                g_print("Vida atual do Player: %d\n", game->battle->EntityOne.entDragon.health);
                if(precision + 55 >= 100)
                    precision = 100;
                else
                    precision += 55;
                g_print("Precisão atual: %d | Dracarys\n", precision);
                settingAttackAnimation(500, 2, 39, "fire_breath_animation", game->fixed, 192);
                g_timeout_add(2000, timedInverseBooleanValue, &game->doors.eFinishedAttack);
                game->battle->EntityTwo.skillsCooldown[2] = 6 + game->minigame->attackRecharge;
                game->battle->totalDamage = causeDamage(enemyAttack, 2.0, precision, &game->battle->EntityOne.entDragon);
                
                game->battle->debuffTurns = 2;
                game->battle->currentDebuffAnimation = 7;
                strcpy(game->battle->currentDebuffType, "Burning");
                strcpy(game->battle->currentDebuffStatus, "burning_status");
            }
            
            // Ataque Rugido
            if(strcmp(game->minigame->eAction, "roar") == 0) {
                strcpy(game->minigame->eAction, "");
                game->battle->EntityTwo.skillsCooldown[3] = 6 + game->minigame->attackRecharge;
                game->battle->totalDamage = 0;
                g_timeout_add(3000, timedInverseBooleanValue, &game->doors.eFinishedAttack);
                playSoundByName(0, "dragon_start_roar", &audioPointer, 0);
                playSoundByName(2000, "dragon_end_roar", &audioPointer, 0);

                shakeScreen(0, GTK_WINDOW(window), 2500, game->battle->difficult * 2 + 6);

                game->battle->debuffTurns = 5;
                game->battle->currentDebuffAnimation = 9;
                strcpy(game->battle->currentDebuffType, "Terrified");
                strcpy(game->battle->currentDebuffStatus, "terrified_status");
            }
            // Aplica o Rugido
            if(game->doors.eFinishedAttack && game->battle->totalDamage == 0) {
                 // Aplicação de debuff
                appliedDebuff =  applyDebuff(game->battle->currentDebuffType, game->battle->debuffTurns, &game->battle->EntityOne, &game->battle->duplicatedDebuff);
                if(appliedDebuff >= 0 && appliedDebuff <= 4 && game->battle->duplicatedDebuff == 0) {
                    updateDebuffAnimation(2, "apply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                }
                if(game->battle->duplicatedDebuff == 1) {
                    updateDebuffAnimation(2, "reapply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                    g_print("Debuff reaplicado com sucesso.\n");
                }
                g_print("Debuff aplicado no slot: %d\n", appliedDebuff);

                game->doors.enemyPlayed = TRUE;
                g_timeout_add(3000, timedSwitchBooleanValue, game);
            }
            // Aplica o dano causado
            if(game->battle->totalDamage > 0 && game->doors.eFinishedAttack) {
                gint beforeHealth = game->battle->EntityOne.entDragon.health;
                GtkLabel *fr6_life_value_ent1 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_life_value_ent1"));
                GtkWidget *fr6_life_bar_ent1 = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_life_bar_ent1"));
                game->battle->totalDamage -= totalDamage * attackDecrease;
                if(game->minigame->criticalChance > 0) { // Taxa crítica
                    if(random_choice(1, 100) <= game->minigame->criticalChance) {
                        logStartAnimation("CRITICAL", "color_FF0000", 1000, 44, 175, 412, 360, 10, game->fixed);
                        game->battle->totalDamage *= 1.5;
                    }
                }

                // Aplicação de debuff
                appliedDebuff =  applyDebuff(game->battle->currentDebuffType, game->battle->debuffTurns, &game->battle->EntityOne, &game->battle->duplicatedDebuff);
                if(appliedDebuff >= 0 && appliedDebuff <= 4 && game->battle->duplicatedDebuff == 0) {
                    updateDebuffAnimation(2, "apply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                }
                if(game->battle->duplicatedDebuff == 1) {
                    updateDebuffAnimation(2, "reapply", &game->battle->EntityOne.entityDebuffs[appliedDebuff], game->battle->currentDebuffAnimation, game->battle->currentDebuffStatus);
                    g_print("Debuff reaplicado com sucesso.\n");
                }
                g_print("Debuff aplicado no slot: %d\n", appliedDebuff);

                gchar *damageText = g_strdup_printf("-%d", game->battle->totalDamage);
                game->battle->EntityOne.entDragon.health -= game->battle->totalDamage;
                if(game->battle->EntityOne.entDragon.health < 0)
                    game->battle->EntityOne.entDragon.health = 0;
                retroBarAnimationStart(500, fr6_life_bar_ent1, beforeHealth, game->battle->EntityOne.entDragon.health);
                logStartAnimation(damageText, "fr5_dragons_defeat", 1000, 45, 116, random_choice(27, 180), random_choice(270, 310), 30, game->fixed);
                g_print("Dano total causado: %d | Taxa crítica %d\n", game->battle->totalDamage, game->minigame->criticalChance);
                g_print("Vida atual do Player pós dano: %d\n", game->battle->EntityOne.entDragon.health);
                for(int i=0; i < 4; i++) 
                    g_print("Debuff slot[%d]: type: %s | Turns left: %d\n", i,game->battle->EntityOne.entityDebuffs[i].type, game->battle->EntityOne.entityDebuffs[i].turns);
                
                labeltextModifier(fr6_life_value_ent1, g_strdup_printf("%d/%d", game->battle->EntityOne.entDragon.health, game->battle->EntityOne.fixedDragon.health));
                game->doors.enemyPlayed = TRUE;
                g_timeout_add(3000, timedSwitchBooleanValue, game);
                g_print("==================================================================\n");
            }

            // Erra o ataque
            if(game->battle->totalDamage == -1 && game->doors.eFinishedAttack) {
                logStartAnimation("MISS", "fr5_dragon_name_common", 1000, 45, 116, random_choice(27, 180), random_choice(270, 310), 30, game->fixed);
                game->doors.enemyPlayed = TRUE;
                g_timeout_add(3000, timedSwitchBooleanValue, game);
            }
        }

    }
 
    // Passagem de turnos
    if(game->battle->turnPlayed == TRUE) {
        startTurn(game->battle, game);
        playSoundByName(0, "turn_change", &audioPointer, 0);
        game->battle->totalDamage = 0;
        if(game->battle->entityTurn == 1) {
            game->doors.pAttackReady = FALSE;
            game->doors.mgMeterPlayed = FALSE;
            game->doors.playerPlayed = FALSE;
            game->doors.cooldownChecked = FALSE;
        }
        else if(game->battle->entityTurn == 2) {
            strcpy(game->minigame->eAction, "");
            game->minigame->enemyRoars = FALSE;
            game->doors.eFinishedAttack = FALSE;
            game->doors.eAttackReady = FALSE;
            game->doors.enemyPlayed = FALSE;
            game->doors.mgChallengerPlayed = FALSE;
        }
        
        labeltextModifier(fr6_tittle_label, g_strdup_printf("Turno: %d", game->battle->actualTurn));
    }

    //g_print("Turno jogado: %d\n", game->battle->turnPlayed);
    //g_print("request atual: %s\n", request);
    strcpy(request, "");

    return TRUE;
}

void sendRequest(GtkButton *btn, gpointer user_data) {
    btn_animation_clicked(GTK_WIDGET(btn), NULL);
    gint requestType = GPOINTER_TO_INT(user_data);
    if(requestType == 1) strcpy(request, "bite");
    if(requestType == 2) strcpy(request, "scratch");
    if(requestType == 3) strcpy(request, "roar");
    if(requestType == 4) strcpy(request, "dracarys");
    if(requestType == 5) strcpy(request, "run");
    //sprintf(request, "%d", GPOINTER_TO_INT(user_data));
}

void updateDebuffAnimation(gint entityNumber, gchar *type, Debuff *debuff, gint animationType, gchar *status) {
    gchar *statusPath = g_strdup_printf("../assets/img_files/%s.png", status);
    GtkBox *fr6_enemydragon_box;
    if(entityNumber == 1) 
        fr6_enemydragon_box = GTK_BOX(gtk_builder_get_object(builder, "fr6_enemydragon_debuff_box"));
    if(entityNumber == 2) 
        fr6_enemydragon_box = GTK_BOX(gtk_builder_get_object(builder, "fr6_playerdragon_debuff_box"));
    
    if(strcmp(type, "apply") == 0) {
        gchar *animationName = g_strdup_printf("%s_apply", status);
        GtkFixed *fixed = GTK_FIXED(gtk_fixed_new());
        GtkImage *fr6_enemydragon_debuff = GTK_IMAGE(gtk_image_new());
        GtkWidget *fr6_enemydragon_animation_debuff = gtk_drawing_area_new();

        gtk_widget_set_size_request(GTK_WIDGET(fr6_enemydragon_animation_debuff), 24, 24);
        gtk_widget_set_size_request(GTK_WIDGET(fixed), 24, 24);

        gtk_fixed_put(fixed, GTK_WIDGET(fr6_enemydragon_debuff), 0, 0);
        gtk_fixed_put(fixed, fr6_enemydragon_animation_debuff, 0, 0);
        
        gtk_box_pack_start(fr6_enemydragon_box, GTK_WIDGET(fixed), FALSE, FALSE, 0);
        gtk_widget_show_all(window);

        gtk_widget_realize(fr6_enemydragon_animation_debuff);
        gtk_widget_queue_draw(fr6_enemydragon_animation_debuff);

        debuff->image = fr6_enemydragon_debuff;
        debuff->video = fr6_enemydragon_animation_debuff;
        debuff->fixed = fixed;
        settingTimedVideoPlay(GTK_WIDGET(fr6_enemydragon_animation_debuff), 0, 31, animationName, 0, NULL, FALSE);
        settingTimedImageModifier(500, GTK_WIDGET(debuff->image), statusPath);
        playSoundByName(0, status, &audioPointer, 0);
    }
    else if(strcmp(type, "reapply") == 0) {
        gchar *animationName = g_strdup_printf("%s_apply", status);
        settingTimedVideoPlay(GTK_WIDGET(debuff->video), 0, 31, animationName, 0, NULL, FALSE);
        playSoundByName(0, status, &audioPointer, 0);
    }
    else if(strcmp(type, "finish") == 0) {
        gchar *animationName = g_strdup_printf("%s_finish", status);
        gtk_image_clear(GTK_IMAGE(debuff->image));
        settingTimedVideoPlay(debuff->video, 0, 31, animationName, 0, NULL, FALSE);
        playSoundByName(0, "debuff_finish", &audioPointer, 0);
        g_timeout_add(1000, timedGtkDestroyObject, debuff->fixed);
    }
}

void settingTimedImageModifier(gint timeout, GtkWidget *widget, gchar *path) {
    gtkData *imgData = g_malloc(sizeof(gtkData));
    imgData->widgetSingle = widget;
    strcpy(imgData->string, path);
    g_timeout_add(timeout, timedImageModifier, imgData);
}

void settingTimedNumbersAnimation(gint timeout, GtkLabel *label, gint range, gint animTime) {
    gtkAnimationData *data = g_malloc(sizeof(gtkAnimationData));
    data->stepDistance = (gfloat) range / (60*animTime);
    data->totalLoops = 60*animTime;
    data->finalStep = range - (data->stepDistance * data->totalLoops);
    data->actualStep = 0.0;  
    data->widget = GTK_WIDGET(label);
    g_timeout_add(timeout, settingNumbersAnimation, data);
}

gboolean timedLvlBarUpdate(gpointer data) {
    gtkLevelUpAnimationData *animData = (gtkLevelUpAnimationData*) data;
    settingUpdatelvlBarAnimation(animData->experience, animData->lbLvl, animData->lbExpText, animData->wdLevelBar, animData->fixed, animData->wdLvlUpText);
    g_timeout_add(8000, timedgFree, animData);
    return FALSE;
}

gboolean settingNumbersAnimation(gpointer data) {
    g_timeout_add(16, timedNumbersAnimation, data);
    return FALSE;
}

gboolean timedNumbersAnimation(gpointer data) {
    gtkAnimationData *animData = (gtkAnimationData*) data;
    if(animData->totalLoops >= 0) {
        gchar *number = g_strdup_printf("%d", (gint) animData->actualStep);
        labeltextModifier(GTK_LABEL(animData->widget), number);
        animData->actualStep += animData->stepDistance;
        animData->totalLoops--;
        return TRUE;
    }
    g_free(animData);
    return FALSE;
}

gboolean timedImageModifier(gpointer data) {
    gtkData *imgData = (gtkData*) data;
    gtk_image_set_from_file(GTK_IMAGE(imgData->widgetSingle), imgData->string);
    g_free(imgData);
    return FALSE;
}

gboolean timedSwitchBooleanValue(gpointer data) {
    Game *gData = (Game *) data;  
    if (gData->battle->turnPlayed == 1)
        gData->battle->turnPlayed = 0;
    else
        gData->battle->turnPlayed = 1;
    return FALSE;
}

gboolean timedInverseBooleanValue(gpointer data) {
    gboolean *bData = (gboolean *) data;
    if(*bData)
        *bData = 0;
    else
        *bData = 1;
    return FALSE;
}

gboolean timedGtkDestroyObject(gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
    return FALSE;
}

gboolean retroBarAnimationLoop(gpointer data) {
    gtkAnimationData *barData = (gtkAnimationData*) data;
    if(barData->totalLoops > 0) 
        barData->isActive = TRUE;
    else
        barData->isActive = FALSE;

    if(barData->isActive == TRUE) {
        gint actualHeight;
        gtk_widget_get_size_request(barData->widget, NULL, &actualHeight);
        barData->actualStep += barData->stepDistance;
        gint actualWidth =  barData->finalPosX - barData->actualStep;
        GtkStyleContext *bar_context = gtk_widget_get_style_context(barData->widget);
        
        if(actualWidth > 116 && barData->changed != TRUE) {
            removeAllStyleClasses(barData->widget);
            gtk_style_context_add_class(bar_context, "fr6_lifebar_green");
            barData->changed = TRUE;
        }
        else if(actualWidth < 117 && actualWidth > 58 && barData->changed == TRUE) {
            removeAllStyleClasses(barData->widget);
            gtk_style_context_add_class(bar_context, "fr6_lifebar_yellow");
            barData->changed = FALSE;
        }
        else if(actualWidth < 59 && actualWidth > 0 && barData->changed != TRUE) {
            removeAllStyleClasses(barData->widget);
            gtk_style_context_add_class(bar_context, "fr6_lifebar_red");
            barData->changed = TRUE;
        }

        gtk_widget_set_size_request(GTK_WIDGET(barData->widget), actualWidth, actualHeight);
        
        if(actualWidth <= 1)
            gtk_widget_set_visible(GTK_WIDGET(barData->widget), FALSE);
        barData->totalLoops--;
        return TRUE;
    }
    
    g_free(barData);    
    return FALSE;
}

gboolean timedSettingMoveWidgetAnimation(gpointer data) {
    gtkAnimationData *widgetData = (gtkAnimationData *) data;
    settingMoveWidgetAnimation(widgetData->timer, widgetData->widget, widgetData->fixed, widgetData->actualPosX, widgetData->actualPosY, widgetData->finalPosX, widgetData->finalPosY);
    g_free(widgetData);
    return FALSE;
}

gboolean timedgFree(gpointer data) {
    g_free(data);
    return G_SOURCE_REMOVE;
}

gboolean timedLabelModifier(gpointer data) {
    gtkData *labelData = (gtkData*) data;
    labeltextModifier(GTK_LABEL(labelData->widgetSingle), labelData->string);
    g_free(labelData);
    return G_SOURCE_REMOVE;
}

gboolean turnOnButton(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data);
    gtk_widget_set_sensitive(button, TRUE);
    return G_SOURCE_REMOVE;
}

void removeAllStyleClasses(GtkWidget *widget) {
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    GList *classes = gtk_style_context_list_classes(context);
    GList *iter = classes;

    while (iter != NULL) {
        const char *class_name = (const char *) iter->data;
        gtk_style_context_remove_class(context, class_name);
        iter = iter->next;
    }
    g_list_free(classes);
}

// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
// Animações
// Inicia uma animação de texto de label após um timeout
gboolean timedLabelAnimation(gpointer data) {
    gtkData *labelData = (gtkData*) data;
    labelTextAnimation(GTK_LABEL(labelData->widgetSingle), labelData->string, labelData->intSingle);
    g_free(labelData);
    return FALSE;
}

// Move uma widget da tela para um novo local em forma de animação
gboolean moveWidgetAnimation(gpointer data) {
    gtkAnimationData *widgetData = (gtkAnimationData*) data;
    gint deslocationX, deslocationY, multiplyStepX, multiplyStepY;
    if(widgetData->totalLoops > -1) {
        multiplyStepX = widgetData->actualPosX < widgetData->finalPosX ? 1 : -1;
        multiplyStepY = widgetData->actualPosY < widgetData->finalPosY ? 1 : -1;
        
        deslocationX = widgetData->stepX * multiplyStepX;
        deslocationY = widgetData->stepY * multiplyStepY;
        
        if(widgetData->totalLoops == 0) {
            if(widgetData->actualPosX + deslocationX != widgetData->finalPosX)
                deslocationX = abs(widgetData->actualPosX - widgetData->finalPosX) * multiplyStepX;
            if(widgetData->actualPosY + deslocationY != widgetData->finalPosY)
                deslocationY = abs(widgetData->actualPosY - widgetData->finalPosY) * multiplyStepY;
        }
        if(widgetData->finalPosX == -1)
            deslocationX = 0;
        if(widgetData->finalPosY == -1)
            deslocationY = 0;

        widgetData->actualPosX += deslocationX;
        widgetData->actualPosY += deslocationY;
    
        gtk_fixed_move(widgetData->fixed, widgetData->widget, widgetData->actualPosX, widgetData->actualPosY);
        widgetData->totalLoops -= 1;
        return TRUE;
    }
   
    g_free(widgetData);
    return FALSE;
}

// Restaura a opacidade de um botão
gboolean btn_animation_rest_opacity(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data); // Converte o gpointer para GtkWidget*
    gtk_widget_set_opacity(button, 1.0); // Restaura a opacidade
    return G_SOURCE_REMOVE; // Remove o timeout
}

void settingUpdatelvlBarAnimation(gint exp, GtkLabel *lvlTxt, GtkLabel *expTxt, GtkWidget *lvlBar, GtkFixed *fixed, GtkWidget *lvlUpTxt) {
    gint lvlUp, actualWidth, lblWidth, lblHeight, barintervalIncrement, beforeWidth;
    gchar cLvl[5], cProgressLvl[50];
    player = getPlayer(playerFile);
    beforeWidth = (gint) (player.actualExp * (100.0 / player.requiredExp));

    if(exp == -13579)
        exp = player.requiredExp - player.actualExp;

    lvlUp = addExperiencetoPlayer(playerFile, exp);
    player = getPlayer(playerFile);
    actualWidth = (int) (player.actualExp * (100.0 / player.requiredExp));
    sprintf(cLvl, "%d", player.level);
    if(player.actualExp != -2 && player.requiredExp != -2)
        sprintf(cProgressLvl, "%d/%d", player.actualExp, player.requiredExp);    
    else
        sprintf(cProgressLvl, "");

    labeltextModifier(lvlTxt, cLvl);
    labeltextModifier(expTxt, cProgressLvl);
    gtk_widget_get_size_request(lvlBar, &lblWidth, &lblHeight);

    if(lvlUp > 0) {
        gtk_fixed_move(fixed, GTK_WIDGET(lvlUpTxt), 846, 26);
        gtk_widget_set_opacity(lvlUpTxt, 1.0);
        gchar levelUpMessage[100];
        sprintf(levelUpMessage, "Level up +%d", lvlUp);
        labeltextModifier(GTK_LABEL(lvlUpTxt), levelUpMessage);
        gtkLevelUpAnimationData *lvlData = g_malloc(sizeof(gtkLevelUpAnimationData));
        lvlData->totalLoops = 20;
        lvlData->currentStep = 20;
        lvlData->fixed = fixed;
        lvlData->wdLvlUpText = lvlUpTxt;
        for(int i=20, k=0; 0 <= i; i--, k++) {
            g_timeout_add(60*(k+1), levelUpAnimation, lvlData);
        }
    }

    if(player.actualExp != -2 && player.requiredExp != -2) {
        if(beforeWidth > actualWidth) 
            beforeWidth = 0;
        
        barintervalIncrement = 400.0 / (actualWidth-beforeWidth);
        gtkLevelUpAnimationData *data = g_malloc(sizeof(gtkLevelUpAnimationData));
        data->totalLoops = actualWidth;
        data->fixed = fixed;
        data->wdLevelBar = lvlBar;
        data->currentStep = beforeWidth;
        for(int i=beforeWidth, j=0; i <= actualWidth; i++, j++) {
            g_timeout_add(barintervalIncrement*j, updateBarAnimation, data);
        }
    }
    else
        gtk_widget_set_size_request(lvlBar, 100, 10);

    updateDataCave();
}

// Exibe o level up animado em cima da barra de nível
gboolean levelUpAnimation(gpointer data) {
    gtkLevelUpAnimationData *animData = (gtkLevelUpAnimationData*) data;
    gtk_fixed_move(animData->fixed, GTK_WIDGET(animData->wdLvlUpText), 846, animData->currentStep+6);
    gtk_widget_set_opacity(animData->wdLvlUpText, animData->currentStep*0.05);
    animData->currentStep--;
    animData->totalLoops--;
    if(animData->totalLoops < 0)
        g_free(animData);
        
    return G_SOURCE_REMOVE;
}

// Aumenta o status da barra de nível
gboolean updateBarAnimation(gpointer data) {
    gtkLevelUpAnimationData *animData = (gtkLevelUpAnimationData*) data;
    gtk_widget_set_size_request(animData->wdLevelBar, animData->currentStep, 10);
    animData->currentStep++;
    animData->totalLoops--;
    if(animData->totalLoops < 0) {
        g_free(animData);
    }
    return G_SOURCE_REMOVE;
}

// Mostra a quantidade de atributos aleatórios gerados ao treinar o dragão
gboolean atributeUpAnimation(gpointer data) {
    int actualHeight = GPOINTER_TO_INT(data);
    int yPosVector[] = {154, 192, 231, 268};
    gtk_fixed_move(fr5_cave, GTK_WIDGET(fr5_cave_health_up), 470, yPosVector[0] - actualHeight);
    gtk_fixed_move(fr5_cave, GTK_WIDGET(fr5_cave_attack_up), 480, yPosVector[1] - actualHeight);
    gtk_fixed_move(fr5_cave, GTK_WIDGET(fr5_cave_defense_up), 482, yPosVector[2] - actualHeight);
    gtk_fixed_move(fr5_cave, GTK_WIDGET(fr5_cave_speed_up), 516, yPosVector[3] - actualHeight);
    gtk_widget_set_opacity(fr5_cave_health_up, 1.0 - actualHeight*0.1);
    gtk_widget_set_opacity(fr5_cave_attack_up, 1.0 - actualHeight*0.1);
    gtk_widget_set_opacity(fr5_cave_defense_up, 1.0 - actualHeight*0.1);
    gtk_widget_set_opacity(fr5_cave_speed_up, 1.0 - actualHeight*0.1);
    return G_SOURCE_REMOVE;
}

// Carrega todas as texturas dos frames
void registerTexturesAnimations() {
    loadAnimationFrames("../assets/img_files/animations/animation_battle_opening/opening_stared_transition", 109, 1); // Animação de abertura de batalha
    loadAnimationFrames("../assets/img_files/animations/animation_battle_opening/animation_battle_transition", 144, 2); // Animação de abertura batalha 2
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/bleeding", 31, 3); // Debuff de sangramento iniciado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/bleeding_finish", 31, 4); // Debuff de sangramento finalizado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/broken_armor", 31, 5); // Debuff de quebra de armadura iniciado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/broken_armor_finish", 31, 6); // Debuff de quebra de armadura finalizado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/burning", 31, 7); // Debuff de queimação iniciado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/burning_finish", 31, 8); // Debuff de queimaçao finalizado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/terrified", 31, 9); // Debuff de terror iniciado
    loadAnimationFrames("../assets/img_files/animations/debuffs_animations/terrified_finish", 31, 10); // Debuff de terror finalizado
    loadAnimationFrames("../assets/img_files/animations/result_animations/victory", 91, 11); // Vitória
    loadAnimationFrames("../assets/img_files/animations/result_animations/defeat", 91, 12); // Derrota
    loadAnimationFrames("../assets/img_files/animations/battle_animations/keypress", 4, 13); // Keypress
    loadAnimationFrames("../assets/img_files/animations/battle_animations/fire_breath_animation", 39, 14); // Sopro de fogo
    loadAnimationFrames("../assets/img_files/animations/battle_animations/scratch_claw_animation_ent1", 36, 15); // Arranhão
    loadAnimationFrames("../assets/img_files/animations/battle_animations/bite_crunch_animation", 30, 16); // Arranhão
    loadAnimationFrames("../assets/img_files/animations/battle_animations/scratch_claw_animation_ent2", 40, 17); // Arranhão
    loadAnimationFrames("../assets/img_files/animations/common/dragon_grow_animation/dragon_grow_animation", 61, 18); // Animação de Evolução

}

// Carrega os frames para o vetor
void loadAnimationFrames(gchar *path, gint totalFrames, gint animationIndex) {
    for (int i = 0; i < totalFrames; i++) {
        gchar filename[256];
        snprintf(filename, sizeof(filename), "%s/frame (%d).png", path, i+1);
        //g_print("Tentando carregar: %s\n", filename);
        
        pAnimationsFrameVector[animationIndex-1][i] = gdk_pixbuf_new_from_file(filename, NULL);
        /*if (pAnimationsFrameVector[animationIndex][i] != NULL) {
            g_print("Frame %d carregado com sucesso.\n", i);
        } else {
            g_printerr("Erro ao carregar frame %d: %s\n", i + 1, filename);
        }*/
    }
}

gboolean on_draw_animation(GtkWidget *widget, cairo_t *cr, gpointer data) {
    AnimationData *animData = (AnimationData *) data;
    if (!animData || animData->finished) return FALSE;

    // Obtém o tempo atual do frame
    gint64 now = g_get_monotonic_time();  // Tempo em microssegundos
    gint64 elapsed = now - animData->startTime; // Tempo decorrido

    // Seta qual será o frame atual
    gint frameDuration = 1000000 / 60; // 60 FPS -> cada frame dura 16.67ms (16666µs)
    animData->currentFrame = elapsed / frameDuration;
    
    if (animData->currentFrame >= animData->totalFrames || (animData->cancelAnimation && *(game->minigame->minigameValue) == 1)) {
        if(animData->cancelAnimation && *(game->minigame->minigameValue) == 1)
            animData->isLoop = 0;
        //g_print("Animação: %s | isLoop: %d\n", animData->animationName, animData->isLoop);
        if(animData->isLoop == 1)
            settingTimedVideoPlay(animData->widget, 0, animData->totalFrames, animData->animationName, 1, animData->cancelAnimation, FALSE);
        animData->finished = TRUE;
        g_signal_handlers_disconnect_by_func(animData->widget, G_CALLBACK(on_draw_animation), animData);
        if(animData->canDestroy)
            g_timeout_add(1000, timedGtkDestroyObject, GTK_WIDGET(animData->widget));
         
        g_timeout_add(500, timedgFree, animData);
        return FALSE;
    }
    

    // Carrega e desenha o frame correto
    GdkPixbuf *frame = pAnimationsFrameVector[animData->animationIndex - 1][animData->currentFrame];
    if (frame) {
        gdk_cairo_set_source_pixbuf(cr, frame, 0, 0);
        cairo_paint(cr);
    }

    g_timeout_add(16, (GSourceFunc) gtk_widget_queue_draw, widget); // Aproximadamente 60 FPS
    return FALSE;
}

void logStartAnimation(gchar *text, gchar *color, gint duration, gint height, gint width, gint x, gint y, gint yDirection, GtkFixed *fixed) {
    gchar colorText[100];
    GtkWidget *label = gtk_label_new(text);
    GtkStyleContext *label_context = gtk_widget_get_style_context(label);
    g_snprintf(colorText, sizeof(colorText), "%s", color);
    
    gtk_fixed_put(GTK_FIXED(fixed), label, x, y); 
    gtk_widget_set_size_request(label, width, height);     
    gtk_style_context_add_class(label_context, "fr5_exp_label");
    gtk_style_context_add_class(label_context, "fr6_stared_font_size");
    gtk_style_context_add_class(label_context, colorText);
    gtk_widget_show(label);
    
    GtkUpAnimationData *data = g_malloc(sizeof(GtkUpAnimationData) * 1);
    data->interactions = yDirection;
    data->container = fixed;
    data->widget = label;
    data->x = x;
    data->y = y;
    data->opacity = 1.0;
    data->opacityDecrease = 1.0 / (gfloat) yDirection;
    g_timeout_add(duration / yDirection , logAnimation, data);
}

gboolean logAnimation(gpointer data) {
    GtkUpAnimationData *animData = (GtkUpAnimationData*) data;
    if(animData->interactions > 0) {
        animData->y -= 1;
        animData->opacity -= animData->opacityDecrease;
        gtk_fixed_move(animData->container, GTK_WIDGET(animData->widget), animData->x, animData->y);
        if(animData->widget != NULL)
            gtk_widget_set_opacity(animData->widget, animData->opacity);
        animData->interactions -= 1;
        return TRUE;
    }

    gtk_widget_destroy(animData->widget);
    g_free(animData);
    return FALSE;
}

// Compatibilidade para rodar a main
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    return SDL_main(__argc, __argv);
}
#endif
