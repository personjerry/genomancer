
#include "Genomancer.hpp"

sfmt_t sfmt;

char gMap[MAPSIZE][MAPSIZE];
char gFrame[FRAMESIZE][FRAMESIZE];

int gPlayerX;
int gPlayerY;

//the game fonts
TTF_Font*  gFont;
TTF_Font*  mapFont;

//just a mask for tiles that have been seen but aren't illuminated
SDL_Surface* gSeenMask;

//the surfaces for the pics and screen, obviously
SDL_Surface* gScreen;

// counters for calculation rates
int tick;
int gLastTick;

// key detecting variables
bool up = 0, down = 0, left = 0, right = 0;      //up down left right are updated whenever the keys are pressed or released, while
bool key[12];

int mtrand()
{
    int a = sfmt_genrand_uint32(&sfmt);
    std::cout << a;
    return a;
}


void saveTiles()
{  //write each tile, i is x; j is y (GRID POSITIONS)
    /*
  std::ofstream outfile(LOC_SAVE_TILES);
  for(int i = 0; i < GRIDWIDTH; i++)
    for (int j = 0; j < GRIDHEIGHT; j++)
    {
      outfile << gCurLevel[i][j].save();
    }
  outfile.close();*/
}
void initMap()
{
    for(int i = 0; i < MAPSIZE;i++)
    {
        for(int k = 0; k < MAPSIZE;k++)
        {
            /*potential map
            int temp = mtrand() % 8;
            switch(temp)
            {
                case 0:
                    gMap[i][k] = '.'; //dirt
                    break;
                case 1:
                    gMap[i][k] = '~'; //water
                    break;
                case 2:
                    gMap[i][k] = '&'; //tree
                    break;
                case 3:
                    gMap[i][k] = '^'; //boulder
                    break;
                case 4:
                    gMap[i][k] = '!'; //fire
                    break;
                case 5:
                    gMap[i][k] = '='; //lava
                    break;
                case 6:
                    gMap[i][k] = ','; //grass
                    break;
                case 7:
                    gMap[i][k] = '*'; //void
                    break;
            }
                    */
            //in reality map starts with only dirt and trees, later levels have more stuff
            int temp = (mtrand() % 2);
            if(temp == 0)
            {
                gMap[i][k] = '.'; //dirt
            }
            else
            {
                gMap[i][k] = '&'; //tree
            }
        }
    }

    // cellular automata

    for(int i = 0; i < 1; i++)
    {
        char tempmap[MAPSIZE][MAPSIZE];
        for(int x = 0; x<MAPSIZE;x++)
        {
            for(int y = 0; y < MAPSIZE;y++)
            {
                int _left = x-1;
                int _right = x+1;
                int _up = y-1;
                int _down = y+1;

                if(_left < 0)
                {
                    _left = MAPSIZE-1;
                }
                if(_right == MAPSIZE)
                {
                    _right = 0;
                }

                if(_up < 0)
                {
                    _up = MAPSIZE-1;
                }
                if(_down == MAPSIZE)
                {
                    _down = 0;
                }


                int total = (gMap[_left][_up]=='&'?1:0) + (gMap[_left][y]=='&'?1:0) + (gMap[_left][_down]=='&'?1:0) + (gMap[x][_up]=='&'?1:0) + (gMap[x][_down]=='&'?1:0) + (gMap[_right][_up]=='&'?1:0) + (gMap[_right][y]=='&'?1:0) + (gMap[_left][_down]=='&'?1:0);
                if (gMap[x][y] == '&')
                {
                    if(total == 4 || total == 5 || total == 6 || total == 7 || total ==8) //S45678 cellular automata rule for survivals
                        tempmap[x][y] = '&';
                    else
                        tempmap[x][y] = '.';
                }
                else if(gMap[x][y] == '.')
                {
                    if(total == 6 || total == 7 || total ==8) //B678 cellular automata rule for births
                        tempmap[x][y] = '&';
                    else
                        tempmap[x][y] = '.';
                }
            }
        }
        for(int x = 0; x<MAPSIZE;x++)
        {
            for(int y = 0; y < MAPSIZE;y++)
            {
                gMap[x][y] = tempmap[x][y];
            }
        }
    }
}


void initPlayer()
{
    gPlayerX = 0;
    gPlayerY = 0;
}


