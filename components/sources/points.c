#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <time.h>
#include "../interfaces/game.h"
#include "../interfaces/textures.h"
#include "../interfaces/player.h"
#include "../interfaces/interactable_items.h"
#include "../interfaces/points.h"

// Checks if coin has collided with base, increments with 1 and spawns random. Returns 0to3 based on who scored other -1
PUBLIC int check_points(int coin_index, Interactable_items coin, Player player1, Player player2, Player player3, Player player4, int *winning_coin){
    int arrayX[29] = {376, 376, 376, 465, 286, 251, 507, 507, 251, 116, 232, 518, 636, 638, 522, 232, 120, 588, 376, 167, 378, 63, 326, 430, 689, 689, 427, 332, 64};
    int arrayY[29] = {373, 292, 462, 375, 375, 248, 248, 506, 506, 233, 125, 125, 231, 522, 624, 624, 520, 374, 581, 378, 170, 327,  64, 64, 326, 422, 686, 686, 424};
    srand(time(NULL));
    int rand_num = rand()%29+0;
    if(get_item_position_x(coin)<=110 && get_item_position_y(coin)<=110){
        switch(coin_index){
            case 0: increment_player_points_3(player1); break; //bitcoin
            case 1: increment_player_points_2(player1); break; //ethereum
            case 2: increment_player_points_1(player1); break; //dogecoin
        }
        set_item_position(coin, arrayX[rand_num], arrayY[rand_num]); //set item to a new random position
        *winning_coin=coin_index; //set the coin that gave the point to the variable winning_coins
        return 0;
    }
    else if(get_item_position_x(coin)>=630 && get_item_position_y(coin)<=110){
        switch(coin_index){
            case 0: increment_player_points_3(player2); break;
            case 1: increment_player_points_2(player2); break;
            case 2: increment_player_points_1(player2); break;;
        }
        set_item_position(coin, arrayX[rand_num], arrayY[rand_num]);
        *winning_coin=coin_index;
        return 1;
    }
    else if(get_item_position_x(coin)<=110 && get_item_position_y(coin)>=630){
        switch(coin_index){
            case 0: increment_player_points_3(player3); break;
            case 1: increment_player_points_2(player3); break;
            case 2: increment_player_points_1(player3); break;
        }
        set_item_position(coin, arrayX[rand_num], arrayY[rand_num]);
        *winning_coin=coin_index;
        return 2;
    }
    else if(get_item_position_x(coin)>=630 && get_item_position_y(coin)>=630){
        switch(coin_index){
            case 0: increment_player_points_3(player4); break;
            case 1: increment_player_points_2(player4); break;
            case 2: increment_player_points_1(player4); break;
        }
        set_item_position(coin, arrayX[rand_num], arrayY[rand_num]);
        *winning_coin=coin_index;
        return 3;
    }
    return -1;
}
//checks player score for win condition and returns player id
PUBLIC bool win_condition(SDL_Renderer* renderer,int score, int player_id){
    if (score >= WIN_CONDITION){
        return true;
    }
    return false;
}