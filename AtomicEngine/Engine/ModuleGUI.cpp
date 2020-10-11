#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"

#include "Glew/include/glew.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	demowindow = false;
	ConfigurationWindowActive = false;
	ConsoleWindowActive = false;
	AboutWindowActive = false;

	fullscreen = false;
	resizable = false;
	borderless = true;
	fulldesktop = false;

	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	fps = 0;

	width = 1280;
	height = 920;
	brightness = 1.0f;
}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

	App->window->SetResizable(resizable);
	App->window->SetBorderless(borderless);

	return true;
}

update_status ModuleGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				
			}

			if (ImGui::MenuItem("Open"))
			{
				
			}

			if (ImGui::MenuItem("Save"))
			{
				
			}

			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				ConfigurationWindowActive = true;
			}
			
			if (ImGui::MenuItem("Console"))
			{
				ConsoleWindowActive = true;
			}
			
			if (ImGui::MenuItem("Wireframe Mode"))
			{
				App->renderer3D->wireframe_mode = !App->renderer3D->wireframe_mode;
			}
			ImGui::EndMenu(); 
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Engine Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/pauraurell/AtomicEngine", NULL, NULL, SW_SHOWNORMAL);
			}

			if (ImGui::MenuItem("ImGui Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
			}

			if (ImGui::MenuItem("Download lastest"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/pauraurell/AtomicEngine", NULL, NULL, SW_SHOWNORMAL);
			}

			if (ImGui::MenuItem("About"))
			{
				AboutWindowActive = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (ConfigurationWindowActive) 
	{
		ImGui::Begin("Configuration", &ConfigurationWindowActive);
		if (ImGui::CollapsingHeader("Application"))
		{
			int fps_cap = 1000 / App->ms_cap;
			if (ImGui::SliderInt("Fps Limit", &fps_cap, 5, 100)) {
				App->ms_cap = 1000 / fps_cap;
			}

			fps_log.erase(fps_log.begin());
			fps_log.push_back(App->fps);
			ms_log.erase(ms_log.begin());
			ms_log.push_back(App->dt * 1000);

			char title[25];
			ImVec2 size(400, 100);
			sprintf_s(title, 25, "Fps %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, size);
			sprintf_s(title, 25, "Milliseconds %.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, size);

		}

		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::SliderFloat("Brightness", &brightness, 0.f, 1.0f);
			App->window->SetBrightness(brightness);

			ImGui::SliderInt("Width", &width, 640, 1920);
			App->window->SetWidth(width);

			ImGui::SliderInt("Height", &height, 480, 1080);
			App->window->SetHeight(height);

			if (ImGui::Checkbox("Fullscreen", &fullscreen))
				App->window->SetFullscreen(fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable))
				App->window->SetResizable(resizable);
			if (ImGui::Checkbox("Borderless", &borderless))
				App->window->SetBorderless(borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fulldesktop))
				App->window->SetFullDesktop(fulldesktop);

		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImVec4 tColor(0.95f, 0.5f, 0.07f, 1.0f);
			ImGui::Text("SDL version:"); ImGui::SameLine();
			ImGui::TextColored(tColor, "%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::Separator();

			int cpu, ram, cache;
			ImGui::Text("CPUs:"); ImGui::SameLine();
			ImGui::TextColored(tColor, "%i", cpu = SDL_GetCPUCount()); ImGui::SameLine();
			ImGui::TextColored(tColor, "(Cache: %iKb)", cache = SDL_GetCPUCacheLineSize());
			ImGui::Text("RAM:"); ImGui::SameLine();
			ImGui::TextColored(tColor, "%i.0GB", ram = (SDL_GetSystemRAM() * 0.001));
			ImGui::Text("Caps: "); ImGui::SameLine();
			//ImGui::TextColored(tColor, "%s", )
		}
		ImGui::End();
	}

	if (ConsoleWindowActive)
	{
		ImGui::Begin("Console", &ConsoleWindowActive);

		ImGui::End();
	}

	if (AboutWindowActive)
	{
		if (ImGui::Begin("About", &AboutWindowActive)) {

			ImGui::TextColored(ImVec4(0.95f, 0.5f, 0.07f, 1.0f),"Atomic Engine");
			ImGui::Separator();

			ImGui::SameLine();
			ImGui::TextWrapped("Atomic Engine is an 3d game engine made by:");
			ImGui::SameLine();
			if (ImGui::SmallButton("Pau Raurell Gomis")) {
				ShellExecuteA(NULL, "open", "https://github.com/pauraurell", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine();
			ImGui::TextWrapped("and");
			ImGui::SameLine();
			if (ImGui::SmallButton("Pol Galan i Morales")) {
				ShellExecuteA(NULL, "open", "https://github.com/pgalmor", NULL, NULL, SW_SHOWNORMAL);
			}

			ImGui::Separator();

			GLint gl_major_version, gl_minor_version;
			glGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
			glGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);
			ImGui::Text("3rd party libraries used: ");
			ImGui::BulletText("SDL"); ImGui::SameLine();
			ImGui::Text("%d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::BulletText("ImGui ");ImGui::SameLine();
			ImGui::Text("%s", ImGui::GetVersion());
			ImGui::BulletText("OpenGL "); ImGui::SameLine();
			ImGui::Text("%d.%d", gl_major_version, gl_minor_version);
			ImGui::BulletText("Glew ");	ImGui::SameLine();
			ImGui::Text("%d.%d.%d", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
			ImGui::BulletText("MathGeoLib"); ImGui::SameLine();
			ImGui::Text("1.5");

			ImGui::Separator();

			ImGui::Text("License: ");
			ImGui::Spacing();
			ImGui::TextWrapped("MIT License");
			ImGui::Spacing();
			ImGui::TextWrapped("Copyright(c) 2020 pauraurell & pgalmor");
			ImGui::Spacing();
			ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy"
				"of this softwareand associated documentation files(the Software), to deal"
				"in the Software without restriction, including without limitation the rights"
				"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"
				"copies of the Software, and to permit persons to whom the Software is"
				"furnished to do so, subject to the following conditions :");
			ImGui::Spacing();
			ImGui::TextWrapped("The above copyright noticeand this permission notice shall be included in all"
				"copies or substantial portions of the Software.");
			ImGui::Spacing();
			ImGui::TextWrapped("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
				"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
				"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
				"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
				"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
				"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
				"SOFTWARE.");
		}
		ImGui::End();
	}

	//Demo Window
	if (demowindow) { ImGui::ShowDemoWindow(&demowindow); }

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate()
{


	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{

	fps_log.clear();
	ms_log.clear();
	return true;
}
