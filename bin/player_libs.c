#include "dlibs.h"

Dragon getplayerDragon(FILE *pFile, char *name, char *element);
Player getPlayer(FILE *pFile);
Dragon trainplayerDragon(Dragon dragon, int lvls);

int initPlayer(FILE *pFile, Player *newPlayer);
int changePlayerStatus(FILE *pFile, int level, int points, int actualExp, int requiredExp, int progressPoints, int actualProgress, Dragon *dragon);
int addExperiencetoPlayer(FILE *pFile, int exp);

int initPlayer(FILE *pFile, Player *newPlayer) {
    int reqExp, lvl;
    FILE *experienceFile = fopen("../files/lvls_experience.txt", "r");
    fscanf(experienceFile, "%d %d", &lvl, &reqExp);
    rewind(experienceFile);
    fclose(experienceFile);
    *newPlayer = (Player){0};
    newPlayer->level = 0;
    newPlayer->actualProgress = 0;
    newPlayer->progressPoints = 0;
    newPlayer->actualExp = 0;
    newPlayer->requiredExp = reqExp;
    newPlayer->trainPoints = 0;
    //memset(&newPlayer->dragon, 0, sizeof(newPlayer->dragon));  // Reseta o dragão do player
    if(fwrite(newPlayer, sizeof(Player), 1, pFile) != 0)
        return 1;
    fflush(pFile);
    return 0;
}

int changePlayerStatus(FILE *pFile, int level, int points, int actualExp, int requiredExp, int progressPoints, int actualProgress, Dragon *dragon) {
    Player player = getPlayer(pFile);
    if(level != -1)
        player.level = level;
    if(points != -1)
        player.trainPoints = points;
    if(actualExp != -1)
        player.actualExp = actualExp;
    if(requiredExp != -1)
        player.requiredExp = requiredExp;
    if(progressPoints != -1)
        player.progressPoints = progressPoints;
    if(actualProgress != -1)
        player.actualProgress = actualProgress;
    if(dragon != NULL)
        player.dragon = *dragon;
    
    reinsFile(pFile);
    if(fwrite(&player, sizeof(Player), 1, pFile) != 1)
        return 1;
    fflush(pFile);
    rewind(pFile);
    return 0;
}

int addExperiencetoPlayer(FILE *pFile, int exp) {
    int reqExp = 0, lvl, lvlUp = 0;
    FILE *experienceFile = fopen("../files/lvls_experience.txt", "r");
    Player player = getPlayer(pFile);
    player.actualExp += exp;

    while(player.actualExp >= player.requiredExp && player.level < 100) {
        //printf("act: %d | required: %d\n", player.actualExp, player.requiredExp);
        player.actualExp -= player.requiredExp;
        player.level++;
        player.trainPoints++;
        lvlUp++;
        while(1) {
            fscanf(experienceFile, "%d %d", &lvl, &reqExp);
            if(lvl == player.level+1 || player.level == 100) 
                break; 
        }
        player.requiredExp = reqExp;
    }
    if(player.level == 100) {
        player.requiredExp = -2;
        player.actualExp = -2;
    }
    changePlayerStatus(pFile, player.level, player.trainPoints, player.actualExp, player.requiredExp, -1, -1, NULL);
    return lvlUp;
}

Player getPlayer(FILE *pFile) {
    Player player;
    rewind(pFile);
    fread(&player, sizeof(Player), 1, pFile);
    rewind(pFile);
    return player;
}

Dragon trainplayerDragon(Dragon dragon, int lvls) {
    for(int i=0; i < lvls; i++) {
        // Possível individualidade
        /*
        if(g_strcmp0(dragon.elemental, "ice") == 0) {
            dragon.health += random_choice(3, 5);
            dragon.defense += 2;
        }
        else if(g_strcmp0(dragon.elemental, "fire") == 0) {
            dragon.attack += 1;
        }
        else if(g_strcmp0(dragon.elemental, "wind") == 0) {
            dragon.speed += random_choice(1, 4);
            dragon.health += random_choice(1, 3);
        }
        */
            
        dragon.attack += random_choice(1, 3);
        dragon.defense += random_choice(1, 2);
        dragon.speed += random_choice(2, 4);
        dragon.health += random_choice(17, 23);
        dragon.level += 1;
    }
    return dragon;
}

Dragon getplayerDragon(FILE *pFile, char *name, char *element) {
    Dragon newDragon;
    Player player = getPlayer(pFile);
    if(strlen(player.dragon.name) == 0) {
        newDragon.level = 0;        
        newDragon.health = 20;
        newDragon.abs_age = 0;
        newDragon.attack = 4;
        newDragon.defense = 4;
        newDragon.speed = 4;
        strcpy(newDragon.elemental, element);
        strcpy(newDragon.name, name);
        changePlayerStatus(pFile, -1, -1, -1, -1, -1, -1, &newDragon);
    }
    else
        newDragon = player.dragon;

    g_print("Novo dragão: %s do elemento: %s adicionado.\n", newDragon.name, newDragon.elemental);
    return newDragon;
}
