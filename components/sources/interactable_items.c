#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../interfaces/interactable_items.h"
#include "../interfaces/textures.h"

typedef struct Position{
    int x,y;
}Position;

struct Interactable_items_type{
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect* rectangle;
    Position position;
};

PUBLIC Interactable_items new_Interactable_Item(SDL_Renderer* renderer, SDL_Texture* texture, int starting_x, int starting_y){
    printf("\n---INTERACTABLE_ITEMS---\n");
    Interactable_items item = malloc(sizeof(struct Interactable_items_type));
    item->renderer=renderer;
    item->texture=texture;
    item->position.x=starting_x;
    item->position.y=starting_y;
    item->rectangle=create_rectangle(item->texture, starting_x, starting_y);
    return item;
}

PUBLIC void render_interactable_item(Interactable_items item){
    if(get_item_position_x(item)==800 && get_item_position_y(item)==500){
    }
    else{
        SDL_RenderCopy(item->renderer, item->texture,NULL, item->rectangle);
    }
}

PUBLIC SDL_Rect* get_interactable_item(Interactable_items item){
    return item->rectangle;
}
PUBLIC int get_int_item_height(Interactable_items item){
    return item->rectangle->h;
}
PUBLIC int get_int_item_width(Interactable_items item){
    return item->rectangle->w;
}
PUBLIC int get_item_position_x(Interactable_items item){
    return item->rectangle->x;
}
PUBLIC int get_item_position_y(Interactable_items item){
    return item->rectangle->y;
}
PUBLIC void set_item_position(Interactable_items item, int position_x, int position_y){
    item->rectangle->x=position_x;
    item->rectangle->y=position_y;
}
PUBLIC void set_item_width_height(Interactable_items item, float width, float height){
    item->rectangle->w = width;
    item->rectangle->h = height;
}
