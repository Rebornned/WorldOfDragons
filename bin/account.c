#include "dlibs.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

// Compilation: gcc -o account account.c files_libs.c sorts_libs.c
// Cd bin: cd C:/Users/Amage/Desktop/Programacao/GKT_C/WorldOfDragons/bin


int newAccount(FILE *pFile, char *user, char *email, char *pass);
int stringCount(char *string, char *substring, int lower);
int checkEmail(char *email);
int containSpecialchar(char *email);
int random_choice(int min, int max);

/*
int main() {
    FILE *accountsFile = createAccountslistfile();
    FILE *beastsFile = createBeastslistfile();
    
    Dragon newDragon;
    Player newPlayer;

    char string[] = "sim";
    char inputStr[1500];
    int inputInt;
    //FILE *playerFile = getAccountfile("Saleh");
    srand(time(NULL));
    //Dragon playerDragon = getplayerDragon(playerFile, "Drogon");
    //printf("Name: %s | Attack: %d\n", playerDragon.name, playerDragon.attack);
    //printfDragonvector(newvector, beastsLength(beastsFile));
    //printf("%d\n", delAccountinlist(accountsFile, "Saleh"));
    //printf("%d | \n", newAccount(accountsFile, "Sarah", "Sah@gmail.com", "1234"));

    printf("%d\n", newAccount(accountsFile, "Rambo", "rahs@gmail.com", "1234"));
    FILE *accountFile = getAccountfile("Rambo");
    initPlayer(accountFile, &newPlayer);

    printf("%d\n", validateAccount(accountsFile, "Rambo", "1234"));
    Player actP = getPlayer(accountFile);
    printf("\nPlayer status \nlevel: %d\nactualXp: %d\nrequiredXp: %d\nprogress: %d\npoints: %d\n", 
    actP.level, actP.actualExp, actP.requiredExp, actP.progress, actP.trainPoints);

    printf("exp: %d\n", addExperiencetoPlayer(accountFile, 250000));
    actP = getPlayer(accountFile);
    printf("\nPlayer status \nlevel: %d\nactualXp: %d\nrequiredXp: %d\nprogress: %d\npoints: %d\n", 
    actP.level, actP.actualExp, actP.requiredExp, actP.progress, actP.trainPoints);

    //printf("%d\n", changePlayerStatus(playerFile, 10, 175, 150, 2, NULL));
    //newDragon = getplayerDragon(accountFile, "Meraxes");
    //newDragon = trainplayerDragon(newDragon, 8);
    //printf("%d\n", changePlayerStatus(accountFile, 10, 175, 150, 2, &newDragon));
    //printf("Lvl: %d | Health: %d | Attack: %d | Defense: %d | Speed: %d\n", newDragon.level, newDragon.health,
    //newDragon.attack, newDragon.defense, newDragon.speed);
    //actP = getPlayer(playerFile);
    //printf("\nPlayer status \nlevel: %d\nactualXp: %d\nrequiredXp: %d\nprogress: %d\n", 
    //actP.level, actP.actualExp, actP.requiredExp, actP.progress);
    //printf("%d\n", changePassword(accountsFile ,"rahs@gmail.com", "123456781", "123456781"));
    //printf("%d\n", validateAccount(accountsFile, "Rambo", "1234"));
    //printf("%d | \n", changePassword(accountsFile ,"Saleh@gmail.com", "12345678", "12345678"));
    //readAccountvector(accountsFile);
    //printf("%d\n", delAccountinlist("Sa"));
    //reinsFile(accountsFile);
    return 0;
}
*/
int newAccount(FILE *pFile, char *user, char *email, char *pass) {
    Account account;
    Player newPlayer;
    strcpy(account.username, user);
    strcpy(account.email, email);
    strcpy(account.password, pass);

    if(checkEmail(email) == 0) {
        if(overwriteAccount(pFile, email, user) == 0) {
            addAccountinlist(&account, pFile);
        }
        else {
            return -1;
        }
    }
    else
        return -2;
    return 0;
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

int random_choice(int min, int max) {
    return rand() % (max - min + 1) + min;
}