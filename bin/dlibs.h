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
    char name[100];
    int level;
    int attack;
    int defense;
    int speed;
    int health;
} Dragon;

int newAccount(FILE *pFile, char user[], char email[], char pass[]);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);
FILE * createAccountslistfile();
int createAccountfile(char *username);
int overwriteAccount(FILE * pFile, char *email, char *username);
int accountsLength(FILE *pFile);
Account * readAccountvector(FILE *pFile);
int addAccountinlist(Account *account);
int delAccountinlist(char *username);
int validateAccount(FILE *pFile, char *username, char *password);
void reinsFile(FILE *pFile);
