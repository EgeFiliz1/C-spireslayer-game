#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "player.h"

#define DAMAGE 15
bool real_add = true;
// --- YARDIMCI FONKSİYONLAR (Üstte olmalı) ---

int location_added_card(Player *player, char *card_name) {
    for (int i = 0; i < player->card_count; i++) {
        if (strcmp(card_name, player->card_list[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

bool is_relic_added(Player *player, char *relic_name) {
    for (int i = 0; i < player->relic_count; i++) {
        if (strcmp(relic_name, player->relic_list[i].name) == 0) {
            return true;
        }
    }
    return false;
}

int compare_cards(const void *a, const void *b) {
    return strcmp(((Card *)a)->name, ((Card *)b)->name);
}

int compare_relics(const void *a, const void *b) {
    return strcmp(((Relic *)a)->name, ((Relic *)b)->name);
}
// 1. İksirleri karşılaştırma (ASCII/strcmp)
int compare_potions(const void *a, const void *b) {
    return strcmp(((Potion *)a)->name, ((Potion *)b)->name);
}

int get_total_card_count_by_name(Player *player, char *card_name) {
    int idx = location_added_card(player, card_name);
    if (idx != -1) {
        // Ödev kuralı: Hem normal hem de + olanların toplamını döndürür
        return player->card_list[idx].number_of_card + player->card_list[idx].number_of_upgrade_card;
    }
    return 0; // Kart hiç yoksa 0
}

int get_total_upgraded_card_count_by_name(Player *player, char *card_name) {
    int idx = location_added_card(player, card_name);
    if (idx != -1) {
        // Sadece '+' almış olanların sayısını döndürür
        return player->card_list[idx].number_of_upgrade_card;
    }
    return 0;
}
// --- ANA FONKSİYONLAR ---

void initial_player(Player *player) {
    player->gold = 0;
    player->hp = 80;
    player->max_hp = 80;
    player->floor = 0;
    strcpy(player->current_room, "NONE");
    
    player->card_capacity = 16;
    player->card_count = 0;
    player->total_card_count = 0;
    player->total_upgrade_card_count = 0;
    player->card_list = (Card *) malloc(player->card_capacity * sizeof(Card));
    
    player->relic_capacity = 16;
    player->relic_count = 0;
    player->relic_list = (Relic *) malloc(player->relic_capacity * sizeof(Relic));
    
    player->potion_count = 0;
    player->potion_list = (Potion *) malloc(3 * sizeof(Potion));

    player->exhaust_card =0;
}

void add_card(Player *player, char *name) {
    int select = location_added_card(player, name);
    if (select >= 0) {
        player->card_list[select].number_of_card += 1;
        player->total_card_count += 1;
        // card_count artmaz çünkü yeni bir kart türü eklemedik
    } else {
        if (player->card_count >= player->card_capacity) {
            player->card_capacity *= 2;
            player->card_list = (Card *) realloc(player->card_list, player->card_capacity * sizeof(Card));
        }
        Card new_card;
        initial_card(&new_card, name); 
        player->card_list[player->card_count] = new_card;
        player->card_count += 1; // Yeni tür eklendi
        player->total_card_count += 1;
    }
    if(real_add){
        printf("Card added: %s\n", name);
    }
    
}

void print_deck(Player *player) {
    if (player->total_card_count == 0) {
        printf("None\n");
        return;
    }
    qsort(player->card_list, player->card_count, sizeof(Card), compare_cards);
    bool first = true;
    for (int i = 0; i < player->card_count; i++) {
        Card c = player->card_list[i];
        if (c.number_of_card > 0) {
            if (!first) printf(", ");
            printf("%d %s%s", c.number_of_card, c.name, c.is_exhaust ? "*" : "");
            first = false;
        }
        if (c.number_of_upgrade_card > 0) {
            if (!first) printf(", ");
            printf("%d %s+%s", c.number_of_upgrade_card, c.name, c.is_exhaust ? "*" : "");
            first = false;
        }
    }
    printf("\n");
}

bool add_relic(Player *player, char *name) {
    if (is_relic_added(player, name)) {
        printf("Already has relic: %s\n", name);
        return false;
    }
    if (player->relic_count >= player->relic_capacity) {
        player->relic_capacity *= 2;
        player->relic_list = (Relic *) realloc(player->relic_list, player->relic_capacity * sizeof(Relic));
    }
    strncpy(player->relic_list[player->relic_count].name, name, 1023);
    player->relic_list[player->relic_count].name[1023] = '\0';
    player->relic_count += 1;
    printf("Relic obtained: %s\n", name);
    return true;
}

bool add_potion(Player *player, char *name) {
    if (player->potion_count >= 3) {
        printf("Potion belt is full\n");
        return false;
    }
    strncpy(player->potion_list[player->potion_count].name, name, 1023);
    player->potion_list[player->potion_count].name[1023] = '\0';
    player->potion_count += 1;
    printf("Potion obtained: %s\n", name);
    return true;
}

void print_relics(Player *player) {
    if (player->relic_count == 0) {
        printf("None\n");
        return;
    }
    qsort(player->relic_list, player->relic_count, sizeof(Relic), compare_relics);
    for (int i = 0; i < player->relic_count; i++) {
        printf("%s", player->relic_list[i].name);
        if (i < player->relic_count - 1) printf(", ");
    }
    printf("\n");
}

// 2. İksirleri listeleme
void print_potions(Player *player) {
    if (player->potion_count == 0) {
        printf("None\n");
    } else {
        // Basmadan önce mutlaka sıralıyoruz
        qsort(player->potion_list, player->potion_count, sizeof(Potion), compare_potions);

        /* for (int i = 0; i < player->potion_count; i++) {
            printf("%s", player->potion_list[i].name);
            if (i < player->potion_count - 1) {
                printf(", ");
            }
        } */
        bool is_first = true;
        for(int i=0; i<player->potion_count;){
            int count = 1;
            int temp = 0;
            for(int j = i+1; j<player->potion_count; j++){
                if(strcmp(player->potion_list[i].name, player->potion_list[j].name) !=0){
                    temp = j;
                    break;
                }else{
                    count += 1;
                }

            }

            if(!is_first){
                printf(", ");
            }
            printf("%d %s", count, player->potion_list[i].name);
            is_first = false;
            if(temp ==0){
                temp = 3;
            }
            i = temp;
        }
        printf("\n");
    }
}

void print_exhaust(Player *player) {
    // bool has_exhaust = false;
    
    // Önce hiç exhaust kart var mı diye bir bakalım (None basmak için)
    //for (int i = 0; i < player->card_count; i++) {
    //    if (player->card_list[i].is_exhaust) {
    //        has_exhaust = true;
    //        break;
    //    }
    //}
    
    if (player->exhaust_card ==0) {
        printf("None\n");
        return;
    }

    // Kartları alfabetik sıraya diz (Zaten diziliyse tekrar yapmana gerek yok)
    qsort(player->card_list, player->card_count, sizeof(Card), compare_cards);

    bool first = true;
    for (int i = 0; i < player->card_count; i++) {
        Card c = player->card_list[i];
        
        // Sadece exhaust olanları basıyoruz
        if (c.is_exhaust) {
            if (!first) printf(", ");
            printf("%s", c.name);
            first = false;
            /* // Normal kopyalar
            if (c.number_of_card > 0) {
                if (!first) printf(", ");
                printf("%d %s*", c.number_of_card, c.name);
                first = false;
            }
            // Upgraded kopyalar
            if (c.number_of_upgrade_card > 0) {
                if (!first) printf(", ");
                printf("%d %s+*", c.number_of_upgrade_card, c.name);
                first = false;
            } */
        }
    }
    printf("\n");
}
// --- DİĞER FONKSİYONLAR ---

void add_gold(Player *player, int gold) {
    player->gold += gold;
    printf("Gold obtained\n");
}

void update_max_hp(Player *player, int new_value) {
    player->max_hp += new_value;
    printf("Max health increased to %d\n", player->max_hp);
}

void buy_card(Player *player, char *name, int price) {
    if (price > player->gold) printf("Not enough gold\n");
    else {
        add_card(player, name);
        player->gold -= price;
    }
}

void buy_relic(Player *player, char *name, int price) {
    if (price > player->gold) printf("Not enough gold\n");
    else if (add_relic(player, name)) player->gold -= price;
}

void buy_potion(Player *player, char *name, int price) {
    if (price > player->gold) printf("Not enough gold\n");
    else if (add_potion(player, name)) player->gold -= price;
}

void upgrade_card(Player *player, char *card_name) {
    int select = location_added_card(player, card_name);
    if (select < 0 || player->card_list[select].number_of_card == 0) {
        printf("Card not found: %s\n", card_name);
    } else {
        player->card_list[select].number_of_card -= 1;
        player->card_list[select].number_of_upgrade_card += 1;
        player->total_upgrade_card_count += 1;
        printf("Card upgraded: %s\n", card_name);
    }
}

void enter_room(Player *player, char *room_name) {
    strncpy(player->current_room, room_name, 31);
    player->current_room[31] = '\0';
    player->floor += 1; 
    printf("Entered %s room\n", room_name); 
}

void heal_process(Player *player, int heal) {
    player->hp += heal;
    if (player->hp > player->max_hp) player->hp = player->max_hp;
    printf("Ironclad heals to %d\n", player->hp);
}

void take_process(Player *player, int damage) {
    player->hp -= damage;
    if (player->hp < 0) player->hp = 0;
    printf("Ironclad health drops to %d\n", player->hp);
}

void remove_card(Player *player, char *card_name){
    int select = location_added_card(player,card_name);
    if(select <0){
        printf("Card not found: %s\n",card_name);
    }else{
        if(player->card_list[select].number_of_card<=0){
            printf("Card not found: %s\n",card_name);
        }else{
            player->card_list[select].number_of_card -=1;
            player->total_card_count -=1;
            printf("Card removed: %s\n", card_name);
        }

    }
}

bool discard_potion(Player *player, char *potion_name, bool is_print){
    int index = -1;
    for(int i=0; i<player->potion_count; i++){
        if(strcmp(potion_name, player->potion_list[i].name) == 0){
            index = i;
            break;
        }
    }

    if(index >= 0){
        for(int j= index; j<player->potion_count-1; j++){
            player->potion_list[j] = player->potion_list[j+1];
        }
        memset(&player->potion_list[player->potion_count-1], 0, sizeof(Potion));
        player->potion_count -=1;
        if(is_print){
            printf("Potion discarded: %s\n", potion_name);
        }
        return true;
        
    }else{
        if(is_print){
            printf("Potion not found: %s\n",potion_name);
        }
        return false;
    }
}

void sell_card(Player *player, char *card_name, int price){
    int select = location_added_card(player, card_name);

    if(select<0){
        printf("Card not found: %s\n", card_name);
    }else{
        if(player->card_list[select].number_of_card >0){
            printf("Card sold: %s\n",card_name);
            player->gold += price;
            player->card_list[select].number_of_card -=1;
            player->total_card_count -=1;
        }else{
            printf("Card not found: %s\n", card_name);
        }

    }
}

void sell_upgraded_card(Player *player, char *card_name, int price){
    int select = location_added_card(player, card_name);

    if(select<0){
        printf("Upgraded card not found: %s\n", card_name);
    }else{
        if(player->card_list[select].number_of_upgrade_card >0){
            printf("Upgraded card sold: %s\n",card_name);
            player->gold += price;
            player->card_list[select].number_of_upgrade_card -=1;
            player->total_card_count -=1;
            player->total_upgrade_card_count -=1;
        }else{
            printf("Upgraded card not found: %s\n", card_name);
        }

    }
}

void marks_card(Player *player, char *card_name){
    int select = location_added_card(player,card_name);

    if(select<0){
        real_add = false;
        add_card(player,card_name);
        player->card_list[player->card_count-1].number_of_card = 0;
        player->total_card_count -=1;
        player->card_list[player->card_count-1].is_exhaust = true;
        real_add = true;
        printf("Card marked as exhaust: %s\n", card_name);
        player->exhaust_card +=1;

    }else{
        if(player->card_list[select].is_exhaust){
            printf("Card already exhausts: %s\n",card_name);
        }else{
            player->card_list[select].is_exhaust = true;
            printf("Card marked as exhaust: %s\n", card_name);
            player->exhaust_card += 1;
        }
    }
}
void sell_potion(Player *player, char *potion_name, int price){
    bool is_remove = discard_potion(player,potion_name, false);
    if(is_remove){
        printf("Potion sold: %s\n", potion_name);
        player-> gold += price;
    }else{
        printf("Potion not found: %s\n", potion_name);
    }
}


void remove_upgrade_card(Player *player, char *card_name){
    int select = location_added_card(player,card_name);
    if(select <0){
        printf("Upgraded card not found: %s\n",card_name);
    }else{
        if(player->card_list[select].number_of_upgrade_card<=0){
            printf("Upgraded card not found: %s\n",card_name);
        }else{
            player->card_list[select].number_of_upgrade_card -=1;
            player->total_card_count -=1;
            player->total_upgrade_card_count -=1;
            printf("Upgraded card removed: %s\n", card_name);
        }

    }
}

bool fight_process(Player *player, Codex *codex, char* enemy_name){
    bool is_card_win = false;
    bool is_potion_win = false;
    int select = location_added_enemy(codex,enemy_name);
    if(select<0){
        return false;
    }else{
        is_card_win = card_control_for_fight(player,codex,select);
        is_potion_win = potion_control_for_fight(player,codex,select);
        if(is_card_win || is_potion_win){
            return true;
        }else{
            return relic_control_for_fight(player,codex,select);
        }
    }
}

bool card_control_for_fight(Player *player, Codex *codex, int index){
    bool return_value = false;
    if(player->card_count > 0){
        for(int i=0; i<codex->enemy_list[index].card_count; i++){
            for(int j =0; j<player->card_count;j++){
                if(strcmp(codex->enemy_list[index].card_list[i].name, player->card_list[j].name) ==0){
                    if(player->card_list[j].number_of_card >0){
                        return_value = true;
                        if(player->card_list[j].is_exhaust){
                            player->card_list[j].number_of_card -=1;
                            player->total_card_count -=1;
                        }

                    }else if(player->card_list[j].number_of_upgrade_card>0){
                        return_value = true;
                        if(player->card_list[j].is_exhaust){
                            player->card_list[j].number_of_upgrade_card -=1;
                            player->total_card_count -=1;
                            player->total_upgrade_card_count -=1;
                        }
                    }

                }
            }
        }
    }
    return return_value;
}

bool potion_control_for_fight(Player *player, Codex *codex, int index){
    bool return_value = false;
    if(player->potion_count>0){
        for(int i=0; i<codex->enemy_list[index].potion_count; i++){
            for(int j=0; j <player->potion_count; j++){
                if(strcmp(codex->enemy_list[index].potion_list[i].name, player->potion_list[j].name) ==0){
                    discard_potion(player, player->potion_list[j].name, false);
                    return_value = true;
                }
            }
        }


    }
    return return_value;

}

bool relic_control_for_fight(Player *player, Codex *codex, int index){

    for(int i=0; i<codex->enemy_list[index].relic_count;i++){
        if(is_relic_added(player, codex->enemy_list[index].relic_list[i].name)){
            return true;
        }
    }
    return false;
}

void fight_without_gold(Player *player, Codex *codex, char *enemy_name){
    
    if(fight_process(player,codex,enemy_name)){
        int select = location_added_enemy(codex,enemy_name);
        codex->enemy_list[select].defeats_number +=1;
        printf("Ironclad defeats %s\n", enemy_name);

    }else{
        if(player->hp >DAMAGE){
            player->hp = player->hp - DAMAGE;
        }else{
            player->hp = 0;
        }
        printf("Ironclad is outmatched and flees with %d hp remaining\n", player->hp);
    }

}

void fight_for_gold(Player *player, Codex *codex, char *enemy_name, int price){
    if(fight_process(player,codex,enemy_name)){
        int select = location_added_enemy(codex,enemy_name);
        codex->enemy_list[select].defeats_number += 1;
        printf("Ironclad defeats %s and gains %d gold\n", enemy_name,price);
        player->gold += price;
    }else{
        if(player->hp >DAMAGE){
            player->hp = player->hp - DAMAGE;
        }else{
            player->hp = 0;
        }
        printf("Ironclad is outmatched and flees with %d hp remaining\n", player->hp);
    }
}