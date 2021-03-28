/**
 * Manage the rendering of the meshes, currently we only render linked list meshes which I intend to change.
 */ 
#ifndef RENDER_INCLUDE
#define RENDER_INCLUDE
#include "Mesh.h"
typedef struct Mesh Mesh;

/**
 * Render a linked list mesh
 * @param mesh Tthe mesh to be rendered.
 */ 
void renderLinkedListMesh(Mesh *mesh);

#endif