#include <iostream>
#include <opencv2/aruco.hpp>
#include "tigl.h"
#include <GLFW/glfw3.h>

#include "Util/FiloIO.h"
#include "Util/JSONParser.h"
#include "Data/Matter/Matter.h"
#include "Data/Matter/Atom.h"

double lastUpdateTime;
void update();
void draw();
void init();

#include "CardScanning/ArucoHandler.h"
#include "Util/JSONParser.h"

using namespace camvis;

int main()
{	
	
	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	auto window = glfwCreateWindow(1000, 800, "CamistryVision", NULL, NULL);
	
	Aruco::ArucoHandler a = Aruco::ArucoHandler();
	a.start();

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	nlohmann::json jsonObject = FileIO::loadJsonFile("Resources/VisualCamistryJSON.json");

	camvis::JsonParser::deserializeList<data::Matter>(jsonObject);

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

	//std::cout << deltaTime << std::endl;
}

void draw()
{

}
