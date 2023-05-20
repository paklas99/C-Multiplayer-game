#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <string.h>
#include <time.h>
#include "../interfaces/textures.h"
#include "../interfaces/player.h"
#include "../interfaces/object.h"
#include "../interfaces/interactable_items.h"
#include "../interfaces/network_UDP.h"
#include "../interfaces/map.h"

bool check_Collision(SDL_Rect* a, SDL_Rect* b);

int y_pos_old=0;
int x_pos_old=0;

// Check for Y-axis collision, set new position and return new velocity
PUBLIC int check_player_collision_Y(Player player_1, Player player_2, int y_pos, int y_vel, int y_vel_old){
    if(check_Collision(get_player_rectangle(player_1), get_player_rectangle(player_2))){
        y_pos_old=y_pos;
        y_vel = -1*y_vel;
        y_pos += y_vel / 60;
        if(y_pos==y_pos_old){
            y_vel = y_vel_old;
            y_pos += y_vel / 60;
        }
        set_player_position_y(player_1, (int) y_pos);
    }
    return y_vel;
}

// Check for X-axis collision, set new position and return new velocity
PUBLIC int check_player_collision_X(Player player_1, Player player_2, int x_pos, int x_vel, int x_vel_old){
    if(check_Collision(get_player_rectangle(player_1), get_player_rectangle(player_2))){
        x_pos_old=x_pos;
        x_vel = -1*x_vel;
        x_pos += x_vel / 60;
        if(x_pos==x_pos_old){
            x_vel = x_vel_old;
            x_pos += x_vel / 60;
        }
        set_player_position_x(player_1, (int) x_pos);
    }
    return x_vel;
}

PUBLIC int check_hit_collsion(Player player_1, Player player_2){
    if(check_Collision(get_player_rectangle(player_1), get_player_rectangle(player_2))){
        return 1;
    }
    else{
        return 0;
    }
}

PUBLIC int check_map_collision_Y(Player player, int y_pos, int y_vel){
    if(y_pos<-2){
        y_vel = -1*y_vel;
        y_pos += 3 + y_vel / 60;
        set_player_position_y(player, (int) y_pos);
    }
    if(y_pos>(get_map_height()-get_player_height(player))+2){
        y_vel = -1*y_vel;
        y_pos += -3 + y_vel / 60;
        set_player_position_y(player, (int) y_pos);
    }
    if(y_pos<=0 || y_pos>=get_map_height()-get_player_height(player)){
        y_vel = -1*y_vel;
        y_pos += y_vel / 60;
        set_player_position_y(player, (int) y_pos);
    }
    return y_vel;
}

PUBLIC int check_map_collision_X(Player player, int x_pos, int x_vel){
    if(x_pos<-2){
        x_vel = -1*x_vel;
        x_pos += 3 + x_vel / 60;
        set_player_position_x(player, (int) x_pos);
    }
    if(x_pos>(get_map_width()-get_player_width(player))+2){
        x_vel = -1*x_vel;
        x_pos += -3 + x_vel / 60;
        set_player_position_x(player, (int) x_pos);
    }
    if(x_pos<=0 || x_pos>=get_map_width()-get_player_width(player)){
        x_vel = -1*x_vel;
        x_pos += x_vel / 60;
        set_player_position_x(player, (int) x_pos);
    }
    return x_vel;
}

PUBLIC int check_wall_collision_Y(Player player, Object wall, int y_pos, int y_vel, int y_vel_old){
    if(check_Collision(get_player_rectangle(player), get_object_rectangle(wall))){
        y_pos_old=y_pos;
        y_vel = -1*y_vel;
        y_pos += y_vel / 60;
        if(y_pos==y_pos_old){
            y_vel = y_vel_old;
            y_pos += y_vel / 60;
        }
        set_player_position_y(player, (int) y_pos);
    }
    return y_vel;
}

PUBLIC int check_wall_collision_X(Player player, Object wall, int x_pos, int x_vel, int x_vel_old){
    if(check_Collision(get_player_rectangle(player), get_object_rectangle(wall))){
        x_pos_old=x_pos;
        x_vel = -1*x_vel;
        x_pos += x_vel / 60;
        if(x_pos==x_pos_old){
            x_vel = x_vel_old;
            x_pos += x_vel / 60;
        }
        set_player_position_x(player, (int) x_pos);
    }
    return x_vel;
}
//Checks if collision with boost and moves and/or updates your speed
PUBLIC bool check_boost_collision(Player player_1, Object boost){
    int arrayX[24] = {435, 335, 329, 433, 466, 308, 283, 470, 139, 131, 306, 449, 599, 599, 451, 307, 471, 491, 285,  27,  27, 275, 481, 741,};
    int arrayY[24] = {333, 333, 430, 429, 623, 636, 159, 160, 440, 445, 604, 602, 445, 323, 167, 163, 482, 737, 735, 477, 281,  27,  31, 275,};
    int speed;
    srand(time(NULL));
    int rand_num = rand()%24+0;
    if(check_Collision(get_player_rectangle(player_1), get_object(boost))){     //if collision, move boost outside map
        set_player_speed(player_1,400);
        set_object_position(boost, 1300, 800);
        return 1;
    }
    else{//if the boost is stuck, move it to a random place on the map
        if((get_player_speed(player_1) <= 230 && get_player_speed(player_1) >= 210) && (get_object_x(boost)==1300 && get_object_y(boost)==800)){
            set_object_position(boost,arrayX[rand_num],arrayY[rand_num]);
            return 1;
        }else if(get_player_speed(player_1) == 300){    //moves boost to random spot on map,random of the array, not completly random
            set_object_position(boost,arrayX[rand_num],arrayY[rand_num]);
            speed = get_player_speed(player_1);
            set_player_speed(player_1,speed-1);
            return 1;
        }else if(get_player_speed(player_1) == 200){    //if speed is normal(200), let it be normal
            return 0;
        }else{  //decrements speed from 400-200, acts as a timer/delay
            speed = get_player_speed(player_1);
            set_player_speed(player_1,speed-1);
            return 0;
        }
    }
    return 0;
}

// Check for collision between player and coin
PUBLIC void check_collision_player_coin(Player player, int coin_index, Interactable_items bitcoin, int c_x_pos, int c_y_pos, int x_pos, int y_pos, bool drop, bool *pick){
    if(check_Collision(get_player_rectangle(player), get_interactable_item(bitcoin))){
        if(*pick){
            set_has_coin_index(player, coin_index);
            *pick=false;
        }
        if (get_has_coin_index(player)>(-1)){
            set_item_position(bitcoin, (int) x_pos + 8, (int) y_pos + 15);
        }if(drop){
            set_item_position(bitcoin, c_x_pos, c_y_pos);
            set_has_coin_index(player, -1);

        }
    }      
}

// Function to detect collision between rectangles
bool check_Collision(SDL_Rect* a, SDL_Rect* b){ 
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    //Calculate the sides of rect A
    leftA = a->x;
    rightA = a->x + a->w;
    topA = a->y;
    bottomA = a->y + a->h;
    //Calculate the sides of rect B
    leftB = b->x;
    rightB = b->x + b->w;
    topB = b->y;
    bottomB = b->y + b->h;
    //If any of the sides from A are outside of B
    if(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB){
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}