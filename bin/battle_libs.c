#include "dlibs.h"

#define True 0
#define False 1

int causeDamage(int damage, float multiplicator, int precision, Dragon *enemy);
int binarySearch(int item, int vec[], int length);
int startTurn(Battle *battleInstance);
int debuffTick(Debuff *debuff, Entity *entity);
int applyDebuff(gchar *debuffType, gint turns, Entity *entity);


void setBattleVariables(Battle *battleInstance, Dragon playerEnt, Dragon enemyEnt, Player player) {
    int cooldownsVector[4] = {0, 0, 0, 0};
    battleInstance->actualTurn = 1;
    battleInstance->turnPlayed = FALSE;

    // Verifica a quantidade de xp recebida de acordo com o balanceamento
    if(playerEnt.level > enemyEnt.level)
        if(playerEnt.level - enemyEnt.level > 1)
            battleInstance->expReward = player.requiredExp * 0.5;
        if(playerEnt.level - enemyEnt.level >= 4)
            battleInstance->expReward = player.requiredExp * 0.25;
        if(playerEnt.level - enemyEnt.level >= 10)
            battleInstance->expReward = player.requiredExp * 0.1;
    else
        battleInstance->expReward = player.requiredExp * 4 + 200;
    
    // Seta os cooldowns de todos os ataques para 0 inicialmente 
    battleInstance->EntityOne.entDragon = playerEnt;
    battleInstance->EntityOne.fixedDragon = playerEnt;
    memset(battleInstance->EntityOne.skillsCooldown, 0, sizeof(battleInstance->EntityOne.skillsCooldown));
    
    battleInstance->EntityTwo.entDragon = enemyEnt;
    battleInstance->EntityTwo.fixedDragon = enemyEnt;
    memset(battleInstance->EntityTwo.skillsCooldown, 0, sizeof(battleInstance->EntityTwo.skillsCooldown));

    // Decide quem vai atacar primeiro
    if(playerEnt.speed > enemyEnt.speed) battleInstance->entityTurn = 1;
    else if(enemyEnt.speed > playerEnt.speed) battleInstance->entityTurn = 2;
    else if(playerEnt.speed == enemyEnt.speed) battleInstance->entityTurn = random_choice(1, 2);

    // Zera os buffs e debbufs de ambas as entidades
    for(int i=0; i<4; i++)  {
        strcpy(battleInstance->EntityOne.entityBuffs[i].type, "");
        battleInstance->EntityOne.entityBuffs[i].turns = 0;
        strcpy(battleInstance->EntityOne.entityDebuffs[i].type, "");
        battleInstance->EntityOne.entityDebuffs[i].turns = 0;

        strcpy(battleInstance->EntityTwo.entityBuffs[i].type, "");
        battleInstance->EntityTwo.entityBuffs[i].turns = 0;
        strcpy(battleInstance->EntityTwo.entityDebuffs[i].type, "");
        battleInstance->EntityTwo.entityDebuffs[i].turns = 0;
    }
}

int startTurn(Battle *battleInstance) {
    Battle *bI = battleInstance;
    bI->turnPlayed = FALSE;
    
    // Atualizando tempo de recarga dos ataques
    for(int i=0; i < 4; i++) {
        if(bI->EntityOne.skillsCooldown[i] > 0)
            bI->EntityOne.skillsCooldown[i]--;
        if(bI->EntityTwo.skillsCooldown[i] > 0)
            bI->EntityOne.skillsCooldown[i]--;
    }

    // Aplicando Efeitos de debuff e atualizando recarga
    for(int i; i < 10; i++) {
        debuffTick(&bI->EntityOne.entityDebuffs[i], &bI->EntityOne);
        debuffTick(&bI->EntityTwo.entityDebuffs[i], &bI->EntityTwo);
    }
    // Aplicando Efeitos de buff e atualizando recarga...

    // Em andamento
    bI->actualTurn++;


}

int causeDamage(int damage, float multiplicator, int precision, Dragon *enemy) {
    int choiceVector[precision], randomNumber, canHit = True, totalDamage = 0;
    for(int j=0; j < precision; j++) {
        choiceVector[j] = j;
    }
    randomNumber = random_choice(0, 99);
    if(binarySearch(randomNumber, choiceVector, precision) == False) {
        canHit = False;
        printf("MISS!!!\n");
        return -1;
    }
    if(canHit == True) {
        totalDamage = damage * multiplicator - enemy->defense*0.3;
    }
    printf("Totaldamage: %d\n", totalDamage);
    return totalDamage;
}

int applyDebuff(gchar *debuffType, gint turns, Entity *entity) {
    if(strlen(debuffType) == 0 || !entity)
        return -1;
    
    gint availableSlot = -1;

    for(int i=0; i < 4; i++) {
        if(strcmp(debuffType, entity->entityDebuffs[i].type) == 0) {
            availableSlot = -1;
            return -2;
        }
        if(strlen(entity->entityDebuffs->type) > 0) {
            availableSlot = i;
            break;
        }
    } 
    if(availableSlot != -1) {
        if(strcmp(debuffType, "Carbonized") == 0)
            entity->entDragon.defense -= (entity->entDragon.defense*0.2);
        if(strcmp(debuffType, "Terrified") == 0)
            entity->entDragon.attack -= (entity->entDragon.attack*0.2);
        entity->entityDebuffs[availableSlot].turns = turns;
        strcpy(entity->entityDebuffs[availableSlot].type, debuffType);
        return 0;
    }

    return 1;
}

int debuffTick(Debuff *debuff, Entity *entity) {
    if(strlen(debuff->type) == 0 || !entity)  
        return 1;

    if(strcmp(debuff->type, "Bleeding") == 0)
        entity->entDragon.health -= (entity->entDragon.health*0.05);

    debuff->turns -= 1;
    if(debuff->turns == 0) {
        if(strcmp(debuff->type, "Carbonized") == 0)
            entity->entDragon.defense = entity->fixedDragon.defense;
        
        if(strcmp(debuff->type, "Terrified") == 0)
            entity->entDragon.attack = entity->fixedDragon.attack;

        strcpy(debuff->type, "");
    }
    return 0;
}

int binarySearch(int item, int vec[], int length) {
    int start = 0, end = length-1, center = (start+end) / 2;
    while(end > start) {
        if(vec[center] == item) 
            return True;
        if(item > vec[center]) {
            start = center+1;
            center = (start+end) / 2;
        }
        else {
            end = center;
            center = (start+end) / 2;
        }
    }
    return False;
}