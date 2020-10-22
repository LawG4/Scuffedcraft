//=====================================================================================
//The headerfile for the render manager
//we also have the abstraction for cubes in this file too, since they will just be 
//identified by their number in other circumstances  
//=====================================================================================
#pragma once
//All objects may be cubular, however it would be a waste to compare when cubes interact
//instead we can just compare the squares in the same orientation. 
//Therefore the rendering will be square based. 
//
//Each Gameobject class will have a list of faces which make up their instances
//and then each instance of that game object will have an offset which will place 
//its verticies into the right place.

#include <grrlib.h>
#include <stdlib.h>

//A face has it's relative coordinates and a texture,
//then a model will have a list of faces,
//then a game object will have a model and an offset
struct Face_VT
{
	//cordinates are listed clockwise
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
	f32 z1, z2, z3, z4;
};

struct Face_TX
{
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
};

struct Model
{
	GRRLIB_texImg *tex;
	struct Face_VT *faces;
	struct Face_TX *texCoords;
	int   size;
};

struct GameObject
{
	struct Model model; 
	f32 x,y,z;
};

//These are going to be static faces, to represent the 6 different faces of the cube, they will be unmutable 
//and they will be copied throughout the progam, remember that a cube's centre of mass will be in the middle
//there for the coordinates are relative to the centre of the cube
extern struct Face_VT Face_Cube_T;
extern struct Face_VT Face_Cube_B;
extern struct Face_VT Face_Cube_N; 
extern struct Face_VT Face_Cube_S;
extern struct Face_VT Face_Cube_E;
extern struct Face_VT Face_Cube_W;

void renderModel(struct Model model );
//make the model
struct Model makeTestCube();
//render the model
void renderTestCube(struct Model model, f32 param, f32 x, f32 y, f32 z);