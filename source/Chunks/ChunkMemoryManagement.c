/**
 * Lets create some form of chunk cacheing system. There will be three layers to this: 
 *   Calculations Layer - The layer closest to the player where calculations will take place
 *   Rendered Layer     - The chunks that get rendered but no calculations take place 
 *   LRA Layer          - The chunks that are dormant, has a Least recently accessed queue to free chunk data.
 * The chunk memory management system plans to pass chunks from layer to layer until finally it gets 
 * to the bottom of the LRA layer, and then when a new dormant chunk enters the LRA it finally gets freed 
 *
 * The reason this is done is because if the player goes back and forth between two chunks at the border
 * There's no point in freeing and reallocating the chunk at the edge of the render distance 
 * So we want to hang onto a chunk a little bit before we release it
 * 
 * I think it makes the most sense to keep the chunks in the rendering layer in order. since we're only
 * reordering pointers and not the chunks themselves, this should be relitively cheap and as a result 
 * we can get the index of where things are relative to the player for free
 */
#include "Chunk.h"
#include <math.h>
#include "../Render/block.h"
#include "../Logs/Log.h"


/*Maximum number of chunks that we do calculations on around the player*/
#define PLAYER_CHUNK_RADIUS 1
#define PLAYER_CHUNK_CIRC (2 * PLAYER_CHUNK_RADIUS + 1)
#define PLAYER_CHUNK_CACHE (PLAYER_CHUNK_CIRC * PLAYER_CHUNK_CIRC)

/*Maximum number of chunks that we will render. centered on the player*/
#define RENDER_CHUNK_RADIUS 2
#define RENDER_CHUNK_CIRC (2 * RENDER_CHUNK_RADIUS + 1)
#define RENDER_CHUNK_CACHE (RENDER_CHUNK_CIRC * RENDER_CHUNK_CIRC)

/*Maximum number of chunks that will be in the least recently accessed cacheing system*/
#define LRA_CHUNK_CACHE 5

/*Stores the index of the player centred chunk in managed chunk memory*/
#define MANAGED_CENTRE_CHUNK_INDEX (RENDER_CHUNK_CIRC * (RENDER_CHUNK_RADIUS) + RENDER_CHUNK_RADIUS)

/**
 * Allocate memory for the Cache of chunks that will be rendered, within this cache the 
 * Calculation chunks will also be stored to avoid redundency.
 */
Chunk *renderedChunks[RENDER_CHUNK_CACHE];
/**
 * Allocate memory for the chunks that go into the Least Recently Accessed Chunks cache
 */
Chunk *LRAChunks[LRA_CHUNK_CACHE];

/*This is the function that starts the Memory allocator, it is called on boot only once,
Starts to fill the memory with the initial chunks surounding the player so for the rest
of the runtime we only need to worry about chunk memory when a player crosses a boundry*/
void initChunkMemoryManager(f32 xPlayer, f32 zPlayer)
{
    /*Get the global coordinates of the south east corner of the chunk containing the player*/
    s32 xPlayerChunk = getChunkCornerX(xPlayer);
    s32 zPlayerChunk = getChunkCornerZ(zPlayer);
    
    /*Then fill the render chunk memory manager with chunks that suround the player*/
    u8 arrayIndex = 0;
    for (s32 z = 0; z < RENDER_CHUNK_CIRC; z++)
    {
        for (s32 x = 0; x < RENDER_CHUNK_CIRC; x++)
        {
            /*Convert the array index over to the current chunk's global position*/
            s32 xChunk = xPlayerChunk - (x - RENDER_CHUNK_RADIUS) * CHUNK_WIDTH;
            s32 zChunk = zPlayerChunk - (z - RENDER_CHUNK_RADIUS) * CHUNK_DEPTH;
            
            /*Log the chunks x,z coordinates*/
            char message[50];
            sprintf(message, "Chunk id: %u,\t x: %d   \t z: %d", arrayIndex, xChunk, zChunk);
            logMessage(message);

            /*Load the chunk into memory, since we know the chunks around the player have 
            already been generated within the rendering distance, we can simply pull them
            from file storage
            :Note we don't actually have file storage yet, so for now this will just call
            a function to generate a new one*/ 
            Chunk *chunk = loadChunk(xChunk, zChunk);
           
            /*Turn the Block ID list for this chunk into a mesh*/
            meshifyChunk(chunk);

            /*Add this chunk to the list of chunks to be rendered*/
            renderedChunks[arrayIndex] = chunk;
            arrayIndex ++;
        }
    }
}

/**
 * Render the chunks that are inside the memory manager, use the Wii's fixed function pipeline to set
 * the view matrix of the chunk, to transform it from chunk local space to global space
 */ 
void renderManagedChunks()
{   
    for (u8 i = 0; i < RENDER_CHUNK_CACHE; i++)
    {
        if(!renderedChunks[i])
        {
            i++;
            continue;
        }
        GRRLIB_ObjectView(renderedChunks[i]->xChunk, 0, renderedChunks[i]->zChunk, 0, 0, 0, 1, 1, 1);
        renderLinkedListMesh(renderedChunks[i]->mesh);
    }
}

/**
 * Relies on a defined MANAGED_CENTRE_CHUNK_INDEX, which stores the index of the central chunk at compile time 
 */
