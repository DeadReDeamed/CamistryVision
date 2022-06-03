#include <iostream>
#include <opencv2/aruco.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

//Delete for test
#include "handlers/SceneHandler.h"
#include "handlers/DataHandler.h"

double lastUpdateTime;
void update();
void draw();
void init();
#include "CardScanning/ArucoHandler.h"

handlers::SceneHandler* sceneHandler;
int main()
{

	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	auto window = glfwCreateWindow(1000, 800, "CamistryVision", NULL, NULL);

	Aruco::ArucoHandler a = Aruco::ArucoHandler();
	a.start();
	
	handlers::DataHandler::getInstance()->scenesA.push_back({0, });
	sceneHandler = new handlers::SceneHandler(&a);
	sceneHandler->changeScene(0);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

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
	
}

void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

	std::cout << deltaTime << std::endl;
}

void draw()
{
	
}