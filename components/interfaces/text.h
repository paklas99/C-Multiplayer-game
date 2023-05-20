#ifndef text_h
#define text_h
#define PUBLIC
#define PRIVATE static
#include <stdbool.h>
#include <SDL2/SDL.h>


// Public
PUBLIC typedef struct Text_type* Text;
PUBLIC Text new_Text(SDL_Renderer* renderer,char* message,int starting_x,int starting_y,int size);
PUBLIC SDL_Texture* get_text(Text text, int texture);
PUBLIC SDL_Rect* create_rectang(SDL_Texture* texture, int x, int y);
PUBLIC void destroy_Text(Text text);
PUBLIC void render_text(Text text);
PUBLIC int get_text_x(Text text);
PUBLIC int get_text_y(Text text);
PUBLIC int get_text_w(Text text);
PUBLIC int get_text_h(Text text);

// Private
PRIVATE SDL_Texture* create_text(SDL_Renderer* renderer, SDL_Surface* surface);
PRIVATE SDL_Texture* create_text_from_font(SDL_Renderer* renderer, char* string,int size);

#endif // text_h