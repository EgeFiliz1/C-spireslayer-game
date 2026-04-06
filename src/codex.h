#ifndef CODEX_H
#define CODEX_H
#include <stdbool.h>
#include "enemy.h"

typedef struct
{
    Enemy *enemy_list;
    int enemy_count;
    int enemy_capacity;
}Codex;

int compare_enemy_cards(const void *a, const void *b);
int compare_enemy_relics(const void *a, const void *b);
int compare_enemy_potions(const void *a, const void *b);
int location_added_enemy(Codex *codex, char *enemy_name);
void initial_codex(Codex *codex);
void learn_card(Codex *codex, char *enemy_name, char *card_name);
int adding_enemy(Codex *codex, Enemy enemy);
void learn_relic(Codex *codex, char *enemy_name, char *relic_name);
void learn_potion(Codex *codex, char *enemy_name, char *potion_name);
int defeated_number_for_enemy(Codex *codex, char *enemy_name);
void effective_against_enemy(Codex *codex, char *enemy_name);



#endif
