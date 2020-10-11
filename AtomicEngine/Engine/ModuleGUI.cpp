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
	ConfigurationWindowActive = false;

	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	fps = 0;
}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init();

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
	//Example Window
	ImGui::Begin("Test ImGui Window Title", NULL);
	ImGui::Text("Test ImGui Window Text");
	ImGui::End();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				return UPDATE_STOP;
			}

			if (ImGui::MenuItem("Open"))
			{
				return UPDATE_STOP;
			}

			if (ImGui::MenuItem("Save"))
			{
				return UPDATE_STOP;
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
				//Do something
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
		ImGui::End();
	}

	if (ConsoleWindowActive)
	{
		ImGui::Begin("Console", &ConsoleWindowActive);

		ImGui::End();
	}

	//Demo Window
	ImGui::ShowDemoWindow();

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
