
#include <SDL2/SDL.h>
#include <GL/glew.h>
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


	while (!quit) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);

		/*-----------
		UPDATE
		-----------*/
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

		/*-----------
		RENDER
		-----------*/

		box.draw(projection, view, modelShader, stBox, light.position);
		light.draw(projection, view, lightShader);
		 
		SDL_GL_SwapWindow(window);
	}

	return SystemOpenGLInit::shutDown(window, context);
};