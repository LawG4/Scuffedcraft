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
//= Face and model support functions                                                    =
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

	face->next = NULL;

	return face;
}

void addFaceToModel(struct Model *model, struct Face *face)
{
	//simply take the new element and push it to be the first
	//element in the linked list.
	//use a double pointer so that we can edit this pointer itself
	//struct Face * nextFace = *face;
	face->next = model->firstFace;
	model->firstFace = face;
	model->size++;
}

void deallocateModel(struct Model *model)
{
	//take the first element of the model
	struct Face *face = model->firstFace;
	while(face != NULL)
	{
		//set the next face to be destroyed
		struct Face *nextFace = face->next;
		//destroy this face, one face has a set size, 
		//so we can just free it because each face will be malloced
		//instead of the components 
		free(face);
		//make the next one 
		face = nextFace;
	}
	model->size = 0;
}



//=======================================================================================
//= Rendering the models                                                                =
//=======================================================================================

//So this will be the method to render a model. it renders the model at the centre coordinates
//so an example of use will be that each chunk will share the same texture, and will consist 
//of a list of faces, the cube objects themselves will not be a model and will instead be their
//own object to allow for face aligning.
void renderModel(struct Model model )
{
	//Begin the draw call, model is drawn to local coordinates 
	//unless it has it's location set outside this call.
	//we still need the size of the model
	GRRLIB_SetTexture(model.tex, 0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, model.size * 4);
	
	//set the first face 
	struct Face *face = model.firstFace;
	//loop through all the faces until the end.
	while(face != NULL)
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

		//set the next face
		face = face->next;
	}
	GX_End();
}

//Create a test cube by adding one face at a time. 
struct Model makeTestCube()
{
	struct Model model;
	model.tex = GRRLIB_LoadTexture(albedo_cube);

	addFaceToModel(&model, createTranslatedFace(Face_Cube_T, grass_Tex_T, 0,0,0));
	addFaceToModel(&model, createTranslatedFace(Face_Cube_B, grass_Tex_A, 0,0,0));
	addFaceToModel(&model, createTranslatedFace(Face_Cube_N, grass_Tex_A, 0,0,0));
	addFaceToModel(&model, createTranslatedFace(Face_Cube_S, grass_Tex_A, 0,0,0));
	addFaceToModel(&model, createTranslatedFace(Face_Cube_E, grass_Tex_A, 0,0,0));
	addFaceToModel(&model, createTranslatedFace(Face_Cube_W, grass_Tex_A, 0,0,0));
	
	return model;
}

//Render the test cube in a set position 
void renderTestCube(struct Model model, f32 param, f32 x, f32 y, f32 z)
{
	GRRLIB_ObjectView(x,y,z,param,param,param,1,1,1);
	renderModel(model);
}