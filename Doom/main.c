//
//  SDL2 Doom
//  Julorapido 2023
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define TPI 3.1415926545 * 2
#define PI 3.1415926545
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533  // degree in radians
typedef uint32_t u32;
typedef float    f32;

typedef struct vi_s { int x, y; } vi;
typedef struct v2_s { f32 x, y; } v2;
typedef uint16_t u16;

float FixAng(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}
float degToRad(float a) { return a*M_PI/180.0;}
float radToDeg(float a) { return a*180/PI;}


const int width = 512;
const int height = 512;
const int map_rw = 8;
// 1 for normal wall
// 2 for big textured wall
// 3 for door wall
const int map[64] = {
    2,2,2,2,2,2,2,2,
    2,1,0,0,0,0,2,2,
    2,0,1,0,0,0,0,2,
    2,0,0,0,0,0,0,2,
    2,0,1,0,0,0,0,2,
    1,0,0,1,0,1,0,2,
    2,0,0,0,0,0,1,2,
    1,1,1,2,2,2,2,2,
};
int All_Textures[]= //all 32x32 textures
{
 //Checkerboard
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
//Brick
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 //Window
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
       
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
       
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 //Door
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,

 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,1, 1,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,1,1,1,1,0,1, 1,0,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
};

static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    SDL_Window *doom_window;
    SDL_Renderer *doom_renderer;
    
    //The surface contained by the window
    SDL_Surface* gScreenSurface;
    //cool background
    SDL_Surface* s_mountains;
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
            if(map[(map_rw * y )+ x] == 0){SDL_SetRenderDrawColor(rendr, 240, 240, 240, 255);}else{
                SDL_SetRenderDrawColor(rendr, 0, 0, 0, 255);
            }
            SDL_RenderFillRect(rendr, &map_rect);
            
           // printf("x: %d, y :%d , width: %d, \n", x_p, y_p, x_l);
            map_i++;
        }
    }
    SDL_SetRenderDrawColor(rendr, 100, 100, 100, 255);
}

float raydist(float r_x , float r_y, float ang){
    double abs_wth = fabsf(r_x - state.camera.pos.x );
    double abs_hgth = fabsf(r_y - state.camera.pos.y );
    
    return (sqrt( (abs_wth) * (abs_wth) + (abs_hgth) * (abs_hgth ) ));
}

int ceiling_(void){
    SDL_SetRenderDrawColor(state.doom_renderer, 60, 60, 60, 255);

    for (int i = 256; i > 0; i --){
        SDL_SetRenderDrawColor(state.doom_renderer, 50 + i /3,  50 + i /3, 50 +  i /3, 255);
        SDL_RenderDrawLine(state.doom_renderer, -width/2,i , width, i);
    }
    SDL_SetRenderDrawColor(state.doom_renderer, 60, 60, 60, 255);
    return 0;
}

//void plyr_line_equation(SDL_Rect rect, SDL_Rect collision_rect){
    // float yA = (state.camera.delta_pos.y * rect.y); float xA = (state.camera.delta_pos.x * rect.x);
   // float yB = state.camera.pos.y;   float xB = state.camera.pos.x;
   // float coef = (yA - yB) / (xB - xA);
    // ya = coef * xA + b
    // b = ya - coef * xA
   // float p  = yA - (coef * xA);
    // y = coef(x) + p;
    // x = coef+ p / y;
   // collision_rect.x = 2*coef + p / yA;
    
