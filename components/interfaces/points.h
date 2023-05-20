#ifndef points_h
#define points_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../interfaces/text.h"

#define WIN_CONDITION 30


PUBLIC bool win_condition(SDL_Renderer* renderer, int score, int player_id);
PUBLIC int check_points(int coin_index, Interactable_items coin, Player player1, Player player2, Player player3, Player player4, int *winning_coin);

#endif // points_h