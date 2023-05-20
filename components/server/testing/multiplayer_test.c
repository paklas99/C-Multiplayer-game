/**
 * hello6_keyboard.c - Move the sprite using the arrow keys or WASD
 */

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

#define WINDOW_WIDTH (400)
#define WINDOW_HEIGHT (400)

// speed in pixels/second
#define SPEED (300)

#define CLIENTS 4

typedef struct Position{
	int x,y;
}Position;
typedef struct Data{
	Position player[CLIENTS];
	int client_ID;
}Data;

int main(int argc, char** argv)
{
    int packet_size=sizeof(Data);
	printf("%zu\n",sizeof(Data));
    UDPsocket socket;
	IPaddress server_address;
	UDPpacket *packet_sending;
    UDPpacket *packet_recieving;
    Data server_data;
    Data client_data;

     if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    if (!(socket = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

    /* Resolve server name  */
    char* ip_string="192.168.1.179";
	if (SDLNet_ResolveHost(&server_address, ip_string, 2000) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 2000): %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    printf("%s\n",ip_string);

    if (!((packet_sending = SDLNet_AllocPacket(packet_size))&&(packet_recieving = SDLNet_AllocPacket(packet_size))))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}


    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Establish connection
    int client_ID=-1;
    printf("Establishing connection.\n");
    while(client_ID==-1){
        printf("Attempting to reach server.\n");
        packet_sending->address.host=server_address.host;
        packet_sending->address.port=server_address.port;
        SDLNet_UDP_Send(socket,-1,packet_sending);
        if (SDLNet_UDP_Recv(socket,packet_recieving)){
            memcpy(&server_data,packet_recieving->data,sizeof(Data));
            client_ID=server_data.client_ID;
        }
        SDL_Delay(1000);
    }
    printf("Connection established. I am Client %d!\n",1+client_ID);

    SDL_Window* win = SDL_CreateWindow("Hello World!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load the image into memory using SDL_image library function
    SDL_Surface* surface = IMG_Load("server/test_drawing.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load the image data into the graphics hardware's memory
    SDL_Texture* tex1 = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture* tex2 = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture* tex3 = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture* tex4 = SDL_CreateTextureFromSurface(rend, surface);

    SDL_FreeSurface(surface);
    if (!tex1 || !tex1) // fixa
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // struct to hold the position and size of the sprite
    SDL_Rect rectangle[CLIENTS];

    // get and scale the dimensions of texture
    SDL_QueryTexture(tex1, NULL, NULL, &rectangle[0].w, &rectangle[0].h);
    rectangle[0].w /= 4;
    rectangle[0].h /= 4;
    rectangle[1].h = rectangle[0].h;
    rectangle[1].w = rectangle[0].w;    
    rectangle[2].h = rectangle[0].h;
    rectangle[2].w = rectangle[0].w;    
    rectangle[3].h = rectangle[0].h;
    rectangle[3].w = rectangle[0].w;    

    // start sprite in center of screen
    float x_pos = (WINDOW_WIDTH - rectangle[0].w) / 2;
    float y_pos = (WINDOW_HEIGHT - rectangle[0].h) / 2;
    float x_posOld = x_pos;
    float y_posOld = y_pos;
    float x_vel = 0;
    float y_vel = 0;
    rectangle[0].y = 0;
    rectangle[0].x = 0;
    rectangle[1].y = 100;
    rectangle[1].x = 100;
    rectangle[2].y = 200;
    rectangle[2].x = 300;
    rectangle[3].y = 400;
    rectangle[3].x = 400;

    // keep track of which inputs are given
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    // set to 1 when window close button is pressed
    int close_requested = 0;
    
    
    // animation loop
    while (!close_requested)
    {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close_requested = 1;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }

        // determine velocity
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;
        if (left && !right) x_vel = -SPEED;
        if (right && !left) x_vel = SPEED;

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // Send, recieve
        if(x_posOld != x_pos || y_posOld != y_pos){
            printf("%d %d\n", (int) x_pos, (int) y_pos);
            client_data.player[client_ID].x=x_pos;
            client_data.player[client_ID].y=y_pos;
            client_data.client_ID=client_ID;
            memcpy(packet_sending->data,&client_data,sizeof(Data));
            packet_sending->len=sizeof(Data);
            packet_sending->address.host=server_address.host;
		    packet_sending->address.port=server_address.port;
            SDLNet_UDP_Send(socket,-1,packet_sending);
            x_posOld = x_pos;
            y_posOld = y_pos;
        }      
        if (SDLNet_UDP_Recv(socket,packet_recieving)){
            memcpy(&server_data,packet_recieving->data,sizeof(Data));
            for(int i=0;i<CLIENTS;i++){
                if(i!=client_ID){
                    rectangle[i].x=server_data.player[i].x;
                    rectangle[i].y=server_data.player[i].y;
                }
            }
            printf("UDP Packet recieved.\n");
        }

    

        // collision detection with bounds
        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= WINDOW_WIDTH - rectangle[client_ID].w) x_pos = WINDOW_WIDTH - rectangle[client_ID].w;
        if (y_pos >= WINDOW_HEIGHT - rectangle[client_ID].h) y_pos = WINDOW_HEIGHT - rectangle[client_ID].h;

        // set the positions in the struct
        rectangle[client_ID].y = (int) y_pos;
        rectangle[client_ID].x = (int) x_pos;
        
        // clear the window
        SDL_RenderClear(rend);
        //printf("1-%d:%d 2-%d:%d\n",dest.x, dest.y, secondDest.x, secondDest.y);
        // draw the image to the window
        SDL_RenderCopy(rend, tex1, NULL, &rectangle[0]);
        SDL_RenderCopy(rend, tex2, NULL, &rectangle[1]);
        SDL_RenderCopy(rend, tex2, NULL, &rectangle[2]);
        SDL_RenderCopy(rend, tex2, NULL, &rectangle[3]);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }
    
    // clean up resources before exiting
    SDL_DestroyTexture(tex1);
    SDL_DestroyTexture(tex2);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
