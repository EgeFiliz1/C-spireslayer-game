#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "player.h"
#include "card.h"


void initial_player(Player *player){
    player ->gold = 0;
    player ->hp = 80;
    player ->max_hp = 80;
    player ->floor = 0;
    player ->current_room = "None";
    player ->card_capacity = 16;
    player ->card_list = (Card *) malloc (player -> card_capacity * sizeof(Card));
    player ->card_count =0; //listenin sırasını tutacak
    player ->total_card_count =0;
    player ->total_upgrade_card_count =0;
    player ->relic_capacity =16;
    player ->relic_count =0; //relic listesinin sırasını tutacak
    player ->relic_list = (char **) malloc (player -> relic_capacity * sizeof(char *));
    player ->potion_list = (char **) malloc (3*sizeof(char *));
    player -> potion_count = 0;
    
}

void add_gold(Player *player, int gold){
    player ->gold += gold;
    printf("Gold obtained\n");
}

void update_max_hp(Player *player, int new_value){
    player ->max_hp += new_value;
    printf("Max health increased to %d\n", player ->max_hp);
}

void add_card(Player *player, char *name){
    int select = location_added_card(player, name);
    if(select >= 0){
        player ->card_list[select].number_of_card += 1;
        player ->total_card_count += 1;
        player->card_count += 1;
        printf("Card added: %s\n", name);
    }else{
        if(player -> card_capacity <= player ->card_count){
            player ->card_capacity *= 2;
            player -> card_list = (Card *) realloc(player->card_list,player->card_capacity * sizeof(Card));
        }
        Card new_card;
        initial_card(&new_card, name); 
        
        player -> card_list[player ->card_count] = new_card;
        player ->total_card_count += 1;
        player->card_count += 1;
        printf("Card added: %s\n", name);

    }

}

bool add_relic(Player *player, char *name, int size){
    if(is_relic_added(player,name)){
        printf("Already has relic: %s\n",name);
        return false;
    }else{
        if(player ->relic_capacity <= player ->card_count){
            player -> relic_capacity *= 2;
            player ->relic_list = (char **) realloc(player->relic_list, player->relic_capacity * sizeof(char*));
        }

        player ->relic_list[player->relic_count] = (char*) malloc(size*sizeof(char)+1);
        strncpy(player->relic_list[player->relic_count], name, size+1);
        player->relic_count += 1;
        printf("Relic obtained: %s\n", name);
        return true;
    }


}


bool add_potion(Player *player, char *name, int size){
    if(player->potion_count >=3){
        printf("Potion belt is full\n");
        return false;
    }else{
        player ->potion_list[player->potion_count] = (char *) malloc (size*sizeof(char)+1);
        strncpy(player->potion_list[player->potion_count], name,size+1);
        player->potion_count += 1;
        printf("Potion obtained: %s\n",name);
        return true;
    }
}

void buy_card(Player *player, char *name, int price){
    
    if(price > player ->gold){
        printf("Not enough gold\n");
    }else{
        add_card(player,name);
        player->gold -= price;
    }

}

void buy_relic(Player *player, char *name, int price,int size){
    if(price > player ->gold){
        printf("Not enough gold\n");
    }else{
        if(add_relic(player,name,size)){
            player->gold -= price;
        }

    }
}
void buy_potion(Player *player, char *name, int price,int size){
    if(price > player ->gold){
        printf("Not enough gold\n");
    }else{
        if(add_potion(player,name,size)){
            player->gold -= price;
        }

    }
}

void upgrade_card(Player *player, char *card_name){
    int select = location_added_card(player,card_name);
    if(select <0 || player->card_list[select].number_of_card ==0){
        printf("Card not found: %s\n",card_name);
    }else{
        player->card_list[select].number_of_card -=1;
        player->card_list[select].number_of_upgrade_card +=1;
        player->total_upgrade_card_count += 1;
        printf("Card upgraded: %s\n",card_name);
        printf("%d\n",player->card_list[select].number_of_card);
    }
}

void enter_room(Player *player, char *room_name){
    player->current_room = room_name;
    player->floor += 1;
    printf("Entered %s room\n",room_name);
    printf("%d\n",player->floor);
}

void heal_process(Player *player, int heal){
    player -> hp += heal;
    if(player-> hp > player->max_hp){
        player->hp = player->max_hp;
    }

    printf("Ironclad heals to %d\n",player->hp);

}

void take_process(Player *player, int damage){
    player->hp -= damage;

    if(player->hp <0){
        player->hp = 0;
    }

    printf("Ironclad health drops to %d\n", player->hp);
}

int location_added_card(Player *player, char *card_name){
    
    for(int i=0; i<player->card_count; i++){
        if(strcmp(card_name, player ->card_list[i].name) == 0){
            return i;
        }

    }
    return -1;
}

bool is_relic_added(Player *player, char *relic_name){

    for(int i = 0; i < player->relic_count; i++){

        if(strcmp(relic_name, player->relic_list[i]) == 0){
            return true;
        }
    }
    return false;

}