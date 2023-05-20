#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "../interfaces/initialization.h"
#include <string.h>


struct Init_type{
    SDL_Window* window;
    SDL_Renderer* renderer;
}Init_type;

Initialization new_Init(void){
    Initialization init = malloc(sizeof(Init_type));

    printf("\n---INITIALIZATION---\n");

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return NULL;
    }
    printf("Video and timer initialized.\n");
    init->window = SDL_CreateWindow(GAME_TITLE,
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!init->window){
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return NULL;
    }
    printf("Window initialized.\n");

    if (TTF_Init()==-1) {
        printf("TTF_Init: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    // start SDL with audio support
    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
        printf("AUDIO_Init: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    init->renderer = SDL_CreateRenderer(init->window, -1, render_flags);
    if (!init->renderer){
      printf("Error creating renderer: %s\n", SDL_GetError());
      SDL_DestroyWindow(init->window);
      SDL_Quit();
    }
    printf("Renderer initialized.\n");

    return init;
}
SDL_Renderer* get_renderer(Initialization init){
    return init->renderer;
}
PUBLIC int get_window_width(void){
    return WINDOW_WIDTH;
}
PUBLIC int get_window_height(void){
    return WINDOW_HEIGHT;
}
PUBLIC void destroy_Initialization(Initialization init){
    free(init);
    printf("Initialization memory freed.\n");
}