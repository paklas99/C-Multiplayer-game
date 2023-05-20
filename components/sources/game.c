#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include "../interfaces/game.h"
#include "../interfaces/textures.h"
#include "../interfaces/player.h"
#include "../interfaces/collision_detection.h"
#include "../interfaces/interactable_items.h"
#include "../interfaces/points.h"
#include "../interfaces/text.h"
#include "../interfaces/network_UDP.h"
#include "../interfaces/network_TCP.h"
#include "../interfaces/map.h"
#include "../interfaces/menu.h"

int thread_UDP_recieve(void* tdur){
    Thread_data_UDP_recieve* p=tdur;
    SDL_SpinLock lock=0;
    while(!p->terminate){
		if(SDLNet_UDP_Recv(p->socket,p->packet)){
		    SDL_AtomicLock(&lock);
            memcpy(p->data,p->packet->data,p->packet->len);
		    SDL_AtomicUnlock(&lock);
        }
        SDL_Delay(1);
    }
    printf("Thread terminated.\n");
    return 0;
}

struct Game_type{
    Map maps[MAPS];
    Player players[PLAYERS];
    Text player_score[PLAYERS];
    Text win_message;
    Text exit_game;
    Interactable_items coins[COINS];
    Object boost;
    bool return_to_menu;
    bool quit;
}Game_type;

//The music that will be played
Mix_Chunk *pickaudio   = NULL;
Mix_Chunk *dropaudio   = NULL;
Mix_Chunk *point       = NULL; 
Mix_Chunk *Select_Game = NULL;

