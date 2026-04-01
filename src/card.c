#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "card.h"


void initial_card(Card *card, char *name){
    strncpy(card->name, name, sizeof(card->name)-1);
    card->name[sizeof(card->name)-1] = '\0';
    card ->number_of_card =1;
    card ->number_of_upgrade_card = 0;
    card ->is_exhaust = false;

}