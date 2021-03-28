/**
 * Define how a mesh is constructed, at this point we're going with a linked list implementation, this is because the 
 * mesh that represents a chunk might not be of a fixed size, and the most important thing for us to render is the chunk. 
 * 
 * If we need anything else we can make a specific render function for it.
 */
#include "Mesh.h"

/*Iterate through the linked list of faces and free them.*/
void freeMesh(Mesh *mesh)
{
    /*TODO: Implement this, currentky just a stub*/
}

/*Add a face to a linked list that represents a mesh*/
void addFaceToMesh(Mesh *mesh, Face *face)
{
    /*Need to use a double pointer to edit the pointer itself*
    Push it to the front of the linked list*/
	face->next = mesh->firstFace;
	mesh->firstFace = face;
	mesh->faceCount++;
}

/*Creates a translated face used for constructing a cube*/
Face *createTranslatedFace(QuadVT templateFace, QuadTX templateTex, f32 xOffset, f32 yOffset, f32 zOffset)
{
    /*allocate the space required for the new face.*/
	Face *face = malloc(sizeof(Face)); 
	/*now copy the baseVerticies into the new one*/
	face->vt = templateFace;
	face->tx = templateTex;
	/*now move all the base verticies by the offset.*/
	face->vt.x1 = face->vt.x1 + xOffset;
	face->vt.x2 = face->vt.x2 + xOffset;
	face->vt.x3 = face->vt.x3 + xOffset;
	face->vt.x4 = face->vt.x4 + xOffset;

	face->vt.y1 = face->vt.y1 + yOffset;
	face->vt.y2 = face->vt.y2 + yOffset;
	face->vt.y3 = face->vt.y3 + yOffset;
	face->vt.y4 = face->vt.y4 + yOffset;

	face->vt.z1 = face->vt.z1 + zOffset;
	face->vt.z2 = face->vt.z2 + zOffset;
	face->vt.z3 = face->vt.z3 + zOffset;
	face->vt.z4 = face->vt.z4 + zOffset;

	face->next = NULL;
	return face;
}



