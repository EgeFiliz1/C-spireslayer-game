#ifndef ENEMY_H
#define ENEMY_H

#include "card.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
    char name[1024];
} Enemy_relic;

typedef struct {
    char name[1024];
} Enemy_potion;

typedef struct {
    char name[1024];
} Enemy_card;

typedef struct {
    char enemy_name[1024];
    Enemy_card *card_list;
    int card_count;
    int card_capacity;
    Enemy_relic *relic_list;
    int relic_count;
    int relic_capacity;
    Enemy_potion *potion_list;
    int potion_count;
    int potion_capacity;
    int defeats_number;
} Enemy;

void initial_enemy(Enemy *enemy, char *name);
void enemy_learn_card(Enemy *enemy, char *card_name, bool is_new);
int location_card(Enemy *enemy, char *card_name);
int location_relic(Enemy *enemy, char *relic_name);
int location_potion(Enemy *enemy, char *potion_name);
void enemy_learn_relic(Enemy *enemy, char *relic_name, bool is_new);
void enemy_learn_potion(Enemy *enemy, char *potion_name, bool is_new);


#endif
