//=======================================================================================
//= Welcome to the entry point for the wii minecraft clone, let's call it Scuffedcraft  =
//= I appologise for all the coding sins that will be commited in this project          =
//= Wear a mask!!!                                                                      =
//= Build no : 0.0.0                                                                    =
//= Developer Lawrence                                                                  =
//=======================================================================================

#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/font.h"

//includes from my project
#include "RenderManager.h"
#include "Player.h"
//=====================================================================================
//Entry point to the program, initialiase the wii, load the textures and enter
//the main game loop, I like to keep the top of my main file clear so a lot of 
//files will be abstracted out, perhaps unneccacarily so. anyway a larger 
//explanation might be added here of how everything works, maybe...
//=====================================================================================
int main() {
    //Using GRRLib we can initialise the wii a lot easier and quicker, for example
    //the video mode object is already premade for us in within the library.
    GRRLIB_Init();
    WPAD_Init();

    //Store the x,y,z coordinates of the lplayer are stored here.
    f32 x = 0.0f;
    f32 y = 2.0f;
    f32 z = 13.0f;

    //load the texture for the font
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    //might turn off anti aliasing so that it runs better and also looks more 
    //voxel like, also the textures are low res, so with anti aliasing on it 
    //looks like actual garbage.
    GRRLIB_Settings.antialias = false;
    GRRLIB_SetBackgroundColour(0x8C, 0xBE, 0xD6, 0xFF);
    GRRLIB_Camera3dSettings(x, y, z, 0, 1, 0, 0, 0, 0);


    //Create some test chunks
    struct Chunk chunk00 = createWaveyChunk(0,0);
    struct Chunk chunk10 = createWaveyChunk(16,0);
    struct Chunk chunkm10 = createWaveyChunk(-16,0);
    
    //initialise the player
    initPlayer();

    while(1) {
        //I actually have no idea what the case of this is, but if we don't scan the Wii
        //mote for a home button press every single frame then we just get a black screen
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);

        //GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x + 1, y, z);
        processPlayerInput();
        GRRLIB_3dMode(0.1,1000,70,0,0);

        //Render all the map chunks
        renderChunk(chunk00);
        renderChunk(chunk10);
        renderChunk(chunkm10);



        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        //print the debug version and the x y z corrdinates
        GRRLIB_Printf(0, 1, tex_font, 0x000000FF, 0.8, "SCUFFEDCRAFT 0.0.0 :X = %0.1f   Y = %0.1f   Z = %0.1f", getPlayerX(), getPlayerY(), getPlayerZ());

        GRRLIB_Render();
    }
    //Free the memory used by font texture.
    GRRLIB_FreeTexture(tex_font);
    //Free the memory used by GRRLib
    GRRLIB_Exit(); 

    exit(0);
}

