#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "player.h"
#include "codex.h"

#define NUMBER_OF_RESERVED_KEYWORDS 40
#define NUMBER_OF_ROOM 7

const char *reserved_keywords[] = {
    "Ironclad", "gains", "gold", "max", "hp", "card", "relic", "potion",
    "buys", "for", "removes", "upgraded", "upgrades", "enters", "room",
    "learns", "is", "effective", "against", "fights", "heals", "takes",
    "damage", "discards", "sells", "marks", "as", "exhaust", "Total",
    "Floor", "Where", "Deck", "size", "Relics", "Potions", "What",
    "Defeated", "Health", "Exhausts", "Exit"};

const char *room_name[] = {"Monster", "Elite", "Rest", "Shop", "Treasure", "Event", "Boss"};
int price = 0;

void delete_space_from_right(char *str)
{
    int lenght = strlen(str);
    int i = lenght - 1;

    while (i >= 0 && str[i] == ' ')
    {
        i--;
    }
    
    str[i + 1] = '\0';
}
void delete_space_from_left(char *str)
{
    int i = 0;
    while (str[i] == ' ')
    {
        i++;
    }
    if (i > 0)
    {
        memmove(str, str + i, strlen(str + i) + 1);
    }
}
bool is_valid_int(char *int_str)
{
    if (int_str[0] == '0')
    {
        return false;
    }

    for (int i = 0; int_str[i] != '\0'; i++)
    {
        if (!isdigit(int_str[i]))
        {
            return false;
        }
    }
    return true;
}
bool for_n_good_control(char *second_sentence)
{
    char *temp = strtok(second_sentence, " \n");
    if (temp == NULL)
    {
        printf("INVALID\n");
    }
    else if (strcmp(temp, "for") == 0)
    {
        temp = strtok(NULL, " \n");
        if (temp == NULL)
        {
            printf("INVALID\n");
        }
        else if (is_valid_int(temp))
        {
            int value = atoi(temp);
            price = value;
            temp = strtok(NULL, "\n");
            if (temp != NULL)
            {
                delete_space_from_right(temp);
            }
            if (temp == NULL)
            {
                printf("INVALID\n");
            }
            else if (strcmp(temp, "gold") == 0)
            {
                temp = strtok(NULL, "\n");
                if (temp == NULL)
                {
                    return true;
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return false;
}
bool name_control(char *name)
{
    // for döngüsü içinde hem alfabetik olması hem de birden fazla boşluğun yan yana olması kontrol edildi.
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (isspace(name[i]) && isspace(name[i + 1]))
        {
            printf("INVALID\n");
            return false;
        }
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            printf("INVALID\n");
            return false;
        }
    }
    char *divided_name;
    // name boşluklarından bölünecek ve reserved keywords içerip içermediğine bakılacak
    divided_name = strtok(name, " \0");
    while (divided_name != NULL)
    {
        for (int i = 0; i < NUMBER_OF_RESERVED_KEYWORDS; i++)
        {
            if (strcmp(divided_name, reserved_keywords[i]) == 0)
            {
                printf("INVALID\n");
                return false;
            }
        }
        divided_name = strtok(NULL, " \0");
    }
    return true;
}
bool drawing_name_at_end(char *name, char *part, int size)
{
    delete_space_from_right(part);
    delete_space_from_left(part);
    strncpy(name, part, size - 1);
    name[size - 1] = '\0';
    return name_control(part);
}
bool sentence_fragmentation(char *main_part, const char *target, char *first_sentence, char *second_sentence)
{
    char *second_start = strstr(main_part, target);
    if (second_start == NULL)
    {
        return false;
    }
    else
    {
        int lenght = second_start - main_part;

        strncpy(first_sentence, main_part, lenght);
        first_sentence[lenght] = '\0';
        strcpy(second_sentence, second_start);
        return true;
    }
}
bool learn_sentence_process(char *main_part, char *e_name, int size)
{

    char *temp = strtok(main_part, " \n");
    temp = strtok(NULL, " \n");

    if (temp == NULL)
    {
        printf("INVALID\n");
        return false;
    }
    else if (strcmp(temp, "effective") == 0)
    {
        temp = strtok(NULL, " \n");
        if (temp == NULL)
        {
            printf("INVALID\n");
            return false;
        }
        else if (strcmp(temp, "against") == 0)
        {
            temp = strtok(NULL, "\n");
            if (temp == NULL)
            {
                printf("INVALID\n");
                return false;
            }
            else
            {
                return drawing_name_at_end(e_name, temp, size);
            }
        }
        else
        {
            printf("INVALID\n");
            return false;
        }
    }
    else
    {
        printf("INVALID\n");
        return false;
    }
}

bool buy_process(char *main_part, char *first_sentence, char *second_sentence, char *name, int size)
{
    if (sentence_fragmentation(main_part, " for ", first_sentence, second_sentence))
    {
        if (drawing_name_at_end(name, first_sentence, size))
        {
            if (for_n_good_control(second_sentence))
            {
                return true;
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return false;
}
int control_card_relic_potion(char *main_part, char *name, int size)
{
    char *temp = strtok(main_part, " ");
    char *temp2 = strtok(NULL, "\0");
    if (temp2 != NULL)
    {
        if (drawing_name_at_end(name, temp2, size))
        {

            if (strcmp(temp, "card") == 0)
            {
                return 1;
            }
            else if (strcmp(temp, "relic") == 0)
            {
                return 2;
            }
            else if (strcmp(temp, "potion") == 0)
            {
                return 3;
            }
            else
            {
                printf("INVALID\n");
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
}
bool room_control(char *room)
{
    delete_space_from_right(room);
    delete_space_from_left(room);
    for (int i = 0; i < NUMBER_OF_ROOM; i++)
    {
        if (strcmp(room, room_name[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
int main()
{
    Player player;
    initial_player(&player);
    Codex codex;
    initial_codex(&codex);
    char text[1024];
    char *part;

    printf(">> ");
    fflush(stdout);

    while (fgets(text, sizeof(text), stdin))
    {   
        printf("» ");
        fflush(stdout);
        delete_space_from_left(text);
        delete_space_from_right(text);
        /* if(strcmp(text, "Exit") != 0){
            printf(">> ");
        } */
        price = 0;
        part = strtok(text, " \n");
        char before_part[1024] = "";
        char after_name[1024] = "";
        

        if (strcmp(part, "Ironclad") == 0)
        {
            // Ironclad ile başlayan kısım
            part = strtok(NULL, " \n");
            if (part == NULL)
            {
                printf("INVALID\n");
            }
            // gains buys tamam
            else if (strcmp(part, "gains") == 0)
            {
                part = strtok(NULL, " \n");

                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";

                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            add_card(&player, name);
                        }
                    }
                }
                else if (strcmp(part, "relic") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            add_relic(&player, name);
                        }
                    }
                }
                else if (strcmp(part, "potion") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            add_potion(&player, name);
                        }
                    }
                }
                else
                {
                    if (is_valid_int(part))
                    {
                        // Geçerli bir sayıysa
                        int value = atoi(part);
                        part = strtok(NULL, " \n");
                        if (part == NULL)
                        {
                            printf("INVALID\n");
                        }
                        else if (strcmp(part, "gold") == 0)
                        {
                            part = strtok(NULL, " \n");
                            if (part == NULL)
                            {
                                add_gold(&player, value);
                            }
                            else
                            {
                                printf("INVALID\n");
                            }
                        }
                        else if (strcmp(part, "max") == 0)
                        {
                            part = strtok(NULL, " \n");
                            if (part == NULL)
                            {
                                printf("INVALID\n");
                            }
                            else if (strcmp(part, "hp") == 0)
                            {
                                part = strtok(NULL, " \n");
                                if (part == NULL)
                                {
                                    update_max_hp(&player, value);
                                }
                                else
                                {
                                    printf("INVALID\n");
                                }
                            }
                            else
                            {
                                printf("INVALID\n");
                            }
                        }
                        else
                        {
                            // sayıdan sonra geçersiz bir ifade gelirse
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
            }
            else if (strcmp(part, "buys") == 0)
            {
                part = strtok(NULL, " \n");

                char name[1024] = "";

                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (buy_process(part, before_part, after_name, name, sizeof(name)))
                        {
                            buy_card(&player, name, price);
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else if (strcmp(part, "relic") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (buy_process(part, before_part, after_name, name, sizeof(name)))
                        {
                            buy_relic(&player, name, price);
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else if (strcmp(part, "potion") == 0)
                {

                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (buy_process(part, before_part, after_name, name, sizeof(name)))
                        {
                            buy_potion(&player, name, price);
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            // removes tamam
            else if (strcmp(part, "removes") == 0)
            {
                part = strtok(NULL, " \n");
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            remove_card(&player,name);
                        }
                    }
                }
                else if (strcmp(part, "upgraded") == 0)
                {
                    part = strtok(NULL, " \n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else if (strcmp(part, "card") == 0)
                    {
                        part = strtok(NULL, "\n");
                        if (part == NULL)
                        {
                            printf("INVALID\n");
                        }
                        else
                        {
                            char name[1024] = "";
                            if (drawing_name_at_end(name, part, sizeof(name)))
                            {
                                remove_upgrade_card(&player,name);
                            }
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            // upgrades içeriği parçalandı işlemlere hazır
            else if (strcmp(part, "upgrades") == 0)
            {
                part = strtok(NULL, " \n");

                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            upgrade_card(&player, name);
                        }
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (strcmp(part, "enters") == 0)
            {
                part = strtok(NULL, "\n");
                if (part != NULL)
                { // NULL kontrolü hayat kurtarır
                    if (sentence_fragmentation(part, "room", before_part, after_name))
                    {
                        if (room_control(before_part))
                        {
                            delete_space_from_right(after_name);
                            delete_space_from_left(after_name);
                            if (strcmp(after_name, "room") == 0)
                            {
                                enter_room(&player, before_part);
                            }
                            else
                            {
                                printf("INVALID\n");
                            }
                        }
                        else
                        {
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (strcmp(part, "learns") == 0)
            {
                int select = 0;
                char first_name[1024] = "";
                char enemy_name[1024] = "";
                part = strtok(NULL, "\n");
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else
                {
                    delete_space_from_left(part);
                    if (sentence_fragmentation(part, " is ", before_part, after_name))
                    {
                        select = control_card_relic_potion(before_part, first_name, sizeof(first_name));
                        if (select != 0)
                        {
                            if (learn_sentence_process(after_name, enemy_name, sizeof(enemy_name)))
                            {
                                switch (select)
                                {
                                case 1:
                                    learn_card(&codex,enemy_name,first_name);
                                    break;
                                case 2:
                                    learn_relic(&codex, enemy_name,first_name);
                                    break;
                                case 3:
                                    learn_potion(&codex, enemy_name, first_name);
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
            }
            else if (strcmp(part, "fights") == 0)
            {
                part = strtok(NULL, "\n");
                char name[1024] = "";
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else
                {
                    if (sentence_fragmentation(part, " for ", before_part, after_name))
                    {

                        if (drawing_name_at_end(name, before_part, sizeof(name)) && for_n_good_control(after_name))
                        {
                            fight_for_gold(&player,&codex,name,price);
                        }
                    }
                    else
                    {
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            fight_without_gold( &player, &codex, name);
                        }
                    }
                }
            }
            // heals içeriği parçalandı işlemlere hazır
            else if (strcmp(part, "heals") == 0)
            {
                part = strtok(NULL, " \n");
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else
                {
                    if (is_valid_int(part))
                    {
                        // Geçerli bir sayıysa
                        int value = atoi(part);
                        part = strtok(NULL, " \n");
                        if (part == NULL)
                        {
                            printf("INVALID\n");
                        }
                        else if (strcmp(part, "hp") == 0)
                        {
                            part = strtok(NULL, " \n");
                            if (part == NULL)
                            {
                                heal_process(&player, value);
                            }
                            else
                            {
                                printf("INVALID\n");
                            }
                        }
                        else
                        {
                            // sayıdan sonra geçersiz bir ifade gelirse
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        // Hatalı bir sayıysa veya sayı dışında istenmeyen bir girdiyse
                        printf("INVALID\n");
                    }
                }
            }
            // takes içeriği parçalandı işlemlere hazır
            else if (strcmp(part, "takes") == 0)
            {
                part = strtok(NULL, " \n");
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else
                {
                    if (is_valid_int(part))
                    {
                        // Geçerli bir sayıysa
                        int value = atoi(part);
                        part = strtok(NULL, " \n");
                        if (part == NULL)
                        {
                            printf("INVALID\n");
                        }
                        else if (strcmp(part, "damage") == 0)
                        {
                            part = strtok(NULL, " \n");
                            if (part == NULL)
                            {
                                take_process(&player, value);
                            }
                            else
                            {
                                printf("INVALID\n");
                            }
                        }
                        else
                        {
                            // sayıdan sonra geçersiz bir ifade gelirse
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        // Hatalı bir sayıysa veya sayı dışında istenmeyen bir girdiyse
                        printf("INVALID\n");
                    }
                }
            }
            // discards içeriği parçalandı işlemlere hazır
            else if (strcmp(part, "discards") == 0)
            {
                part = strtok(NULL, " \n");

                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "potion") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else
                    {
                        char name[1024] = "";
                        if (drawing_name_at_end(name, part, sizeof(name)))
                        {
                            discard_potion(&player,name,true);
                        }
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (strcmp(part, "sells") == 0)
            {
                part = strtok(NULL, " \n");
                char name[1024] = "";
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (buy_process(part, before_part, after_name, name, sizeof(name)))
                        {
                            sell_card(&player,name, price);
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else if (strcmp(part, "upgraded") == 0)
                {
                    part = strtok(NULL, " \n");
                    if (part == NULL)
                    {
                        printf("INVALID\n");
                    }
                    else if (strcmp(part, "card") == 0)
                    {
                        part = strtok(NULL, "\n");
                        if (part != NULL)
                        {
                            if (buy_process(part, before_part, after_name, name, sizeof(name)))
                            {
                                sell_upgraded_card(&player, name, price);
                            }
                        }
                        else
                        {
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else if (strcmp(part, "potion") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (buy_process(part, before_part, after_name, name, sizeof(name)))
                        {
                            sell_potion(&player, name,price);
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (strcmp(part, "marks") == 0)
            {
                part = strtok(NULL, " \n");
                char name[1024] = "";
                if (part == NULL)
                {
                    printf("INVALID\n");
                }
                else if (strcmp(part, "card") == 0)
                {
                    part = strtok(NULL, "\n");
                    if (part != NULL)
                    {
                        if (sentence_fragmentation(part, " as ", before_part, after_name))
                        {
                            delete_space_from_left(before_part);
                            delete_space_from_right(before_part);
                            strncpy(name, before_part, sizeof(name));
                            name[sizeof(name) - 1] = '\0';
                            if (name_control(before_part))
                            {
                                part = strtok(after_name, " \n");
                                part = strtok(NULL, "\n");
                                if (part != NULL)
                                {
                                    delete_space_from_right(part);
                                }
                                if (part == NULL)
                                {
                                    printf("INVALID\n");
                                }
                                else if (strcmp(part, "exhaust") == 0)
                                {
                                    marks_card(&player,name);
                                }
                                else
                                {
                                    printf("INVALID\n");
                                }
                            }
                        }
                        else
                        {
                            printf("INVALID\n");
                        }
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                printf("INVALID\n");
            }
        }

        else if (strcmp(part, "Total") == 0) {
            part = strtok(NULL, " \n"); // "gold" veya "card" bekliyoruz
            
            if (part == NULL) {
                printf("INVALID\n");
            }
            // DURUM 1: Total gold ?
            else if (strcmp(part, "gold") == 0) {
                part = strtok(NULL, " \n"); // "?" bekliyoruz
                if (part != NULL && strcmp(part, "?") == 0) {
                    if (strtok(NULL, " \n") == NULL) {
                        printf("%d\n", player.gold);
                    } else printf("INVALID\n");
                } else printf("INVALID\n");
            }
            // 2. Total upgraded card <name> ?
            else if (strcmp(part, "upgraded") == 0) {
                part = strtok(NULL, " \n"); // "card" bekliyoruz
                if (part != NULL && strcmp(part, "card") == 0) {
                    char *rest = strtok(NULL, "\n");
                    if (rest != NULL) {
                        delete_space_from_left(rest);
                        delete_space_from_right(rest);
                        int len = strlen(rest);
                        if (len > 2 && rest[len-1] == '?' && rest[len-2] == ' ') {
                            rest[len-2] = '\0';
                            delete_space_from_right(rest);
                            // SADECE UPGRADED SAYISINI BASIYORUZ
                            if(name_control(rest)){
                                printf("%d\n", get_total_upgraded_card_count_by_name(&player, rest));
                            }
                        } else printf("INVALID\n");
                    } else printf("INVALID\n");
                } else printf("INVALID\n");
            }
            // DURUM 2: Total card <card_name> ?
            else if (strcmp(part, "card") == 0) {
                // Satırın geri kalanını (" Shrug It Off ?") tek parça al
                char *rest = strtok(NULL, "\n"); 
                
                if (rest == NULL) {
                    printf("INVALID\n");
                } else {
                    delete_space_from_left(rest); // Başındaki boşluğu temizle
                    delete_space_from_right(rest);
                    int len = strlen(rest);
                    
                    // Son iki karakter " ?" olmalı (Boşluk + Soru İşareti)
                    if (len > 2 && rest[len-1] == '?' && rest[len-2] == ' ') {
                        rest[len-2] = '\0'; // " ?" kısmını kestik, rest artık sadece "Shrug It Off"
                        delete_space_from_right(rest); // Varsa sondaki ekstra boşlukları temizle
                        
                        // İsim geçerli mi (reserved keyword vs.) kontrol et
                        if (name_control(rest)) {
                            printf("%d\n", get_total_card_count_by_name(&player, rest));
                        }
                    } else {
                        printf("INVALID\n"); // "?" eksikse veya bitiş hatalıysa
                    }
                }
            } else {
                printf("INVALID\n");
            }
        }


        else if (strcmp(part, "Floor") == 0) {
            part = strtok(NULL, " \n");
            if (part != NULL && strcmp(part, "?") == 0) {
                    
                if (strtok(NULL, " \n") == NULL) {
                    printf("%d\n", player.floor);
                } else {
                printf("INVALID\n");
                }
            } else {
                printf("INVALID\n");
            }
        }

        else if (strcmp(part, "Where") == 0) {
            part = strtok(NULL, " \n");
            if (part != NULL && strcmp(part, "?") == 0) {
                if (strtok(NULL, " \n") == NULL) {
                    printf("%s\n", player.current_room);
                } else {
                    printf("INVALID\n");
                }
            } else {
                printf("INVALID\n");
            }
        }

        else if (strcmp(part, "Health") == 0) {
            part = strtok(NULL, " \n");
            if (part != NULL && strcmp(part, "?") == 0) {
                if (strtok(NULL, " \n") == NULL) {
                    printf("%d/%d\n", player.hp, player.max_hp);
                } else {
                    printf("INVALID\n");
                }
            } else {
                printf("INVALID\n");
            }
        }


        else if (strcmp(part, "Relics") == 0) {
                part = strtok(NULL, " \n");
                if (part != NULL && strcmp(part, "?") == 0) {
                    if (strtok(NULL, " \n") == NULL) {
                        // player.c içinde yazacağın listeleme fonksiyonunu çağır
                        print_relics(&player); 
                    } else {
                        printf("INVALID\n");
                    }
                } else {
                    printf("INVALID\n");
                }
        }
        else if (strcmp(part, "Potions") == 0) {
            part = strtok(NULL, " \n");
            if (part != NULL && strcmp(part, "?") == 0) {
                // Soru işaretinden sonra başka bir şey olmamalı
                if (strtok(NULL, " \n") == NULL) {
                    print_potions(&player);
                } else {
                    printf("INVALID\n");
                }
            } else {
                printf("INVALID\n");
            }
        }

        else if (strcmp(part, "Deck") == 0) {
            part = strtok(NULL, " \n"); // "?" veya "size" bekliyoruz
            
            if (part == NULL) {
                printf("INVALID\n");
            }
            // 1. Durum: Deck ? (Listeleme)
            else if (strcmp(part, "?") == 0) {
                if (strtok(NULL, " \n") == NULL) {
                    print_deck(&player);
                } else {
                    printf("INVALID\n");
                }
            }
            // 2. Durum: Deck size ? (Toplam Sayı)
            else if (strcmp(part, "size") == 0) {
                part = strtok(NULL, " \n"); // "?" bekliyoruz
                if (part != NULL && strcmp(part, "?") == 0) {
                    if (strtok(NULL, " \n") == NULL) {
                        // Sadece toplam sayıyı basıyoruz
                        printf("%d\n", player.total_card_count);
                    } else {
                        printf("INVALID\n");
                    }
                } else {
                    printf("INVALID\n");
                }
            }
            else {
                printf("INVALID\n");
            }
        }

        else if (strcmp(part, "Exhausts") == 0) {
            part = strtok(NULL, "\n"); // "?" bekliyoruz
            delete_space_from_left(part);
            delete_space_from_right(part);
            if (part != NULL && strcmp(part, "?") == 0) {
                print_exhaust(&player);
            } else {
                printf("INVALID\n");
            }
        }
        else if(strcmp(part, "Defeated")==0){
            part = strtok(NULL, "\n");
            if (part!= NULL){
                delete_space_from_right(part);
                int len =strlen(part);
                if(len>2 && part[len-1] == '?' && part[len-2] == ' '){
                    part[len-2] = '\0';
                    delete_space_from_left(part);
                    delete_space_from_right(part);
                    char name[1024] = "";
                    strncpy(name,part, sizeof(name));
                    if(name_control(part)){
                        printf("%d\n", defeated_number_for_enemy(&codex, name));
                    }
                }
                else{
                    printf("INVALID\n");
                }

            }else{
                printf("INVALID\n");
            }

        }
        else if(strcmp(part, "What")==0){
            part = strtok(NULL," \n");
            if(part != NULL && strcmp(part, "is") ==0){
                part = strtok(NULL," \n");
                if(part != NULL && strcmp(part, "effective") ==0){
                    part = strtok(NULL," \n");
                    if(part != NULL && strcmp(part, "against") ==0){
                        part = strtok(NULL, "\n");
                        if (part!= NULL){
                            delete_space_from_right(part);
                            int len =strlen(part);
                            if(len>2 && part[len-1] == '?' && part[len-2] == ' '){
                                part[len-2] = '\0';
                                delete_space_from_left(part);
                                delete_space_from_right(part);
                                char name[1024] = "";
                                strncpy(name,part, sizeof(name));
                                if(name_control(part)){
                                    effective_against_enemy(&codex, name);
                                }
                            }else{
                                printf("INVALID\n");
                            }

                        }else{
                            printf("INVALID\n");
                        }
                    }else{
                        printf("INVALID\n");
                    }
                }else{
                    printf("INVALID\n");
                }
            }else{
                printf("INVALID\n");
            }

        }
        else if(strcmp(part, "Exit")==0){
            part = strtok(NULL, "\n");
            if(part == NULL){
                return 0;
            }else{
                printf("INVALID\n");
            }
            
        }
        else
        {
            printf("INVALID\n");
        }
        printf(">> ");
        fflush(stdout);
    }
}

// gelen ifadeyi ayırma noktaları gains, buys, removes, upgrades, enters, learns, fights, heals, takes, discards,sells,
// marks-