#include "dlibs.h"

Dragon getplayerDragon(FILE *pFile, char *name);
Player getPlayer(FILE *pFile);
Dragon trainplayerDragon(Dragon dragon, int lvls);
int random_choice(int min, int max);

int initPlayer(FILE *pFile, Player *newPlayer);
int changePlayerStatus(FILE *pFile, int level, int points, int actualExp, int requiredExp, int progress, Dragon *dragon);
int addExperiencetoPlayer(FILE *pFile, int exp);

int initPlayer(FILE *pFile, Player *newPlayer) {
    int reqExp, lvl;
    FILE *experienceFile = fopen("../files/lvls_experience.txt", "r");
    fscanf(experienceFile, "%d %d", &lvl, &reqExp);
    rewind(experienceFile);
    fclose(experienceFile);

    newPlayer->level = 0;
    newPlayer->progress = 0;
    newPlayer->actualExp = 0;
    newPlayer->requiredExp = reqExp;
    newPlayer->trainPoints = 0;
    if(fwrite(newPlayer, sizeof(Player), 1, pFile) != 0)
        return 1;
    fflush(pFile);
    return 0;
}

int changePlayerStatus(FILE *pFile, int level, int points, int actualExp, int requiredExp, int progress, Dragon *dragon) {
    Player player = getPlayer(pFile);
    if(level != -1)
        player.level = level;
    if(points != -1)
        player.trainPoints = points;
    if(actualExp != -1)
        player.actualExp = actualExp;
    if(requiredExp != -1)
        player.requiredExp = requiredExp;
    if(progress != -1)
        player.progress = progress;
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
    changePlayerStatus(pFile, player.level, player.trainPoints, player.actualExp, player.requiredExp, -1, NULL);
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
        dragon.attack += random_choice(1, 3);
        dragon.defense += random_choice(1, 2);
        dragon.speed += random_choice(2, 4);
        dragon.health += random_choice(17, 23);
        dragon.level += 1;
    }
    return dragon;
}

Dragon getplayerDragon(FILE *pFile, char *name) {
    Dragon newDragon;
    newDragon.level = 0;        
    newDragon.health = 20;
    newDragon.abs_age = 0;
    newDragon.attack = 4;
    newDragon.defense = 4;
    newDragon.speed = 4;
    strcpy(newDragon.name, name);
    changePlayerStatus(pFile, 0, 0, 0, 0, 0, &newDragon);
    return newDragon;
}