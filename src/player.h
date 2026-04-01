#ifndef PLAYER_H
#define PLAYER_H
#include "card.h"

typedef struct 
{
    Card *card_list;
    int card_capacity;
    int card_count;
    int total_card_count;
    int total_upgrade_card_count;
    char **relic_list;
    int relic_capacity;
    int relic_count;
    char **potion_list;
    int potion_count;
    int gold;
    int hp;
    int max_hp;
    int floor;
    char *current_room;
}Player;

void initial_player(Player *player);
void add_gold(Player *player, int gold);
void update_max_hp(Player *player, int new_value);
void add_card(Player *player, char *name);
bool is_added_card(char *card_name);

int location_added_card(Player *player, char *card_name);
bool add_relic(Player *player, char *name, int size);
bool is_relic_added(Player *player, char *relic_name);
bool add_potion(Player *player, char *name, int size);
void buy_card(Player *player, char *name, int price);
void buy_relic(Player *player, char *name, int price,int size);
void buy_potion(Player *player, char *name, int price,int size);
void upgrade_card(Player *player, char *card_name);
void enter_room(Player *player, char *room_name);
void heal_process(Player *player, int heal);
void take_process(Player *player, int damage);
#endif