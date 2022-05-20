#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

double lastUpdateTime;
void update();
void draw();
void init();
#include "CardScanning/ArucoHandler.h"
#include "Components/Component.h"
#include "Components/DrawComponent.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

camvis::component::DrawComponent* comp;

int main()
{	
	
	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	auto window = glfwCreateWindow(1000, 800, "CamistryVision", NULL, NULL);
	
	//Aruco::ArucoHandler a = Aruco::ArucoHandler();
	//a.start();

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();
	tigl::shader->enableTexture(true);
	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void init()
{
	lastUpdateTime = glfwGetTime();
	comp = new camvis::component::DrawComponent(new camvis::data::Model("C:\\Users\\david\\Documents\\School\\3DG\\wk3_fps\\wk3_fps\\models\\steve\\steve.obj"));
}

void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;
}

int rot = 0;

void draw()
{

	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	rot += 1;
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	glEnable(GL_CULL_FACE);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glm::mat4 model(1.0f);
	tigl::shader->setModelMatrix(model);

	model = glm::translate(model, glm::vec3(0, -5, -50));
	model = glm::rotate(model, glm::radians(rot * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	tigl::shader->setModelMatrix(model);
	comp->draw();
	
	//model = glm::translate(model, glm::vec3(-5, 0, 0));
	//tigl::shader->setModelMatrix(model);
	//comp->draw();

	//model = glm::translate(model, glm::vec3(0, -5, 0));
	//tigl::shader->setModelMatrix(model);
	//comp->draw();

	//model = glm::translate(model, glm::vec3(0, 0, -5));
	//tigl::shader->setModelMatrix(model);
	//comp->draw();
	

}
