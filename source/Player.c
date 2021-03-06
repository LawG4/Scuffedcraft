//=======================================================================================
//= This section is code is entirely for handling the movement of the player in 3d      =
//= space and handling the collisions with the mesh data                                = 
//=======================================================================================
#include <grrlib.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h> //gamecibe controller I hope??
#include <math.h> //for camera 
#include "Collision.h" // for the collision with the terrain.

//Start with the player coordinates that won't be accessable outside this file
//initial positions for this basic build
f32 x = 0;
f32 y = 4;
f32 z = 0;

//Looking angles for the player. 
//Yaw is rotation around the y axis, ie looking your head left and right
//Pitch is roation around the x axis, ie looking up and down
f32 yaw = 0;
f32 pitch  = 0;

//player initialisation
void initPlayer()
{
    //start the gamecube controller pad.
    PAD_Init();
    //initially set the player to be looking straight forward at certain x,y,z position
    GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x + 1, y, z);
}

//Process the user input and reflect those changes players data. 
void processPlayerInput()
{
    //For whatever reason we can't scan for the wii button presses in here, that just
    //gets a crash?
    //Until I get a controller we're limited on keyboard so we want to reduce delta by a factor.
    f32 scaleFactor = 70.0f; 
    //Scan the gamecube controller attached.
    PAD_ScanPads();
    //Gather the stick input
    int stickLY = PAD_StickY(0);
    int stickLX = PAD_StickX(0);
    int stickRX = PAD_SubStickX(0);
    int stickRY = PAD_SubStickY(0);
    //the delta that results from the stick input
    f32 deltaX = (float)stickLY / (scaleFactor * 100);
    f32 deltaZ = (float)stickLX / (scaleFactor * 100);

    //yaw is the walking direction, pitch is up to sky
    f32 deltaY = (float)stickRX / (scaleFactor * 10);
    f32 deltaP = (float)stickRY / (scaleFactor * 10);

    //Update the looking angle first so we can head in the right direction
    //bind the looking angle inbetween a set range
    yaw = fmod(yaw + deltaY, 6.2831853f);
    //yaw = yaw + deltaY;
    pitch  = pitch  + deltaP;
    if(pitch > 1.570796326f) pitch = 1.570796326f;
    if(pitch < -1.570796326f) pitch = -1.570796326f;

    //Take the delta components in the x,z directions rotate them around the y axis
    //have to negate the angle for some reason, probably just rotation matrix used
    x = x + deltaX *  cos(-yaw) + deltaZ * sin(-yaw);
    z = z + deltaX * -sin(-yaw) + deltaZ * cos(-yaw);

    //now update the camera view
    GRRLIB_Camera3dSettings(x,y,z, 0, 1, 0, x + cos(yaw), y + sin(pitch), z + sin(yaw));
}



///just some getters
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
    //replace the z value for if we're touching the a box at x = 0, y = 4 and any positive z value
    struct PlayerCylinder player = {x,y,z, 1.8f, 0.25};
    int result = collideCylinderWithCube(player, 0.5, 3.5, 0.5);
    float FormatedResult = (float)(result) * 100.0f; 
    return z + FormatedResult;
}