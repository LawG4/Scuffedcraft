//=======================================================================================
//= This section is about the Collision system for the game                             =
//=                                                                                     = 
//= Since the world is made out of static cubes in a grid we can limit the collision    =
//= system to a finite set of interactions so that we can accelerate them as much as    =
//= possible, we can represent a player as a cylinder have a different method of        =
//= collision for vertical and horizontal parts of a block with a cylinder.             =
//=                                                                                     =
//= We'll also need some kind of ray casting system for when a user clicks into the     =
//= environment                                                                         =
//=======================================================================================

//We choose a cylinder for the player, because we don't want to use a rectangle and 
//get caught in one block wide spaces, also it saves us time rotating the box.

//Define the player cylinder's origin by the bottom centre, store the full height and then 
//the radius squared, as we can tell when an point is inside from just those variables
// C = {(x,y,z) in R3 : 0 < (y - cy) < h and (x - cx)^2 + (y - cy)^2 < r^2}
struct PlayerCylinder
{
    //origin
    float x,y,z;
    //Need the full height to go from top of the box from bottom.
    float height;
    //Radius squared 
    float r2;
};

//So a cylinder can collide with a cube in 3 different ways, North facing faces, East facing faces and Horizontal faces.

//Collide the cylinder with just the axis alligned cubes that are the blocks 
int collideCylinderWithCube(struct PlayerCylinder player, float x, float y, float z);
