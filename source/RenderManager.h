//=======================================================================================
//= The headerfile for the render manager                                               =
//= we also have the abstraction for cubes in this file too, since they will just be    =
//= identified by their number in other circumstances                                   =
//=======================================================================================
#pragma once
#include <grrlib.h>
#include <stdlib.h>



//=======================================================================================
//= Start the file with the structure definitions                                       =
//=======================================================================================

//A model is a linked list of faces, this is because a model can be exapanded during its
//creation, for instance when we take a chunk's representation and turn it into a model
//we add one face at a time. To this end a model is made as a linked list of faces, with
//a texture object. A face contains a list of vertices and texture coordinates and a 
//pointer to the next one. Then we assume for now that different types of game objects
//will have the information to correctly translate the object 
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

struct Face{
	struct Face_VT vt;
	struct Face_TX tx;
	struct Face *next;
};

struct Model
{
	//Still need the size of the size of the model used for GRRLIB rendering
	int size;
	GRRLIB_texImg *tex;
	struct Face *firstFace;
};



//=======================================================================================
//= Global variables, primarily used to be copied to other things, we want to avoid     =
//= Changing the state of these as they will effect the whole program.                  =
//=======================================================================================

//These are going to be static faces, to represent the 6 different faces of the cube, they will be unmutable 
//and they will be copied throughout the progam, remember that a cube's centre of mass will be in the middle
//there for the coordinates are relative to the centre of the cube
extern struct Face_VT Face_Cube_T;
extern struct Face_VT Face_Cube_B;
extern struct Face_VT Face_Cube_N; 
extern struct Face_VT Face_Cube_S;
extern struct Face_VT Face_Cube_E;
extern struct Face_VT Face_Cube_W;



//=======================================================================================
//= Function declarations from RenderManager.c                                          =
//=======================================================================================

//We'll start with this section which begins with the model and faces utility such as 
//a "constructor" for faces. 

//This face generater is used to copy the prebuild cube face verticies and then place 
//them in a relative position, this will be used primaraily in generating the chunk.
struct Face *createTranslatedFace(struct Face_VT baseVerticies, struct Face_TX baseTex, f32 offsetX, f32 offsetY, f32 offsetZ);

//We want to be able to add a model to the linked list, thankfully order of the faces 
//in a model doesn't matter, so we can push the face to the front of the linked list.
void addFaceToModel(struct Model *model, struct Face *face);

//We need a method to deallocate the model's resources, not neccarily delete them 
void dealocateModel(struct Model *model);

//We'll add in methods to specifically clear the correct faces from the models 


//Next we move onto the rendering support functions, at the end of the day we want to 
//call some kind of render function that will render the whole frame with one call,
//for now this section is primarily going to be support functions as well as some 
//test functions. 
void renderModel(struct Model model );
//make the model
struct Model makeTestCube();
//render the model
void renderTestCube(struct Model model, f32 param, f32 x, f32 y, f32 z);