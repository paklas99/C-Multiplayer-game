#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../interfaces/text.h"
#include "../interfaces/textures.h"
#include <SDL2/SDL_ttf.h>

#define MESSAGES 20 // används ej

struct Text_type{
    SDL_Renderer* renderer;
    SDL_Texture* text_texture;
    SDL_Rect* rectangle;
}Text_type;


// Public
PUBLIC Text new_Text(SDL_Renderer* renderer,char* message,int starting_x,int starting_y,int size){
    // printf("\n---TEXT---\n");

    Text text = malloc(sizeof(Text_type));

    text->renderer=renderer;
    text->text_texture = create_text_from_font(renderer,message,size);
    text->rectangle=create_rectangle(text->text_texture,starting_x,starting_y);

    return text;
}
PUBLIC void render_text(Text text){
    SDL_RenderCopy(text->renderer,text->text_texture,NULL,text->rectangle);
}
PUBLIC SDL_Texture* get_text(Text text, int texture){
    return text->text_texture;
}

//similar to create_texture_from_font but adjusted for procedural text
PRIVATE SDL_Texture* create_text_from_font(SDL_Renderer* renderer, char* string,int size){
    // printf("Loading font: %s\n",font);

    char* font_path = "resources/fonts/SilomBol.ttf";
    SDL_Color color = {255,255,255}; //white

    // Font
    TTF_Font* font;
    font = TTF_OpenFont(font_path,size);
    if(!font_path){
        printf("could not open font: %s\n", SDL_GetError());
        return NULL;
    }
    
    // Surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, string, color);
    if (!surface){
        printf("Error creating surface: %s\n",SDL_GetError());
        return NULL;
    }
    // printf("Surface created: %s\n",file);

    // Texture
    SDL_Texture* texture = create_texture(renderer,surface);
    if(texture==NULL) return NULL;

    return texture;
}

PUBLIC void destroy_Text(Text text){
    for(int i=0; i<MESSAGES; i++){
        SDL_DestroyTexture(text->text_texture);
        printf("Texture %d destroyed.\n",i);
    }
    free(text);
    printf("Textures memory freed.\n");
}
PUBLIC int get_text_x(Text text){
    return text->rectangle->x;
}
PUBLIC int get_text_y(Text text){
    return text->rectangle->y;
}
PUBLIC int get_text_w(Text text){
    return text->rectangle->w;
}
PUBLIC int get_text_h(Text text){
    return text->rectangle->h;
}