/**
 * Chunks are the main system that controls the blocks that make up the world. The world is split into chunks so that it can stretch on infinitly 
 * and parts of the world can be loaded in and out of memory. A chunk has a coordinate that represents it's south east corner (+x is north +z is west)
 * This way, we can simply add the local coordinates to the corner coordinates, and place the chunk into world space.
 * 
 * We dedicate a set amount of memory for the chunks, then we dynamically swap chunks in and out of this reserved statically allocated memory.
 * Then calculations can be reduced to per chunk calculations. I'm thinking it might be best to always store the chunks in the memoty manager in order
 * That way we don't have to worry about doing lookups, we can just know where they are in relation to their offset from the player.
 *
 * Positive X direction is considered West
 * Positive Z direction is considered North
 *
 *              +Z                   ________________________
 *              ^                   |                       |
 *              |                   |                       |
 *              |                   |                       |
 *  +X  <-------+------->  -X       |                       |
 *              |                   |                       |
 *              |                   |                       |
 *              \/                  |_______________________| (Chunk coordinates)
 *             -Z
 */ 

#ifndef CHUNK_INCLUDE
#define CHUNK_INCLUDE
#include <gctypes.h>
#include "../Render/Mesh.h"
#include "../Render/Render.h"

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 32

typedef struct Mesh Mesh;
/*Exposes the functions for the chunking systems that will be required externally*/

/**
 * Define the chunk, that is the list of block IDS, and the number that uniquely identifies a chunk along with the mesh
 * @param xChunk The global x coordinate of the south east corner of the chunk
 * @param zChunk The global z coordinate of the south east corner of the chunk
 * @param blockList This is a list of unsigned ints which represent a series of blocks
 * @param mesh This is the mesh that gets generated from the block list.
 */
typedef struct Chunk
{
    s32 xChunk;
    s32 zChunk;
    u8 *blockList;
    Mesh *mesh;
}Chunk;

/**
 * Takes the x coordinate in global space and converts it to the x coordinate of the south east corner of the chunk containing x
 * @param xGlobal the x coordinate in global space
 * @returns The x coordinate of the chunk's south east corner
 */
s32 getChunkCornerX(f32 xGlobal);

/**
 * Takes the z coordinate in global space and converts it to the z coordinate of the south east corner of the chunk containing z
 * @param zGlobal the z coordinate in global space
 * @returns The z coordinate of the chunk's south east corner
 */
s32 getChunkCornerZ(f32 zGlobal);

/**
 * Allocates and loads a chunk from storage based on chunk coordinates, if no chunk exists in storage, make one using the seed
 * @param x The global x coordinate of the south east corner of the chunk
 * @param z The global z coordinate of the south east corner of the chunk
 * @note At the moment we don't have chunk loading, so this does the fake chunk loading
 * @note Only generates the integer list doesn't create the mesh associated with it
 */
Chunk *loadChunk(s32 x, s32 z);

/**
 * Frees the allocation of the chunk that you specify.
 * @param The chunk to be deallocated
 */
void freeChunk(Chunk** chunk);

/**
 * Get the BlockID which is an u8 representing a block at the specified local coordinate in the chunk
 * @param x Local x coordinate of the block
 * @param y Local y coordinate of the block
 * @param z Local z coordinate of the block
 * @return The ID of the block
 */
u8 getBlockIDAt(Chunk chunk, s8 x, s8 y, s8 z);

/**
 * Insert a block into a chunk at the specified coordinates
 * @param chunk Pointer to the chunk that we are inserting into
 * @param block The block ID that is getting inserted into this chunk
 * @param x Local x coordinate of the block
 * @param y Local y coordinate of the block
 * @param z Local z coordinate of the block
 */ 
void InsertBlockIDAt(Chunk *chunk, u8 block, s8 x, s8 y, s8 z);

/**
*Fills the chunk memory manager on boot based around the players location 
* @param xPlayer The player's starting x coordinate on boot
* @param zPlayer The player's starting z coordinate on boot
*/
void initChunkMemoryManager(f32 xPlayer, f32 zPlayer);

/**
 * Render the memory managed chunks
 */
void renderManagedChunks(); 

/**
 * Get the chunk corresponding to the offset from the chunk the player is currently stored in, it has to be a managed chunk
 * @param x the x offset
 * @param z the z offset
 */ 
Chunk *getManagedChunkAtOffsetFromPlayer(s8 x, s8 z);

/**
 * Once per frame every frame the player file needs to pass the player coordinates to the chunk memory management
 * if the player has moved chunks then we need to shift around the memory managemed chunks
 * @param x x coordinate of the player in global space
 * @param z z coordinate of the player in global space 
 */
void playerPositionBasedChunkMemoryManagementUpdate(f32 x, f32 z);

/**
 * Takes an already allocated chunk and and sets the blocklist to a flat surface.
 * @param chunk The chunk that is getting turned into a flat surface at y =4
 * @param x x Coordinate of the South eastern corner of the chunk.
 * @param z z Coordinate of the South eastern corner of the chunk 
 * @return A freshly allocated chunk which is flat at the coordinates specified
 */
void createFlatChunk(Chunk *chunk, s32 x, s32 z);

#endif