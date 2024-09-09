#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dlibs.h"
#include <unistd.h>
#include <locale.h>

FILE * createAccountslistfile();
int createAccountfile(char *username);
int overwriteAccount(FILE * pFile, char *email, char *username);
int accountsLength(FILE *pFile);
Account * readAccountvector(FILE *pFile);
int addAccountinlist(Account *account);
void reinsFile(FILE *pFile);
int delAccountinlist(char *username);
int validateAccount(FILE *pFile, char *username, char *password);

/*int main() {
    //createAccountfile();
    //createAccountfile("Email@hotmail.com");
    return 0;
}
*/
FILE * createAccountslistfile() {
    FILE *pFile = fopen("../accounts/accountsList.bin", "rb");

    if(!(pFile)) {
        pFile = fopen("../accounts/accountsList.bin", "ab");
    }
    return pFile;
}

int createAccountfile(char *username) {
    char accountName[200];
    sprintf(accountName, "../accounts/account_%s.bin", username);
    FILE *pFile = fopen(accountName, "wb");
    fwrite(username, sizeof(char), strlen(username), pFile);
    fclose(pFile);
}

int addAccountinlist(Account *account) {
    FILE *pFile = fopen("../accounts/accountsList.bin", "ab");
    fwrite(account, sizeof(Account), 1, pFile);
    return 0;
}

int delAccountinlist(char *username) {
    FILE *pFile = fopen("../accounts/accountsList.bin", "ab");
    char delfilename[200];
    sprintf(delfilename, "../accounts/account_%s.bin", username);
    remove(delfilename);

    int length = accountsLength(pFile);
    Account *vector = readAccountvector(pFile);
    if(vector == NULL) {
        return 1;
    }
    reinsFile(pFile);
    for(int i=0; i < length; i++) {
        if(strcmp(username, vector[i].username) != 0)
            fwrite(&vector[i], sizeof(Account), 1, pFile);
    }
    rewind(pFile);
    free(vector);
    return 0;
}

Account * readAccountvector(FILE *pFile) {
    int length = accountsLength(pFile);
    Account *vector = (Account *) malloc(sizeof(Account) * length);

    if(vector == NULL)
        return NULL;
    
    Account account;
    rewind(pFile);
    int count = 0;
    while(fread(&account, sizeof(Account), 1, pFile) != 0) {
        vector[count++] = account;
    }
    rewind(pFile);
    return vector;
}

int accountsLength(FILE *pFile) {
    fseek(pFile, 0, SEEK_END);
    return ftell(pFile) / sizeof(Account);
}

int overwriteAccount(FILE *pFile, char *email, char *username) {
    Account *vector = readAccountvector(pFile);
    for(int i=0; i < accountsLength(pFile); i++) {
        if(strcmp(vector[i].email, email) == 0 || strcmp(vector[i].username, username) == 0) 
            return 1;
    }
    free(vector);
    return 0;
}

int validateAccount(FILE *pFile, char *username, char *password) {
    Account *vector = readAccountvector(pFile);
    for(int i=0; i < accountsLength(pFile); i++) {
        printf("user: %s | senha: %s\n", &vector[i].username, &vector[i].password);
        if(strcmp(vector[i].username, username) == 0 && strcmp(vector[i].password, password) == 0) {
            free(vector);
            return 0;
        }
    } 
    free(vector);
    return 1;
}

void reinsFile(FILE *pFile) {
    int fd = fileno(pFile);
    fflush(pFile);
    ftruncate(fd, 0);
    rewind(pFile);
}

