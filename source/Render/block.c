
#include "block.h"
#include "../gfx/albedo_cube.h"

void meshifyChunk(Chunk *chunk)
{
    /*For every element in the list loop through until we find a not air block and
    turn it into a block*/
    for (s8 y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (s8 z = 0; z < CHUNK_DEPTH; z++)
        {
            for (s8 x = 0; x < CHUNK_WIDTH; x++)
            {
                if(getBlockIDAt(*chunk, x, y, z) != 0)
                {
                    /*For now just add the block to the chunk*/
                    Face *face = createTranslatedFace(Face_Cube_T, grass_Tex_T, x, y, z);
                    addFaceToMesh(chunk->mesh, face);
                }
            }
        } 
    }
    chunk->mesh->tex = GRRLIB_LoadTexture(albedo_cube);
}

/**
 * Fill in the constant variables from the header file
 */
/*Fill in the basic values for the templates*/
/*****************************  x1     x2     x3     x4     y1     y2     y3     y4     z1     z2     z3    z4 */
QuadVT Face_Cube_N = {-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f};
QuadVT Face_Cube_S = { 0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f};
QuadVT Face_Cube_T = {-0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f};
QuadVT Face_Cube_B = {-0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f};
QuadVT Face_Cube_E = {-0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f};
QuadVT Face_Cube_W = { 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f};
/****************************  x1    x2     x3   x4    y1    y2    y3    y4*/  
/*Top grass texture coordinates*/
QuadTX grass_Tex_T = {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f}; 
/*Sides grass texture coordinates*/
QuadTX grass_Tex_A = {0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f}; 