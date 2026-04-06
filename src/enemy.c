#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "enemy.h"

int location_card(Enemy *enemy, char *card_name) {
    for (int i = 0; i < enemy->card_count; i++) {
        if (strcmp(card_name, enemy->card_list[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

int location_relic(Enemy *enemy, char *relic_name){
    for(int i=0; i< enemy->relic_count; i++){
        if(strcmp(relic_name, enemy->relic_list[i].name)==0){
            return i;
        }
    }
    return -1;
}

int location_potion(Enemy *enemy, char *potion_name){
    for(int i=0; i< enemy->potion_count; i++){
        if(strcmp(potion_name, enemy->potion_list[i].name) ==0){
            return i;
        }
    }
    return -1;
}


void initial_enemy(Enemy *enemy, char *name){
    strncpy(enemy->enemy_name, name, sizeof(enemy->enemy_name)-1);
    enemy->enemy_name[sizeof(enemy->enemy_name)-1] = '\0';
    
    enemy->card_count =0;
    enemy->card_capacity=8;
    enemy->card_list = (Enemy_card *) malloc(enemy->card_capacity * sizeof(Enemy_card));

    enemy->relic_count=0;
    enemy->relic_capacity =8;
    enemy->relic_list = (Enemy_relic *) malloc(enemy->relic_capacity * sizeof(Enemy_relic));

    enemy->potion_count=0;
    enemy->potion_capacity =8;
    enemy->potion_list = (Enemy_potion *) malloc(enemy->potion_capacity  * sizeof(Enemy_potion));

    enemy->defeats_number = 0;
}


void enemy_learn_card(Enemy *enemy, char *card_name, bool is_new){
    int select = location_card(enemy,card_name);
    if(select>=0){
        
        printf("Effectiveness already noted\n");
    }else{
        if(enemy->card_capacity <= enemy->card_count){
        enemy->card_capacity *=2;
        enemy->card_list = (Enemy_card *) realloc(enemy->card_list, enemy->card_capacity*sizeof(Enemy_card));
        }

        strncpy(enemy->card_list[enemy->card_count].name, card_name, 1023);
        enemy->card_list[enemy->card_count].name[1023] = '\0';
        enemy->card_count +=1;
        if(is_new){
            printf("Codex entry created: %s\n", enemy->enemy_name);
        }else{
            printf("Codex entry updated: %s\n", enemy->enemy_name);
        }

    }

}

void enemy_learn_relic(Enemy *enemy, char *relic_name, bool is_new){
    int select = location_relic(enemy, relic_name);
    
    if(select>=0){
        printf("Effectiveness already noted\n");
    }else{
        if(enemy->relic_capacity <= enemy->relic_count){
            enemy->relic_capacity *=2;
            enemy->relic_list = (Enemy_relic *) realloc(enemy->relic_list, enemy->relic_capacity * sizeof(Enemy_relic));
        }

        strncpy(enemy->relic_list[enemy->relic_count].name, relic_name, 1023);
        enemy->relic_list[enemy->relic_count].name[1023] = '\0';
        enemy->relic_count += 1;
        if(is_new){
            printf("Codex entry created: %s\n", enemy->enemy_name);
        }else{
            printf("Codex entry updated: %s\n", enemy->enemy_name);
        }

    }


}

void enemy_learn_potion(Enemy *enemy, char *potion_name, bool is_new){
    int select = location_potion(enemy, potion_name);

    if(select >=0){
        printf("Effectiveness already noted\n");
    }else{
        if(enemy->potion_capacity <= enemy->potion_count){
            enemy->potion_capacity *=2;
            enemy->potion_list = (Enemy_potion *) realloc(enemy->potion_list, enemy->potion_capacity * sizeof(Enemy_potion));
        }
        
        strncpy(enemy->potion_list[enemy->potion_count].name, potion_name, 1023);
        enemy->potion_list[enemy->potion_count].name[1023] = '\0';
        enemy->potion_count += 1;
        if(is_new){
            printf("Codex entry created: %s\n", enemy->enemy_name);
        }else{
            printf("Codex entry updated: %s\n", enemy->enemy_name);
        }

    }
}