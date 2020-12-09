#include "Globals.h"
#include "ModuleGUI.h"
#include "Application.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleWindow.h"
#include "Glew/include/glew.h"
#include "AtTime.h"

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	demowindow = false;
	ConfigurationWindowActive = false;
	ConsoleWindowActive = true;
	AboutWindowActive = false;
	HierarchyWindowActive = true;
	InspectorWindowActive = true;

	fullscreen = false;
	resizable = true;
	borderless = true;
	fulldesktop = false;

	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	fps = 0;

	width = 1280;
	height = 920;
	brightness = 1.0f;
	wireframe_selected = false;

	lightChecker = false;
	smoothChecker = false;
	faceCullingChecker = false;
	gridChecker = true;
	printInspector = false;

	selectedObj = nullptr;

	r = 0.05;
	g = 0.05;
	b = 0.05;
	gridSize = 100;
}

ModuleGUI::~ModuleGUI()
{

}

bool ModuleGUI::Init()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;

	bool visible = true;
	if (ImGui::Begin("Game Time Buttons", &visible, flags))
	{
		if (ImGui::Button("Play")) 
		{ 
			App->StartGame();
		}

		ImGui::SameLine();

		if (Time::GameTime.paused == false)
		{
			if (ImGui::Button("Pause")) { Time::GameTime.Pause(); }
		}

		else 
		{ 
			if (ImGui::Button("Resume")) { Time::GameTime.Resume(); } 
		}
		
		ImGui::SameLine();

		if (ImGui::Button("Stop")) 
		{ 
			App->StopGame(); 
		}

	}
	ImGui::End();
		
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		HierarchyWindowActive = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		InspectorWindowActive = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		ConsoleWindowActive = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->renderer3D->wireframe_mode = !App->renderer3D->wireframe_mode;
	}

	DockingSpace(dockingWin);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project", 0, false, false))
			{}
			
			if (ImGui::MenuItem("Open" ,0, false, false))
			{}

			if (ImGui::MenuItem("Save", "Ctrl + S", false, false))
			{}
		
			if (ImGui::MenuItem("Exit", "Alt + F4"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit", false))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					App->importer->LoadMesh("Assets/3D Objects/Primitives/CubePrimitive.fbx", "Cube");
				}

				if (ImGui::MenuItem("Sphere"))
				{
					App->importer->LoadMesh("Assets/3D Objects/Primitives/SpherePrimitive.fbx","Sphere");
				}

				if (ImGui::MenuItem("Pyramid"))
				{
					App->importer->LoadMesh("Assets/3D Objects/Primitives/PyramidPrimitive.fbx", "Pyramid");
				}

				if (ImGui::MenuItem("Cylinder"))
				{
					App->importer->LoadMesh("Assets/3D Objects/Primitives/CylinderPrimitive.fbx", "Cylinder");
				}

				if (ImGui::MenuItem("Cone"))
				{
					App->importer->LoadMesh("Assets/3D Objects/Primitives/ConePrimitive.fbx", "Cone");
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Camera"))
			{
				App->scene_intro->CreateGameObject(true);
			}

			if (ImGui::MenuItem("Empty Game Object"))
			{
				App->scene_intro->CreateGameObject();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				ConfigurationWindowActive = true;
			}
			
			if (ImGui::MenuItem("Console", "Ctrl + K"))
			{
				ConsoleWindowActive = true;
			}
			
			if (ImGui::MenuItem("Wireframe Mode", "F1", wireframe_selected))
			{
				App->renderer3D->wireframe_mode = !App->renderer3D->wireframe_mode;
			}

			if (ImGui::MenuItem("View Hierarchy", "Ctrl + H"))
			{
				HierarchyWindowActive = true;
			}

			if (ImGui::MenuItem("View Inspector", "Ctrl + I"))
			{
				InspectorWindowActive = true;
			}
			 

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Classic"))
				{
					ImGui::StyleColorsClassic();
				}

				if (ImGui::MenuItem("Dark"))
				{
					ImGui::StyleColorsDark();
				}

				if (ImGui::MenuItem("Light"))
				{
					ImGui::StyleColorsLight();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window", false))
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
				ShellExecuteA(NULL, "open", "https://github.com/pauraurell/AtomicEngine/releases", NULL, NULL, SW_SHOWNORMAL);
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
			ImGui::TextColored(tColor, "%s", GetCaps());
			ImGui::Text("GPU:");ImGui::SameLine(); 
			ImGui::TextColored(tColor, "%s", (const char*)glGetString(GL_RENDERER));
		}

		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Mouse Position:"); ImGui::SameLine();
			ImGui::Text ("%i, %i", App->input->GetMouseX(), App->input->GetMouseY());
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			if (ImGui::Button("Go to origin")) { App->camera->GoToOrigin(); }
			ImGui::SliderFloat("Camera Speed", &App->camera->cam_speed, 0.05f, 1.0f);
			ImGui::SliderFloat("Shift multiplier", &App->camera->speed_multiplier, 1.01, 3);
			ImGui::SliderInt("Camera Sensitivity", &App->camera->sensitivity, 1, 10);
		}

		if (ImGui::CollapsingHeader("Renderer"))
		{
			if (ImGui::Checkbox("Lightning", &lightChecker)) {
				App->renderer3D->SetLight(lightChecker);
			}

			if (ImGui::Checkbox("Face culling", &faceCullingChecker)) {
				App->renderer3D->SetFaceCulling(faceCullingChecker);
			}

			if (ImGui::Checkbox("Polygon Smooth", &smoothChecker)) {
				App->renderer3D->SetPolygonSmooth(smoothChecker);
			}
	
			if (ImGui::Checkbox("Show grid", &gridChecker)) {
				App->scene_intro->grid = gridChecker;
			}

			if (ImGui::Checkbox("Show Bounding Boxes", &bbChecker)) {
				bbChecker != bbChecker;
			}

			ImGui::SliderInt("Grid size", &gridSize, 0, 200);
			
			ImGui::Separator();
			ImGui::Text("Background Color");
			ImGui::SliderFloat("Red", &r, 0, 1);
			ImGui::SliderFloat("Green", &g, 0, 1);
			ImGui::SliderFloat("Blue", &b, 0, 1);
		}

		if (ImGui::CollapsingHeader("Time"))
		{
			ImGui::Columns();
			ImGui::Text("Frame Count:  %i frames", App->frames_since_start);
			ImGui::Text("Time since start up:  %.3f seconds", Time::RealTime.GetTimeSinceStartup());
			ImGui::Text("Real time dt:  %.3f seconds", Time::RealTime.dt);
			ImGui::Separator();
			ImGui::Text("Game time: %.3f seconds", Time::GameTime.GetTimeSinceStartup());
			ImGui::Text("Game time Scale %.1f", Time::GameTime.timeScale);
			ImGui::Text("Game time dt:  %.3f seconds", Time::GameTime.dt);
		}

		ImGui::End();
	}

	if (ConsoleWindowActive)
	{
		ImGui::Begin("Console", &ConsoleWindowActive);
		for (int i = 0; i < logs.size(); i++)
		{
			ImGui::Text("%s", logs[i].c_str()); 
		}
		ImGui::End();
	}

	if (HierarchyWindowActive)
	{
		ImGui::Begin("Hierarchy", &HierarchyWindowActive);
		

		if (App->scene_intro->game_objects.size() > 0) 
		{
			for (int j = 0; j < App->scene_intro->game_objects.size(); j++) 
			{
				const char* name = App->scene_intro->game_objects[j]->name.c_str();
				
				if (ImGui::TreeNodeEx(name, App->scene_intro->game_objects[j]->flag))
				{
					ImGui::TreePop();
				} 
				if (ImGui::IsItemClicked(0))
				{
					for (int i = 0; i < App->scene_intro->game_objects.size(); i++)
					{
						if (App->scene_intro->game_objects[i] != App->scene_intro->game_objects[j])
						{
							App->scene_intro->game_objects[i]->flag = ImGuiTreeNodeFlags_None;
						}
					}

					if (App->scene_intro->game_objects[j] == selectedObj) {
						selectedObj = nullptr;
						App->scene_intro->game_objects[j]->flag = ImGuiTreeNodeFlags_None;
						printInspector = false;
					}

					else {
						selectedObj = App->scene_intro->game_objects[j];
						printInspector = true;
					}
				}		

				if (ImGui::IsItemClicked(1))
				{
					ImGui::OpenPopup("DeletePopUp");
				}
			}

			if (ImGui::BeginPopup("DeletePopUp"))
			{
				if (ImGui::MenuItem("Delete"))
				{
					App->scene_intro->DeleteGameObject(selectedObj);
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

	if (InspectorWindowActive)
	{
		ImGui::Begin("Inspector", &InspectorWindowActive);
		if (printInspector) {
			selectedObj->flag = ImGuiTreeNodeFlags_Selected;
			strcpy(buff, selectedObj->name.c_str());
			ImGui::Checkbox("Enabled", &selectedObj->active); ImGui::SameLine(); 
			if (ImGui::InputText(" ", buff, IM_ARRAYSIZE(buff), ImGuiInputTextFlags_EnterReturnsTrue)) {
				selectedObj->name.assign(buff);
			}
			printInspector = true;
			
			ImGui::Separator();
			if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Checkbox("Freeze", &selectedObj->GetCTransform()->active);
				ImGui::Text("Position"); ImGui::SameLine(); ImGui::Text("x: %.1f", &selectedObj->GetCTransform()->pos.x); ImGui::SameLine(); ImGui::Text("y: %.1f", &selectedObj->GetCTransform()->pos.y); ImGui::SameLine(); ImGui::Text("z: %.1f", &selectedObj->GetCTransform()->pos.z);
				ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::Text("x: %.1f", &selectedObj->GetCTransform()->rot.x); ImGui::SameLine(); ImGui::Text("y: %.1f", &selectedObj->GetCTransform()->rot.y); ImGui::SameLine(); ImGui::Text("z: %.1f", &selectedObj->GetCTransform()->rot.z);
				ImGui::Text("Scale"); ImGui::SameLine(); ImGui::Text("x: 1.0", &selectedObj->GetCTransform()->scale.x); ImGui::SameLine(); ImGui::Text("y: 1.0", &selectedObj->GetCTransform()->scale.y); ImGui::SameLine(); ImGui::Text("z: 1.0", &selectedObj->GetCTransform()->scale.z);
			}
			if (selectedObj->GetCMesh() != nullptr)
			{
				ImGui::Separator();
				if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Checkbox("Active", &selectedObj->GetCMesh()->active); ImGui::SameLine();
					if (ImGui::Button("Delete Component")) { selectedObj->DeleteComponent(selectedObj->GetCMesh()); }
					if (selectedObj->GetCMesh() != nullptr) 
					{
						ImGui::Text("File:"); ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.95f, 0.5f, 0.07f, 1.0f), selectedObj->GetCMesh()->m->filename);
						ImGui::Separator(); ImGui::NextColumn();
						ImGui::Text("Draw:");
						ImGui::Checkbox("Vertex Normals", &selectedObj->GetCMesh()->m->vNormals);
						ImGui::Checkbox("Face Normals", &selectedObj->GetCMesh()->m->fNormals);
						ImGui::Separator();
						ImGui::Text("Indexes: %i", selectedObj->GetCMesh()->m->num_index);
						ImGui::Text("Normals: %i", selectedObj->GetCMesh()->m->num_normals);
						ImGui::Text("Vertex: %i", selectedObj->GetCMesh()->m->num_vertex);
						ImGui::Text("Tex Coords: %i", selectedObj->GetCMesh()->m->num_texcoords);
					}
				}
			}
			if (selectedObj->GetCMaterial() != nullptr)
			{
				ImGui::Separator();
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (selectedObj->GetCMaterial()->hasTex == true)
					{
						ImGui::Checkbox("Disable", &selectedObj->GetCMaterial()->active); ImGui::SameLine();
						if (ImGui::Button(" Delete Component ")) { selectedObj->DeleteComponent(selectedObj->GetCMaterial()); 
						selectedObj->GetCMesh()->m->color = false;
						}
						if (selectedObj->GetCMaterial() != nullptr)
						{
							ImGui::Text("Texture path:"); ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.95f, 0.5f, 0.07f, 1.0f), selectedObj->GetCMaterial()->tex->texName);
							ImGui::Text("Size: %u x %u px", selectedObj->GetCMaterial()->tex->w, selectedObj->GetCMaterial()->tex->h);
							ImGui::Checkbox("Checkers Texture", &selectedObj->GetCMaterial()->tex->checkers);
							ImGui::Separator();
							float color[3] = { selectedObj->GetCMesh()->m->r, selectedObj->GetCMesh()->m->g, selectedObj->GetCMesh()->m->b };
							ImGui::Checkbox("Use Diffuse Texture", &selectedObj->GetCMaterial()->tex->visible);
							ImGui::Checkbox("Use Albedo", &selectedObj->GetCMesh()->m->color);
							ImGui::ColorPicker3("", color, ImGuiColorEditFlags_Float);
							selectedObj->GetCMesh()->m->r = color[0]; selectedObj->GetCMesh()->m->g = color[1]; selectedObj->GetCMesh()->m->b = color[2];
						}
					}
					else
					{
						ImGui::Checkbox("Disable", &selectedObj->GetCMaterial()->active); ImGui::SameLine();
						if (ImGui::Button(" Delete Component ")) { selectedObj->DeleteComponent(selectedObj->GetCMaterial()); selectedObj->GetCMesh()->m->color = false;
						}
						if (selectedObj->GetCMaterial() != nullptr)
						{
							ImGui::Text("Texture path:"); ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.95f, 0.5f, 0.07f, 1.0f), "No texture...");
							if(selectedObj->GetCMesh() != nullptr)
							{
								ImGui::Checkbox("Use Albedo", &selectedObj->GetCMesh()->m->color);
								float color[3] = { selectedObj->GetCMesh()->m->r, selectedObj->GetCMesh()->m->g, selectedObj->GetCMesh()->m->b };
								ImGui::ColorPicker3("", color, ImGuiColorEditFlags_Float);
								selectedObj->GetCMesh()->m->r = color[0]; selectedObj->GetCMesh()->m->g = color[1]; selectedObj->GetCMesh()->m->b = color[2];
							}
						}
					}
				}
			}
			if (selectedObj->GetCCamera() != nullptr)
			{
				ImGui::Separator();
				if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Checkbox("Disable", &selectedObj->GetCCamera()->active); ImGui::SameLine();
					if (ImGui::Button("Delete Component")) { selectedObj->DeleteComponent(selectedObj->GetCCamera()); }
				}
			}
			ImGui::Separator();
			if (ImGui::Button("Add Component..."))
			{
				ImGui::OpenPopup("AddPopUp");
			}
			if (ImGui::BeginPopup("AddPopUp"))
			{
				if (selectedObj->GetCMaterial() == nullptr)
				{
					if (ImGui::MenuItem("Material..."))
					{
						if (selectedObj->GetCMaterial() == nullptr)
						{
							selectedObj->CreateComponent(ComponentType::Material);
						}
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::MenuItem("Camera..."))
					{
						if (selectedObj->GetCCamera() == nullptr)
						{
							selectedObj->CreateComponent(ComponentType::Camera);
						}
						ImGui::CloseCurrentPopup();
					}
				}
				else 
				{
					if (ImGui::IsMouseClicked(0)) 
					{ 
						atLOG("This GameObject already has a Component Material!");
						ImGui::CloseCurrentPopup(); 
					}
					if (ImGui::MenuItem("Material...", 0, false, false))
					{						
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
		
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
			ImGui::BulletText("Assimp"); ImGui::SameLine();
			ImGui::Text("3.1.1");
			ImGui::BulletText("Devil"); ImGui::SameLine();
			ImGui::Text("1.8.0"/*std::to_string(IL_VERSION).c_str()*/);

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
	logs.clear();
	return true;
}

const char* ModuleGUI::GetCaps()
{
	caps.clear();

	if (SDL_Has3DNow()) { caps.append("3D Now, "); }
	if (SDL_HasAVX()) { caps.append("AVX, "); }
	if (SDL_HasAVX2()) { caps.append("AVX2, "); }
	if (SDL_HasAltiVec()) { caps.append("AltiVec, "); }
	if (SDL_HasMMX()) { caps.append("MMX, "); }
	if (SDL_HasRDTSC()) { caps.append("RDTSC, "); }
	if (SDL_HasSSE()) { caps.append("SSE, "); }
	if (SDL_HasSSE2()) { caps.append("SSE2, "); }
	if (SDL_HasSSE3()) { caps.append("SSE3, "); }
	if (SDL_HasSSE41()) { caps.append("SSE41, "); }
	if (SDL_HasSSE42()) { caps.append("SSE42"); }

	return caps.data();
}

void ModuleGUI::DrawUi()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

update_status ModuleGUI::DockingSpace(bool* open)
{
	update_status ret = UPDATE_CONTINUE;

	static bool fullscreen = true;
	static bool padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else { dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode; }

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) { window_flags |= ImGuiWindowFlags_NoBackground; }

	if (!padding) { ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); }
	ImGui::Begin("DockSpace", open, window_flags);
	if (!padding) { ImGui::PopStyleVar(); }
	if (fullscreen) { ImGui::PopStyleVar(2); }

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	return ret;
}
