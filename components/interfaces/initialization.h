#ifndef initialization_h
#define initialization_h
#include <stdlib.h>
#include <SDL2/SDL.h>
#define PUBLIC
#define PRIVATE static

#define GAME_TITLE "KALABALIK"
#define WINDOW_HEIGHT 768 
#define WINDOW_WIDTH 1024 

typedef struct Init_type* Initialization;

PUBLIC Initialization new_Init();
PUBLIC SDL_Renderer* get_renderer(Initialization init);
PUBLIC void destroy_Initialization(Initialization init);

#endif // initialization_h