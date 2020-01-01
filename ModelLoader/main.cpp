

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

//#include <bullet/btBulletCollisionCommon.h>
//#include <bullet/btBulletDynamicsCommon.h>
#include <iostream>

#include "SysOpenGLInit.h"
#include "SysOpenGLSetting.h"
#include "MngrShader.h"
#include "ModelLoader.h"
#include "CameraFreeLook.h"
#include "Input.h"
#include "Shader.h"
#include "Light.h"
#include "STModel.h"

SDL_Window* window;
SDL_GLContext context;
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 720;
bool quit = false;

int main(int argc, char* args[]) {
	/*-----------
	OPENGL / SDL
	-----------*/
	if (unsigned int e = SystemOpenGLInit::defaults(window, context, SCREEN_WIDTH, SCREEN_HEIGHT, 3, 3) > 0) return e;
	SysOpenGLSetting::defaults(window, SCREEN_WIDTH, SCREEN_HEIGHT);


	/*---
	IMGUI
	----*/
	bool err = glewInit() != GLEW_OK;
	if (err) {
		std::cout << "Error loading GLew for Imgui" << std::endl;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330");

	/*--------------
	INITIALIZATIONS
	--------------*/
	ModelLoader box = ModelLoader();
	Input input = Input();
	CameraFreeLook camera = CameraFreeLook(SCREEN_WIDTH, SCREEN_HEIGHT);
	Light light = Light();
	Shader modelShader = Shader("shaders/model_diffuse_lighting.vert", "shaders/model_diffuse_lighting.frag");
	Shader lightShader = Shader("shaders/light.vert", "shaders/light.frag");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();

	/*-----------
	LOAD MODELS
	-----------*/
	STModel stBox;
	stBox.position = glm::vec3(0, 0, 0);
	box.loadModel("assets/hospitalroom/hp194.obj");

	light.load(glm::vec3(0, 16, 0));

	double dt = 1.0f / 60.0; 
	bool wireframeToggle = false;

	while (!quit) {
		/*-----------
		UPDATE
		-----------*/
		input.update(dt);
		if (input.isLShift()) {
			SysOpenGLSetting::mouseCursor(false);
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

			if (input.isA()) camera.strafeLeft(dt);
			if (input.isD()) camera.strafeRight(dt);
			if (input.isW()) camera.forward(dt);
			if (input.isS()) camera.backward(dt);

			if (input.isMouseMotion()) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				camera.mousePositionUpdate(dt, x, y);
			}
		}
		else {
			SysOpenGLSetting::mouseCursor(true);
		}
		if (input.isQuit()) quit = true;
		if (input.isO()) SysOpenGLSetting::wireframe(true);
		if (input.isP()) SysOpenGLSetting::wireframe(false);


		view = camera.getViewMatrix();

		/*-----------
		RENDER MODELS
		-----------*/
		box.draw(projection, view, modelShader, stBox, light.position);
		light.draw(projection, view, lightShader);

		/*-----------
		IMGUI RENDER
		-----------*/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		ImGui::Begin("Information", NULL, window_flags);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::SetWindowPos(ImVec2(0, 0), true);
		ImGui::SetNextWindowSize(ImVec2(200, SCREEN_HEIGHT), 0);
		ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::Text("Position %.3f, %.3f, %.3f", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		ImGui::Text("Yaw %.3f", camera.yaw);
		ImGui::Text("Pitch %.3f", camera.pitch);

		ImGui::End();

		light.gui();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		 
		/*-----------
		SWAP BUFFER
		-----------*/
		SDL_GL_SwapWindow(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);
	}

	return SystemOpenGLInit::shutDown(window, context);
};