#ifndef map_h
#define map_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "../interfaces/textures.h"
#include "../interfaces/object.h"

#define MAP_WIDTH 768
#define MAP_HEIGHT 768

typedef struct Map_type *Map;

PUBLIC Map new_Map(SDL_Renderer* renderer, Textures textures);
PUBLIC int get_map_width();
PUBLIC int get_map_height();
PUBLIC int get_map_number_of_walls(Map m);
PUBLIC Object get_map_wall(Map m,int index);
PUBLIC void render_map(Map m);

#endif // map_h