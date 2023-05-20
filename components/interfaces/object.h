#ifndef object_h
#define object_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

typedef struct Object_type *Object;

PUBLIC Object new_Object(SDL_Renderer* renderer, SDL_Texture* texture,int starting_x, int starting_y);
PUBLIC void render_object(Object object);
PUBLIC int get_object_width(Object object);
PUBLIC int get_object_height(Object object);
PUBLIC SDL_Rect* get_object_rectangle(Object object);
PUBLIC void set_object_scale(Object item,float h, float w);
PUBLIC int get_object_x(Object object);
PUBLIC int get_object_y(Object object);
PUBLIC int get_object_w(Object object);
PUBLIC int get_object_h(Object object);
PUBLIC void set_object_position(Object object, int position_x, int position_y);
PUBLIC SDL_Rect* get_object(Object object);


#endif // object_h