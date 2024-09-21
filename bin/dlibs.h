#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char username[50];
    char email[50];
    char password[30];
} Account;

typedef struct {
    char name[50];
    char history[1500];
    char img_path[400];
    char length[150];
    char age[150];
    int abs_age;
    int level;
    int attack;
    int defense;
    int speed;
    int health;
} Dragon;

typedef struct {
    Account account;
    Dragon dragon;
} Player;

int newAccount(FILE *pFile, char user[], char email[], char pass[]);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);

Account * readAccountvector(FILE *pFile);
Dragon * readBeastvector(FILE *pFile);
Dragon getplayerDragon(FILE *pFile, char *name);

int addAccountinlist(Account *account, FILE *pFile);
int delAccountinlist(FILE *pFile, char *username);
int changePassword(FILE *pFile, char *email, char *password, char *confirmPass);

int addBeastinlist(Dragon *dragon, FILE *pFile);
int delBeastinlist(FILE *pFile, char *name);

int validateAccount(FILE *pFile, char *username, char *password);
FILE * getAccountfile(char *username);
int overwriteAccount(FILE * pFile, char *email, char *username);

Dragon * bubbleSort(int type, Dragon * vector, int length);
int printfDragonvector(Dragon * vector, int length);

FILE * createAccountslistfile();
FILE * createBeastslistfile();
void reinsFile(FILE *pFile);
int accountsLength(FILE *pFile);
int beastsLength(FILE *pFile);


