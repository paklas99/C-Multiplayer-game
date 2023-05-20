#ifndef player_h
#define player_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>

typedef struct Player_type *Player;

// PUBLIC
PUBLIC Player new_Player(SDL_Renderer* renderer, SDL_Texture* texture, int starting_x, int starting_y, int speed);
PUBLIC int get_player_position_x(Player player);
PUBLIC int get_player_position_y(Player player);
PUBLIC int get_player_height(Player player);
PUBLIC int get_player_width(Player player);
PUBLIC int get_player_speed(Player player);
PUBLIC int get_player_points(Player player);
PUBLIC void set_player_points(Player player, int points);
PUBLIC char * convert_player_points(Player player);
PUBLIC void increment_player_points_1(Player player);
PUBLIC void increment_player_points_2(Player player);
PUBLIC void increment_player_points_3(Player player);
PUBLIC SDL_Rect* get_player_rectangle(Player player);
PUBLIC void set_player_speed(Player player, int new_speed);
PUBLIC void set_player_position_x(Player player, int position_x);
PUBLIC void set_player_position_y(Player player, int position_y);
PUBLIC int get_frame(Player player);
PUBLIC void set_frame(Player player, int f);
PUBLIC int get_has_coin_index(Player player);
PUBLIC void set_has_coin_index(Player player, int h);
PUBLIC void render_player(Player player);
PUBLIC void set_player_position(Player player, int position_x, int position_y);
PUBLIC int get_player_has_coin(Player player);
PUBLIC int get_player_punched(Player player);
PUBLIC void set_player_punched(Player player, int punched);
PUBLIC int get_player_got_hit(Player player);
PUBLIC void set_player_got_hit(Player player, int got_hit);
PUBLIC int get_has_boost(Player player);
PUBLIC int set_has_boost(Player player,int h);

// PRIVATE
PRIVATE void define_sprites(Player player);
PRIVATE void define_one_sprite(Player player, int frame_number, int x, int y);
    

#endif // player_h