#include "dlibs.h"

#define True 0
#define False 1

int causeDamage(int damage, float multiplicator, int precision, Dragon *enemy);
int binarySearch(int item, int vec[], int length);

int main() {
    Dragon newdragon;
    srand(time(NULL));
    newdragon.defense = 150;
    for(int i=0; i < 10; i++) {
        causeDamage(150, 2.0, 55, &newdragon);
        sleep(1);
    }

}

int causeDamage(int damage, float multiplicator, int precision, Dragon *enemy) {
    int choiceVector[precision], randomNumber, canHit = True, totalDamage = 0;
    for(int j=0; j < precision; j++) {
        choiceVector[j] = j;
        //printf("%d ", j);
    }
    randomNumber = random_choice(0, 99);
    if(binarySearch(randomNumber, choiceVector, precision) == False) {
        canHit = False;
        printf("MISS!!!\n");
        return 1;
    }
    if(canHit == True) {
        totalDamage = damage * multiplicator - enemy->defense*0.3;
    }
    printf("Totaldamage: %d\n", totalDamage);
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