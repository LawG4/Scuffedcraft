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

//A model is made up of a list of faces and a texture. a face is made up of its vertex 
//coordinates and its texture coordinates. The method we're aiming for here is to render 
//a model one face at a time. the local to world translation needs to be applied to the
//model or else it won't be placed into the world properly at all. a model is a linked 
//list instead of statically allocated, this is because when we create a model for the 
//chunk it can expand in size.

//Vertex points for the Face
struct Face_VT
{
	//cordinates are listed clockwise
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
	f32 z1, z2, z3, z4;
};
//Texture coordinates for the Face
struct Face_TX
{
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
};
//A Linked list element representing this face of the model
struct Face{
	struct Face_VT vt;
	struct Face_TX tx;
	struct Face *next;
};
//The struct representing a model, containing the starting node for the face and the size
//and texture
struct Model
{
	//Still need the size of the size of the model used for GRRLIB rendering
	int size;
	GRRLIB_texImg *tex;
	struct Face *firstFace;
};

//A chunk will have (hopefully) two models, a Terain model and a water model, they're 
//stored seperatley as they will have different rendering techniques. A model will have
// an list of integers which represent the blocks stored within it. a chunk will also 
//have a starting base point which will represent the bottom left corner of the chunk in
// the XZ coordinate. that way translating from global to local coordinates is as simple
// as subtract or add the base point.

//A struct containting all the information required to render a chunk
struct Chunk
{
	//Translation from local to global x coord
	int baseX;
	//Translation from local to global z coord
	int baseZ;
	//8bit int array representing the blocks in the chunk
	u8 *chunkData;
	//model representing the chunk.
	struct Model terrain;
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

//Constants that are used for chunk sizes.
extern int CHUNK_X;
extern int CHUNK_Y;
extern int CHUNK_Z;
//size of XY cuz it get calculated an awful lot
extern int CHUNK_XY; 
//Size of entire chunk cuz following same previous naming convention
extern int CHUNK_XYZ;


//=======================================================================================
//= Render                                                                              =
//=======================================================================================
//Render introduction
//

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

//=======================================================================================
//= Chunks                                                                              =
//=======================================================================================
//Chunk Introduction
//Here's how a chunk works, you have a list of unsigned integers, it is a one dimensional
// array which has it's structure implied using the chunk size variables. specifically 
//it goes [z][y][x]. This list of integers represents the chunk data, an entry at x,y,z 
//is the block ID at that coordinate, the x,y,z coordinate are loccal to the chunk data. 
//To translate this globally the chunk has a translation point, which refers to the global 
//point at the bottom top left. ie the chunk with translation point 0,0,0 will be the 
//chunk to the bottom right of the origin.

//Chunk functions

//Take an integer list representing chunk data and transform it into a locally positioned model STILL TO BE OPTIMISED
struct Model createChunkModel(u8 *chunkData);
//Create a simple tester chunk that ins't flat and takes the chunk base location of the chunk into account.
//Remove when we have a working noise generator function.
struct Chunk createWaveyChunk(int baseX, int baseY);
//Renders the chunk, added here instead of the regular render model because it'll be expanded
void renderChunk(struct Chunk chunk);
//Generate a flat chunk ID list, not a chunk in whole just the block data
u8 *createTestChunkID();




//make the model
struct Model makeTestCube();
//render the model to be replaced with a couple different types of render function.
void renderTestCube(struct Model model, f32 param, f32 x, f32 y, f32 z);