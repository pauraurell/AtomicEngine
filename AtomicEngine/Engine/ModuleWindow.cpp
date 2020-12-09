#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	atLOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		atLOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			atLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	atLOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetBrightness(float bright)
{
	SDL_SetWindowBrightness(window, bright);
}

void ModuleWindow::SetWidth(int width)
{
	SDL_SetWindowSize(window, width, App->gui->height);
}

void ModuleWindow::SetHeight(int height)
{
	SDL_SetWindowSize(window, App->gui->width, height);
}

void ModuleWindow::SetFullscreen(bool state)
{
	if (state)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if (!state)
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable(bool state)
{
	if (state)
		SDL_SetWindowResizable(window, SDL_TRUE);
	else if (!state)
		SDL_SetWindowResizable(window, SDL_FALSE);
}

void ModuleWindow::SetBorderless(bool state)
{
	if (state)
		SDL_SetWindowBordered(window, SDL_TRUE);
	else if (!state)
		SDL_SetWindowBordered(window, SDL_FALSE);
}

void ModuleWindow::SetFullDesktop(bool state)
{
	if (state)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if (!state)
		SDL_SetWindowFullscreen(window, 0);
}
