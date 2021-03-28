#include "Render.h"
#include "block.h"

/*Iterate through a linked list of faces and */
void renderLinkedListMesh(Mesh *mesh)
{
	if(!mesh)
	{
		return;
	}
    /*Begin the draw call, we need to tell GX which primative we're rendering and how many of them*/
	GRRLIB_SetTexture(mesh->tex, 0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, mesh->faceCount * 4);
	
	/*Set the first face, and then loop through the mesh until the end
	Use a for loop to avoid iterating too far*/
	Face *face = mesh->firstFace;
	for (size_t i = 0; i < mesh->faceCount; i++) 
	{
		GX_Position3f32(face->vt.x1, face->vt.y1, face->vt.z1);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(face->tx.x1, face->tx.y1);

		GX_Position3f32(face->vt.x2, face->vt.y2,face->vt.z2);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(face->tx.x2, face->tx.y2);

		GX_Position3f32(face->vt.x3, face->vt.y3, face->vt.z3);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(face->tx.x3, face->tx.y3);

		GX_Position3f32(face->vt.x4, face->vt.y4, face->vt.z4);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(face->tx.x4,face->tx.y4);
		
		/*set the next face*/
		face = face->next;
	}
	GX_End();
}