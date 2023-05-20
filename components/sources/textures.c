#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../interfaces/textures.h"
#include <SDL2/SDL_ttf.h>

#define TEXTURES 50

struct Textures_type{
    SDL_Texture* array[TEXTURES];
}Textures_type;

// Public
PUBLIC Textures new_Textures(SDL_Renderer* renderer){
    printf("\n---TEXTURES---\n");
    Textures textures = malloc(sizeof(Textures_type));

    // Menu
    textures->array[TEXTURES_MENU_BACKGROUND]=create_texture_from_image(renderer,"resources/images/menu/background.png");
    textures->array[TEXTURES_MENU_MAIN]=create_texture_from_image(renderer,"resources/images/menu/main.png");
    textures->array[TEXTURES_MENU_HOST]=create_texture_from_image(renderer,"resources/images/menu/host.png");
    textures->array[TEXTURES_MENU_JOIN]=create_texture_from_image(renderer,"resources/images/menu/join.png");
    textures->array[TEXTURES_MENU_HELP]=create_texture_from_image(renderer,"resources/images/menu/help.png");
    textures->array[TEXTURES_MENU_TEST]=create_texture_from_image(renderer,"resources/images/menu/test.png");
    textures->array[TEXTURES_MENU_QUIT]=create_texture_from_image(renderer,"resources/images/menu/quit.png");
    //Items
    textures->array[TEXTURES_COINS_BITCOIN]=create_texture_from_image(renderer,"resources/images/coins/bitcoin.png");
    textures->array[TEXTURES_COINS_ETHEREUM]=create_texture_from_image(renderer,"resources/images/coins/ethereum.png");
    textures->array[TEXTURES_COINS_DOGE]=create_texture_from_image(renderer,"resources/images/coins/doge.png");

    // Objects
    textures->array[TEXTURES_GAME_MAP_1]=create_texture_from_image(renderer,"resources/images/maps/map.png");
    textures->array[TEXTURES_BATHROOM]=create_texture_from_image(renderer,"resources/images/objects/bathroom.png");
    textures->array[TEXTURES_BLUE_CAR]=create_texture_from_image(renderer,"resources/images/objects/blue_car.png");
    textures->array[TEXTURES_BLUE_HOUSE]=create_texture_from_image(renderer,"resources/images/objects/blue_house_high.png");
    textures->array[TEXTURES_BLUE_TRUCK]=create_texture_from_image(renderer,"resources/images/objects/blue_truck.png");
    textures->array[TEXTURES_EMPTY_6x6]=create_texture_from_image(renderer,"resources/images/objects/empty_6x6.png");
    textures->array[TEXTURES_GLASS_HOUSE_nodoor]=create_texture_from_image(renderer,"resources/images/objects/glass_house_nodoor.png");
    textures->array[TEXTURES_GLASS_HOUSE_door]=create_texture_from_image(renderer,"resources/images/objects/glass_house_withdoor.png");
    textures->array[TEXTURES_GREEN_SHOP]=create_texture_from_image(renderer,"resources/images/objects/green_shop.png");
    textures->array[TEXTURES_LIGHT_HOUSE]=create_texture_from_image(renderer,"resources/images/objects/light_house.png");
    textures->array[TEXTURES_PLAYER_HOUSE]=create_texture_from_image(renderer,"resources/images/objects/player_house.png");
    textures->array[TEXTURES_POLICE_CAR]=create_texture_from_image(renderer,"resources/images/objects/police_car.png");
    textures->array[TEXTURES_POOL_CORNER]=create_texture_from_image(renderer,"resources/images/objects/pool_corner.png");
    textures->array[TEXTURES_RED_CAR]=create_texture_from_image(renderer,"resources/images/objects/red_car.png");
    textures->array[TEXTURES_RED_HOUSE_HIGH]=create_texture_from_image(renderer,"resources/images/objects/red_house_high.png");
    textures->array[TEXTURES_RED_STORE]=create_texture_from_image(renderer,"resources/images/objects/red_store.png");
    textures->array[TEXTURES_RED_TRUCK]=create_texture_from_image(renderer,"resources/images/objects/red_truck.png");
    textures->array[TEXTURES_SINGLE_BRIDGE]=create_texture_from_image(renderer,"resources/images/objects/single_bridge.png");
    textures->array[TEXTURES_SMALL_SHOP]=create_texture_from_image(renderer,"resources/images/objects/small_shop.png");
    textures->array[TEXTURES_TRUCK]=create_texture_from_image(renderer,"resources/images/objects/truck.png");
    textures->array[TEXTURES_UTILITY_HOUSE]=create_texture_from_image(renderer,"resources/images/objects/utility_house.png");
    textures->array[TEXTURES_YELLOW_CAR]=create_texture_from_image(renderer,"resources/images/objects/yellow_car.png");
    textures->array[TEXTURES_SPEED_BOOST_UP]=create_texture_from_image(renderer,"resources/images/objects/speed_boost.png");

    //  Walls
    textures->array[TEXTURES_HORIZONTAL_THICK_WALL]=create_texture_from_image(renderer,"resources/images/objects/horizontal_thick_wall.png");
    textures->array[TEXTURES_HORIZONTAL_THIN_WALL]=create_texture_from_image(renderer,"resources/images/objects/horizontal_thin_wall.png");
    textures->array[TEXTURES_VERTICAL_THICK_WALL]=create_texture_from_image(renderer,"resources/images/objects/vertical_thick_wall.png");
    textures->array[TEXTURES_VERTICAL_THIN_WALL]=create_texture_from_image(renderer,"resources/images/objects/vertical_thin_wall.png");
    textures->array[TEXTURES_L_WALL_HORIZONTAL]=create_texture_from_image(renderer,"resources/images/objects/L_wall_part_horizontal.png");
    textures->array[TEXTURES_L_WALL_VERTICAL]=create_texture_from_image(renderer,"resources/images/objects/L_wall_part_vertical.png");
    textures->array[TEXTURES_ZAG_WALL_HORIZONTAL_1]=create_texture_from_image(renderer,"resources/images/objects/zag_wall_horizontal_1.png");
    textures->array[TEXTURES_ZAG_WALL_HORIZONTAL_2]=create_texture_from_image(renderer,"resources/images/objects/zag_wall_horizontal_2.png");
    textures->array[TEXTURES_ZAG_WALL_VERTICAL_1]=create_texture_from_image(renderer,"resources/images/objects/zag_wall_vertical_1.png");
    textures->array[TEXTURES_ZAG_WALL_VERTICAL_2]=create_texture_from_image(renderer,"resources/images/objects/zag_wall_vertical_2.png");

    // Player sprites
    textures->array[TEXTURES_PLAYER_1]=create_texture_from_image(renderer,"resources/images/players/player_1.png");
    textures->array[TEXTURES_PLAYER_2]=create_texture_from_image(renderer,"resources/images/players/player_2.png");
    textures->array[TEXTURES_PLAYER_3]=create_texture_from_image(renderer,"resources/images/players/player_3.png");
    textures->array[TEXTURES_PLAYER_4]=create_texture_from_image(renderer,"resources/images/players/player_4.png");

    // Music
    textures->array[TEXTURES_MUSIC_OFF]=create_texture_from_image(renderer,"resources/Music/Image/music_off.png");
    textures->array[TEXTURES_MUSIC_ON]=create_texture_from_image(renderer,"resources/Music/Image/music_on.png");

    return textures;
}
PUBLIC SDL_Texture* get_texture(Textures textures, int texture){
    return textures->array[texture];
}
PUBLIC SDL_Rect* create_rectangle(SDL_Texture* texture, int x, int y){
    // printf("Creating rectangle...\n");
    SDL_Rect* rectangle=malloc(sizeof(SDL_Rect));

    SDL_QueryTexture(texture,NULL,NULL,&rectangle->w,&rectangle->h);
    rectangle->x=x;
    rectangle->y=y;
    printf("Rectangle created. Dimensions=%dx%d.\n",rectangle->w,rectangle->h);
    return rectangle;
}

