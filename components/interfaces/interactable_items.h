#ifndef interactable_items_h
#define interactable_items_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>

typedef struct Interactable_items_type *Interactable_items;

PUBLIC Interactable_items new_Interactable_Item(SDL_Renderer* renderer, SDL_Texture* texture, int starting_x, int starting_y);
PUBLIC void render_interactable_item(Interactable_items item);
PUBLIC SDL_Rect* get_interactable_item(Interactable_items item);
PUBLIC int get_int_item_height(Interactable_items item);
PUBLIC int get_int_item_width(Interactable_items item);
PUBLIC int get_item_position_x(Interactable_items item);
PUBLIC int get_item_position_y(Interactable_items item);
PUBLIC void set_item_position(Interactable_items item, int position_x, int position_y);
PUBLIC void set_item_width_height(Interactable_items item, float width, float height);

#endif //interactable_items_h
