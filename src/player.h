#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "codex.h"
#include <stdbool.h>
#include <string.h>


typedef struct {
    char name[1024];
} Relic;

typedef struct {
    char name[1024];
} Potion;

typedef struct {
    Card *card_list;
    int card_capacity;
    int card_count; // Eşsiz kart türü sayısı (Dizi boyutu)
    int total_card_count; // Toplam kart adedi (Query cevabı için)
    int total_upgrade_card_count;
    Relic *relic_list;
    int relic_capacity;
    int relic_count;
    Potion *potion_list;
    int potion_count;
    int gold;
    int hp;
    int max_hp;
    int floor;
    char current_room[32];
    int exhaust_card;
} Player;

// Fonksiyon Prototipleri
void initial_player(Player *player);
int compare_cards(const void *a, const void *b);
int compare_relics(const void *a, const void *b);
int compare_potions(const void *a, const void *b);
void add_gold(Player *player, int gold);
void update_max_hp(Player *player, int new_value);
void add_card(Player *player, char *name);
bool add_relic(Player *player, char *name);
bool add_potion(Player *player, char *name);
void buy_card(Player *player, char *name, int price);
void buy_relic(Player *player, char *name, int price);
void buy_potion(Player *player, char *name, int price);
void upgrade_card(Player *player, char *card_name);
void enter_room(Player *player, char *room_name);
void heal_process(Player *player, int heal);
void take_process(Player *player, int damage);
void print_relics(Player *player);
void print_deck(Player *player);
void print_exhaust(Player *player);
bool is_relic_added(Player *player, char *relic_name);
int location_added_card(Player *player, char *card_name);
void print_potions(Player *player);
void remove_card(Player *player, char *card_name);
void remove_upgrade_card(Player *player, char *card_name);
bool discard_potion(Player *player, char *potion_name, bool is_print);
void sell_card(Player *player, char *card_name, int price);
void sell_upgraded_card(Player *player, char *card_name, int price);
void sell_potion(Player *player, char *potion_name, int price);
void marks_card(Player *player, char *card_name);
bool card_control_for_fight(Player *player, Codex *codex, int index);
bool potion_control_for_fight(Player *player, Codex *codex, int index);
bool relic_control_for_fight(Player *player, Codex *codex, int index);
bool fight_process(Player *player, Codex *codex, char* enemy_name);
void fight_without_gold(Player *player, Codex *codex, char *enemy_name);
void fight_for_gold(Player *player, Codex *codex, char *enemy_name, int price);
int get_total_card_count_by_name(Player *player, char *card_name);
int get_total_upgraded_card_count_by_name(Player *player, char *card_name);

#endif