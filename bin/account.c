#include "dlibs.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

void newAccount(char *user, char *email, char *pass);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);

int main() {
    //newAccount("teste", "gmail", "1234");
    char string[] = "Um lobo apenas uiva diante de outro lobo, outrora outro lobo uivara.";
    printf("Vezes que aparece: %d", stringCount(string, "Lobo", 0));


}

void newAccount(char *user, char *email, char *pass) {
    Account account;
    strcpy(account.username, user);
    strcpy(account.email, email);
    strcpy(account.password, pass);

    printf("user: %s\n", account.username);
    printf("email: %s\n", account.email);
    printf("senha: %s\n", account.password);
}

int checkEmail(char *email) {
    // Validar um email
    char *domains[] = {"@gmail.com", "@outlook.com", "@hotmail.com", "@yahoo.com"};
    
    if(stringCount(email, "@", 1) != 1) {
        return 0;
    
    }
}

int stringCount(char *string, char *substring, int lower) {
    int count = 0;

    for(int i = 0, j = 0; i < strlen(string); i++) {
        char strChar = string[i];
        char subChar = substring[j]; 
        
        if(lower == 1) {
            strChar = tolower(string[i]);
            subChar = tolower(substring[j]);
        }
    
        if(strChar == subChar)
            j++;
        else
            j = 0;
        if(j == strlen(substring)) {
            j = 0;
            count++;
        }
    }
    return count;
}