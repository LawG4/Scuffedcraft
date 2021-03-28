/**
 * Handels the specifics of rendering a block that is different from rendering a generic mesh, that is loading in a texture atlas and storing copies of a 
 * generic Quad mesh so that can be copied and translated into chunk local space to construct a chunk mesh.
 * It also handles turning a block list into a mesh so that it can be rendered.
 */
#ifndef BLOCK_INCLUDE
#define BLOCK_INCLUDE 

#include "Mesh.h"
typedef struct QuadVT QuadVT;
typedef struct QuadTX QuadTX;

/**
 * Recieves a pointer to a chunk and transforms the block list into a mesh and adds the mesh to the chunk
 * @param chunk The pointer to the chunk to meshify
 */
void meshifyChunk(Chunk *chunk);

/**
 * Some basic faces that are used as templates to create a block
 */

/*A Quad representing a Top face centred at (0,0)*/
extern QuadVT Face_Cube_T;
/*A Quad representing a Bottom face centred at (0,0)*/
extern QuadVT Face_Cube_B;
/*A Quad representing a North face centred at (0,0)*/
extern QuadVT Face_Cube_N; 
/*A Quad representing a South face centred at (0,0)*/
extern QuadVT Face_Cube_S;
/*A Quad representing a East face centred at (0,0)*/
extern QuadVT Face_Cube_E;
/*A Quad representing a West face centred at (0,0)*/
extern QuadVT Face_Cube_W;
/*Grass Textures*/
extern QuadTX grass_Tex_T;
extern QuadTX grass_Tex_A;
#endif