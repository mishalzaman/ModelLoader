
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "SysOpenGLInit.h"
#include "SysOpenGLSetting.h"
#include "MngrShader.h"
#include "ModelLoader.h"
#include "CameraFreeLook.h"
#include "Input.h"
#include "Shader.h"

SDL_Window* window;
SDL_GLContext context;
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 720;
bool quit = false;


int main(int argc, char* args[]) {
	if (unsigned int e = SystemOpenGLInit::defaults(window, context, SCREEN_WIDTH, SCREEN_HEIGHT, 3, 3) > 0) return e;
	SysOpenGLSetting::defaults(window, SCREEN_WIDTH, SCREEN_HEIGHT);

	ModelLoader silentHillRoom = ModelLoader();
	Input input = Input();
	CameraFreeLook camera = CameraFreeLook(SCREEN_WIDTH, SCREEN_HEIGHT);
	Shader modelShader = Shader("shaders/model_diffuse.vert", "shaders/model_diffuse.frag");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();


	silentHillRoom.loadModel("assets/hospitalroom/hp194.obj");

	double dt = 1.0f / 60.0; 

	while (!quit) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);

		// UPDATE
		input.update(dt);
		if (input.isQuit()) quit = true;
		if (input.isA()) camera.strafeLeft(dt);
		if (input.isD()) camera.strafeRight(dt);
		if (input.isW()) camera.forward(dt);
		if (input.isS()) camera.backward(dt);
		if (input.isO()) SysOpenGLSetting::wireframe(true);
		if (input.isP()) SysOpenGLSetting::wireframe(false);
		if (input.isMouseMotion()) {
			int x, y;
			SDL_GetMouseState(&x, &y);
			camera.mousePositionUpdate(dt, x, y);
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		}

		view = camera.getViewMatrix();

		// draw
		silentHillRoom.draw(projection, view, modelShader);
		 
		SDL_GL_SwapWindow(window);
	}

	return SystemOpenGLInit::shutDown(window, context);
};