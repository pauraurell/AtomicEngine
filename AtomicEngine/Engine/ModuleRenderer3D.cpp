#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "MathGeoLib\include\MathGeoLib.h"

#pragma comment (lib, "glu32.lib")   
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib") 

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	wireframe_mode = false;
	cube_render = false;
	rectangle_render = false;
	pyramid_render = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum error = glewInit();
	if (error != GL_NO_ERROR)
	{
		LOG("Error initializing glew library ! %s", SDL_GetError());
		ret = false;
	}
	else
	{
		LOG("using Glew %s", glewGetString(GLEW_VERSION));
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 2.5f, 1);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	
		//LoadMeshBuffer();
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update()
{
	//LINE
	//glLineWidth(2.0f);
	//glBegin(GL_LINES);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 10.f, 0.f);
	//glEnd();
	//CUBE WITH DIRECT MODE
	/*glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles
	glVertex3f(0.f, 2.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 2.f, 2.f);
	glVertex3f(0.f, 2.f, 0.f);
	glVertex3f(2.f, 2.f, 0.f);
	glVertex3f(2.f, 0.f, 2.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 2.f);
	glVertex3f(2.f, 2.f, 0.f);
	glVertex3f(2.f, 2.f, 2.f);
	glVertex3f(0.f, 2.f, 0.f);
	glVertex3f(2.f, 2.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(0.f, 2.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 2.f, 2.f);
	glVertex3f(2.f, 0.f, 2.f);
	glVertex3f(2.f, 2.f, 2.f);
	glVertex3f(0.f, 2.f, 2.f);
	glVertex3f(0.f, 0.f, 2.f);
	glVertex3f(2.f, 0.f, 2.f);
	glVertex3f(0.f, 2.f, 2.f);
	glVertex3f(2.f, 2.f, 2.f);
	glVertex3f(2.f, 2.f, 0.f);
	glVertex3f(0.f, 2.f, 2.f);
	glVertex3f(2.f, 2.f, 0.f);
	glVertex3f(0.f, 2.f, 0.f);
	glVertex3f(0.f, 0.f, 2.f);
	glVertex3f(2.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, 2.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(2.f, 0.f, 0.f);
	glEnd();*/
	//glLineWidth(1.0f);
	//CUBE WITH VERTEX ARRAYS
	/*GLfloat vertices[] =
	{
		0.f, 2.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 2.f,
		0.f, 0.f, 2.f,
		0.f, 2.f, 2.f,
		0.f, 2.f, 0.f,
		2.f, 2.f, 0.f,
		2.f, 0.f, 2.f,
		2.f, 0.f, 0.f,
		2.f, 0.f, 2.f,
		2.f, 2.f, 0.f,
		2.f, 2.f, 2.f,
		0.f, 2.f, 0.f,
		2.f, 2.f, 0.f,
		2.f, 0.f, 0.f,
		0.f, 2.f, 0.f,
		2.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 2.f, 2.f,
		2.f, 0.f, 2.f,
		2.f, 2.f, 2.f,
		0.f, 2.f, 2.f,
		0.f, 0.f, 2.f,
		2.f, 0.f, 2.f,
		0.f, 2.f, 2.f,
		2.f, 2.f, 2.f,
		2.f, 2.f, 0.f,
		0.f, 2.f, 2.f,
		2.f, 2.f, 0.f,
		0.f, 2.f, 0.f,
		0.f, 0.f, 2.f,
		2.f, 0.f, 0.f,
		2.f, 0.f, 2.f,
		0.f, 0.f, 2.f,
		0.f, 0.f, 0.f,
		2.f, 0.f, 0.f
	};
	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, vertices, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
	
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate()
{
	
	CheckWireframeMode();
	//RenderMesh(App->importer->myMesh);

	if (cube_render) { CreatePrimitive(Cube); }
	if (rectangle_render) { CreatePrimitive(pRectangle); }
	if (pyramid_render) { CreatePrimitive(Pyramid); }

	
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::RenderMesh(mesh mesh) {
	uint vertex_buffer = 0;

	glGenBuffers(1, (GLuint*)&(vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.num_vertex * 3, mesh.vertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_normals);
	glNormalPointer(GL_FLOAT, 0, NULL);

	uint index_buffer = 0;
	glGenBuffers(1, (GLuint*)&(index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_index, mesh.index, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glDrawElements(GL_TRIANGLES, mesh.num_index, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);


}

void ModuleRenderer3D::LoadMeshBuffer() {
	mesh *mesh= &App->importer->myMesh;

	glGenBuffers(1, (GLuint*)mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->id_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&mesh->id_normals);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);
}

void ModuleRenderer3D::CreatePrimitive(Primitives type) {

	switch (type)
	{
	case Cube: CreateCube();
		break;

	case pRectangle: CreateRectangle();
		break;

	case Pyramid: CreatePyramid();
		break;

	}
}

void ModuleRenderer3D::CreateCube() {
	GLfloat vertices[] = { 2.f, 2.f, 0.f,
			0.f, 2.f, 0.f,
			0.f, 0.f, 0.f,
			2.f, 0.f, 0.f,
			2.f, 0.f, -2.f,
			2.f, 2.f, -2.f,
			0.f, 2.f, -2.f,
			0.f, 0.f, -2.f, };          // 8 of vertex coords

	GLubyte indices[] = { 0,1,2, 2,3,0,   // 36 of indices
				 0,3,4, 4,5,0,
				 0,5,6, 6,1,0,
				 1,6,7, 7,2,1,
				 7,4,3, 3,2,7,
				 4,7,6, 6,5,4 };

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

}

void ModuleRenderer3D::CreateRectangle() {
	GLfloat vertices[] = { 2.f, 2.f, 0.f,
				-2.f, 2.f, 0.f,
				-2.f, 0.f, 0.f,
				2.f, 0.f, 0.f,
				2.f, 0.f, -2.f,
				2.f, 2.f, -2.f,
				-2.f, 2.f, -2.f,
				-2.f, 0.f, -2.f, };          // 8 of vertex coords

	GLubyte indices[] = { 0,1,2, 2,3,0,   // 36 of indices
				 0,3,4, 4,5,0,
				 0,5,6, 6,1,0,
				 1,6,7, 7,2,1,
				 7,4,3, 3,2,7,
				 4,7,6, 6,5,4 };

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	// draw a cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::CreatePyramid() {
	float pyramid_vertices[] = { -1.0f,0.0f,-1.0f,
			1.0f ,0.0f, -1.0f,
			1.0f ,0.0f, 1.0f,
			-1.0f ,0.0f, 1.0f,
			-0.0f ,1.5f, -0.0f
	};

	uint pyramid_indices[]{ 0,1,2,
		2,3,0,3,2,4,
		0,3,4,2,1,4,
		1,0,4
	};

	// activate and specify pointer to vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, pyramid_vertices);

	// draw a pyramid
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, pyramid_indices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleRenderer3D::CheckWireframeMode() {
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		wireframe_mode = !wireframe_mode;

	if (wireframe_mode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		App->gui->wireframe_selected = true;
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		App->gui->wireframe_selected = false;
	}
}

