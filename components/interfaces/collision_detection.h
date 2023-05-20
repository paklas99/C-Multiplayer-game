#ifndef collision_detection_h
#define collision_detection_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../interfaces/object.h"

typedef struct Player_type *Player;
typedef struct Position_values *Position;
typedef struct Interactable_items_type *Interactable_items;

PUBLIC int check_player_collision_Y(Player player_1, Player player_2, int y_pos, int y_vel, int y_vel_old);
PUBLIC int check_player_collision_X(Player player_1, Player player_2, int x_pos, int x_vel, int x_vel_old);
PUBLIC int check_map_collision_Y(Player player, int y_pos, int y_vel);
PUBLIC int check_map_collision_X(Player player, int x_pos, int x_vel);
PUBLIC int check_wall_collision_Y(Player player, Object wall, int y_pos, int y_vel, int y_vel_old);
PUBLIC int check_wall_collision_X(Player player, Object wall, int x_pos, int x_vel, int x_vel_old);
PUBLIC bool check_boost_collision(Player player_1, Object boost);
PUBLIC void check_collision_player_coin(Player player, int coin_index, Interactable_items bitcoin, int c_x_pos, int c_y_pos, int x_pos, int y_pos, bool drop, bool *pick);
PUBLIC bool check_Collision(SDL_Rect* a, SDL_Rect* b);
PUBLIC int check_hit_collsion(Player player_1, Player player_2);

#endif // collision_detection_h