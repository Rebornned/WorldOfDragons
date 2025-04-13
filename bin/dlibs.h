#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <ctype.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <epoxy/gl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <sys/time.h>

/*
typedef struct {
    gchar path[200];
    gchar animationName[150];
    GdkPixbuf *actualTexture;
    GtkWidget *loader;
    GtkFixed *fixed;
    gint totalLoops;
    gint actualFrame;
    gint animationIndex;
    gint posX;
    gint posY;
    gint width;
    gint height;
} gtkMediaData;
*/
typedef struct {
    GtkWidget *widget;
    gint animationIndex;
    gint currentFrame;
    gint totalFrames;
    guint timeoutID;
    gboolean finished;
    gint64 startTime;  // Novo campo para armazenar tempo inicial
} AnimationData;

typedef struct {
    char name[150];
    char description[600];
    float multiplicator;
    int cooldownAttack;
    int recharge;
    int precision;
} Attack;

typedef struct {
    char username[50];
    char email[50];
    char password[30];
} Account;

typedef struct {
    char name[50];
    char tittle[150];
    char history[1500];
    char img_path[400];
    char length[150];
    char age[150];
    int unlock_id;
    int abs_age;
    int level;
    int attack;
    int defense;
    int speed;
    int health;
} Dragon;

typedef struct {
    int level;
    int actualExp;
    int requiredExp;
    int trainPoints;
    int progressPoints;
    int actualProgress;
    Dragon dragon;
} Player;

typedef struct {
    gchar type[100];
    gint turns;
    gint slot;
    GtkFixed *fixed;
    GtkWidget *video;
    GtkImage *image;
} Debuff;

typedef struct {
    char type[100];
    int turns;
} Buff;

typedef struct {
    Dragon entDragon;
    Dragon fixedDragon;
    Buff entityBuffs[4];
    Debuff entityDebuffs[4];
    gint skillsCooldown[4];
} Entity;

typedef struct {
    Entity EntityOne;
    Entity EntityTwo;
    gint entityTurn;
    gint turnPlayed;
    gint actualTurn;
    gchar winnerEnt[100];
    gint expReward;
    gint difficult;
} Battle;

typedef struct {
    gchar pAction[100];
    gboolean minigamePlayed;
    gint *minigameValue;
    gint minigameResultValue;
    gint vectorRange[4][4];
} MiniGame;
typedef struct {
    Battle *battle;
    MiniGame *minigame;
    GtkFixed *fixed;
    GtkWidget *actualTurn;
    GtkWidget *pHealthBar;
    GtkWidget *eHealthBar;
    GtkStack *optionsStack;
    GtkLabel *pHealthText;
    GtkLabel *battleText;
    GtkLabel *turnsText;
    GtkBuilder *builder;
} Game;

int newAccount(FILE *pFile, char user[], char email[], char pass[]);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);
int random_choice(int min, int max);

int initPlayer(FILE *pFile, Player *newPlayer);
Player getPlayer(FILE *pFile);
int changePlayerStatus(FILE *pFile, int level, int points, int actualExp, int requiredExp, int progressPoints, int actualProgress, Dragon *dragon);
Dragon trainplayerDragon(Dragon dragon, int lvls);
int addExperiencetoPlayer(FILE *pFile, int exp);

Account * readAccountvector(FILE *pFile);
Dragon * readBeastvector(FILE *pFile);
Attack * readAttackvector(FILE *pFile);
Dragon getplayerDragon(FILE *pFile, char *name);

int addAccountinlist(Account *account, FILE *pFile);
int delAccountinlist(FILE *pFile, char *username);
int changePassword(FILE *pFile, char *email, char *password, char *confirmPass);

int addBeastinlist(Dragon *dragon, FILE *pFile);
int delBeastinlist(FILE *pFile, char *name);

int addAttackinlist(Attack *attack, FILE *pFile);

int validateAccount(FILE *pFile, char *username, char *password);
FILE * getAccountfile(char *username);
int overwriteAccount(FILE * pFile, char *email, char *username);

Dragon * bubbleSort(int type, Dragon * vector, int length);
int printfDragonvector(Dragon * vector, int length);

FILE * createAccountslistfile();
FILE * createBeastslistfile();
FILE * createAttackslistfile();
void reinsFile(FILE *pFile);
int accountsLength(FILE *pFile);
int beastsLength(FILE *pFile);
int attacksLength(FILE *pFile);

void setBattleVariables(Battle *battleInstance, Dragon playerEnt, Dragon enemyEnt, Player player, gint dragonIndex);
int startTurn(Battle *battleInstance, Game *game);
int causeDamage(int damage, float multiplicator, int precision, Dragon *enemy);
int debuffTick(Debuff *debuff, Entity *entity, gint entityNumber, Game *game);
int applyDebuff(gchar *debuffType, gint turns, Entity *entity, gint *duplicated);

// Animations
void logStartAnimation(gchar *text, gchar *color, gint duration, gint height, gint width, gint x, gint y, gint yDirection, GtkFixed *fixed);
void retroBarAnimationStart(gint timer, GtkWidget *widget, gint actualValue, gint newValue);
gboolean on_draw_animation(GtkWidget *widget, cairo_t *cr, gpointer data);
void settingTimedVideoPlay(GtkWidget *widget, gint timeout, gint totalFrames, gchar *animationName);
gboolean delayedStartAnimation(gpointer data);
void startAnimation(GtkWidget *widget, gint animationIndex, gint totalFrames);
void settingTimedImageModifier(gint timeout, GtkWidget *widget, gchar *path);
void updateDebuffAnimation(gint entityNumber, gchar *type, Debuff *debuff, gint animationType, gchar *status);

// Game
gboolean onBattle(gpointer data);