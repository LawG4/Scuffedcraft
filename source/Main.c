/**
 * 
 * Welcome to scuffedcraft, the massively scuffed clone of minecraft ment to run on the nintendo wii
 * The project utilises GRRLib and DevkitPPC, give them a look, perhaps donate to them?
 *
 * Positive X direction is considered West
 * Positive Z direction is considered North
 *
 *              +Z
 *              ^
 *              |
 *              |
 *  +X  <-------+------->  -X
 *              |
 *              |
 *             \/
 *             -Z
 */

#include <grrlib.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>
#include "gfx/font.h"

/*Includes from my project*/
#include "Chunks/Chunk.h"
#include "Entities/Player.h"
#include "Logs/Log.h"

/**
 * Entry point to the program, initialiase the wii, load the textures and enter
 * the main game loop, I like to keep the top of my main file clear so a lot of 
 * files will be abstracted out, perhaps unneccacarily so.
 */
int main() {

    /*Initialise all the GRRLib settings we need in Main*/
    GRRLIB_Init();
    WPAD_Init();
    GRRLIB_Settings.antialias = false;
    GRRLIB_SetBackgroundColour(0x8C, 0xBE, 0xD6, 0xFF);
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    /*Start up all my projects management systems*/
    initLog();
    initPlayer();
    initChunkMemoryManager(getPlayerX(), getPlayerY());
    
    logMessage("Entering rendering loop");
    getManagedChunkAtOffsetFromPlayer(getPlayerX(), getPlayerZ());

    while(1) {
        /*You have to scan the wii mode for an exit condition in the main loop or 
        else you get stuck with a black screen*/
        WPAD_ScanPads();
        if(WPAD_ButtonsDown(0) & (WPAD_BUTTON_HOME | PAD_BUTTON_START)) break;

        processPlayerInput();

        /*3D Rendering section*/
        GRRLIB_3dMode(0.1,1000,70,0,0);
        /*Render the chunks that are inside the chunk memory manager*/
        renderManagedChunks();

        /*Switch to 2D mode to do the UI*/
        GRRLIB_2dMode();
        GRRLIB_Printf(0, 1, tex_font, 0x000000FF, 0.8, "SCUFFEDCRAFT 0.0.1 :X = %0.1f   Y = %0.1f   Z = %0.1f", getPlayerX(),getPlayerY(), getPlayerZ());

        /*Call this at the end of a GRRLIB rendering sequence*/
        GRRLIB_Render();
    }
    
    /*Close and free the resources from my project management systems*/
    logMessage("Closing Log");

    /*Free GRRLib memory, that is the font texture, and GRRLib itself*/
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_Exit(); 

    return 0;
}

