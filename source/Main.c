//=====================================================================================
// Welcome to the entry point for the wii minecraft clone, let's call it Scuffedcraft
// I appologise for all the coding sins that will be commited in this project
// Wear a mask!!!
// Build no : 0.0.0
// Developer Lawrence
//=====================================================================================

#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>

#include "gfx/font.h"

//includes from my project
#include "RenderManager.h"

//=====================================================================================
//Entry point to the program, initialiase the wii, load the textures and enter
//the main game loop, I like to keep the top of my main file clear so a lot of 
//files will be abstracted out, perhaps unneccacarily so. anyway a larger 
//explanation might be added here of how everything works, maybe...
//=====================================================================================
int main() {
    float a=0;
    int cubeZ=0;

    //Using GRRLib we can initialise the wii a lot easier and quicker, for example
    //the video mode object is already premade for us in within the library.
    GRRLIB_Init();
    WPAD_Init();

    //we could using this opertunity to load all the textures since they're so small
    //and won't be disposing of them at any point, perhaps.

    //load the texture for the font
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    //might turn off anti aliasing so that it runs better and also looks more 
    //voxel like
    GRRLIB_Settings.antialias = false;
    GRRLIB_SetBackgroundColour(0x8C, 0xBE, 0xD6, 0xFF);
    GRRLIB_Camera3dSettings(0.0f,0.0f,13.0f, 0,1,0, 0,0,0);

    //make our test model for now and test it
    struct Model testCube = makeTestCube();

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) cubeZ++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) cubeZ--;

        //GRRLIB_Camera3dSettings(13.0f * sin(a),0.0f,5.0f * cos(a), 0,1,0, 0,0,0);
        GRRLIB_3dMode(0.1,1000,70,0,0);

        renderTestCube(testCube, a,  2,  0,  4);
        renderTestCube(testCube, a,  0, -8,  0);
        renderTestCube(testCube, a,  0,  1, -4);
        
        a+=1.0f;

        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        GRRLIB_Printf(0, 1, tex_font, 0x000000FF, 1, "SCUFFEDCRAFT 0.0.0");

        GRRLIB_Render();
    }
    //Free the memory used by font texture.
    GRRLIB_FreeTexture(tex_font);
    //Free the memory used by GRRLib
    GRRLIB_Exit(); 

    exit(0);
}

