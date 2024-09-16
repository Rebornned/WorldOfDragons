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
    char name[25];
    char history[500];
    char img_path[300];
    int level;
    int age;
    int attack;
    int defense;
    int speed;
    int health;
} Dragon;

int newAccount(FILE *pFile, char user[], char email[], char pass[]);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);

Account * readAccountvector(FILE *pFile);
Dragon * readBeastvector(FILE *pFile);
Dragon getplayerDragon(FILE *pFile, char *name);

int addAccountinlist(Account *account);
int delAccountinlist(FILE *pFile, char *username);
int changePassword(FILE *pFile, char *email, char *password, char *confirmPass);

int addBeastinlist(Dragon *dragon);
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


