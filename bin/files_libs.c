#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dlibs.h"

FILE * createAccountslistfile();
int createAccountfile(char *username);
int overwriteAccount(FILE * pFile, char *email, char *username);
int accountsLength(FILE *pFile);
Account * readAccountvector(FILE *pFile);
int addAccountinlist(Account *account);

/*int main() {
    //createAccountfile();
    //createAccountfile("Email@hotmail.com");
    return 0;
}
*/
FILE * createAccountslistfile() {
    FILE *pFile = fopen("../accounts/accountsList.bin", "r");

    if(!(pFile)) {
        pFile = fopen("../accounts/accountsList.bin", "ab");
    }
    return pFile;
}

int addAccountinlist(Account *account) {
    FILE *pFile = fopen("../accounts/accountsList.bin", "ab");
    fwrite(account, sizeof(Account), 1, pFile);
    return 0;
}

int createAccountfile(char *username) {
    srand(time(NULL));
    char accountName[200];
    int randomNumber = 99999 + rand() % (99999 - 1 + 1);
    sprintf(accountName, "../accounts/account_%d.bin", randomNumber);
    FILE *pFile = fopen(accountName, "wb");
    fwrite(username, sizeof(char), strlen(username), pFile);
    fclose(pFile);
}

Account * readAccountvector(FILE *pFile) {
    int length = accountsLength(pFile);
    Account *vector = (Account *) malloc(sizeof(Account) * length);

    if(vector == NULL)
        return NULL;
    
    Account account;
    rewind(pFile);
    int count = 0;
    while(fread(&account, sizeof(Account), 1, pFile) != 0)
        vector[count++] = account;
    
    rewind(pFile);
    return vector;
}

int accountsLength(FILE *pFile) {
    fseek(pFile, 0, SEEK_END);
    return ftell(pFile) / sizeof(Account);
}

int overwriteAccount(FILE *pFile, char *email, char *username) {
    Account *vector = readAccountvector(pFile);
    for(int i=0; i < accountsLength(pFile); i++)
        if(strcmp(vector[i].email, email) == 0 || strcmp(vector[i].username, username) == 0) 
            return 1;
    return 0;
}