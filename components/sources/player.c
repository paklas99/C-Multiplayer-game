#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../interfaces/player.h"
#include "../interfaces/textures.h"
#include "../interfaces/game.h"


#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 48
#define SPRITE_QTY 21

struct Player_type{
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect* rectangle;
    SDL_Rect sprites[SPRITE_QTY];
    int speed;
    int frame;
    int points;
    int has_coin_index;     // is -1 if player doesnt have a coin. Otherwise its the index of the picked coin.
    int has_boost;
    int punched;            // If I punched somebody "punched" becomes the index of the punched player.
    int got_hit;            // If I got punched, my "got_hit" becomes 1, otherwise -1.

}Player_type;

PUBLIC Player new_Player(SDL_Renderer* renderer, SDL_Texture* texture, int starting_x, int starting_y, int speed){
    printf("\n---PLAYER---\n");
    Player player = malloc(sizeof(struct Player_type));
    player->renderer=renderer;
    player->texture=texture;
    player->rectangle=create_rectangle(player->texture,starting_x,starting_y);
    define_sprites(player);
    player->rectangle->w=PLAYER_WIDTH;
    player->rectangle->h=PLAYER_HEIGHT;
    player->speed=speed*1000/FPS;
    player->frame=7;
    player->points=0;
    player->has_coin_index=-1;
    player->punched=-1;
    player->got_hit=0;
    return player;
}

PUBLIC SDL_Rect* get_player_rectangle(Player player){
    return player->rectangle;
}
PUBLIC int get_player_punched(Player player){
    return player->punched;
}
PUBLIC int get_player_got_hit(Player player){
    return player->got_hit;
}
PUBLIC int get_player_has_coin(Player player){
    return player->has_coin_index;
}
PUBLIC int get_player_height(Player player){
    return player->rectangle->h;
}
PUBLIC int get_player_width(Player player){
    return player->rectangle->w;
}
PUBLIC int get_player_position_x(Player player){
    return player->rectangle->x;
}
PUBLIC int get_player_position_y(Player player){
    return player->rectangle->y;
}
PUBLIC int get_player_speed(Player player){
    return player->speed;
}
PUBLIC void set_player_speed(Player player, int new_speed){
    player->speed=new_speed;
}
PUBLIC void set_player_position_x(Player player, int position_x){
    player->rectangle->x=position_x;
}
PUBLIC void set_player_position_y(Player player, int position_y){
    player->rectangle->y=position_y;
}
PUBLIC void set_player_position(Player player, int position_x, int position_y){
    player->rectangle->x=position_x;
    player->rectangle->y=position_y;
}
PUBLIC void set_player_got_hit(Player player, int got_hit){
    player->got_hit=got_hit;
}
PUBLIC void set_player_punched(Player player, int punched){
    player->punched=punched;
}
PUBLIC int get_frame(Player player){
    return player->frame;
}
PUBLIC void set_frame(Player player, int f){
    player->frame=f;
}
PUBLIC int get_has_coin_index(Player player){
    return player->has_coin_index;
}
PUBLIC void set_has_coin_index(Player player, int h){
    player->has_coin_index=h;
}
PUBLIC int get_has_boost(Player player){
    return player->has_boost;
}
PUBLIC int set_has_boost(Player player,int h){
    player->has_boost=h;
}
PUBLIC void render_player(Player player){
    SDL_RenderCopy(player->renderer,player->texture,&player->sprites[player->frame],player->rectangle);
}
PUBLIC int get_player_points(Player player){
    return player->points;
}
PUBLIC void set_player_points(Player player, int points){
    player->points=points;
}
 // converts player->points from int to string and returns that string
PUBLIC char * convert_player_points(Player player){
    static char buffer[20];
    sprintf(buffer,"%d",player->points);
    return buffer;
}
PUBLIC void increment_player_points_1(Player player){
    (player->points)++;
}
PUBLIC void increment_player_points_2(Player player){
    (player->points)+=2;
}
PUBLIC void increment_player_points_3(Player player){
    (player->points)+=3;
}
PRIVATE void define_one_sprite(Player player, int frame_number, int x, int y){
    int sprites_w, sprites_h;
    SDL_QueryTexture(player->texture, NULL, NULL, &sprites_w, &sprites_h);
    player->sprites[frame_number].x=x;
    player->sprites[frame_number].y=y;
    player->sprites[frame_number].h=sprites_h/3;
    player->sprites[frame_number].w=sprites_w/7;
}
PRIVATE void define_sprites(Player player){
    define_one_sprite(player, 0,0,0);
    define_one_sprite(player, 1,0,64);
    define_one_sprite(player, 2,0,128);
    define_one_sprite(player, 3,32,0);
    define_one_sprite(player, 4,32,64);
    define_one_sprite(player, 5,32,128);
    define_one_sprite(player, 6,64,0);
    define_one_sprite(player, 7,64,64);
    define_one_sprite(player, 8,64,128);
    define_one_sprite(player, 9,96,0);
    define_one_sprite(player, 10,96,64);
    define_one_sprite(player, 11,96,128);
    define_one_sprite(player, 12,128,0);
    define_one_sprite(player, 13,128,64);
    define_one_sprite(player, 14,128,128);
    define_one_sprite(player, 15,160,0);
    define_one_sprite(player, 16,160,64);
    define_one_sprite(player, 17,160,128);
    define_one_sprite(player, 18,192,0);
    define_one_sprite(player, 19,192,64);
    define_one_sprite(player, 20,192,128);
}
