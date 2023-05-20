#ifndef game_h
#define game_h
#define PUBLIC
#define PRIVATE static
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../interfaces/textures.h"
#include "../interfaces/player.h"
#include "../interfaces/menu.h"

#define MAPS 1
#define PLAYERS 4
#define FPS 60
#define COINS 3
#define DELAY 1000/FPS

//Sartpositioner
#define SP_P1_X 35
#define SP_P1_Y 25
#define SP_P2_X 705
#define SP_P2_Y 25
#define SP_P3_X 35
#define SP_P3_Y 695
#define SP_P4_X 705
#define SP_P4_Y 695
//coins
#define SP_C0_X 376
#define SP_C0_Y 329
#define SP_C1_X 325
#define SP_C1_Y 418
#define SP_C2_X 420
#define SP_C2_Y 418
#define SP_EXIT_X 840
#define SP_EXIT_Y 700

#define SP_BOOST_X 368
#define SP_BOOST_Y 368

typedef struct Game_type* Game;

// Public
PUBLIC Game new_Game(SDL_Renderer* renderer, Textures textures, bool multiplayer, Menu menu);
PUBLIC void destroy_Game(Game game);
PUBLIC void destroy_game_audio();
PUBLIC bool game_quit(Game game);

// Private
PRIVATE void sprite_regulator(Player player, int *frame_time, int *sprite_number, int frame_begin);

#endif // game_h


