/**
 * Used to represent the mesh that makes up a model and can be rendered to the scene currently we have implemented a mesh to be a linked list of faces,
 * a face consisting of a Quad vertex and Quad texture coordinates. This is because a quad is the primary struct that makes up everything in a voxel 
 * game engine, a linked list is used because the chunk mesh needs to be resized if the player changes it.
 * 
 * TODO: Look into if GX actually uses different primatives when using quad mode, or if it just auto segments them
 * TODO: Look into automatically reallocated array, because linked lists will be HORRIBLE for cache misses considering we use hundreds of face elements
 *       per frame, and since a mesh can only be resized at most once per frame, this would be the better implementation. 
 */
#ifndef MESH_INCLUDE
#define MESH_INCLUDE
#include <grrlib.h>
#include "../Chunks/Chunk.h"
typedef struct Chunk Chunk;

/*A structure that represents the vertex coordinates of a Quad*/
typedef struct QuadVT
{
    /*Coordinates listed clockwise*/
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
	f32 z1, z2, z3, z4;
}QuadVT;

/*A structure that represents the texture coordinates of a Quad*/
typedef struct QuadTX
{
    /*Coordinates listed clockwise*/
	f32 x1, x2, x3, x4;
	f32 y1, y2, y3, y4;
}QuadTX;

/**
 * Represents a face, that is the vertex and texture coordinates, along with a pointer to the next face in the mesh.
 * @param vt The vertex coordinates of this face
 * @param tx The texture coordinates of this face
 * @param next A pointer to the next face in the mesh 
 */
typedef struct Face{
    QuadVT vt;
	QuadTX tx;
	struct Face *next;
}Face;

/**
 * Represents a mesh as list of faces, and meta data about the mesh 
 * @param faceCount The number of faces in this mesh 
 * @param tex The GRRLib texture associated to this mesh
 * @param firstFace A pointer to the first face in the linked list
 */
typedef struct Mesh
{
    u32 faceCount;
    GRRLIB_texImg *tex;
	Face *firstFace;
}Mesh;

/**
 * Free the linked list of faces that make up the mesh
 * @param mesh The mesh to be freed
 */
void freeMesh(Mesh *mesh);

/**
 * Adds a quad face to the end of a face list representing the mesh
 * @param mesh The mesh that needs a face added to it
 * @param face The face to be added to a mesh 
 */ 
void addFaceToMesh(Mesh *mesh, Face *face);

/**
 * Takes a template face and translates it by the offset variables
 * @param templateFace A Quad face used as a template
 * @param templateTex The texture coordinates used for this face
 * @param xOffset The offset in the x direction
 * @param yOffset The offset in the y direction
 * @param zOffset the offset in the z direction
 */ 
Face *createTranslatedFace(QuadVT templateFace, QuadTX templateTex, f32 xOffset, f32 yOffset, f32 zOffset);
#endif