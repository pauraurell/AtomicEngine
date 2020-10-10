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
				//Do something
			}
			
			if (ImGui::MenuItem("Console"))
			{
				//Do something
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


	return true;
}
