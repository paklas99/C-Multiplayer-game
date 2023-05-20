#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include "../interfaces/menu.h"
#include "../interfaces/game.h"
#include "../interfaces/textures.h"
#include "../interfaces/text.h"
#include "../interfaces/object.h"
#include "../interfaces/initialization.h"
#include "../interfaces/network_UDP.h"
#define PUBLIC
#define PRIVATE static

typedef struct text{
    Text join_1;
    Text join_2;
    Text settings;
    Text settings_1;
    Text settings_2;
    Text test;
}text;

typedef struct exits{
    Text join;
    Text settings;
    Text test;
}exits;

typedef struct title{
    Text join;
    Text settings;
    Text test;
}title;

struct Menu_type{
    Object background;
    Object music_on;
    Object music_off;
    Object audio_on;
    Object audio_off;
    Text game_name;
    Text _main;
    Text join;
    Text settings;
    Text test;
    Text quit;
    Text ip_adress_join;
    Text main_quit;
    text text;
    exits exits;
    title title;
    bool music_run;
    bool music_pause;
    bool effect_mute;
    bool in_test;
    bool in_join;
    bool in_settings;
    bool been_in_join;
    char text_join[100];
}Menu_type;

//The music that will be played
Mix_Music *Music = NULL;
Mix_Chunk *Select = NULL;
Mix_Chunk *Mute   = NULL;

