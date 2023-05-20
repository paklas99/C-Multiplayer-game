#ifndef textures_h
#define textures_h
#define PUBLIC
#define PRIVATE static
#include <stdbool.h>
#include <SDL2/SDL.h>

#define TEXTURES_MENU_BACKGROUND    0
#define TEXTURES_MENU_MAIN          1
#define TEXTURES_MENU_HOST          2
#define TEXTURES_MENU_JOIN          3
#define TEXTURES_MENU_HELP          4
#define TEXTURES_MENU_TEST          5
#define TEXTURES_MENU_QUIT          6
#define TEXTURES_GAME_MAP_1         7
#define TEXTURES_PLAYER_1           8
#define TEXTURES_PLAYER_2           9
#define TEXTURES_PLAYER_3           10
#define TEXTURES_PLAYER_4           11
#define TEXTURES_BATHROOM           12
#define TEXTURES_BLUE_CAR           13
#define TEXTURES_BLUE_HOUSE         14
#define TEXTURES_BLUE_TRUCK         15
#define TEXTURES_EMPTY_6x6          16
#define TEXTURES_GLASS_HOUSE_nodoor 17
#define TEXTURES_GLASS_HOUSE_door   18
#define TEXTURES_GREEN_SHOP         19
#define TEXTURES_LIGHT_HOUSE        20
#define TEXTURES_PLAYER_HOUSE       21
#define TEXTURES_POLICE_CAR         22
#define TEXTURES_POOL_CORNER        23
#define TEXTURES_RED_CAR            24
#define TEXTURES_RED_HOUSE_HIGH     25
#define TEXTURES_RED_STORE          26
#define TEXTURES_RED_TRUCK          27
#define TEXTURES_SINGLE_BRIDGE      28
#define TEXTURES_SMALL_SHOP         29
#define TEXTURES_TRUCK              30
#define TEXTURES_UTILITY_HOUSE      31
#define TEXTURES_YELLOW_CAR         32
#define TEXTURES_COINS_BITCOIN      33
#define TEXTURES_COINS_ETHEREUM     34

#define TEXTURES_HORIZONTAL_THICK_WALL 35
#define TEXTURES_HORIZONTAL_THIN_WALL  36
#define TEXTURES_VERTICAL_THICK_WALL   37
#define TEXTURES_VERTICAL_THIN_WALL    38
#define TEXTURES_L_WALL_HORIZONTAL     39
#define TEXTURES_L_WALL_VERTICAL       40
#define TEXTURES_ZAG_WALL_HORIZONTAL_1 41
#define TEXTURES_ZAG_WALL_HORIZONTAL_2 42
#define TEXTURES_ZAG_WALL_VERTICAL_1   43
#define TEXTURES_ZAG_WALL_VERTICAL_2   44
#define TEXTURES_COINS_DOGE            45

#define TEXTURES_SPEED_BOOST_UP     46 
 
#define TEXTURES_MUSIC_OFF          47
#define TEXTURES_MUSIC_ON           48

// Public
PUBLIC typedef struct Textures_type* Textures;
PUBLIC Textures new_Textures(SDL_Renderer* renderer);
PUBLIC SDL_Texture* get_texture(Textures textures, int texture);
PUBLIC SDL_Rect* create_rectangle(SDL_Texture* texture, int x, int y);
PUBLIC SDL_Texture* create_texture(SDL_Renderer* renderer, SDL_Surface* surface);
PUBLIC void destroy_Textures(Textures textures);

// Private
PRIVATE SDL_Texture* create_texture_from_image(SDL_Renderer* renderer, char* file);
PRIVATE SDL_Texture* create_texture_from_font(SDL_Renderer* renderer, char* file, char* message,int size,SDL_Color textColor);

#endif // textures_h