void updateTicks()
{
  // Lock surface if needed
  if (SDL_MUSTLOCK(gScreen))
    if (SDL_LockSurface(gScreen) < 0)
      return;
  int tick = SDL_GetTicks();
  if (tick <= gLastTick)
  {
    SDL_Delay(1);
    return;
  }
  while (gLastTick < tick)
  {
    gLastTick += 1000 / PHYSICSFPS;
  }
}

void updateScreen()
{


    //load map frame and draw
    SDL_Rect rect;
    for(int i = 0; i < FRAMESIZE;i++)
    {
        rect.x = 24*i;
        for(int k = 0; k < FRAMESIZE;k++)
        {
            rect.y = 24*k;
            int x = gPlayerX-(FRAMESIZE-1)/2+i;
            if(x < 0)
            {
                x += MAPSIZE;
            }
            else if (x >= MAPSIZE)
            {
                x -= MAPSIZE;
            }
            int y = gPlayerY-(FRAMESIZE-1)/2+k;
            if(y < 0)
            {
                y += MAPSIZE;
            }
            else if (y >= MAPSIZE)
            {
                y -= MAPSIZE;
            }
            gFrame[i][k] = gMap[x][y];
            char temp[2];
            sprintf(temp, "%c",gFrame[i][k]);
            SDL_Surface* imgTemp = TTF_RenderText_Shaded(mapFont ,temp, SDL_Color{255,255,255},SDL_Color{25,25,25} );
            SDL_BlitSurface(imgTemp , NULL , gScreen , &rect);
            SDL_FreeSurface(imgTemp) ;
        }
    }

    //draw entities
    rect.x = rect.y = (FRAMESIZE-1)*24/2;
    SDL_Surface* imgTemp = TTF_RenderText_Shaded(mapFont ,"@", SDL_Color{255,255,255},SDL_Color{25,25,25} );
    SDL_BlitSurface(imgTemp , NULL , gScreen , &rect);

  // Unlock if needed
  if (SDL_MUSTLOCK(gScreen))
      SDL_UnlockSurface(gScreen);
  // Tell SDL to update the whole gScreen
  SDL_UpdateRect(gScreen, 0, 0, TOTALWIDTH, TOTALHEIGHT);
}

void updateWorld()
{
    //make sure we don't do 9001 moves per second
    static int cooldown = INITCD;
    if(cooldown == 0)
    {

        //preliminary world movement
        int newY = gPlayerY, newX = gPlayerX;
        if(up)
        {
            newY--;
            if(newY < 0)
                newY += MAPSIZE;
        }
        else if(down)
        {
            newY++;
            if(newY >= MAPSIZE)
                newY -= MAPSIZE;
        }
        if(left)
        {
            newX--;
            if(newX < 0)
                newX += MAPSIZE;
        }
        else if(right)
        {
            newX++;
            if(newX >= MAPSIZE)
                newX -= MAPSIZE;
        }

        if(gMap[newX][newY] == '.' && (newX != gPlayerX || newY != gPlayerY)) //if valid move was made, we need to set cooldown
        {
            gPlayerX = newX;
            gPlayerY = newY;
            cooldown = INITCD;
        }
    }
    else
    {
        cooldown--;
    }
}

void cleanup()
{
    TTF_CloseFont(mapFont);
    TTF_CloseFont(gFont);
}