// Public
PUBLIC Menu new_Menu(SDL_Renderer *renderer, Textures textures){
    printf("\n---MENU---\n");
    Menu menu = malloc(sizeof(Menu_type));
    menu->effect_mute=true;
    menu->music_run=true;
    menu->in_join=false;
    menu->in_test=false;
    menu->in_settings=false;
    menu->been_in_join=false;
    strcpy(menu->text_join,"\0");

    // Create rectangles
    menu->background=new_Object(renderer,get_texture(textures,TEXTURES_MENU_BACKGROUND),0,0);
    menu->music_on=new_Object(renderer,get_texture(textures,TEXTURES_MUSIC_ON),290,480);
    menu->music_off=new_Object(renderer,get_texture(textures,TEXTURES_MUSIC_OFF),290,480);
    menu->audio_on=new_Object(renderer,get_texture(textures,TEXTURES_MUSIC_ON),555,480);
    menu->audio_off=new_Object(renderer,get_texture(textures,TEXTURES_MUSIC_OFF),555,480);
    menu->game_name=new_Text(renderer,"Kalabalik",350,50,95);
    menu->_main=new_Text(renderer,"Main",50,70,100);
    menu->join=new_Text(renderer,"Join",40,250,80);
    menu->title.join=new_Text(renderer,"Join",50,50,105);
    menu->text.join_1=new_Text(renderer,"Click here to join",40,350,80);
    menu->text.join_2=new_Text(renderer,"IP-adress:",40,250,80);
    menu->exits.join=new_Text(renderer,"Esc - Exit",40,650,80);
    menu->settings=new_Text(renderer,"Settings",40,350,80);
    menu->title.settings=new_Text(renderer,"Settings",50,150,105);
    menu->text.settings_1=new_Text(renderer,"Music",260,400,70);
    menu->text.settings_2=new_Text(renderer,"Audio",530,400,70);
    menu->exits.settings=new_Text(renderer,"Esc - Exit",40,650,80);
    menu->test=new_Text(renderer,"Test",40,550,80);
    menu->title.test=new_Text(renderer,"Test",50,50,105);
    menu->text.test=new_Text(renderer,"1 - Gameplan",40,250,80); 
    menu->exits.test=new_Text(renderer,"Esc - Exit",40,650,80);
    menu->quit=new_Text(renderer,"Quit",50,50,105);
    menu->main_quit=new_Text(renderer,"Esc - Quit",40,650,80);
    
    //Load music
    Music = Mix_LoadMUS( "resources/Music/Audio/music.mp3" );
    if( Music == NULL )
    {
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }
    Select = Mix_LoadWAV( "resources/Music/Audio/menu_select.wav");
    if ( Select == NULL)
    {
        printf("Failed to load effect sound! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }
    Mute = Mix_LoadWAV( "resources/Music/Audio/audio_select.mp3" );
    if ( Mute == NULL)
    {
        printf("Failed to load mute sound! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }

    Mix_PlayMusic(Music, -1);

    //If there is no music playing
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( Music, -1 );
    }

    printf("Main menu...\n");
    Menu_main(renderer,textures,menu);


    return menu;
}
PUBLIC void destroy_Menu(Menu menu){
    free(menu);
    printf("Menu memory freed.\n");
}
PUBLIC void destroy_menu_audio(){
    Mix_FreeMusic(Music);
    Mix_FreeChunk(Select);
    Mix_FreeChunk(Mute);
    Music = NULL;
    Select = NULL;
    Mute = NULL;
    Mix_Quit();
    printf("Menu music memory freed.\n");
}

// Private
PRIVATE char Menu_main(SDL_Renderer* renderer, Textures textures, Menu menu){
    bool quit_requested=false;
    while(!quit_requested){
        SDL_RenderClear(renderer);
        render_object(menu->background);
        render_text(menu->game_name);
        render_text(menu->_main);
        render_text(menu->join);
        render_text(menu->settings);
        render_text(menu->main_quit);
        SDL_RenderPresent(renderer);
        switch(Menu_input(renderer,textures,menu,menu->_main)){
            case '2': case 'j': quit_requested=Menu_join(renderer,textures,menu); break;
            case '3': case 'n': quit_requested=Menu_settings(renderer,textures,menu); break;
            case '4': case 'T': quit_requested=Menu_test(renderer,textures,menu); break; 
            case 'q': case 'b': quit_requested=Menu_quit(renderer,textures,menu); break;
            default: break;
        }
    }
}
PRIVATE bool Menu_join(SDL_Renderer* renderer, Textures textures, Menu menu){
    Game game=NULL;
    while(true){
        menu->in_join=true;
        Initialization init;
        SDL_RenderClear(renderer);
        render_object(menu->background);
        render_text(menu->game_name);
        render_text(menu->title.join);
        render_text(menu->exits.join);
        render_text(menu->text.join_1);
        render_text(menu->text.join_2);
        if (menu->been_in_join == true) render_text(menu->ip_adress_join);
        SDL_RenderPresent(renderer);
        switch(Menu_input(renderer,textures,menu,menu->join)){
            case 'b': case 'Q': menu->in_join=false; return false;
            case 'r': 
                SDL_RenderClear(renderer);
                render_object(menu->background);
                render_text(menu->game_name);
                render_text(menu->title.join);
                render_text(menu->exits.join);
                render_text(menu->text.join_1);
                render_text(menu->text.join_2);
                if (menu->been_in_join == true) render_text(menu->ip_adress_join);
                SDL_RenderPresent(renderer);
                break;
            case 'x':
                menu->text_join;
                game=new_Game(renderer,textures,true,menu);
                if(game_quit(game)) return Menu_quit(renderer,textures,menu);
                if(!game_quit(game)) menu->in_join=false; return false;
                break;
            case 'q': return Menu_quit(renderer,textures,menu);
            default: break; 
        }
    }
}
PRIVATE bool Menu_settings(SDL_Renderer* renderer, Textures textures, Menu menu){
    Game game=NULL;
    menu->in_settings=true;
    while(true){
        SDL_RenderClear(renderer);
        render_object(menu->background);
        if(menu->music_run == true) render_object(menu->music_on); 
        if(menu->music_pause == true) render_object(menu->music_off); 
        if(menu->effect_mute == true) render_object(menu->audio_on); 
        if(menu->effect_mute == false) render_object(menu->audio_off); 
        render_text(menu->game_name);
        render_text(menu->title.settings);
        render_text(menu->exits.settings);
        render_text(menu->text.settings_1);
        render_text(menu->text.settings_2);
        SDL_RenderPresent(renderer);
        switch(Menu_input(renderer,textures,menu,menu->settings)){
            case 'b': case 'Q': menu->in_settings=false; return false;
            case 'c': 
                //If there is no music playing
                if( Mix_PlayingMusic() == 0 )
                {
                    //Play the music
                    Mix_PlayMusic( Music, -1 );
                }
                //If music is being played
                else
                {
                    //If the music is paused
                    if( Mix_PausedMusic() == 1 )
                    {
                        //Resume the music
                        Mix_ResumeMusic();
                        menu->music_run = true;
                        menu->music_pause = false;
                    }
                    //If the music is playing
                    else
                    {
                        //Pause the music
                        Mix_PauseMusic();
                        menu->music_pause = true;
                        menu->music_run = false;
                    }
                }
                break;
            case 'g':
                if ( menu->effect_mute == false ){ 
                    menu->effect_mute=true; 
                }    
                else if ( menu->effect_mute == true ){
                    menu->effect_mute=false; 
                }    
                break;
            case 'q': return Menu_quit(renderer,textures,menu);
        }
    }
}
PRIVATE bool Menu_test(SDL_Renderer* renderer, Textures textures, Menu menu){
    Game game = NULL;
    menu->in_test=true;
    while(true){
        SDL_RenderClear(renderer);
        render_object(menu->background);
        render_text(menu->game_name);
        render_text(menu->title.test);
        render_text(menu->text.test);
        render_text(menu->exits.test);
        SDL_RenderPresent(renderer);
        switch(Menu_input(renderer,textures,menu,menu->test)){
            case '1': case 'a': 
                game = new_Game(renderer,textures,false,menu); 
                if(game_quit(game)) return Menu_quit(renderer,textures,menu);
                break;
            case 'b': case 'Q': menu->in_test=false; return false;
            case 'q': return Menu_quit(renderer,textures,menu); 
        }
    }
}

PRIVATE bool Menu_quit(SDL_Renderer* renderer, Textures textures, Menu menu){
    SDL_RenderClear(renderer);
    render_object(menu->background);
    render_text(menu->game_name);
    render_text(menu->quit);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
    return true;
}

PRIVATE char Menu_input(SDL_Renderer* renderer, Textures textures, Menu menu, Text rectangle){
    SDL_Event event;
    int mouse_x, mouse_y;
    while(true){
        while (SDL_PollEvent(&event)){
            SDL_GetMouseState(&mouse_x, &mouse_y);
            switch (event.type){
                case SDL_QUIT: return 'q'; break;
                case SDL_MOUSEBUTTONDOWN:
                    if(mouse_x > get_text_x(menu->text.test) && 
                            mouse_x < get_text_x(menu->text.test) + get_text_w(menu->text.test) && 
                            mouse_y > get_text_y(menu->text.test) && 
                            mouse_y < get_text_y(menu->text.test) + get_text_h(menu->text.test) && 
                            menu->in_test == true){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                            return 'a';
                    }
                    else if(mouse_x > get_text_x(menu->join) && 
                            mouse_x < get_text_x(menu->join) + get_text_w(menu->join) && 
                            mouse_y > get_text_y(menu->join) && 
                            mouse_y < get_text_y(menu->join) + get_text_h(menu->join) &&
                            menu->in_join == false){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                        return 'j';
                    }
                    else if(mouse_x > get_text_x(menu->text.join_1) &&
                            mouse_x < get_text_x(menu->text.join_1) + get_text_w(menu->text.join_1) &&
                            mouse_y > get_text_y(menu->text.join_1) &&
                            mouse_y < get_text_y(menu->text.join_1) + get_text_h(menu->text.join_1) &&
                            menu->in_join == true){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                            return 'x';        
                    }
                    else if(mouse_x > get_text_x(menu->settings) && 
                            mouse_x < get_text_x(menu->settings) + get_text_w(menu->settings) && 
                            mouse_y > get_text_y(menu->settings) && 
                            mouse_y < get_text_y(menu->settings) + get_text_h(menu->settings) &&
                            menu->in_settings == false){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                        return 'n';
                    }
                    else if(mouse_x > get_text_x(menu->test) && 
                            mouse_x < get_text_x(menu->test) + get_text_w(menu->test) && 
                            mouse_y > get_text_y(menu->test) && 
                            mouse_y < get_text_y(menu->test) + get_text_h(menu->test)){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                        return 'T';
                    }
                    else if(mouse_x > get_text_x(menu->exits.settings) && 
                            mouse_x < get_text_x(menu->exits.settings) + get_text_w(menu->exits.settings) && 
                            mouse_y > get_text_y(menu->exits.settings) && 
                            mouse_y < get_text_y(menu->exits.settings) + get_text_h(menu->exits.settings)){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0 );
                        return 'Q';
                    }
                    else if(mouse_x > get_object_x(menu->music_on) && 
                            mouse_x < get_object_x(menu->music_on) + get_object_w(menu->music_on) && 
                            mouse_y > get_object_y(menu->music_on) && 
                            mouse_y < get_object_y(menu->music_on) + get_object_h(menu->music_on)){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Mute, 0 );
                        return 'c';
                    }
                    else if(mouse_x > get_object_x(menu->audio_on) && 
                            mouse_x < get_object_x(menu->audio_on) + get_object_w(menu->audio_on) && 
                            mouse_y > get_object_y(menu->audio_on) && 
                            mouse_y < get_object_y(menu->audio_on) + get_object_h(menu->audio_on)){
                            if(menu->effect_mute) Mix_PlayChannel(-1, Mute, 0 );
                        return 'g';
                    }
                    break;
                case SDL_TEXTINPUT:
                    if ( menu->in_join == true )
                    {
                        strcat(menu->text_join,event.text.text);
                        menu->ip_adress_join=new_Text(renderer,menu->text_join,400,250,80);
                        menu->been_in_join=true;
                        return 'r';
                    }
                    break;
                case SDL_KEYDOWN: if(menu->effect_mute) Mix_PlayChannel(-1, Select, 0);
                    if ( event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && strlen(menu->text_join) > 0 && menu->in_join == true){
                        int size1 = strlen(menu->text_join);// Total size of string
                        menu->text_join[size1-1]='\0';      // Remove the last char
                        menu->ip_adress_join=new_Text(renderer,menu->text_join,400,250,80);
                        menu->been_in_join=true;
                        return 'r';
                    }
                switch (event.key.keysym.scancode){ 
                    case SDL_SCANCODE_ESCAPE:  return 'b';
                    case SDL_SCANCODE_1:  return '1';
                    case SDL_SCANCODE_2:  return '2';
                    case SDL_SCANCODE_3:  return '3';
                    case SDL_SCANCODE_4:  return '4';
                    case SDL_SCANCODE_T:  return 't'; 
                }
                break;
            }         
        }
    }
}
PUBLIC bool menu_mute(Menu menu){
    return menu->effect_mute;
}
PUBLIC char * get_ip_join_array(Menu menu){
    static char buffers[100];
    sprintf(buffers,"%s",menu->text_join);
    return buffers;
}