//}
void renderPlayr(SDL_Renderer* rendr, SDL_Rect rect){
    SDL_SetRenderDrawColor(state.renderer, 40, 0, 255, 255);
    SDL_SetRenderDrawColor(state.doom_renderer, 60, 60, 60, 255);

    //SDL_RenderDrawLine(rendr, rect.x, rect.y,  (state.camera.delta_pos.x *  rect.x), (state.camera.delta_pos.y * rect.y));
    // raycast
    int r, mx, my, mp, dof;
    float rx, ry, ra, x_offst, y_offst, disT;
    ra = state.camera.angle - DR * 30; if(ra < 0){ra += 2 * PI;} if (ra > 2*PI ){ra -= 2*PI;}
    for (r = 0; r < 60; r ++){
        bool main_h_wall = false;
        //Horizontal - lines
        dof = 0;
        bool big_H_Wl = false;
        float h_rayLen = 10000000;
        float h_ray_x = state.camera.pos.x; float h_ray_y = state.camera.pos.y;
        float inv_Tan = -1/tan(ra); // look down
        // RX AND RY   => NEAREST DRAWED LINE POSITION <=
        if(ra > PI){ ry= ((( (int)state.camera.pos.y>>6)<<6)-0.0001);// looking downward // SHIFT 6 * 6 VALUES BECAUSE UNIT IS 64 (512 x 512 sdl window) !!!
            rx = (state.camera.pos.y - ry) * inv_Tan + state.camera.pos.x; //  y = (m * x )+ p
            y_offst = -(height / map_rw); x_offst = -y_offst*inv_Tan;// 512 / 8 = 64
        }
        if(ra <  PI){ ry= (( (int)state.camera.pos.y>>6)<<6) + 64;// looking upward
            rx = (state.camera.pos.y - ry) * inv_Tan + state.camera.pos.x;
            y_offst = height / map_rw; x_offst = -y_offst*inv_Tan; // 512 / 8 = 64
        }
        if (ra == 0 || ra == PI || ra == 3.1415926545){rx = state.camera.pos.x; ry = state.camera.pos.y; dof = 8;} // lookgin straight left or right
        while (dof < 8) {
            mx= (int)(rx)>>6; my = (int)(ry)>>6; mp = my * map_rw + mx;
            if (mp > 0 && mp < 64 && (map[mp] > 0) ){dof = 8;// hit wall
               // if( map[mp] == 2){big_H_Wl = true;}
                h_ray_x = rx; h_ray_y = ry; h_rayLen = raydist(rx, ry, ra);
            }
            else{
                rx += x_offst; ry += y_offst;
                dof++;
            }
        }
 
        
        //Vertical - lines
        bool big_V_Wl = false;
        dof = 0;
        float v_rayLen = 1000000;
        float v_ray_x = state.camera.pos.x; float v_ray_y = state.camera.pos.y;
        float d_Tan = -tan(ra);
        if(ra>P2&&ra<P3){// looking left
            rx = ((( (int)state.camera.pos.x>>6)<<6)-0.0001);
            ry = (state.camera.pos.x - rx) * d_Tan + state.camera.pos.y;//  y = (m * x )+ p
            x_offst = -(height / map_rw); y_offst = -x_offst*d_Tan;
        }
        if(ra<P2||ra>P3){// looking right
            rx = (( (int)state.camera.pos.x>>6)<<6) + 64;
            ry = (state.camera.pos.x - rx) * d_Tan + state.camera.pos.y;//  y = (m * x )+ p
            x_offst = height / map_rw; y_offst = -x_offst*d_Tan;
        }
        if (ra == 0 || ra == PI ){rx = state.camera.pos.x; ry = state.camera.pos.y; dof = 8;}
        while (dof < 8) {
            mx= (int)(rx)>>6; my = (int)(ry)>>6; mp = my * map_rw + mx;
            
            if (mp > 0 && mp < 64 && (map[mp] > 0) ){dof = 8;// hit wall
               // if( map[mp] == 2){big_V_Wl = true;}
                v_ray_x = rx; v_ray_y = ry; v_rayLen = raydist(rx, ry, ra);
            }
            
            else{// offset + 1
                rx += x_offst; ry += y_offst;
                dof++;
            }
        }
        
        
        // SHADERS
        bool is_dark = false; bool is_vert = false;
        if (h_rayLen < v_rayLen){rx = h_ray_x; ry =  h_ray_y; disT = h_rayLen;
            main_h_wall = big_H_Wl;
            SDL_SetRenderDrawColor(state.doom_renderer, 200, 30, 100, 255);
            is_dark = true;
        }
        
        if (v_rayLen < h_rayLen){rx = v_ray_x; ry =  v_ray_y; disT = v_rayLen;
            main_h_wall = big_V_Wl;
            is_vert = true;
            SDL_SetRenderDrawColor(state.doom_renderer, 230, 30, 100, 255);
            SDL_SetRenderDrawColor(state.renderer, 255, 30, 100, 255);
        }
        
        SDL_RenderDrawLine(rendr, rect.x + 3, rect.y + 3, rx, ry);
        // 3D Walls
        
        // FIX FISHEYE
        float ca = state.camera.angle - ra;  if(ca < 0){ca += 2 * PI;} if (ca > 2*PI ){ca -= 2*PI;}
        disT = disT * cosf(ca);
    
        float lineH = (map_rw * 3.5 * height) / disT;     /// LINE HEIGHT or wall height
        ///
        float text_yOff = 0;
        float prevOff = (height / 2.5) - ( lineH / 2);
        float wallHght = lineH*2 + prevOff - (main_h_wall ? (prevOff - lineH*2) :  prevOff);
        float tex_step = 32.0 / (float) wallHght;
        float tex_x_step = 32.0 / (float) (rx);

        ///
        if(lineH > height - 20){text_yOff = (lineH - (height - 20) ) / 2 ;lineH = height - 20;} // CLIP WALL HEIGHT
        float lineOffst = (height / 2.5) - ( lineH / 2); ////   LINE OFFSET
    
        //int splitted = width / 60 + 1;
        //float textureHeight =  (lineH*2+lineOffst) / (splitted  + 1);
        //printf("%d", map[mp]);
        //for (int p = 0; p < splitted + 1 ; p ++){
            
            // 9 by 9 approx
            // if(!is_dark){ SDL_SetRenderDrawColor(state.doom_renderer, 230 + p, 30, 100, 255);
            // }else{ SDL_SetRenderDrawColor(state.doom_renderer, 200 - p, 30, 100, 255); }
            
 
            //SDL_RenderDrawLine(state.doom_renderer,  r+p+(9*r), main_h_wall ? lineOffst - lineH*2 :  lineOffst,  r+p+(9*r),   lineH*2+lineOffst);
            
            //FLOOR
            //float floor__x = r+p+(9*r);
            //float floor_top_y =( main_h_wall ? lineOffst - lineH*2 : lineOffst); float floor_bottom_y = lineH*2+lineOffst;
                
        //}
        //float wallHght = lineH*2+lineOffst - (main_h_wall ? (lineOffst - lineH*2) :  lineOffst);
        
        int y; float text_y = text_yOff * tex_step  ;// float tex_step = 32.0 / (float) wallHght;
        int text_x = is_vert ?  (int) ((ry - 64 *  (int)(ry / 64)) / 2) : (int) ((rx - 64 *  (int)(rx / 64)) / 2);
        //printf("%d\n", text_x);
        if(ra > 180){text_x = 31 - text_x;}// flip textures
        //int text_skin = 0;
        //if(map[mp]){text_skin = map[mp] >= 0 && map[mp] <= 4 ? map[mp] : 0;}

        SDL_Rect line_rect; line_rect.w = 10; line_rect.h = 1; line_rect.x = r + (9 * r);
        for(y = main_h_wall ? lineOffst-lineH*2 :  lineOffst; y <  lineH*2+lineOffst; y ++){
             line_rect.y = y;
             int clr = All_Textures[(int) (text_y) * 32  + (text_x) + (1024)];
             SDL_SetRenderDrawColor(state.doom_renderer, 40, 30,  (clr == 1 ? 200 :  70) + (is_dark ? -20 : 30), 255);
             SDL_RenderDrawRect(state.doom_renderer, &line_rect); SDL_RenderFillRect(state.doom_renderer, &line_rect);
             text_y += tex_step;
        }
        
        //floors ray casting from :
        //
        // strght(dist plyr feets to P)     plyr height
        // ----------------------------- = -----------------------------------------------------
        //  dist from plyr to wl            #row at mid wall height - #row at Wall botom height
        //
        // to :
        //
        // strght(dist plyr feets to P) = (plyr height * dist from plyr to wl) / (#row at mid wall height - #row at Wall botom height)
        //
        //float P1 = height/2 * (disT) / ( (height/2) -  lineOffst);
        y = 0;
        line_rect.h = 1;
        for(y = lineH*2+lineOffst; y < height; y ++){
            line_rect.y = y;
            float r = (lineH*2+lineOffst - 0.8 * y) - (100) * 1.5f;
            float plyr_hght = 32.0;
            float strght_line_dist = (plyr_hght * disT )/ r;
            float beta = fabs(state.camera.angle - ra);
            float curvd_dist = strght_line_dist / cos(beta);
            float floor_x = state.camera.pos.x + cos(ra) * curvd_dist; floor_x =  floor_x - (32 * (int) (floor_x/32) );
            float floor_y = state.camera.pos.y + sin(ra) * curvd_dist; floor_y = floor_y - (32 * (int) (floor_y/32));
            SDL_RenderDrawRect(state.doom_renderer, &line_rect); SDL_RenderFillRect(state.doom_renderer, &line_rect);
            int clr = All_Textures[(int) (floor_y) * 32  + (int) (floor_x) ];
            SDL_SetRenderDrawColor(state.doom_renderer, 40, 30,  (clr == 1 ? 200 :  70), 255);
        }

        ra +=  DR; if(ra < 0){ra += 2 * PI;} if (ra > 2*PI ){ra -= 2*PI;}
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
        state.window = SDL_CreateWindow("Doom Map", SDL_WINDOWPOS_CENTERED - 200 , SDL_WINDOWPOS_CENTERED ,width, height, SDL_WINDOW_SHOWN);
        state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        state.doom_window = SDL_CreateWindow("SDL_Doom", SDL_WINDOWPOS_CENTERED + 200 , SDL_WINDOWPOS_CENTERED ,  width, height , SDL_WINDOW_SHOWN);
        state.doom_renderer = SDL_CreateRenderer(state.doom_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if( state.window == NULL || state.doom_window == NULL ){printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );}else{
            
            //Get window surface
            //screenSurface = SDL_GetWindowSurface( window );
            //SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ));
            //SDL_UpdateWindowSurface(window);

            SDL_Rect p_rect;
            p_rect.x = width / 2;p_rect.y = width / 2;
            p_rect.w = 10;p_rect.h = 10;
            
           // state.gScreenSurface = SDL_GetWindowSurface(state.doom_window);
           // state.s_mountains = SDL_LoadBMP( "mountains.bmp" );
            //if( state.s_mountains == NULL ){
              //  printf( "Unable to load image %s! SDL Error: %s\n", "mountains.bmp", SDL_GetError() );
           // }

            // BG Clr
            SDL_SetRenderDrawColor(state.renderer, 100, 100, 100, 255);
            SDL_SetRenderDrawColor(state.doom_renderer, 70, 70, 70, 255);

            SDL_Event e; bool quit = false;
            bool drwd = false;
            while(!quit){
                /// Clear Screen
                SDL_RenderClear(state.renderer); SDL_RenderClear(state.doom_renderer);

                state.camera.pos.x =  p_rect.x; state.camera.pos.y =  p_rect.y;
                

                // DRAW MAP
                renderMap(state.renderer);

                ceiling_();
                // draw plyr view
                renderPlayr(state.renderer, p_rect);
                
                
                // Modify Color & Draw Player Rectangle & line
                SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
                SDL_RenderDrawRect(state.renderer, &p_rect); SDL_RenderFillRect(state.renderer, &p_rect);
                
                
                // BG clr
                SDL_SetRenderDrawColor(state.renderer, 100, 100, 100, 255);
                SDL_SetRenderDrawColor(state.doom_renderer, 60, 60, 60, 255);

                // Show whats drawn
                SDL_RenderPresent(state.renderer);
                SDL_RenderPresent(state.doom_renderer);

                int x_fset = 12; if(state.camera.angle < P3 && state.camera.angle > P2){x_fset = -12;}
                int y_fset = 12; if(state.camera.angle > PI && state.camera.angle < TPI){y_fset = -12;}
                
                int grid_x = state.camera.pos.x / 64;
                int grid_x_offset =  (state.camera.pos.x + x_fset) / 64;
                int inv_x_offset =  (state.camera.pos.x - x_fset) / 64;

                int grid_y = state.camera.pos.y / 64;
                int grid_y_offset =  (state.camera.pos.y + y_fset) / 64;
                int inv_y_offset =  (state.camera.pos.y - y_fset) / 64;

                
                // key movs <= =>
                bool collision = false;
                while(SDL_PollEvent(&e) ){
                    if(e.type == SDL_QUIT) {quit = true;}
                    if (e.type == SDL_KEYDOWN){
                      //  printf("%c",  e.key.keysym.sym );
                        switch( e.key.keysym.sym ){
                               case SDLK_LEFT:
                                   //printf("left \n");
                                   // rotate
                                   state.camera.angle += 0.15;
                                   if(state.camera.angle > 2 * PI){state.camera.angle -= 2* PI;}
                                   state.camera.delta_pos.x = cos(state.camera.angle) * 5; state.camera.delta_pos.y = sin(state.camera.angle) * 5;
                                   break;
                               case SDLK_RIGHT:
                                   state.camera.angle -= 0.15;
                                   if(state.camera.angle < 0){state.camera.angle += 2* PI;}
                                   state.camera.delta_pos.x = cos(state.camera.angle) * 5; state.camera.delta_pos.y = sin(state.camera.angle) * 5;
                                   break;
                               case SDLK_UP:
                                 //  printf("up \n");
                                   //p_rect.y -= 2;
                                    if(map[(8 * grid_y_offset ) + grid_x]  || map[(8 * grid_y ) + grid_x_offset] ||
                                       map[(8 * grid_y_offset ) + grid_x_offset]  ){
                                        puts("forward collision");
                                    }else{
                                        p_rect.x += state.camera.delta_pos.x;
                                        p_rect.y += state.camera.delta_pos.y;
                                    }
                                   break;
                               case SDLK_DOWN:
                                 //  printf("down \n");
                                   //p_rect.y += 2;
                                    if(map[(8 * inv_y_offset ) + grid_x]  || map[(8 * grid_y ) + inv_x_offset] ||
                                       map[(8 * inv_y_offset ) + inv_x_offset]  ){
                                        puts("backward collision");
                                    }else{
                                        p_rect.x -= state.camera.delta_pos.x;
                                        p_rect.y -= state.camera.delta_pos.y;
                                    }
                       
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
