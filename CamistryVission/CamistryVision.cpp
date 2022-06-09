#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

#include "../Components/AtomComponent.h"
#include "../Components/ElectronComponent.h"

#define DEBUG_ENABLED
#include "handlers/SceneHandler.h"
#include "handlers/DataHandler.h"

#include "CardScanning/ArucoHandler.h"
#include "CardScanning/MarkerData.h"
#include "Util/JSONParser.h"
#include "debuging/DebugWindow.h"
#include "debuging/imgui/imgui.h"

double lastUpdateTime;
void update();
void draw();
void init();

using namespace camvis;

GLFWwindow* window;
Aruco::ArucoHandler a;
bool showStatsWindow = true;

unsigned int cameraTexture;

handlers::SceneHandler* sceneHandler;
int main()
{

	std::cout << "start camistry vision" << std::endl;

	a = Aruco::ArucoHandler();
	a.start();

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

	// Starting the debug gui
#ifdef DEBUG_ENABLED
	debugging::DebugWindow::init(window);
#endif // DEBUG_ENABLED

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
#ifdef DEBUG_ENABLED
		debugging::DebugWindow::startFrame();
#endif

		update();
		draw();

#ifdef DEBUG_ENABLED
		debugging::DebugWindow::endFrame();
#endif
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void init()
{
	lastUpdateTime = glfwGetTime();	

	handlers::DataHandler::getInstance()->loadData("Resources/VisualCamistryJSON.json", "Resources/scenes.json");

	sceneHandler = new handlers::SceneHandler(&a);
	sceneHandler->changeScene(0);


}

bool showGeneralDebug = true;
void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

	// Updating the scene
	sceneHandler->update(deltaTime);
	
#ifdef DEBUG_ENABLED
	// Show Frame statistics
	ImGui::Begin("Stats", &showGeneralDebug);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();
#endif
}

int rot = 0;

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);	
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	// Draw the scene
	sceneHandler->draw();
}