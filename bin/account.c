#include "dlibs.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

int newAccount(FILE *pFile, char *user, char *email, char *pass);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);
int containSpecialchar(char *email);

int main() {
    //char *string = "Um lobo apenas uiva diante de outro lobo, outrora outro lobo uivara.";
    //char new_email[200];
    
    FILE *accountsList = createAccountslistfile();
    //while(1) {
    //    scanf("%s", &new_email);
    //    printf("email: %s | result: %d\n\n", new_email, checkEmail(new_email));
    //}
    //printf("%d\n", newAccount(accountsList, "Sa", "Sah@gmail.com", "1234"));
    //printf("%d\n", validateAccount(accountsList, "Sa", "1234"));
    //printf("%d\n", validateAccount(accountsList, "Sa", "12345"));
    //printf("%d\n", delAccountinlist("Sa"));
    reinsFile(accountsList);
    return 0;
}

int newAccount(FILE *pFile, char *user, char *email, char *pass) {
    Account account;
    strcpy(account.username, user);
    strcpy(account.email, email);
    strcpy(account.password, pass);

    if(checkEmail(email) == 0) {
        if(overwriteAccount(pFile, email, user) == 0) {
            addAccountinlist(&account);
            createAccountfile(user);
        }
        else   
            printf("Conta já cadastrada.\n");
    }
    else
        return 1;
    /*
    printf("user: %s\n", account.username);
    printf("email: %s\n", account.email);
    printf("senha: %s\n", account.password);*/
}

int checkEmail(char *email) {
    int domainsCounts = 0;
    // Validar um email
    char *domains[] = {"@gmail.com", "@outlook.com", "@hotmail.com", "@yahoo.com"};
    char findDomain[40];

    // Verifica se tem apenas um @ no endereço de email
    if(stringCount(email, "@", 1) != 1)
        return 1;

    // Verifica o email contém pelo menos algum dos domínios da lista
    for(int i=0; i < 4; i++)
        if(stringCount(email, domains[i], 1) != 0) {
            strcpy(findDomain, domains[i]);        
            domainsCounts++;
            break;
        }

    // Caso não encontre nenhum dos domínios, o email é invalido.
    if(domainsCounts == 0 || strlen(findDomain) == strlen(email)) {
        return 1;
    }

    // Verifica se o email está terminando exatamente com o dominio.
    char *result = strstr(email, findDomain);
    int ocorrencyDomain = strstr(email, findDomain) - email;
    if (result != NULL)
        if(strlen(email) - strlen(findDomain) != ocorrencyDomain)
            return 1;

    // Verifica se o email contém caracteres especiais além do permitido.
    if(containSpecialchar(email) == 1)
        return 1; 

    return 0;
}

int stringCount(char *string, char *substring, int lower) {
    int count = 0;
    if(strlen(substring) > strlen(string)) {
        return 1;
    }
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

int containSpecialchar(char *email) {
    for(int i=0; i<strlen(email); i++) {
        //printf("char: %c | alnum: %d | está em @-+_.: %d\n", email[i], isalnum(email[i]), strchr("@-+_.", email[i]) != NULL);
        if(email[0] == '.' || (email[i] == '.' && email[i+1] == '.') || email[i] == '.' && email[i+1] == '@')
            return 1;
        if(isalnum(email[i]) == 0 && strchr("@-+_.", email[i]) == 0)
            return 1;
    }
    return 0;
}