PUBLIC Game new_Game(SDL_Renderer* renderer, Textures textures, bool multiplayer, Menu menu){
    
    Network_UDP network_UDP=NULL;
    Network_TCP network_TCP=NULL;
    int client=0;
    Thread_data_UDP_recieve tdur;
    SDL_Thread* start_thread_UDP_recieve=NULL;
    SDL_SpinLock lock=0;
    if(multiplayer){
        network_UDP=new_Network_UDP(menu);
        network_TCP=new_Network_TCP();
        client=get_client_ID(network_UDP);
        tdur=get_thread_data_UDP(network_UDP);
        start_thread_UDP_recieve=SDL_CreateThread(thread_UDP_recieve,"Thread: UDP recieve",(void*)&tdur);
    }
    
    printf("\n---GAME---\n");
    Game game= malloc(sizeof(Game_type));
    game->return_to_menu=false;
    game->quit=false;
    if(multiplayer && network_quit(network_UDP)){
        game->quit=true;
    }

    game->maps[0]=new_Map(renderer,textures);
    game->players[0]=new_Player(renderer,get_texture(textures,TEXTURES_PLAYER_1),SP_P1_X,SP_P1_Y,12);
    game->players[1]=new_Player(renderer,get_texture(textures,TEXTURES_PLAYER_2),SP_P2_X,SP_P2_Y,12);
    game->players[2]=new_Player(renderer,get_texture(textures,TEXTURES_PLAYER_3),SP_P3_X,SP_P3_Y,12);
    game->players[3]=new_Player(renderer,get_texture(textures,TEXTURES_PLAYER_4),SP_P4_X,SP_P4_Y,12);
    game->coins[0]=new_Interactable_Item(renderer,get_texture(textures,TEXTURES_COINS_BITCOIN),SP_C0_X,SP_C0_Y);
    game->coins[1]=new_Interactable_Item(renderer,get_texture(textures,TEXTURES_COINS_ETHEREUM),SP_C1_X,SP_C1_Y);
    game->coins[2]=new_Interactable_Item(renderer,get_texture(textures,TEXTURES_COINS_DOGE), SP_C2_X,SP_C2_Y);
    game->boost=new_Object(renderer,get_texture(textures,TEXTURES_SPEED_BOOST_UP),SP_BOOST_X,SP_BOOST_Y);
    game->win_message=new_Text(renderer,"",250,250,40);
    game->exit_game=new_Text(renderer,"Esc - Exit",SP_EXIT_X,SP_EXIT_Y,30);


    bool upload_ready=0;
    bool upload_punched_ready=0;

    //init av variabler i severn
    if(multiplayer){
        upload_boost_position(network_UDP,0,get_object_x(game->boost),get_object_y(game->boost));
        upload_punched(network_UDP,-1);
        upload_points(network_UDP, 0);
        upload_got_hit(network_UDP, 0);
        upload_item_position(network_UDP, 0, SP_C0_X, SP_C0_Y);
        upload_item_position(network_UDP, 1, SP_C1_X, SP_C1_Y);
        upload_item_position(network_UDP, 2, SP_C2_X, SP_C2_Y);
        upload_player_frame(network_UDP, 7);

        switch(client){
            case 0:
                upload_player_position(network_UDP, SP_P1_X, SP_P1_Y);
                break;
            case 1:
                upload_player_position(network_UDP, SP_P2_X,SP_P2_Y);
                break;
            case 2:
                upload_player_position(network_UDP, SP_P3_X,SP_P3_Y);
                break;
            case 3:
                upload_player_position(network_UDP, SP_P4_X,SP_P4_Y);
                break;
        }
        upload_ready=1;

    }

    for(int i=0; i <COINS; i++){
        set_item_width_height(game->coins[i], 20, 20);
    }

    int frame_time=0, sprite_number=0, up_frame_begin=9, down_frame_begin=6, left_frame_begin=3, right_frame_begin=0, right_fight_begin=12, left_fight_begin=15, knocked_frame_begin=18;
    int who_scored=-1;
    bool game_over = false, update_coin=0;
    int old_points_player[CLIENTS]={0};

    pickaudio = Mix_LoadWAV( "resources/Music/Audio/pick_up.mp3" );
    if ( pickaudio == NULL )
    {
        printf("Failed to load effect sound! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }
    dropaudio = Mix_LoadWAV( "resources/Music/Audio/drop.wav" );
    if ( dropaudio == NULL )
    {
        printf("Failed to load effect sound! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }
    point = Mix_LoadWAV( "resources/Music/Audio/point_sound.wav" );
    if ( point == NULL )
    {
        printf("Failed to load point effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }
    Select_Game = Mix_LoadWAV( "resources/Music/Audio/menu_select.wav");
    if ( Select_Game == NULL)
    {
        printf("Failed to load effect sound! SDL_mixer Error: %s\n", Mix_GetError() );
        return NULL;
    }


    // Sprite position and velocity
    int x_pos = get_player_position_x(game->players[client]);
    int y_pos = get_player_position_y(game->players[client]);
    int x_posOld=x_pos;
    int y_posOld=y_pos;
    int x_vel = 0;
    int y_vel = 0;
    int y_vel_old=0;
    int x_vel_old=0;
    int old_frame=7;


    // Input
    int up=0;
    int down=0;
    int left=0;
    int right=0;
    bool pick=0;
    bool drop=0;
    int punched_counter=0;
    int hit=0;
    int old_got_hit=0;
    int back_to_normal=0;
    int lost_coin_index=-1;
    int old_hit=0;
    int stolen_coin=-1;

    char the_message[40]; //local string to hold temp. message for print
    //init the players scores on screen
    //printf("init players scores\n");
    for(int i=0;i<PLAYERS;i++){
        set_player_points(game->players[i], 0);
        sprintf(the_message,"PLAYER %d: %d",i+1, 0);
        game->player_score[i]=new_Text(renderer,the_message,838,45+i*97,23);
    }
    //printf("event loop\n");
    // Game-loopen/Animeringsloopen
    SDL_Event event;
    int mouse_x, mouse_y;
    while(!game->return_to_menu && !game->quit){
        while (SDL_PollEvent(&event)){
            SDL_GetMouseState(&mouse_x,&mouse_y);
            switch (event.type){
                case SDL_QUIT:game->quit=true;break;if(multiplayer)close_recieve_thread(&tdur);break;
                case SDL_MOUSEBUTTONDOWN: 
                if( mouse_x > 840 && mouse_x < 840 + 115 && mouse_y > 700 && mouse_y < 700 + 30){
                    if(menu_mute(menu)) Mix_PlayChannel(-1, Select_Game, 0);
                    game->return_to_menu=true;break;
                }
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:game->return_to_menu=true;if(multiplayer)close_recieve_thread(&tdur);break;
                        case SDL_SCANCODE_W:up=-1; break;
                        case SDL_SCANCODE_UP:up=-1; break;
                        case SDL_SCANCODE_S:down=1; break;
                        case SDL_SCANCODE_DOWN:down=1; break;
                        case SDL_SCANCODE_A:left=-1; break;
                        case SDL_SCANCODE_LEFT:left=-1; break;
                        case SDL_SCANCODE_D:right=1; break;
                        case SDL_SCANCODE_RIGHT:right=1; break;
                        case SDL_SCANCODE_E:
                        if(menu_mute(menu)) Mix_PlayChannel(-1, pickaudio, 0);          
                        pick=true; 
                        break;
                        case SDL_SCANCODE_G:
                        if(menu_mute(menu)) Mix_PlayChannel(-1, dropaudio, 0);
                        drop=true; 
                        break;
                        case SDL_SCANCODE_SPACE: hit=1;break;
                    }
                break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode){
                        case SDL_SCANCODE_W:up=0; break;
                        case SDL_SCANCODE_UP:up=0; break;
                        case SDL_SCANCODE_S:down=0; break;
                        case SDL_SCANCODE_DOWN:down=0; break;
                        case SDL_SCANCODE_A:left=0; break;
                        case SDL_SCANCODE_LEFT:left=0; break;
                        case SDL_SCANCODE_D:right=0; break;
                        case SDL_SCANCODE_RIGHT:right=0; break;
                        case SDL_SCANCODE_SPACE: hit=0; break;

                    }
                break;
            } 
        }

        // init velocity
        x_vel = y_vel = 0;
        
        for(int i; i<CLIENTS; i++){
        old_points_player[i]=get_player_points(game->players[i]);
        }
        old_frame=get_frame(game->players[client]);
        // If got hit
        for(int i=0;i<CLIENTS;i++){
            if(get_player_punched(game->players[i])==client){
                set_player_got_hit(game->players[client], 1);
                if(get_has_coin_index(game->players[client])>-1){
                    set_item_position( game->coins[get_has_coin_index(game->players[client])],get_player_position_x(game->players[i]), get_player_position_y(game->players[i])+30 );
                    stolen_coin=get_has_coin_index(game->players[client]);
                    set_has_coin_index(game->players[client],-1);
                }
                back_to_normal=1;
            }
        }
        if((get_player_got_hit(game->players[client]))!=1){
            if(hit && right){
                sprite_regulator(game->players[client], &frame_time, &sprite_number, right_fight_begin);
            }
            if(hit && left){
                sprite_regulator(game->players[client], &frame_time, &sprite_number, left_fight_begin);
            }
            //up
            if (up && !down && !right && !left){
                y_vel = -get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, up_frame_begin);
            }
            //down
            if (down && !up && !right && !left){
                y_vel = get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, down_frame_begin);
            }
            //left
            if (left && !right && !up && !down){
                x_vel = -get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, left_frame_begin);
            }
            //right
            if (right && !left && !down && !up){
                x_vel = get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, right_frame_begin);
            }
            //diagonal down&right
            if (down && right && !up && !left){
                y_vel = get_player_speed(game->players[client]);
                x_vel = get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, right_frame_begin);
            }
            //diagonal down&left
            if (down && left && !up && !right){
                y_vel = get_player_speed(game->players[client]);
                x_vel = -get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, left_frame_begin);
            }
            //diagonal up&right
            if (up && right && !left && !down){
                y_vel = -get_player_speed(game->players[client]);
                x_vel = get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, right_frame_begin);
            }
            //diagonal up&left
            if (up && left && !down && !right){
                y_vel = -get_player_speed(game->players[client]);
                x_vel = -get_player_speed(game->players[client]);
                sprite_regulator(game->players[client], &frame_time, &sprite_number, left_frame_begin);
            }

            // update positions
            x_pos += x_vel / 60;
            y_pos += y_vel / 60;
        }

        else{
            punched_counter++;
            sprite_regulator(game->players[client], &frame_time, &sprite_number, knocked_frame_begin);
            if(punched_counter==100){
                set_player_got_hit(game->players[client], 0);
                set_frame(game->players[client], down_frame_begin+1);
                back_to_normal=1;
                punched_counter=0;
            }
        }

        // set the positions in the struct and check for object collision, if collision is detected, save the newly calculated 
        // velocity and positions for use in the next loop.
        set_player_position_y(game->players[client], y_pos);
    
        y_vel=check_map_collision_Y(game->players[client], y_pos, y_vel);
        for(int i=0;i<CLIENTS;i++){
            if(i!=client){
                y_vel=check_player_collision_Y(game->players[client], game->players[i], y_pos, y_vel, y_vel_old);
            }
        }
        for(int i=0;i<get_map_number_of_walls(game->maps[0]);i++){
            y_vel=check_wall_collision_Y(game->players[client],get_map_wall(game->maps[0],i), y_pos, y_vel, y_vel_old);
        }
        y_pos=get_player_position_y(game->players[client]);
        // X axis
        set_player_position_x(game->players[client], x_pos);
        // If player clicked Space_bar and punched someone else.
        if(old_hit==0 && hit==1){
            for(int i=0;i<CLIENTS;i++){
                if(i!=client){
                    if(check_hit_collsion(game->players[client], game->players[i])){ //if collision and player dont have coin
                        set_player_punched(game->players[client], i);
                        upload_punched_ready=1;
                        old_hit=0;
                    }    
                }
            }
        }
        else{
            if(old_hit==1 && hit==1){
                set_player_punched(game->players[client], -1);
                upload_punched_ready=1;
            }
        }
        old_hit=hit;
        x_vel=check_map_collision_X(game->players[client], x_pos, x_vel);
        for(int i=0;i<CLIENTS;i++){
            if(i!=client){
                x_vel=check_player_collision_X(game->players[client], game->players[i], x_pos, x_vel, x_vel_old);
            }
        }
        for(int i=0;i<get_map_number_of_walls(game->maps[0]);i++){
            x_vel=check_wall_collision_X(game->players[client],get_map_wall(game->maps[0],i), x_pos, x_vel, x_vel_old);
        }
        x_pos=get_player_position_x(game->players[client]);
        // Saves old velocity
        if(y_vel_old!=y_vel){
            y_vel_old=y_vel;
        }
        if(x_vel_old!=x_vel){
            x_vel_old=x_vel;
        }
        
         //check collision player and bitcoin  (0), ethereum (1), doge (2)
        for(int i=0; i<COINS;i++){
            check_collision_player_coin(game->players[client],i, game->coins[i],get_item_position_x(game->coins[i]),get_item_position_y(game->coins[i]),get_player_position_x(game->players[client]),get_player_position_y(game->players[client]),drop, &pick);
        }
        int winning_coin=-1;
        for(int i=0;i<COINS;i++){
            if(get_has_coin_index(game->players[client])==i){
                who_scored = check_points(i, game->coins[i], game->players[0], game->players[1], game->players[2], game->players[3], &winning_coin);
                if ( who_scored > -1 ){
                    if ( menu_mute(menu) ){
                        Mix_PlayChannel(-1, point, 0);
                    }
                }
            }
        }

        // set both drop and pick to false if coin don't collide with the player 
         if (!check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[0])) &&
            !check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[1])) &&
            !check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[2]))){

            pick=false;
            drop=false;
        }
        
        // set both drop & pick to false if pick and drop 
        if (check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[0])) &&
            check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[1])) &&
            check_Collision(get_player_rectangle(game->players[client]), get_interactable_item(game->coins[2]))){
            if (drop == true && pick == true){
                drop = false;
                pick = false;
            }
        }
        // Who wins???
        for(int i=0;i<CLIENTS;i++){
            if(win_condition(renderer,get_player_points(game->players[i]),i)){
                sprintf(the_message,"PLAYER %d WINS!",i+1);
                game->win_message=new_Text(renderer,the_message,250,250,40);
                game_over = true;
            }
        }
        
        bool upload_ready=0;

        //Network - Send, recieve..
        if(multiplayer){

            if((abs(x_pos-x_posOld)>JITTER || abs(y_pos-y_posOld)>JITTER)){
                upload_player_position(network_UDP,(int)x_pos,(int)y_pos);
                upload_has_coin_index(network_UDP, get_has_coin_index(game->players[client]));
                upload_boost_position(network_UDP,0,get_object_x(game->boost),get_object_y(game->boost));
                x_posOld=x_pos;y_posOld=y_pos;
                upload_ready=1;
            }


            if(old_frame!=get_frame(game->players[client])){
                upload_player_frame(network_UDP, get_frame(game->players[client]));
                upload_ready=1;
            }

            for(int i=0;i<COINS;i++){
                if( (get_has_coin_index(game->players[client]))==i || stolen_coin==i){
                    upload_item_position(network_UDP,i,get_item_position_x(game->coins[i]),get_item_position_y(game->coins[i]));
                    if(get_has_coin_index(game->players[client])==winning_coin){
                        upload_points(network_UDP,get_player_points(game->players[client]));
                    }
                    upload_ready=1;
                }
            }

            if(upload_punched_ready){
                upload_punched(network_UDP, get_player_punched(game->players[client]));
                upload_punched_ready=0;
                upload_ready=1;
            }

            if(back_to_normal==1 || (old_got_hit==0 && get_player_got_hit(game->players[client])==1)){
                upload_got_hit(network_UDP, get_player_got_hit(game->players[client]));
                back_to_normal=0;
                upload_ready=1;
            }
            old_got_hit = get_player_got_hit(game->players[client]);


            if(check_boost_collision(game->players[client], game->boost)){
                    upload_boost_position(network_UDP,0,get_object_x(game->boost),get_object_y(game->boost));
                    upload_ready=1;
            }
            else{
                set_object_position(game->boost,download_boost_position_x(network_UDP,0),download_boost_position_y(network_UDP,0));
                upload_boost_position(network_UDP,0,get_object_x(game->boost),get_object_y(game->boost));
            }

            if(upload_ready){
                upload(network_UDP);
                upload_ready=0;
            }

            for(int i=0;i<CLIENTS;i++){
                if(i!=client){
                    set_player_position_x(game->players[i],download_player_position_x(network_UDP,i));
                    set_player_position_y(game->players[i],download_player_position_y(network_UDP,i));
                    set_frame(game->players[i], download_player_frame(network_UDP, i));
                    set_player_punched(game->players[i], download_punched(network_UDP, i));
                    set_object_position(game->boost,download_boost_position_x(network_UDP,0),download_boost_position_y(network_UDP,0));
                    set_player_points(game->players[i], download_points(network_UDP, i));
                    set_has_coin_index(game->players[i], download_has_coin_index(network_UDP, i));
                }
            }
            for(int i=0;i<COINS;i++){
                if( (get_has_coin_index(game->players[client])!=i) ){
                    set_item_position(game->coins[i],download_item_position_x(network_UDP,i),download_item_position_y(network_UDP,i));
                }
            }
        }
        // update player scoreboard
        for(int i=0; i<CLIENTS; i++){
            if(old_points_player[i]!=get_player_points(game->players[i])){
                sprintf(the_message,"PLAYER %d: %d",i+1, get_player_points(game->players[i]));
                game->player_score[i]=new_Text(renderer,the_message,838,45+i*97,23);
                old_points_player[i]=get_player_points(game->players[i]);
            }
        }
        // reset has_coin & stolen_coin
        if(get_has_coin_index(game->players[client])==winning_coin){
            set_has_coin_index(game->players[client], -1);
        }
        if(stolen_coin>-1){
            stolen_coin=-1;
            upload_has_coin_index(network_UDP, -1);
            upload_ready=1;
        }

        // RENDER
        SDL_RenderClear(renderer);
        render_map(game->maps[0]);
        render_text(game->win_message);
        for(int i=0;i<PLAYERS;i++){
            render_player(game->players[i]);

            render_text(game->player_score[i]);
        }
        render_interactable_item(game->coins[0]);
        render_interactable_item(game->coins[1]);
        render_interactable_item(game->coins[2]);
        render_object(game->boost);
        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY);
        
        // Game Over
        if(game_over){
            game->return_to_menu=true;
            close_recieve_thread(&tdur);
            SDL_Delay(2000);
        }
    }
    if(multiplayer)destroy_Network(network_UDP);
    return game;

}

PUBLIC void destroy_Game(Game game){
    free(game);
    printf("Game memory freed.\n");
}
PUBLIC void destroy_game_audio(){
    Mix_FreeChunk(pickaudio);
    Mix_FreeChunk(dropaudio);
    Mix_FreeChunk(point);
    Mix_FreeChunk(Select_Game);
    pickaudio = NULL;
    dropaudio = NULL;
    point = NULL;
    Select_Game = NULL;
    Mix_Quit();
    printf("Game music memory freed.\n");
}
PUBLIC bool game_quit(Game game){
    return game->quit;
}

PRIVATE void sprite_regulator(Player player, int *frame_time, int *sprite_number, int frame_begin){
    int frame_order[]={0,1,2,1};
    (*frame_time)++;
    if((FPS/(*frame_time))<=10){  // Render x times per second
        (*frame_time)=0;
        if(*sprite_number>=4) *sprite_number=0;
        set_frame(player, frame_begin+frame_order[*sprite_number]);
        (*sprite_number)++;
    }
}