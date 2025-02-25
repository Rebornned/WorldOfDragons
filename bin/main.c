#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <direct.h>
#include <math.h>
#include "dlibs.h"

// Compilação necessária para funcionar
// cd C:/Users/Amage/Desktop/Prog/Projects/WorldOfDragons/bin/
// cd C:/Users/Amage/Desktop/Programacao/GKT_C/WorldOfDragons/bin
// gcc -o main main.c files_libs.c sorts_libs.c `pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango`
// Sem terminal: gcc -o main.exe main.c files_libs.c sorts_libs.c account.c player_libs.c battle_libs.c -mwindows `pkg-config --cflags --libs gtk+-3.0 glib-2.0 pango`


// =====================================================================================================
// Estrutura de dados GTK
typedef struct {
    GtkWidget * widgetSingle;
    GtkWidget * widgetVector[100];
    int intSingle;
    float floatSingle;
    int intVector[100];
    float floatVector[200];
    char string[2000];
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
    gint timer;
    gboolean isActive;
} gtkAnimationData;

typedef struct {
    gchar path[200];
    gchar animationName[150];
    GtkImage *loader;
    gint totalLoops;
    gint actualFrame;
    gint animationIndex;
} gtkMediaData;

// Ponteiros globais
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// Animations
GdkPixbuf *pAnimationsFrameVector[10][500];  // Array animations

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
gboolean btn_animation_rest_opacity(gpointer data);
void btn_animation_clicked(GtkWidget *widget, gpointer data);
void set_dragon_in_beastiary(GtkButton *btn, gpointer data);
void sort_dragons_in_beastiary(GtkButton *btn, gpointer data);
void updatelvlBar(GtkWidget *widget, gpointer data);
gboolean updateBarAnimation(gpointer data);
gboolean levelUpAnimation(gpointer data);
gboolean atributeUpAnimation(gpointer data);
void updateDataCave();
void updateColiseum();
void updatelvlDragon(GtkButton *btn, gpointer data);
void labelTextAnimation(GtkLabel *label, char *text, int timer);
void set_attack_in_cave(GtkButton *btn, gpointer data);
void retroBarAnimationStart(gint timer, GtkWidget *widget, gint actualValue, gint newValue);
void settingMoveWidgetAnimation(gint timer, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY);
void settingVideoPlay(GtkImage *loader, gchar *path, gint totalFrames, gchar *animationName);
void settingTimedVideoPlay(gint timeout, GtkImage *loader, gchar *path, gint totalFrames, gchar *animationName);
void settingTimedMoveWidgetAnimation(gint timerAnimation, gint timeout, GtkWidget *widget, GtkFixed *fixed, gint actualX, gint actualY, gint finalPosX, gint finalPosY);
void loadingPixbuffAnimation(char *path, gint totalFrames, gint animationIndex);
void registerAnimations();
gboolean moveWidgetAnimation(gpointer data);
gboolean retroBarAnimationLoop(gpointer data);
gboolean turnOnButton(gpointer data);
gboolean timedSettingMoveWidgetAnimation(gpointer data);
gboolean timedLabelModifier(gpointer data);
gboolean timedLabelAnimation(gpointer data);
gboolean timedgFree(gpointer data);
gboolean updateLoaderFrame(gpointer data);
gboolean timedSettingVideoPlay(gpointer data);
gboolean settingBattleWindow(gpointer data);

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
    srand(time(NULL));
    playerFile = getAccountfile("Rambo");

    initPlayer(playerFile, &player);
    changePlayerStatus(playerFile, 0, 0, 0, 1, 27, 27, NULL);

    // Inicializar ações na tela 5
    sort_dragons_in_beastiary(fr5_btn_dragon1, NULL);
    set_dragon_in_beastiary(fr5_btn_dragon1, GINT_TO_POINTER(0));
    updateDataCave();
    updateColiseum();
    
    // Registrando sinais de callback para botões executarem funções
    registerAnimations();
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

    // Frame 5 Elementos para analisar e limpar
    GtkEntry *fr5_cave_inp_name = GTK_ENTRY(gtk_builder_get_object(builder, "fr5_cave_inp_name"));
    GtkLabel *fr5_cave_dragon_name_error = GTK_LABEL(gtk_builder_get_object(builder, "fr5_cave_dragon_name_error"));

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

    // Frame 5 Main
    if (g_strcmp0(button_name, "fr5_btn_next") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);
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
            updatelvlBar(NULL, GINT_TO_POINTER(1));            
        }
        else
            labeltextModifier(fr5_cave_dragon_name_error, "O nome deve possuir entre 1 e 30 caracteres.");
        
    }

    // Frame 5 Colíseu
    if (g_strcmp0(button_name, "fr5_btn_coliseum_next") == 0) {
        btn_animation_clicked(GTK_WIDGET(btn), NULL);

        if(fr5_actual_dragon_index > 0) {
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

        if(((pBeastVector[dragonIndex].unlock_id-27) * -1) <= player.actualProgress) {
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

            settingTimedMoveWidgetAnimation(330, 3000, fr6_stared_player_dragon_wid, fr6_stared_fixed_animation, -250, 18, 80, -1);
            settingTimedMoveWidgetAnimation(330, 3000, fr6_stared_dragon_border2, fr6_stared_fixed_animation, -262, 4, 68, -1);

            playerDragonName = g_strdup_printf("%s %s", player.dragon.name, dragonStage);
            gtkData *labelAnimationDataPlayer = g_malloc(sizeof(gtkData));
            labelAnimationDataPlayer->intSingle = 1500;
            strcpy(labelAnimationDataPlayer->string, playerDragonName);
            labelAnimationDataPlayer->widgetSingle = GTK_WIDGET(fr6_dragon_name);
            g_timeout_add(3330, timedLabelAnimation, labelAnimationDataPlayer);

            gtkData *labelVersusText = g_malloc(sizeof(gtkData));
            GtkWidget *fr6_label_vs_text = GTK_WIDGET(gtk_builder_get_object(builder, "fr6_label_vs_text"));
            labelVersusText->intSingle = 750;
            strcpy(labelVersusText->string, "VS");
            labelVersusText->widgetSingle = GTK_WIDGET(fr6_label_vs_text);
            g_timeout_add(4830, timedLabelAnimation, labelVersusText);
            
            GtkImage *fr6_video_loader = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_video_loader"));
            settingVideoPlay(fr6_video_loader, "../assets/img_files/animations/animation_battle_opening/render", 109, "battle_opening");

            settingTimedMoveWidgetAnimation(330, 5580, fr6_stared_enemy_dragon_wid, fr6_stared_fixed_animation, 1014, 18, 702, -1);
            settingTimedMoveWidgetAnimation(330, 5580, fr6_stared_dragon_border, fr6_stared_fixed_animation, 1002, 4, 688, -1);
            g_timeout_add(5910, timedLabelAnimation, labelAnimationDataEnemy);
            settingTimedVideoPlay(7600, fr6_video_loader, "../assets/img_files/animations/animation_battle_opening/animation_battle_transition", 139, "battle_opening2");

            g_print("Pode batalhar\n");
            Battle *battleInstance = g_malloc(sizeof(Battle));
            player = getPlayer(playerFile);
            strcpy(player.dragon.img_path, playerDragonImgPath);

            setBattleVariables(battleInstance, player.dragon, pOriginalBeastVector[dragonIndex], player);
            g_timeout_add(9680, settingBattleWindow, battleInstance);
        }
        else {
            g_print("Não pode batalhar\n");
        }
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
    g_signal_connect(fr5_add_xp, "clicked", G_CALLBACK(updatelvlBar), GINT_TO_POINTER(20000));

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
        char actBtnName[100];
        sprintf(actBtnName, "fr5_btn_dragon%d", i+1);
        GObject *actual_btn = gtk_builder_get_object(builder, actBtnName);
        g_signal_connect(actual_btn, "clicked", G_CALLBACK(set_dragon_in_beastiary), GINT_TO_POINTER(i));
    }
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
}