// Private
PUBLIC SDL_Texture* create_texture(SDL_Renderer* renderer, SDL_Surface* surface){
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture){
        printf("Error creating texture: %s\n", SDL_GetError());
        return NULL;
    }
    // printf("Texture loaded: %s\n",file);
    return texture;
}
PRIVATE SDL_Texture* create_texture_from_image(SDL_Renderer* renderer, char* file){
    // printf("Loading texture: %s\n",file);
    
    // Surface
    SDL_Surface* surface = IMG_Load(file);
    if (!surface){
        printf("Error creating surface: %s\n",SDL_GetError());
        return NULL;
    }
    // printf("Surface created: %s\n",file);

    // Texture
    SDL_Texture* texture = create_texture(renderer,surface);
    // printf("Texture loaded: %s\n",file);

    return texture;
}

PRIVATE SDL_Texture* create_texture_from_font(SDL_Renderer* renderer, char* font_path, char* string, int size, SDL_Color textColor){
    // printf("Loading font: %s\n",font);

    // Font
    TTF_Font* font;
    font = TTF_OpenFont(font_path,size);
    if(!font_path){
        printf("could not open font: %s\n", SDL_GetError());
        return NULL;
    }
    
    // Surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, string, textColor);
    if (!surface){
        printf("Error creating surface: %s\n",SDL_GetError());
        return NULL;
    }
    // printf("Surface created: %s\n",file);

    // Texture
    SDL_Texture* texture = create_texture(renderer,surface);
    if(texture==NULL) return NULL;

    // printf("Font texture created: \"%s\"\n",string);
    return texture;
}

PUBLIC void destroy_Textures(Textures textures){
    for(int i=0; i<TEXTURES; i++){
        SDL_DestroyTexture(textures->array[i]);
        // printf("Texture %d destroyed.\n",i);
    }
    free(textures);
    printf("Textures memory freed.\n");
}