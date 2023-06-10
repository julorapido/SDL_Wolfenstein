//
//  main.c
//  Doom
//
//  Created by Jules Sainthorant on 09/06/2023.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

const int width = 800;
const int height = 600;

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

            SDL_Rect rect;
            rect.x = 250;rect.y = 150;
            rect.w = 200;rect.h = 200;
         
            SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);

            // point
            //SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            SDL_Event e; bool quit = false;
            while(!quit){
                /// Clear Screen
                SDL_RenderClear(renderer);
            
                // Modify Color & Draw Rectangle
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderDrawRect(renderer, &rect);
                
                SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255); 
                
                // Show whats drawn
                SDL_RenderPresent(renderer);
                while(SDL_PollEvent(&e) ){
                    if(e.type == SDL_QUIT) {quit = true;} if (e.type == SDL_KEYDOWN){quit = true;}
                    if (e.type == SDL_MOUSEBUTTONDOWN){quit = true;}
            }}
            
        }
        SDL_Delay(2000);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    printf("Hello, World!\n");
    return 0;
}
