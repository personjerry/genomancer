#ifndef GENOMANCER_HPP_INCLUDED

#define PHYSICSFPS 100
#define PI 3.14159265

#define ASCIIZERO 48
#define FONTSIZE 32

#define LOC_FONTS_MAP "data/fonts/map.ttf"
#define LOC_FONTS_MAIN "data/fonts/main.ttf"

#define LOC_SAVE_PLAYER "data/saves/player.sav"
#define LOC_SAVE_TILES "data/saves/tiles.sav"

#define LOC_PICTURE_ICON "data/pics/icon.bmp"

#define MENUHEIGHT 192
#define MENUWIDTH 256

#define TOTALHEIGHT 768
#define TOTALWIDTH 1024

#define MAPSIZE 512
#define FRAMESIZE 31

#define INITCD 20



#if defined(_MSC_VER)
#include "SDL.h"
#include "SDL_image.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#endif

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <math.h>

#include "SFMT.hpp"

extern int warpmod(int number, int mod);

extern SDL_Surface* gScreen;

extern int tick;
extern int gLastTick;

extern TTF_Font*    gFont;
extern TTF_Font*    mapFont;

extern char gMap[MAPSIZE][MAPSIZE];

std::string customItoa(int toconv);

void reorderMonsters();



#define GENOMANCER_HPP_INCLUDED
#endif