void updatelvlBar(GtkWidget *widget, gpointer data) {
    int lvlUp, actualWidth, exp = GPOINTER_TO_INT(data), lblWidth, lblHeight, barintervalIncrement, beforeWidth;
    char cLvl[5], cProgressLvl[50];
    player = getPlayer(playerFile);
    beforeWidth = (int) (player.actualExp * (100.0 / player.requiredExp));

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

    labeltextModifier(fr5_label_lvl, cLvl);
    labeltextModifier(fr5_exp_text, cProgressLvl);
    gtk_widget_get_size_request(fr5_level_bar, &lblWidth, &lblHeight);

    if(lvlUp > 0) {
        gtk_fixed_move(fr5_beastiary, GTK_WIDGET(fr5_levelup_text), 846, 26);
        gtk_widget_set_opacity(fr5_levelup_text, 1.0);
        char levelUpMessage[100];
        sprintf(levelUpMessage, "Level up +%d", lvlUp);
        labeltextModifier(GTK_LABEL(fr5_levelup_text), levelUpMessage);
        for(int i=20, k=0; 0 <= i; i--, k++) {
            g_timeout_add(60*(k+1), levelUpAnimation, GINT_TO_POINTER(i));
        }
    }

    if(player.actualExp != -2 && player.requiredExp != -2) {
        if(beforeWidth > actualWidth) 
            beforeWidth = 0;
        
        barintervalIncrement = 400.0 / (actualWidth-beforeWidth);
        for(int i=beforeWidth, j=0; i <= actualWidth; i++, j++) {
            g_timeout_add(barintervalIncrement*j, updateBarAnimation, GINT_TO_POINTER(i));
        }
    }
    else
        gtk_widget_set_size_request(fr5_level_bar, 100, 10);

    updateDataCave();
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
    btn_animation_clicked(GTK_WIDGET(btn), NULL);
    player = getPlayer(playerFile);
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
    for(int i=0; i < totalBeasts; i++) {
        char actBtnName[100];
        sprintf(actBtnName, "fr5_btn_dragon%d", i+1);
        GtkButton *actual_btn = GTK_BUTTON(gtk_builder_get_object(builder, actBtnName));
        if(((pBeastVector[i].unlock_id-27) * -1) <= player.actualProgress) {
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

        changePlayerStatus(playerFile, -1, player.trainPoints-1, -1, -1, -1, -1, &player.dragon);
        player = getPlayer(playerFile);
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

    if(((pBeastVector[dragonIndex].unlock_id-27) * -1) > player.actualProgress) {
        labeltextModifier(fr5_dragon_name_legendary, "");
        labeltextModifier(fr5_dragon_name_epic, "");
        labeltextModifier(fr5_dragon_name_rare, "");
        labeltextModifier(fr5_dragon_name_common, "Desconhecido");
        gtk_image_set_from_file(GTK_IMAGE(actualImage), "../assets/img_files/beast_unknown_coliseum.png");
        gtk_image_set_from_file(GTK_IMAGE(fr5_coliseum_bg), "../assets/img_files/unknown_background.png");
    }
}

void labelTextAnimation(GtkLabel *label, char *text, int timer) {
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
    barData->totalLoops = (gint) ((gfloat) barSize - barSize * ((gfloat) newValue/actualValue));
    g_timeout_add(timer/barData->totalLoops, retroBarAnimationLoop, barData);
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

void settingTimedVideoPlay(gint timeout, GtkImage *loader, gchar *path, gint totalFrames, gchar *animationName) {
    gtkMediaData *mediaData = g_malloc(sizeof(gtkMediaData));
    mediaData->loader = loader;
    mediaData->totalLoops = totalFrames;
    mediaData->actualFrame = 1;
    if(strcmp(animationName, "battle_opening") == 0) {
        mediaData->animationIndex = 1;
    }
    else if(strcmp(animationName, "battle_opening2") == 0) {
        mediaData->animationIndex = 2;
    }

    strcpy(mediaData->animationName, animationName);
    strcpy(mediaData->path, path);
    g_timeout_add_full(G_PRIORITY_HIGH, timeout, (GSourceFunc) timedSettingVideoPlay, mediaData, NULL);
    //g_timeout_add(timeout, timedSettingVideoPlay, mediaData);
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

void settingVideoPlay(GtkImage *loader, gchar *path, gint totalFrames, gchar *animationName) {
    gtkMediaData *mediaData = g_malloc(sizeof(gtkMediaData));
    mediaData->loader = loader;
    mediaData->totalLoops = totalFrames;
    mediaData->actualFrame = 1;

    if(strcmp(animationName, "battle_opening") == 0) {
        mediaData->animationIndex = 1;
    }
    else if(strcmp(animationName, "battle_opening2") == 0) {
        mediaData->animationIndex = 2;
    }
    strcpy(mediaData->animationName, animationName);
    strcpy(mediaData->path, path);
    g_timeout_add_full(G_PRIORITY_HIGH, 16, (GSourceFunc) updateLoaderFrame, mediaData, NULL);
}

void registerAnimations() {
    loadingPixbuffAnimation("../assets/img_files/animations/animation_battle_opening/render", 109, 1); // Animação de abertura de batalha
    loadingPixbuffAnimation("../assets/img_files/animations/animation_battle_opening/animation_battle_transition", 139, 2); // Animação de abertura batalha 2
}

void loadingPixbuffAnimation(char *path, gint totalFrames, gint animationIndex) {
    char framePath[200];
    for (int i = 1; i <= totalFrames; i++) {
        snprintf(framePath, sizeof(framePath),"%s/frame (%d).png", path, i);
        pAnimationsFrameVector[animationIndex][i] = gdk_pixbuf_new_from_file(framePath, NULL);
    }
}

gboolean settingBattleWindow(gpointer data) {
    Battle *battle = (Battle *) data;
    GtkStack *fr6_stack = GTK_STACK(gtk_builder_get_object(builder, "fr6_stack"));
    GtkImage *fr6_combat_bg = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_bg"));
    GtkLabel *fr6_lvl_name_ent1 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_lvl_name_ent1"));
    GtkLabel *fr6_lvl_name_ent2 = GTK_LABEL(gtk_builder_get_object(builder, "fr6_lvl_name_ent2"));
    GtkImage *fr6_combat_player_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_player_dragon"));
    GtkImage *fr6_combat_enemy_dragon = GTK_IMAGE(gtk_builder_get_object(builder, "fr6_combat_enemy_dragon"));

    gchar ent1_lvl_name[100], ent2_lvl_name[100];
    gchar *battleBackgroundPath = g_strdup_printf("../assets/img_files/battle/%s_bg.png", battle->EntityTwo.entDragon.name);
    gchar *enemyDragonImgPath = g_strdup_printf("../assets/img_files/dragons/battle_%s.png", battle->EntityTwo.entDragon.name);

    gtk_image_set_from_file(fr6_combat_bg, battleBackgroundPath);
    snprintf(ent1_lvl_name, sizeof(ent1_lvl_name), "%d %s", battle->EntityOne.entDragon.level, battle->EntityOne.entDragon.name);
    snprintf(ent2_lvl_name, sizeof(ent2_lvl_name), "%d %s", battle->EntityTwo.entDragon.level, battle->EntityTwo.entDragon.name);

    labeltextModifier(fr6_lvl_name_ent1, ent1_lvl_name);
    labeltextModifier(fr6_lvl_name_ent2, ent2_lvl_name);
    gtk_image_set_from_file(fr6_combat_player_dragon, battle->EntityOne.entDragon.img_path);

    gtk_image_set_from_file(fr6_combat_enemy_dragon, enemyDragonImgPath);

    gtk_stack_set_visible_child_name(fr6_stack, "fr6_combat");

    return FALSE;
}

gboolean retroBarAnimationLoop(gpointer data) {
    gtkAnimationData *barData = (gtkAnimationData*) data;
    if(barData->totalLoops > -1) 
        barData->isActive = TRUE;
    else
        barData->isActive = FALSE;

    if(barData->isActive == TRUE) {
        gint actualWidth, actualHeight;
        gtk_widget_get_size_request(GTK_WIDGET(barData->widget), &actualWidth, &actualHeight);
        gtk_widget_set_size_request(GTK_WIDGET(barData->widget), actualWidth-1, actualHeight);
        barData->totalLoops--;
        return TRUE;
    }
    
    g_free(barData);    
    return FALSE;
}

gboolean timedSettingVideoPlay(gpointer data) {
    gtkMediaData *mediadata = ( gtkMediaData* ) data;
    settingVideoPlay(mediadata->loader, mediadata->path, mediadata->totalLoops, mediadata->animationName);
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

gboolean timedLabelAnimation(gpointer data) {
    gtkData *labelData = (gtkData*) data;
    labelTextAnimation(GTK_LABEL(labelData->widgetSingle), labelData->string, labelData->intSingle);
    g_free(labelData);
    return FALSE;
}

gboolean turnOnButton(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data);
    gtk_widget_set_sensitive(button, TRUE);
    return G_SOURCE_REMOVE;
}

// Animações
// Restaura a opacidade de um botão
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

gboolean btn_animation_rest_opacity(gpointer data) {
    GtkWidget *button = GTK_WIDGET(data); // Converte o gpointer para GtkWidget*
    gtk_widget_set_opacity(button, 1.0); // Restaura a opacidade
    return G_SOURCE_REMOVE; // Remove o timeout
}

// Exibe o level up animado em cima da barra de nível
gboolean levelUpAnimation(gpointer data) {
    int actualHeight = GPOINTER_TO_INT(data);
    gtk_fixed_move(fr5_beastiary, GTK_WIDGET(fr5_levelup_text), 846, actualHeight+6);
    gtk_widget_set_opacity(fr5_levelup_text, actualHeight*0.05);
    return G_SOURCE_REMOVE;
}

// Aumenta o status da barra de nível
gboolean updateBarAnimation(gpointer data) {
    int actualWidth = GPOINTER_TO_INT(data);
    gtk_widget_set_size_request(fr5_level_bar, actualWidth, 10);
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

gboolean updateLoaderFrame(gpointer data) {
    gtkMediaData *mediaData = (gtkMediaData*) data;
    if(mediaData->totalLoops > 0) {
        gchar actualPath[300];
        snprintf(actualPath, sizeof(actualPath), "%s/frame (%d).png", mediaData->path, mediaData->actualFrame);
        //g_print("path atual: %s\n", actualPath);
        //gtk_image_set_from_file(mediaData->loader, actualPath);
        gtk_image_set_from_pixbuf(mediaData->loader, pAnimationsFrameVector[mediaData->animationIndex][mediaData->actualFrame]);
        mediaData->actualFrame += 1;
        mediaData->totalLoops -= 1;
        return TRUE;
    }

    gtk_image_clear(GTK_IMAGE(mediaData->loader));
    g_free(mediaData);
    return FALSE;
}