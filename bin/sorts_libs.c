#include "dlibs.h"

// Sort especial, dependendo do tipo, organiza com base em diferentes tipos de atributo
Dragon * bubbleSort(int type, Dragon * vector, int length) {
    int ordened = 1, sortbyAttribute1, sortbyAttribute2;
    Dragon dragonAux, ent1, ent2;
    
    if(vector == NULL || length == 1)
        return vector;

    while(ordened == 1) {
        ordened = 0;
        for(int i=0; i < length-1; i++) {
            ent1 = vector[i];
            ent2 = vector[i+1];
            
            switch (type)
            {
            case 1:
                sortbyAttribute1 = ent1.level + ent1.abs_age + ent1.attack + ent1.defense + ent1.health + ent1.speed;
                sortbyAttribute2 = ent2.level + ent2.abs_age + ent2.attack + ent2.defense + ent2.health + ent2.speed;
                break;
            
            case 2:
                sortbyAttribute1 = ent1.abs_age;
                sortbyAttribute2 = ent2.abs_age;
                break;
            
            case 3:
                sortbyAttribute1 = ent1.attack;
                sortbyAttribute2 = ent2.attack;
                break;

            case 4:
                sortbyAttribute1 = ent1.defense;
                sortbyAttribute2 = ent2.defense;
                break;

            case 5:
                sortbyAttribute1 = ent1.speed;
                sortbyAttribute2 = ent2.speed;
                break;
            
            case 6:
                sortbyAttribute1 = ent1.health;
                sortbyAttribute2 = ent2.health;
                break;

            default:
                sortbyAttribute1 = ent1.level + ent1.abs_age + ent1.attack + ent1.defense + ent1.health + ent1.speed;
                sortbyAttribute2 = ent2.level + ent2.abs_age + ent2.attack + ent2.defense + ent2.health + ent2.speed;
                break;
            }
            
            if(sortbyAttribute1 < sortbyAttribute2) {
                ordened = 1;
                dragonAux = vector[i];
                vector[i] = vector[i+1];
                vector[i+1] = dragonAux;
            }
        }
        length--;
    }
    return vector;
}

int printfDragonvector(Dragon * vector, int length) {
    if(vector == NULL)
        return 1;

    for(int i=0; i < length; i++) {
        printf("Nome: %s | Historia: %s | Caminho: %s\n", vector[i].name, vector[i].history, vector[i].img_path);
        printf("Level: %d | Idade: %d | Ataque: %d\n", vector[i].level, vector[i].abs_age, vector[i].attack);
        printf("Defesa: %d | Velocidade: %d | Vida: %d\n", vector[i].defense, vector[i].speed, vector[i].health);
        printf("=============================================\n");
    }
    return 0;
}