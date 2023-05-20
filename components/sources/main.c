#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "../interfaces/initialization.h"
#include "../interfaces/textures.h"
#include "../interfaces/menu.h"
#include "../interfaces/text.h"
#include "../interfaces/game.h"

int main(int argc, char* args[]){
    Initialization init = new_Init();
    Textures textures = new_Textures(get_renderer(init));
    Menu menu = new_Menu(get_renderer(init),textures);
    Game game;

    // Memory clean up
    destroy_Initialization(init);
    destroy_Textures(textures);
    destroy_Menu(menu);
    destroy_Game(game);
    destroy_menu_audio();
    destroy_game_audio();
    SDL_Quit();
}