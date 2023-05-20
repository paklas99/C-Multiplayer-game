#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../interfaces/object.h"
#include "../interfaces/textures.h"

struct Object_type{
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect* rectangle;
};

PUBLIC Object new_Object(SDL_Renderer* renderer, SDL_Texture* texture,int starting_x, int starting_y){
    printf("\n---OBJECT---\n");
    Object object = malloc(sizeof(struct Object_type));
    object->renderer=renderer;
    object->texture=texture;
    object->rectangle=create_rectangle(object->texture,starting_x,starting_y);
    return object;
}
PUBLIC void render_object(Object object){
    SDL_RenderCopy(object->renderer,object->texture,NULL,object->rectangle);
}
PUBLIC int get_object_width(Object object){
    return object->rectangle->w;    
}
PUBLIC int get_object_height(Object object){
    return object->rectangle->h;    
}
PUBLIC SDL_Rect* get_object_rectangle(Object object){
    return object->rectangle;
}
PUBLIC void set_object_scale(Object object,float height_scale_factor, float width_scale_factor){
    object->rectangle->h *= height_scale_factor;
    object->rectangle->w *= width_scale_factor;
}
PUBLIC int get_object_x(Object object){
    return object->rectangle->x;
}
PUBLIC int get_object_y(Object object){
    return object->rectangle->y;
}
PUBLIC int get_object_w(Object object){
    return object->rectangle->w;
}
PUBLIC int get_object_h(Object object){
    return object->rectangle->h;
}
PUBLIC void set_object_position(Object object, int position_x, int position_y){
    object->rectangle->x=position_x;
    object->rectangle->y=position_y;
}
PUBLIC SDL_Rect* get_object(Object object){
    return object->rectangle;
}