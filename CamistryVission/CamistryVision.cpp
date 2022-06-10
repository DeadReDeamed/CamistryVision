#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

#include "debuging/imgui/imgui.h"
#include "debuging/DebugWindow.h"
#include "GameObject.h"
#include "Components/AtomComponent.h"
#include "Components/ElectronComponent.h"
#include "Components/MoleculeComponent.h"
#include "Components/RotationComponent.h"
#include "Components/MergeComponent.h"
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

//Temporary
std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing
std::vector<data::Molecule> molecules; // same here


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
	//loads atom and molecule data
	atoms = handlers::DataHandler::getInstance()->atoms;
	molecules = handlers::DataHandler::getInstance()->molecules;

	sceneHandler = new handlers::SceneHandler(&a);
	sceneHandler->changeScene(0);

	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(2);
	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightPosition(0, glm::vec3(0, 20, 0));
	tigl::shader->setLightPosition(1, glm::vec3(0, -20, 0));
	tigl::shader->setLightAmbient(0, glm::vec3(0.2f, 0.2f, 0.3f));
	tigl::shader->setLightAmbient(1, glm::vec3(0.05f, 0.05f, 0.075f));
	tigl::shader->setLightDiffuse(0, glm::vec3(0.9f, 0.9f, 0.9f));
	tigl::shader->setLightDiffuse(1, glm::vec3(0.5f, 0.5f, 0.5f));
	tigl::shader->setLightSpecular(0, glm::vec3(0, 0, 0));
	tigl::shader->setLightSpecular(1, glm::vec3(0, 0, 0));
	tigl::shader->setShinyness(30);
}

bool showGeneralDebug = true;
void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

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