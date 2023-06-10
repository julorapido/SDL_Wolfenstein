//
//  SDL2 Doom
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

const int width = 1024;
const int height = 728;
const int map_rw = 8;
const int map[64] = {
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void renderMap(SDL_Renderer* rendr){
    int map_i = 0;
    for(int x = 0; x < map_rw; x ++){
        const int x_l = width / map_rw; const int y_l = height / map_rw;
        int x_p =  (x * x_l);
        for (int y = 0; y < map_rw; y ++){
            int y_p = (y * y_l);
            
            //stroke color
            SDL_SetRenderDrawColor(rendr, 20, 20, 20, 255);
            SDL_Rect map_rect;
            map_rect.x = x_p ; map_rect.y = y_p;
            map_rect.w = x_l - 1; map_rect.h = y_l - 1;
            if (map_rect.h == 0){
                printf("map rect bug %s \n", SDL_GetError());
            }
            SDL_RenderDrawRect(rendr, &map_rect);
            
            //square conditionnal fill color
            //printf("%d", map_i);
            if(map[map_i] == 0){SDL_SetRenderDrawColor(rendr, 240, 240, 240, 255);}else{
                SDL_SetRenderDrawColor(rendr, 0, 0, 0, 255);
            }
            SDL_RenderFillRect(rendr, &map_rect);
            
           // printf("x: %d, y :%d , width: %d, \n", x_p, y_p, x_l);
            map_i++;
        }
    }
    SDL_SetRenderDrawColor(rendr, 0, 130, 0, 255);
}
int main(int argc, const char * argv[]) {

    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }else{
        //SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Doom", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED ,width, height, SDL_WINDOW_SHOWN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if( window == NULL ){printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );}else{
            
            //Get window surface
            //screenSurface = SDL_GetWindowSurface( window );
            //SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ));
            //SDL_UpdateWindowSurface(window);

            SDL_Rect p_rect;
            p_rect.x = 250;p_rect.y = 150;
            p_rect.w = 50;p_rect.h = 50;
            SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);
            
            SDL_Event e; bool quit = false;
            bool drwd = false;
            while(!quit){
                /// Clear Screen
                SDL_RenderClear(renderer);
            
                // Modify Color & Draw Player Rectangle
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawRect(renderer, &p_rect);
                SDL_RenderFillRect(renderer, &p_rect);
                // Green clr
                SDL_SetRenderDrawColor(renderer, 20, 180, 0, 255);
     
                // DRAW MAP
                renderMap(renderer);
                
                // Show whats drawn
                SDL_RenderPresent(renderer);
                
                // key movs <= =>
                while(SDL_PollEvent(&e) ){
                    if(e.type == SDL_QUIT) {quit = true;}
                    if (e.type == SDL_KEYDOWN){
                      //  printf("%c",  e.key.keysym.sym );
                        switch( e.key.keysym.sym ){
                               case SDLK_LEFT:
                                   //printf("left \n");
                                   p_rect.x -= 14;
                                   break;
                               case SDLK_RIGHT:
                                  // printf("right \n");
                                   p_rect.x += 14;
                                   break;
                               case SDLK_UP:
                                 //  printf("up \n");
                                   p_rect.y -= 14;
                                   break;
                               case SDLK_DOWN:
                                 //  printf("down \n");
                                   p_rect.y += 14;
                                   break;
                               default:
                                   break;
                        }
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN){quit = false;}
                }

            }
            
        }
        SDL_Delay(2000);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    printf("Hello, Doom!\n");
    return 0;
}
