//=======================================================================================
//= This section is about the implementation behind the collision system                = 
//=======================================================================================
#include "Collision.h"
#include <math.h>

int collideCylinderWithCube(struct PlayerCylinder player, float x, float y, float z)
{
    //So we have one axis alligned bounding box (the block) and a cylinder (the player)
    //If the height of the cylinder is outside the bounding box's height, no collision
    float cylinderHeightMin = player.y;
    float cylinderHeightMax = cylinderHeightMin + player.height;
    //The height of the box is set.
    float boxHeightMin = y - 0.5f;
    float boxHeightMax = y + 0.5f;

    //check the heights are in alignment, cylinder is on top
    if(!(boxHeightMin <= cylinderHeightMax && boxHeightMax >= cylinderHeightMin))
    {
        //The box and the cylinder y axis don't line up so we don't intersect
        return 0;
    }

    //Since we know they align height wise, we just need to do a square inside of circle check.
    //Get the point in the square which is closest to the centre of the circle using clamp
    // x = max(box.minx, min(circleCentre.x, box.maxX))
    float closestX = fmax(x - 0.5f, fmin(player.x, x + 0.5f)); 
    float closestZ = fmax(z - 0.5f, fmin(player.z, z + 0.5f)); 

    //How far away is the closest point to the centre of the circle, distance squared
    float distance2 = (closestX - player.x) * (closestX - player.x) + (closestZ - player.z) * (closestZ - player.z);

    //Is the closest point within the cylinders radius, used the squared distances
    if(distance2 > player.r2)
    {
        //outside the radius
        return 0;
    }
    else
    {
        return 1;        
    }
}