void init()
{

  // Initialize SDL's subsystems
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
  {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_WM_SetIcon(SDL_LoadBMP(LOC_PICTURE_ICON), NULL);
  SDL_WM_SetCaption("Genomancer","Genomancer");


  if (TTF_Init() == -1)
  {
    printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
    exit(2);
  }
  mapFont = TTF_OpenFont(LOC_FONTS_MAP, 24);
  if (mapFont == NULL){
    printf("Unable to load font: %s %s \n", LOC_FONTS_MAP, TTF_GetError());
    exit(3);
  }
  gFont = TTF_OpenFont(LOC_FONTS_MAIN, 16);
  if (gFont == NULL){
    printf("Unable to load font: %s %s \n", LOC_FONTS_MAIN, TTF_GetError());
    exit(4);
  }

  // Register SDL_Quit to be called at exit; makes sure things are
  // cleaned up when we quit.
  atexit(cleanup);
  atexit(SDL_Quit);

  /* Get available fullscreen/hardware modes */
  SDL_Rect ** modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
  /* Check is there are any modes available */
  if(modes == (SDL_Rect **)0){
    exit(-1);
  }
  /* Check if or resolution is restricted */
  if(modes == (SDL_Rect **)-1)
  {
    exit(-2);
  }
  else
  {
  /* Do first mode */
    gScreen = SDL_SetVideoMode(TOTALWIDTH,TOTALHEIGHT, 32, SDL_SWSURFACE);
  }


  // currently is #defined width and height, a simple change (see below) and a fullscreen toggle will allow the screen to be custom sized. may cause bugs, until ready use #defs.
  //gScreen = SDL_SetVideoMode(0, 0, 32, SDL_SWSURFACE);

  // If we fail, return error.
  if (gScreen == NULL)
  {
    fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
    exit(-3);
  }

  //initialize randoms
  sfmt_init_gen_rand(&sfmt, time(NULL));


  // clear screen just in case
  SDL_FillRect(gScreen,NULL,0x000000);

  // init key values
  for(int i = 0; i < 12; i++)
  {
      key[i] = false;
  }

  initMap();
  initPlayer();

}


int main(int argc, char *argv[])
{


  init();


  // Main loop: loop forever.
  while (1)
  {
    // Function to process EVERYTHING EXCEPT DRAWING
    updateWorld();
    // Function to process DRAWING ONLY
    updateScreen();

    // Poll for events, and handle the ones we care about.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        /*case SDL_MOUSEMOTION:
          mousex = event.motion.x;
          mousey = event.motion.y;
          break;
        case SDL_MOUSEBUTTONDOWN:
          mouseb = true;
          rbutton = (event.button.button == SDL_BUTTON_RIGHT);
          break;
        case SDL_MOUSEBUTTONUP:
          mouseb = false;
          rbutton = (event.button.button == SDL_BUTTON_RIGHT);
          break;*/
      case SDL_KEYDOWN:
      // VERY TEDIOUS TEST FOR KEY DOWN, AND SET UP DOWN LEFT RIGHT DEPENDING ON KEY PRESSED
      switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            key[0] = true;
            break;
        case SDLK_KP4:
            key[1] = true;
            break;
        case SDLK_RIGHT:
            key[2] = true;
            break;
        case SDLK_KP6:
            key[3] = true;
            break;
        case SDLK_UP:
            key[4] = true;
            break;
        case SDLK_KP8:
            key[5] = true;
            break;
        case SDLK_DOWN:
            key[6] = true;
            break;
        case SDLK_KP2:
            key[7] = true;
            break;
        case SDLK_KP7:
            key[8] = true;
            break;
        case SDLK_KP9:
            key[9] = true;
            break;
        case SDLK_KP3:
            key[10] = true;
            break;
        case SDLK_KP1:
            key[11] = true;
            break;
          default:
          break;
        }
        if(key[0] || key[1] || key[8] || key[11])
            left = true;
        if(key[2] || key[3] || key[9] || key[10])
            right = true;
        if(key[4] || key[5] || key[8] || key[9])
            up = true;
        if(key[6] || key[7] || key[10] || key[11])
            down = true;

        break;

      case SDL_KEYUP:
      // VERY TEDIOUS TEST FOR KEY up, AND SET UP DOWN LEFT RIGHT DEPENDING ON KEY PRESSED
      switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            key[0] = false;
            break;
        case SDLK_KP4:
            key[1] = false;
            break;
        case SDLK_RIGHT:
            key[2] = false;
            break;
        case SDLK_KP6:
            key[3] = false;
            break;
        case SDLK_UP:
            key[4] = false;
            break;
        case SDLK_KP8:
            key[5] = false;
            break;
        case SDLK_DOWN:
            key[6] = false;
            break;
        case SDLK_KP2:
            key[7] = false;
            break;
        case SDLK_KP7:
            key[8] = false;
            break;
        case SDLK_KP9:
            key[9] = false;
            break;
        case SDLK_KP3:
            key[10] = false;
            break;
        case SDLK_KP1:
            key[11] = false;
            break;
          default:
          break;
        }
        if(!key[0] && !key[1] && !key[8] && !key[11])
            left = false;
        if(!key[2] && !key[3] && !key[9] && !key[10])
            right = false;
        if(!key[4] && !key[5] && !key[8] && !key[9])
            up = false;
        if(!key[6] && !key[7] && !key[10] && !key[11])
            down = false;
        break;

        case SDL_QUIT:
            return(0);
      }
    }
  }
  return 0;
}
