#include <iostream>
#include <opencv2/aruco.hpp>
#include "tigl.h"
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

#include "handlers/SceneHandler.h"
#include "handlers/DataHandler.h"

#include "Util/FiloIO.h"
#include "CardScanning/ArucoHandler.h"
#include "CardScanning/MarkerData.h"
#include "Util/JSONParser.h"
#include "Data/Matter/Matter.h"
#include "Data/Matter/Atom.h"
#include "Data/Model/CameraTexture.h"
#include "debuging/DebugWindow.h"
#include "debuging/imgui/imgui.h"


#define DEBUG_ENABLED

double lastUpdateTime;
void update();
void draw();
void init();

using namespace camvis;

GLFWwindow* window;
Aruco::ArucoHandler a;
bool showStatsWindow = true;

data::CameraTexture camTex;
float fov = 30.2f;

handlers::SceneHandler* sceneHandler;

//Temporary
std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing
std::vector<data::Molecule> molecules; // same here
bool isRunning = true;

int main()
{	
	
	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	//480 height
	//640 width
	window = glfwCreateWindow(1920, 1080, "CamistryVision", NULL, NULL);

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

	while (isRunning && !glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	a.stop();
	glfwTerminate();

	return 0;
}

void init()
{

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
		isRunning = false; a.stop(); glfwTerminate(); 
		}
	);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
			if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
				debugging::DebugWindow::isDebugEnabled(true);
		});

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

void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

    sceneHandler->update(deltaTime);

#ifdef DEBUG_ENABLED
	if (debugging::DebugWindow::isDebugEnabled())
	{
		// Show Frame statistics
		ImGui::Begin("Stats", &showGeneralDebug);
		ImGui::Text("Frame time: %.2f", deltaTime);
		ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
		ImGui::End();
	}
#endif
   
}

int rot = 0;

void drawBackground() {

	glDisable(GL_DEPTH_TEST);

	cv::Mat image = a.getLastImage();
	
	camTex.UpdateTexture(image.data, image.rows, image.cols, image.channels() == 3 ? GL_RGB : GL_RGBA);
	camTex.bind();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float widthX = (viewport[2] / 100.f);
	float widthY = (viewport[3] / 100.f);
	glm::mat4 projection = glm::ortho(-widthX, widthX, -widthY, widthY, 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.00f, 1.00f)));
	tigl::shader->enableTexture(true);

	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3( widthX, -widthY, -0.1f),  glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-widthX, -widthY, -0.1f),  glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-widthX,  widthY, -0.1f),  glm::vec2(0, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3( widthX,  widthY, -0.1f),  glm::vec2(1, 0)));
	tigl::end();																

	tigl::shader->enableTexture(false);
}

void draw()
{

	drawBackground();

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);	

	glm::mat4 projection = glm::perspective(glm::radians(fov), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	// Draw the scene
	sceneHandler->draw();
}
