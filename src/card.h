#ifndef CARD_H
#define CARD_H
#include <stdbool.h>

typedef struct
{
    char name[1024];
    int number_of_card;
    int number_of_upgrade_card;
    bool is_exhaust;
}Card;

void initial_card(Card *card, char *name);



#endif