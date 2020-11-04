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

    //Using GRRLib we can initialise the wii a lot easier and quicker, for example
    //the video mode object is already premade for us in within the library.
    GRRLIB_Init();
    WPAD_Init();

    //Store the x,y,z coordinates of the lplayer are stored here.
    f32 x = 0.0f;
    f32 y = 2.0f;
    f32 z = 13.0f;

    //we could using this opertunity to load all the textures since they're so small
    //and won't be disposing of them at any point, perhaps.

    //load the texture for the font
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    //might turn off anti aliasing so that it runs better and also looks more 
    //voxel like
    GRRLIB_Settings.antialias = false;
    GRRLIB_SetBackgroundColour(0x8C, 0xBE, 0xD6, 0xFF);
    GRRLIB_Camera3dSettings(x, y, z, 0, 1, 0, 0, 0, 0);

    //make our test model for now and test it
    //struct Model testCube = makeTestCube();
    //u8 *chunkData = createTestChunkID();
    //struct Model testChunk = createChunkModel(chunkData);


    struct Chunk chunk00 = createWaveyChunk(0,0);
    struct Chunk chunk10 = createWaveyChunk(16,0);
    struct Chunk chunkm10 = createWaveyChunk(-16,0);
    

    while(1) {
        GRRLIB_2dMode();
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT) z--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT) z++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) x--;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) x++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) y++;
        if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) y--;

        //GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, 90 * cos(a / 100), 0, 90 * sin(a / 100));
        GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x + 1, y, z);
        GRRLIB_3dMode(0.1,1000,70,0,0);

        //renderTestCube(testCube, a,  2,  0,  4);
        //renderTestCube(testCube, a,  0, -8,  0);
        //renderTestCube(testCube, a,  0,  1, -4);
        //renderTestCube(testChunk, 0,0,0,0);
        //renderTestCube(testChunk, 0, 16, 0, 16);
        renderChunk(chunk00);
        renderChunk(chunk10);
        renderChunk(chunkm10);


        //a+=1.0f;

        // Switch To 2D Mode to display text
        GRRLIB_2dMode();
        //print the debug version and the x y z corrdinates
        GRRLIB_Printf(0, 1, tex_font, 0x000000FF, 0.8, "SCUFFEDCRAFT 0.0.0 :X = %0.1f   Y = %0.1f   Z = %0.1f", x, y, z);

        GRRLIB_Render();
    }
    //Free the memory used by font texture.
    GRRLIB_FreeTexture(tex_font);
    //Free the memory used by GRRLib
    GRRLIB_Exit(); 

    exit(0);
}

