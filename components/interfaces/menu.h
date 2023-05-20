#ifndef menu_h
#define menu_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdbool.h>
#include "../interfaces/textures.h"
#include "../interfaces/text.h"

typedef struct Menu_type* Menu;

// Public
PUBLIC Menu new_Menu(SDL_Renderer* renderer, Textures textures);
PUBLIC bool menu_mute(Menu menu);
PUBLIC void destroy_Menu(Menu menu);
PUBLIC void destroy_menu_audio();
PUBLIC char * get_ip_join_array(Menu menu);

// Private
PRIVATE char Menu_main(SDL_Renderer* renderer, Textures textures, Menu menu);
PRIVATE bool Menu_join(SDL_Renderer* renderer, Textures textures, Menu menu);
PRIVATE bool Menu_settings(SDL_Renderer* renderer, Textures textures, Menu menu);
PRIVATE bool Menu_test(SDL_Renderer* renderer, Textures textures, Menu menu);
PRIVATE bool Menu_quit(SDL_Renderer* renderer, Textures textures, Menu menu);
PRIVATE char Menu_input(SDL_Renderer* renderer, Textures textures, Menu menu, Text rectangle);


#endif // menu_h
