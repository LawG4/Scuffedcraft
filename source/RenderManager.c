//=======================================================================================
//This subsection of code is all for rendering, so far it is limited to                 =
//just rendering the cubes, but it will also handle the cubes as                        =
//that would be too much abstraction                                                    =
//=======================================================================================

#include "RenderManager.h"
#include "gfx/albedo_cube.h"



//=======================================================================================
//= Global variables                                                                    =
//=======================================================================================
//----------------------------  x1     x2     x3     x4     y1     y2     y3     y4     z1     z2     z3    z4
struct Face_VT Face_Cube_N = {-1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f};
struct Face_VT Face_Cube_S = { 1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
struct Face_VT Face_Cube_T = {-1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f};
struct Face_VT Face_Cube_B = {-1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f};
struct Face_VT Face_Cube_E = {-1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f};
struct Face_VT Face_Cube_W = { 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f};

//-----------------------------x1    x2     x3   x4    y1    y2    y3    y4
struct Face_TX grass_Tex_T = {0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f}; //Top grass texture
struct Face_TX grass_Tex_A = {0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f}; //Grass textures on the sides and also bottom



//=======================================================================================
//= Face and model support functions
//=======================================================================================

struct Face *createTranslatedFace(struct Face_VT baseVerticies, struct Face_TX baseTex, f32 offsetX, f32 offsetY, f32 offsetZ)
{
	//allocate the space required for the new face.
	struct Face *face = malloc(sizeof(struct Face)); 
	//now copy the baseVerticies into the new one
	face->vt = baseVerticies;
	face->tx = baseTex;
	//now move all the base verticies by the offset.
	face->vt.x1 = face->vt.x1 + offsetX;
	face->vt.x2 = face->vt.x2 + offsetX;
	face->vt.x3 = face->vt.x3 + offsetX;
	face->vt.x4 = face->vt.x4 + offsetX;

	face->vt.y1 = face->vt.y1 + offsetY;
	face->vt.y2 = face->vt.y2 + offsetY;
	face->vt.y3 = face->vt.y3 + offsetY;
	face->vt.y4 = face->vt.y4 + offsetY;

	face->vt.z1 = face->vt.z1 + offsetZ;
	face->vt.z2 = face->vt.z2 + offsetZ;
	face->vt.z3 = face->vt.z3 + offsetZ;
	face->vt.z4 = face->vt.z4 + offsetZ;

	return face;
}

void addFaceToModel(struct Model *model, struct Face *face)
{
	//simply take the new element and push it to be the first
	//element in the linked list.
	//TODO finish
}

void deleteModel(struct Model *model)
{
	//TODO finish
}












//So this will be the method to render a model. it renders the model at the centre coordinates
//so an example of use will be that each chunk will share the same texture, and will consist 
//of a list of faces, the cube objects themselves will not be a model and will instead be their
//own object to allow for face aligning.
void renderModel(struct Model model )
{
	//loop over every face. and for now just draw it as a white square
	//Begin the draw call
	GRRLIB_SetTexture(model.tex, 0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, model.size * 4);
	
	for (size_t i = 0; i < model.size; i++)
	{
		GX_Position3f32(model.faces[i].x1, model.faces[i].y1, model.faces[i].z1);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(model.texCoords[i].x1, model.texCoords[i].y1);

		GX_Position3f32(model.faces[i].x2, model.faces[i].y2, model.faces[i].z2);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(model.texCoords[i].x2, model.texCoords[i].y2);

		GX_Position3f32(model.faces[i].x3, model.faces[i].y3, model.faces[i].z3);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(model.texCoords[i].x3, model.texCoords[i].y3);

		GX_Position3f32(model.faces[i].x4, model.faces[i].y4, model.faces[i].z4);
		GX_Color1u32(0xFFFFFFFF);
		GX_TexCoord2f32(model.texCoords[i].x4, model.texCoords[i].y4);
	}
	GX_End();
}

struct Model makeTestCube()
{
	//First lets create a cube based on the paramater
	struct Model model;
	model.size = 6;
	model.faces = malloc(6 * sizeof(struct Face_VT)); //just one face for now

	model.faces[0] = Face_Cube_T;
	model.faces[1] = Face_Cube_B;
	model.faces[2] = Face_Cube_N;
	model.faces[3] = Face_Cube_S;
	model.faces[4] = Face_Cube_E;
	model.faces[5] = Face_Cube_W;

	//now lets fill in the texture coordinates
	model.texCoords = malloc (6 * sizeof(struct Face_TX));
	//each one uses the same texture coordinates 
		model.texCoords[0] = grass_Tex_T;
	for (size_t i = 1; i < 6; i++)
	{
		model.texCoords[i] = grass_Tex_A;
	}
	
	//now give this model a texture 
	model.tex = GRRLIB_LoadTexture(albedo_cube);
	//so now then let's render this bad boi
	//return this model so we're not making it every turn
	return model;

}

void renderTestCube(struct Model model, f32 param, f32 x, f32 y, f32 z)
{
	GRRLIB_ObjectView(x,y,z,0,param,0,1,1,1);
	renderModel(model);
}