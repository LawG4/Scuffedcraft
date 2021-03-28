#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <math.h> 

#include "../Chunks/Chunk.h"

/** Set the initial variables for the player and keep them scoped to the player operations
 * TODO: Abstract this out to an entity struct when we start adding in other entities.
 */
f32 x = -1;
f32 y = 8;
f32 z = 1;


/*Looking angles for the player
Yaw is rotation around the y axis, ie looking left and right
Pitch is rotation arount the z axis, ie looking up and down */
f32 yaw = 1.570796326f;
f32 pitch  = 0;

/**
 * Initialise the player and also any controllers they may happen to have plugged in
 * TODO: Add more controller support
 */
void initPlayer()
{
    /*start the gamecube controller pad.*/
    PAD_Init();
    /*initially set the player to be looking straight forward at certain x,y,z position*/
    GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x, y, z + 1);
}

/**
 * Process the user input and reflect those changes players data. 
 * TODO: Add controller abstraction
 */ 
void processPlayerInput()
{
    /** Currently I am stuck to an emulator and need to scale inputs
    TODO: Get my old wii out of storage*/
    f32 scaleFactor = 70.0f; 
    
    
    /*Gather the stick input*/
    PAD_ScanPads();
    int stickLY = PAD_StickY(0);
    int stickLX = PAD_StickX(0);
    int stickRX = PAD_SubStickX(0);
    int stickRY = PAD_SubStickY(0);
    /*the delta that results from the stick input*/
    f32 deltaX = (float)stickLY / (scaleFactor * 10);
    f32 deltaZ = (float)stickLX / (scaleFactor * 10);

    /*yaw is the walking direction, pitch is up to sky*/
    f32 deltaY = (float)stickRX / (scaleFactor * 10);
    f32 deltaP = (float)stickRY / (scaleFactor * 10);

    /*Update the looking angle first so we can head in the right direction
    bind the looking angle inbetween a set range*/
    yaw = fmod(yaw + deltaY, 6.2831853f);
    pitch  = pitch  + deltaP;
    if(pitch > 1.570796326f) pitch = 1.570796326f;
    if(pitch < -1.570796326f) pitch = -1.570796326f;

    /*Take the delta components in the x,z directions rotate them around the y axis
    have to negate the angle for some reason, probably just rotation matrix used*/
    x = x + deltaX *  cos(-yaw) + deltaZ * sin(-yaw);
    z = z + deltaX * -sin(-yaw) + deltaZ * cos(-yaw);

    GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x + cos(yaw), y + sin(pitch), z + sin(yaw));

    /*Player calculation has finished, so now realign the managed chunks*/
    playerPositionBasedChunkMemoryManagementUpdate(x,z);
}



/**
 * Some getters for the onscreen debugging
 * TODO: Logging
 */ 
float getPlayerX()
{
    return x;
}

float getPlayerY()
{
    return y;
}

float getPlayerZ()
{
    return z;
}