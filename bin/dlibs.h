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