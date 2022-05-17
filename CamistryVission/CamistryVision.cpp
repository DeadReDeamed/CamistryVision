#include <iostream>
#include <opencv2/aruco.hpp>
#include "tigl.h"
#include <GLFW/glfw3.h>

double lastUpdateTime;
void update();
void draw();
void init();
#include "CardScanning/ArucoHandler.h"
int main()
{	
	Aruco::ArucoHandler a = Aruco::ArucoHandler();
	a.start();

	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	auto window = glfwCreateWindow(1000, 800, "CamistryVision", NULL, NULL);
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
		//cv::imshow("Aruco Debug", a.getLastImage());
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
