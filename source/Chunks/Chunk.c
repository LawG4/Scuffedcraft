#include "Chunk.h"
#include <stdlib.h>
#include <math.h>
/**
 * takes a x global position and converts it into the x coordinate of the chunk it is contained in 
 */ 
s32 getChunkCornerX(f32 xGlobal)
{
    /*The global coordinate can be modulod by chunk width to get chunk local x coordinate
    Subtract the local coordinate of the global coordinate to get the corner of the chunk*/

    /*If x is negative then we need to move that alignment east one chunk, because negative
    Chunks round to their west corner.*/
    return xGlobal - fmod(xGlobal, CHUNK_WIDTH) - (xGlobal < 0) * CHUNK_WIDTH;
}

s32 getChunkCornerZ(f32 zGlobal)
{
    return zGlobal - fmod(zGlobal, CHUNK_DEPTH) - (zGlobal < 0) * CHUNK_DEPTH;
}

/**
 *Load a chunk in from stored memory, if one doesn't exist generate a new one
 *TODO: File loading, for now this just always generates a new chunk
 *TODO: Seed based random map generating, right now it's always flat 
 */
Chunk *loadChunk(s32 x, s32 z)
{
    Chunk *chunk = (Chunk*)malloc(sizeof(Chunk));
    chunk->mesh = (Mesh*)malloc(sizeof(Mesh));
    chunk->mesh->faceCount = 0;
    chunk->blockList = (u8*)malloc(sizeof(u8) * CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT);
    chunk->xChunk = x;
    chunk->zChunk = z;
    /*Would put in a check here if the chunk has already been generated before*/

    /*Generate a new chunk, for now it is always flat*/
    createFlatChunk(chunk, x, z);
    return chunk;
}

/*Free the resources used by the chunk*/
void freeChunk(Chunk** chunk)
{
    Chunk* toFree = *chunk;
    free(toFree->blockList);
    freeMesh(toFree->mesh);
    free(toFree);
}

/**
 * Get the block ID at the Local coordinate when given a chunk
 * TODO: Check that the input is actually inbounds
 */
u8 getBlockIDAt(Chunk chunk, s8 x, s8 y, s8 z)
{
   /*The layout starts at the south east bottom of the chunk, thats the coordinate of the chunkID
   From there x coordinate increases first
   Then the z coordinate
   Then the y coordinate*/
   return *(chunk.blockList + x + (CHUNK_WIDTH * z) + (CHUNK_WIDTH * CHUNK_DEPTH * y));
}

/**
 * Insert a block into a chunk using Chunk local coordinates (ie modulo the chunk width height depth)
 * TODO: Check input is inbounds
 */
void InsertBlockIDAt(Chunk *chunk, u8 block, s8 x, s8 y, s8 z)
{
    (chunk->blockList[x + (CHUNK_WIDTH * z) + (CHUNK_WIDTH * CHUNK_DEPTH * y)]) = block;
}

void createFlatChunk(Chunk *chunk, s32 x, s32 z)
{
    for (u8 y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (u8 z = 0; z < CHUNK_DEPTH; z++)
        {
            u8 block = (y == 4) ? 1 : 0;
            for (u8 x = 0; x < CHUNK_WIDTH; x++)
            {
                InsertBlockIDAt(chunk, block, x, y, z);
            }   
        }    
    }

}

