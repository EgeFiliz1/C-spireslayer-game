#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "codex.h"
#include "enemy.h"


int location_added_enemy(Codex *codex, char *enemy_name){
    for (int i = 0; i < codex->enemy_count; i++) {
        if (strcmp(enemy_name, codex->enemy_list[i].enemy_name) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_enemy_cards(const void *a, const void *b) {
    return strcmp(((Enemy_card *)a)->name, ((Enemy_card *)b)->name);
}
int compare_enemy_relics(const void *a, const void *b) {
    return strcmp(((Enemy_relic *)a)->name, ((Enemy_relic *)b)->name);
}
int compare_enemy_potions(const void *a, const void *b) {
    return strcmp(((Enemy_potion *)a)->name, ((Enemy_potion *)b)->name);
}
void initial_codex(Codex *codex){
    codex ->enemy_capacity = 16;
    codex->enemy_count =0;
    codex->enemy_list = (Enemy *) malloc(codex ->enemy_capacity * sizeof(Enemy));
}

void learn_card(Codex *codex, char *enemy_name, char *card_name){
    int select = location_added_enemy(codex, enemy_name);

    if(select<0){
        Enemy enemy;
        initial_enemy(&enemy,enemy_name);
        int index = adding_enemy(codex, enemy );
        enemy_learn_card(&codex->enemy_list[index], card_name, true);
    }else{
        enemy_learn_card(&codex->enemy_list[select],card_name,false);
    }

}

void learn_relic(Codex *codex, char *enemy_name, char *relic_name){
    int select = location_added_enemy(codex,enemy_name);
    if(select<0){
        Enemy enemy;
        initial_enemy(&enemy,enemy_name);
        int index = adding_enemy(codex, enemy );
        enemy_learn_relic(&codex->enemy_list[index],relic_name, true);
    }else{
        enemy_learn_relic(&codex->enemy_list[select],relic_name, false);
    }

}

void learn_potion(Codex *codex, char *enemy_name, char *potion_name){
    int select = location_added_enemy(codex,enemy_name);

    if(select <0){
        Enemy enemy;
        initial_enemy(&enemy, enemy_name);
        int index = adding_enemy(codex,enemy);
        enemy_learn_potion(&codex->enemy_list[index], potion_name, true);
    }else{
        enemy_learn_potion(&codex->enemy_list[select], potion_name, false);
    }
}


int adding_enemy(Codex *codex, Enemy enemy){
    if(codex->enemy_count >= codex->enemy_capacity){
        codex->enemy_capacity *=2;
        codex->enemy_list = (Enemy *) realloc(codex->enemy_list, codex->enemy_capacity*sizeof(Enemy));
    }

    codex->enemy_list[codex->enemy_count] = enemy;
    codex->enemy_count += 1;
    return codex->enemy_count -1;

}

int defeated_number_for_enemy(Codex *codex, char *enemy_name){
    int select = location_added_enemy(codex, enemy_name);
    if(select <0){
        return 0;
    }else{
        return codex->enemy_list[select].defeats_number;
    }
}

void effective_against_enemy(Codex *codex, char *enemy_name){
    bool is_first = true;
    int select = location_added_enemy(codex,enemy_name);
    if(select<0){
        printf("No codex data for %s\n", enemy_name);
    }else{
        if(codex->enemy_list[select].card_count != 0){
            qsort(codex->enemy_list[select].card_list, codex->enemy_list[select].card_count, sizeof(Enemy_card), compare_enemy_cards);
            for(int i = 0 ; i<codex->enemy_list[select].card_count; i++){
                if(!is_first){
                    printf(", ");
                }
                printf("card %s", codex->enemy_list[select].card_list[i].name);
                is_first = false;
            }
        }

        if(codex->enemy_list[select].potion_count !=0){
            qsort(codex->enemy_list[select].potion_list, codex->enemy_list[select].potion_count, sizeof(Enemy_potion), compare_enemy_potions);
            for(int i = 0 ; i<codex->enemy_list[select].potion_count; i++){
                if(!is_first){
                    printf(", ");
                }
                printf("potion %s", codex->enemy_list[select].potion_list[i].name);
                is_first = false;
            }
            
        }

        if(codex->enemy_list[select].relic_count !=0){
            qsort(codex->enemy_list[select].relic_list, codex->enemy_list[select].relic_count, sizeof(Enemy_relic), compare_enemy_relics);

            for(int i =0; i< codex->enemy_list[select].relic_count; i++){
                if(!is_first){
                    printf(", ");
                }
                printf("relic %s", codex->enemy_list[select].relic_list[i].name);
                is_first = false;
            }
        }
    
    printf("\n");
    }
}
