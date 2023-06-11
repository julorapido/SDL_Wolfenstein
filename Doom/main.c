//
//  SDL2 Doom
//  Julorapido 2023
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define PI 3.1415926545

typedef uint32_t u32;
typedef float    f32;

typedef struct vi_s { int x, y; } vi;
typedef struct v2_s { f32 x, y; } v2;
typedef uint16_t u16;

const int width = 512;
const int height = 512;
const int map_rw = 8;
const int map[64] = {
    1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    0,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture, *debug;
    u32 *pixels;
    bool quit;

    //struct { struct sector arr[32]; usize n; } sectors;
    //struct { struct wall arr[128]; usize n; } walls;
    //u16 y_lo[SCREEN_WIDTH], y_hi[SCREEN_WIDTH];

    struct {
        //v2 pos;  RECT POS IS INT (NOT FLOAT)
        vi pos;
        v2 delta_pos;
        f32 angle, anglecos, anglesin;
        int sector;
    } camera;

    bool sleepy;
} state;

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
    SDL_SetRenderDrawColor(rendr, 100, 100, 100, 255);
}

void renderPlayr(SDL_Renderer* rendr, SDL_Rect rect){
    SDL_SetRenderDrawColor(state.renderer, 40, 0, 255, 255);
    SDL_RenderDrawLine(rendr, rect.x, rect.y,  (state.camera.delta_pos.x *  rect.x)- 10, (state.camera.delta_pos.y * rect.y)- 10);
    // raycast
    int r, mx, my, mp, dof;
    float rx, ry, ra, x_offst, y_offst;
    ra = state.camera.angle;
    for (int r = 0; r < 1; r ++){
        //Horizontal - lines
        dof = 0;
        float inv_Tan = -1/tan(ra); // look down
        // RX AND RY   => NEAREST DRAWED LINE POSITION <=
        if(ra > PI){ ry= ((( (int)state.camera.pos.y>>6)<<6)-0.0001);// looking downward // SHIFT 6 * 6 VALUES BECAUSE UNIT IS 64 (512 x 512 sdl window) !!!
            rx = (state.camera.pos.y - ry) * inv_Tan + state.camera.pos.x;
            y_offst = -(height / map_rw); x_offst = -y_offst*inv_Tan;// 512 / 8 = 64
        }
        if(ra <  PI){ ry= (( (int)state.camera.pos.y>>6)<<6) + 64;// looking upward
            rx = (state.camera.pos.y - ry) * inv_Tan + state.camera.pos.x;
            y_offst = height / map_rw; x_offst = -y_offst*inv_Tan; // 512 / 8 = 64
        }
        if (ra == 0 || ra == PI || ra == 3.1415926545){rx = state.camera.pos.x; ry = state.camera.pos.y; dof = 8;} // lookgin straight left or right
       // while (dof < 8) {
            //
      //  }
    }
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
        state.window = SDL_CreateWindow("Doom", SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED ,width, height, SDL_WINDOW_SHOWN);
        state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if( state.window == NULL ){printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );}else{
            
            //Get window surface
            //screenSurface = SDL_GetWindowSurface( window );
            //SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ));
            //SDL_UpdateWindowSurface(window);

            SDL_Rect p_rect;
            p_rect.x = 250;p_rect.y = 150;
            p_rect.w = 10;p_rect.h = 10;
            
            // BG Clr
            SDL_SetRenderDrawColor(state.renderer, 100, 100, 100, 255);

            SDL_Event e; bool quit = false;
            bool drwd = false;
            while(!quit){
                /// Clear Screen
                SDL_RenderClear(state.renderer);
            
                state.camera.pos.x =  p_rect.x;
                state.camera.pos.y =  p_rect.y;
                
                // DRAW MAP
                renderMap(state.renderer);

                // draw plyr
                renderPlayr(state.renderer, p_rect);
                
                // Modify Color & Draw Player Rectangle & line
                SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
                SDL_RenderDrawRect(state.renderer, &p_rect);
                SDL_RenderFillRect(state.renderer, &p_rect);
                
                // BG clr
                SDL_SetRenderDrawColor(state.renderer, 100, 100, 100, 255);
                
                // Show whats drawn
                SDL_RenderPresent(state.renderer);
                
                // key movs <= =>
                while(SDL_PollEvent(&e) ){
                    if(e.type == SDL_QUIT) {quit = true;}
                    if (e.type == SDL_KEYDOWN){
                      //  printf("%c",  e.key.keysym.sym );
                        switch( e.key.keysym.sym ){
                               case SDLK_LEFT:
                                   //printf("left \n");
                                   // rotate
                                   state.camera.angle += 0.1;
                                   if(state.camera.angle > 2 * PI){state.camera.angle -= 2* PI;}
                                   state.camera.delta_pos.x = cos(state.camera.angle) * 5; state.camera.delta_pos.y = sin(state.camera.angle) * 5;
                                   printf("%f \n ", state.camera.angle);
                                   break;
                               case SDLK_RIGHT:
                                  // printf("right \n");
                                   state.camera.angle -= 0.1;
                                   if(state.camera.angle < 0){state.camera.angle += 2* PI;}
                                   state.camera.delta_pos.x = cos(state.camera.angle) * 5; state.camera.delta_pos.y = sin(state.camera.angle) * 5;
                                   break;
                               case SDLK_UP:
                                 //  printf("up \n");
                                   //p_rect.y -= 2;
                                    p_rect.x += state.camera.delta_pos.x;
                                    p_rect.y += state.camera.delta_pos.y;
                                   break;
                               case SDLK_DOWN:
                                 //  printf("down \n");
                                   //p_rect.y += 2;
                                    p_rect.x -= state.camera.delta_pos.x;
                                    p_rect.y -= state.camera.delta_pos.y;
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
        SDL_DestroyWindow(state.window);
        SDL_Quit();
    }

    printf("Hello, Doom!\n");
    return 0;
}
