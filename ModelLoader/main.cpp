
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <bullet/btBulletCollisionCommon.h>
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

	ModelLoader box = ModelLoader(glm::vec3(0, 0,0));
	ModelLoader plane = ModelLoader(glm::vec3(0, -1.75f, 0));
	Input input = Input();
	CameraFreeLook camera = CameraFreeLook(SCREEN_WIDTH, SCREEN_HEIGHT);
	Shader modelShader = Shader("shaders/model_diffuse.vert", "shaders/model_diffuse.frag");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.getViewMatrix();


	box.loadModel("assets/box/box.obj");
	plane.loadModel("assets/plane/plane.obj");

	double dt = 1.0f / 60.0; 


	// Bullet
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

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
		box.draw(projection, view, modelShader);
		plane.draw(projection, view, modelShader);
		 
		SDL_GL_SwapWindow(window);
	}

	return SystemOpenGLInit::shutDown(window, context);
};