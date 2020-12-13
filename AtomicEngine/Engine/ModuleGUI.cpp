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
#include "ModuleSceneIntro.h"


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
	rayChecker = false;

	r = 0.05;
	g = 0.05;
	b = 0.05;
	gridSize = 100;

	showImportWindow = false;
	tempScale = 1.f;
	tempP = { 0.f, 0.f, 0.f };

	origin = true;
	custom_position = false;
	viewtransform = false;
	igLights = false;
	igCameras = false;
	AssetsWindowActive = true;
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

	App->window->GetSize(width, height);

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove;

	bool visible = true;
	if (ImGui::Begin("Game Time Buttons", &visible, flags))
	{
		if (ImGui::Checkbox("Bounding Boxes", &bbChecker)) {
			bbChecker != bbChecker;
		}ImGui::SameLine();

		if (ImGui::Checkbox("Mouse Pick Ray", &rayChecker)) {
			rayChecker != rayChecker;
		}ImGui::SameLine(); ImGui::Text("       "); ImGui::SameLine();

		if (Time::GameTime.GetTimeSinceStartup() > 0) 
		{
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
		else 
		{
			if (ImGui::Button("Play"))
			{
				App->StartGame();
			}

			ImGui::SameLine();
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
			if (ImGui::Button("Go to origin")) { App->camera->Reset(); }
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

		int id = 0;
		AddGOtoHierarchy(App->scene_intro->root, id);

		ImGui::End();
	}

	ImGui::Begin("Assets", &AssetsWindowActive);
	ImGui::End();

	if (InspectorWindowActive)
	{
		ImGui::Begin("Inspector", &InspectorWindowActive);
		if (selectedObj != nullptr) 
		{
			if (printInspector) {
				selectedObj->flag = ImGuiTreeNodeFlags_Selected;
				strcpy(buff, selectedObj->name.c_str());
				ImGui::Checkbox("Enabled", &selectedObj->active); ImGui::SameLine();
				if (ImGui::InputText(" ", buff, IM_ARRAYSIZE(buff), ImGuiInputTextFlags_EnterReturnsTrue)) {
					selectedObj->name.assign(buff);
				}
				ImGui::Text("UUID: %i", selectedObj->UUID);
				printInspector = true;

				ImGui::Separator();
				if (selectedObj->GetCTransform() != nullptr)
				{
					if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("             X"); ImGui::SameLine();
						ImGui::Text("        Y"); ImGui::SameLine();
						ImGui::Text("       Z");
						ComponentTransform* transform = (ComponentTransform*)selectedObj->GetCTransform();

						ImGui::Text("Position "); ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##posx", &transform->pos.x, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetPosition(transform->pos.x, transform->pos.y, transform->pos.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##posy", &transform->pos.y, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetPosition(transform->pos.x, transform->pos.y, transform->pos.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##posz", &transform->pos.z, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetPosition(transform->pos.x, transform->pos.y, transform->pos.z);
							transform->UpdateGlobalMatrix();
						}

						ImGui::Text("Rotation "); ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##rotx", &transform->rot.x, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetRotation(transform->rot.x, transform->rot.y, transform->rot.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##roty", &transform->rot.y, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetRotation(transform->rot.x, transform->rot.y, transform->rot.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##rotz", &transform->rot.z, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetRotation(transform->rot.x, transform->rot.y, transform->rot.z);
							transform->UpdateGlobalMatrix();
						}

						ImGui::Text("Scale    "); ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##scalex", &transform->scale.x, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetScale(transform->scale.x, transform->scale.y, transform->scale.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##scaley", &transform->scale.y, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetScale(transform->scale.x, transform->scale.y, transform->scale.z);
							transform->UpdateGlobalMatrix();
						}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

						if (ImGui::DragFloat("##scalez", &transform->scale.z, 0.05f, 0.f, 0.f, "%.2f")) {
							transform->SetScale(transform->scale.x, transform->scale.y, transform->scale.z);
							transform->UpdateGlobalMatrix();
						}
					}
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
							if (ImGui::Button(" Delete Component ")) {
								selectedObj->DeleteComponent(selectedObj->GetCMaterial());
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
							if (ImGui::Button(" Delete Component ")) {
								selectedObj->DeleteComponent(selectedObj->GetCMaterial()); selectedObj->GetCMesh()->m->color = false;
							}
							if (selectedObj->GetCMaterial() != nullptr)
							{
								ImGui::Text("Texture path:"); ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.95f, 0.5f, 0.07f, 1.0f), "No texture...");
								if (selectedObj->GetCMesh() != nullptr)
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
						ImGui::Checkbox("Disable ", &selectedObj->GetCCamera()->active); ImGui::SameLine();
						if (ImGui::Button("Delete  Component")) { selectedObj->DeleteComponent(selectedObj->GetCCamera()); }
						if (selectedObj->GetCCamera() != nullptr)
						{
							ImGui::Spacing();

							bool fixedVerticalFOV = selectedObj->GetCCamera()->fixedFOV == FixedFOV::FIXED_VERTICAL_FOV;
							bool fixedHorizontalFOV = selectedObj->GetCCamera()->fixedFOV == FixedFOV::FIXED_HORIZONTAL_FOV;

							if (ImGui::RadioButton("Fixed VerticalFOV", fixedVerticalFOV))
								selectedObj->GetCCamera()->fixedFOV = FixedFOV::FIXED_VERTICAL_FOV;
							ImGui::SameLine();
							if (ImGui::RadioButton("Fixed HorizontalFOV", fixedHorizontalFOV))
								selectedObj->GetCCamera()->fixedFOV = FixedFOV::FIXED_HORIZONTAL_FOV;

							ImGui::Spacing();

							//Fixed Vertical FOV Settings
							if (fixedVerticalFOV)
							{
								float verticalFOV = selectedObj->GetCCamera()->frustum.verticalFov * RADTODEG;
								if (ImGui::SliderFloat("Vertical FOV", &verticalFOV, 20.0f, 60.0f))
								{
									selectedObj->GetCCamera()->frustum.verticalFov = verticalFOV * RADTODEG;
									selectedObj->GetCCamera()->frustum.horizontalFov = 2.0f * std::atan(std::tan(selectedObj->GetCCamera()->frustum.verticalFov * 0.5f) * (selectedObj->GetCCamera()->aspectRatio));
								}

								ImGui::Spacing();
								ImGui::Text("Horizontal FOV: %.2f", selectedObj->GetCCamera()->frustum.horizontalFov * RADTODEG);
							}
							//Fixed Horizontal FOV Settings
							else
							{
								float horizontalFOV = selectedObj->GetCCamera()->frustum.horizontalFov * RADTODEG;
								if (ImGui::SliderFloat("Horizontal FOV", &horizontalFOV, 25.0f, 115.0f))
								{
									selectedObj->GetCCamera()->frustum.horizontalFov = horizontalFOV * DEGTORAD;
									selectedObj->GetCCamera()->frustum.verticalFov = 2.0f * std::atan(std::tan(selectedObj->GetCCamera()->frustum.horizontalFov * 0.5f) * (1 / selectedObj->GetCCamera()->aspectRatio));
								}
								ImGui::Spacing();
								ImGui::Text("Vertical FOV: %.2f", selectedObj->GetCCamera()->frustum.verticalFov * RADTODEG);
							}

							ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

							ImGui::DragFloat("Near Plane", &selectedObj->GetCCamera()->frustum.nearPlaneDistance, 0.05f, 0.001f, 50.0f);
							ImGui::DragFloat("Far Plane", &selectedObj->GetCCamera()->frustum.farPlaneDistance, 5.0f, 10.0f, 2000.0f);

							ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

							if (selectedObj->GetCCamera()->beingUsed == false)
							{
								if (ImGui::Button("Use camera")) { App->camera->camera = selectedObj->GetCCamera(); selectedObj->GetCCamera()->beingUsed = true; }
							}
							else
							{
								if (ImGui::Button("Stop using camera")) { App->camera->camera = App->scene_intro->cameras[0]; selectedObj->GetCCamera()->beingUsed = false; App->camera->Reset(); }
							}

							ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

							if (selectedObj->GetCCamera()->camTransform != nullptr)
							{
								if (ImGui::CollapsingHeader("Camera Transform", ImGuiTreeNodeFlags_DefaultOpen))
								{
									ImGui::Text("             X"); ImGui::SameLine();
									ImGui::Text("        Y"); ImGui::SameLine();
									ImGui::Text("       Z");
									ComponentTransform* cTransform = (ComponentTransform*)selectedObj->GetCCamera()->camTransform;

									ImGui::Text("Position "); ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##cposx", &cTransform->pos.x, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetPosition(cTransform->pos.x, cTransform->pos.y, cTransform->pos.z);
										cTransform->UpdateGlobalMatrix();
									}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##cposy", &cTransform->pos.y, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetPosition(cTransform->pos.x, cTransform->pos.y, cTransform->pos.z);
										cTransform->UpdateGlobalMatrix();
									}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##cposz", &cTransform->pos.z, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetPosition(cTransform->pos.x, cTransform->pos.y, cTransform->pos.z);
										cTransform->UpdateGlobalMatrix();
									}

									ImGui::Text("Rotation "); ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##crotx", &cTransform->rot.x, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetRotation(cTransform->rot.x, cTransform->rot.y, cTransform->rot.z);
										cTransform->UpdateGlobalMatrix();
									}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##croty", &cTransform->rot.y, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetRotation(cTransform->rot.x, cTransform->rot.y, cTransform->rot.z);
										cTransform->UpdateGlobalMatrix();
									}ImGui::SameLine(); ImGui::SetNextItemWidth(56.f);

									if (ImGui::DragFloat("##crotz", &cTransform->rot.z, 0.05f, 0.f, 0.f, "%.2f")) {
										cTransform->SetRotation(cTransform->rot.x, cTransform->rot.y, cTransform->rot.z);
										cTransform->UpdateGlobalMatrix();
									}
								}
							}
						}
					}
				}
				ImGui::Separator();
				if (ImGui::Button("Add Component..."))
				{
					ImGui::OpenPopup("AddPopUp");
				}
				if (ImGui::BeginPopup("AddPopUp"))
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
					/*else
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
					}*/
					ImGui::EndPopup();
				}
			}
		}
			
		ImGui::End();
	}

	if (showImportWindow)
	{
		if (ImGui::Begin("Import Options", NULL))
		{
			ImGui::Text("FBX path: %s", file_path);

			ImGui::SetNextItemWidth(70.f);
			if (ImGui::DragFloat("Scale", &tempScale, 0.05f, 0.f, 0.f, "%.1f")) {
				App->scene_intro->tempScale = { tempScale, tempScale, tempScale };
			}

			if (origin == false) {
				custom_position = true; viewtransform = true;
			}
			else if (custom_position == false) { origin = true; viewtransform = false;
			}

			if (ImGui::Checkbox("Import at origin", &origin)) 
			{
				App->scene_intro->tempPosition = {0,0,0};
				custom_position = false;
				viewtransform = false;
			}

			if (ImGui::Checkbox("Import at position", &custom_position))
			{
				origin = false;
				viewtransform = true;
			}

			if (ImGui::Checkbox("Ignore Cameras", &igLights))
			{
			}

			if (ImGui::Checkbox("Ignore Lights", &igCameras))
			{
			}

			if (viewtransform) 
			{
				ImGui::SetNextItemWidth(58.f);
				if (ImGui::DragFloat("X", &tempP.x, 0.05f, 0.f, 0.f, "%.2f")) {
					App->scene_intro->tempPosition.x = tempP.x;
				}ImGui::SameLine();
				ImGui::SetNextItemWidth(58.f);
				if (ImGui::DragFloat("Y", &tempP.y, 0.05f, 0.f, 0.f, "%.2f")) {
					App->scene_intro->tempPosition.y = tempP.y;
				}ImGui::SameLine();
				ImGui::SetNextItemWidth(58.f);
				if (ImGui::DragFloat("Z", &tempP.z, 0.05f, 0.f, 0.f, "%.2f")) {
					App->scene_intro->tempPosition.z = tempP.z;
				}
			}
			
			if (ImGui::Button("Import"))
			{
				showImportWindow = false;
				App->importer->LoadMesh(file_path);
				tempP = { 0,0,0 };
				tempScale = 1;
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")) 
			{
				showImportWindow = false;
				tempP = { 0,0,0 };
				tempScale = 1;
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
	char* file_path = "hola";

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

void ModuleGUI::AddGOtoHierarchy(GameObject* go, int& id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (go == App->scene_intro->root) { flags |= ImGuiTreeNodeFlags_DefaultOpen; }
	if (selectedObj == go) { flags |= ImGuiTreeNodeFlags_Selected; }
	if (go->children.size() == 0) { flags |= ImGuiTreeNodeFlags_Leaf; }

	if (ImGui::TreeNodeEx(go->name.c_str(), flags))
	{
		if (ImGui::IsItemClicked() && go != App->scene_intro->root)
		{
			selectedObj = go;
			printInspector = true;
		}
		ImGui::PushID(id);
		ReparentDragDrop(go, id);
		ImGui::PopID();
		id++;

		GameObjectPopUps();
		for (size_t i = 0; i < go->children.size(); i++)
		{
			AddGOtoHierarchy(go->children[i], id);
		}
		ImGui::TreePop();
	}
}

void ModuleGUI::GameObjectPopUps()
{
	if (selectedObj != nullptr) 
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Delete"))
			{
				App->scene_intro->DeleteGameObject(selectedObj);
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("New Child"))
			{
				selectedObj->CreateNewChild();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void ModuleGUI::ReparentDragDrop(GameObject* go, int& id)
{
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("Hierarchy", &id, sizeof(int));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy"))
		{
			IM_ASSERT(payload->DataSize == sizeof(int));
			int payload_n = *(const int*)payload->Data;
			std::vector<GameObject*> gameObjects = App->scene_intro->GetGameObjects();
			GameObject* target = gameObjects[payload_n];
			target->Reparent(go);
		}
		ImGui::EndDragDropTarget();
	}
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

