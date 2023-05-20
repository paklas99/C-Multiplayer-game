#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "../interfaces/map.h"
#include "../interfaces/object.h"
#include "../interfaces/textures.h"

#define WALLS 32
#define MAP_WIDTH 768
#define MAP_HEIGHT 768

struct Map_type{
    char name[11];
    SDL_Renderer* renderer;
    Object background;
    Object walls[WALLS];
};

PUBLIC Map new_Map(SDL_Renderer* renderer, Textures textures){
    Map m=malloc(sizeof(struct Map_type));
    printf(">--MAP---");
    strcpy(m->name,"Map 1");
    m->renderer=renderer;

    m->background=new_Object(renderer,get_texture(textures,TEXTURES_GAME_MAP_1),0,0);
    m->walls[0]=new_Object(renderer,get_texture(textures,TEXTURES_HORIZONTAL_THICK_WALL),0,368);
    m->walls[1]=new_Object(renderer,get_texture(textures,TEXTURES_HORIZONTAL_THICK_WALL),624,368);
    m->walls[2]=new_Object(renderer,get_texture(textures,TEXTURES_VERTICAL_THICK_WALL),368,624);
    m->walls[3]=new_Object(renderer,get_texture(textures,TEXTURES_VERTICAL_THICK_WALL),368,0);
    m->walls[4]=new_Object(renderer,get_texture(textures,TEXTURES_HORIZONTAL_THIN_WALL),288,208);
    m->walls[5]=new_Object(renderer,get_texture(textures,TEXTURES_HORIZONTAL_THIN_WALL),288,544);
    m->walls[6]=new_Object(renderer,get_texture(textures,TEXTURES_VERTICAL_THIN_WALL),208,288);
    m->walls[7]=new_Object(renderer,get_texture(textures,TEXTURES_VERTICAL_THIN_WALL),544,288);
    m->walls[8]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_1),160,64);
    m->walls[9]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_1),464,64);
    m->walls[10]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_1),160,688);
    m->walls[11]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_1),464,688);
    m->walls[12]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_2),64,160);
    m->walls[13]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_2),592,160);
    m->walls[14]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_2),64,592);
    m->walls[15]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_HORIZONTAL_2),592,592);
    m->walls[16]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_1),64,160);
    m->walls[17]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_1),688,160);
    m->walls[18]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_1),64,464);
    m->walls[19]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_1),688,464);
    m->walls[20]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_2),160,64);
    m->walls[21]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_2),592,64);
    m->walls[22]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_2),160,592);
    m->walls[23]=new_Object(renderer,get_texture(textures,TEXTURES_ZAG_WALL_VERTICAL_2),592,592);
    m->walls[24]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_HORIZONTAL),288,288);
    m->walls[25]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_HORIZONTAL),416,288);
    m->walls[26]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_HORIZONTAL),288,464);
    m->walls[27]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_HORIZONTAL),416,464);
    m->walls[28]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_VERTICAL),288,288);
    m->walls[29]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_VERTICAL),464,288);
    m->walls[30]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_VERTICAL),288,416);
    m->walls[31]=new_Object(renderer,get_texture(textures,TEXTURES_L_WALL_VERTICAL),464,416);
    return m;
}

PUBLIC void render_map(Map m){
    render_object(m->background);
}
PUBLIC int get_map_width(){
    return MAP_WIDTH;    
}
PUBLIC int get_map_height(){
    return MAP_HEIGHT;    
}
PUBLIC int get_map_number_of_walls(Map m){
    return WALLS;
}
PUBLIC Object get_map_wall(Map m,int index){
    return m->walls[index];
}