Chunk *getManagedChunkAtOffsetFromPlayer(s8 x, s8 z)
{
    /*Get the xz position of the last known central chunk*/
    s32 xChunk = renderedChunks[MANAGED_CENTRE_CHUNK_INDEX]->xChunk;
    s32 zChunk = renderedChunks[MANAGED_CENTRE_CHUNK_INDEX]->zChunk;

    char message[150];
    sprintf(message, "Centre Chunk Chunk id: %u,\t x: %d,\t z: %d", MANAGED_CENTRE_CHUNK_INDEX, xChunk, zChunk);
    logMessage(message);

    return NULL;
} 

/**
 * Shift the managed chunks in the cardinal directions, ie the player has moved in the corosponding direction 
 * Now we don't want to free the pointers for the chunk themselves because we want to reuse them so we can 
 * just reorder them, and then free the underlying memory resources.
 * TODO: Currently I'm reusing the same mesh and just changing the ModelToWorld matrix implement actually 
 *          swapping the memory resources
 * TODO: Push resources onto the least recently accessed
 */

void shiftManagedChunksWest(f32 xPlayer, f32 zPlayer)
{
    for (s32 z = 0; z < RENDER_CHUNK_CIRC; z++)
    {
        Chunk *first = renderedChunks[z * RENDER_CHUNK_CIRC];
        for (s32 x = 0; x < RENDER_CHUNK_CIRC - 1; x++)
        {
            /*For each chunk look for the one to the West (Right)*/
            renderedChunks[z * RENDER_CHUNK_CIRC + x] = renderedChunks[z * RENDER_CHUNK_CIRC + x + 1];   
        }
        first->xChunk -= RENDER_CHUNK_CIRC * CHUNK_WIDTH; 
        renderedChunks[z * RENDER_CHUNK_CIRC + RENDER_CHUNK_CIRC - 1] = first;
    }
}

void shiftManagedChunksEast(f32 xPlayer, f32 zPlayer)
{
    for (s32 z = 0; z < RENDER_CHUNK_CIRC; z++)
    {
        Chunk *first = renderedChunks[z * RENDER_CHUNK_CIRC + RENDER_CHUNK_CIRC - 1];
        for (s32 x = RENDER_CHUNK_CIRC - 1; x > 0 ; x--)
        {
            /*For each chunk look for the one to the East (Left)*/
            renderedChunks[z * RENDER_CHUNK_CIRC + x] = renderedChunks[z * RENDER_CHUNK_CIRC + x - 1];   
        }
        first->xChunk += RENDER_CHUNK_CIRC * CHUNK_WIDTH; 
        renderedChunks[z * RENDER_CHUNK_CIRC] = first;
    }
}

void shiftManagedChunksNorth(f32 xPlayer, f32 zPlayer)
{
    for (s32 x = 0; x < RENDER_CHUNK_CIRC; x++)
    {
        Chunk *first = renderedChunks[(RENDER_CHUNK_CIRC - 1) * RENDER_CHUNK_CIRC + x];
        for (s32 z = RENDER_CHUNK_CIRC - 1; z > 0 ; z--)
        {
            /*For each chunk look for the one to the North (Up)*/
            renderedChunks[z * RENDER_CHUNK_CIRC + x] = renderedChunks[(z - 1) * RENDER_CHUNK_CIRC + x];   
        }
        first->zChunk += RENDER_CHUNK_CIRC * CHUNK_DEPTH; 
        renderedChunks[x] = first;
    }
}

void shiftManagedChunksSouth(f32 xPlayer, f32 zPlayer)
{
    for (s32 x = 0; x < RENDER_CHUNK_CIRC; x++)
    {
        Chunk *first = renderedChunks[x];
        for (s32 z = 0; z < RENDER_CHUNK_CIRC - 1; z++)
        {
            /*For each chunk look for the one to the South (Down)*/
            renderedChunks[z * RENDER_CHUNK_CIRC + x] = renderedChunks[(z + 1) * RENDER_CHUNK_CIRC + x];   
        }
        first->zChunk -= RENDER_CHUNK_CIRC * CHUNK_DEPTH; 
        renderedChunks[(RENDER_CHUNK_CIRC - 1) * RENDER_CHUNK_CIRC + x] = first;
    }
}

/**
 * Every single frame once per frame we are going to send the player's position to the memory manager 
 * If the player has moved chunks then we need to shift the order of the managed chunks and possibly load and unload
 */
void playerPositionBasedChunkMemoryManagementUpdate(f32 x, f32 z)
{
    /*Get the xz position of the last known central chunk*/
    s32 xChunk = renderedChunks[MANAGED_CENTRE_CHUNK_INDEX]->xChunk;
    s32 zChunk = renderedChunks[MANAGED_CENTRE_CHUNK_INDEX]->zChunk;
    /*Compare the player's current chunk position and compare*/
    s32 xPlayerChunk = getChunkCornerX(x);
    s32 zPlayerChunk = getChunkCornerZ(z);
    if((xChunk == xPlayerChunk) && (zChunk == zPlayerChunk))
    {
        /*Remained in the same chunk don't need to reshift the order memory*/
        return;
    }
    else
    {
        /*Get how many chunks the player has moved*/
        xChunk = (xPlayerChunk - xChunk) / CHUNK_WIDTH;
        zChunk = (zPlayerChunk - zChunk) / CHUNK_DEPTH;
        /*Shift the chunks*/
        if(xChunk <= -1)
        {
            shiftManagedChunksWest(x,z);
        }
        else if(xChunk >= 1)
        {
            shiftManagedChunksEast(x,z);
        }

        if(zChunk <= -1)
        {
            shiftManagedChunksSouth(x,z);
        }
        else if(zChunk >= 1)
        {
            shiftManagedChunksNorth(x,z);
        }
    }
}