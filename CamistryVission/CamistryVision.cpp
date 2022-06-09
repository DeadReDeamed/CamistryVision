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

	handlers::DataHandler::getInstance()->loadData("Resources/VisualCamistryJSON.json");

	sceneHandler = new handlers::SceneHandler(&a);
	sceneHandler->changeScene(0);

	// Create first test gameobject
	GameObject* testCore = new GameObject();
	
	int atomIndex = 1;

<<<<<<<<< Temporary merge branch 1
	std::map<int, int> atomMap;
	for (auto& a : molecules[1].atoms) {
		if (atomMap.count(a.atomNumber)) {
			atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
		}
		else {
			atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
		}
=========
	//load and init atom from the json data
	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	component::AtomComponent* atomComponent = new component::AtomComponent(atoms[atomIndex].atomNumber + atoms[atomIndex].neutrons);
	testCore->addComponent(atomComponent);

	std::vector<component::Shell*> shells;

	//load all electrons from the json data.
	for (size_t i = 0; i < atoms[atomIndex].electrons.size(); i++)
	{
		component::Shell* shell = new component::Shell();
		shell->amount = atoms[atomIndex].electrons[i];
		shell->distance = 10 + (2 * i);
		shell->speed = glm::vec3(30.0f + (i * 3), 30.0f + (i * 3), 30.0f + (i * 3));
		shells.push_back(shell);
>>>>>>>>> Temporary merge branch 2
	}
	
	component::MoleculeComponent* molecule = new component::MoleculeComponent(atomMap, atoms);
	testCore->addComponent(molecule);
	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	component::RotationComponent* rotate = new component::RotationComponent();
	testCore->addComponent(rotate);
	testCore->scale(glm::vec3(1, 1, 1));
	gameObjects.push_back(testCore);

	//load and init atom from the json data
	//testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	//component::AtomComponent* atomComponent = new component::AtomComponent(atoms[atomIndex].atomNumber + atoms[atomIndex].neutrons);
	//testCore->addComponent(atomComponent);

	//std::vector<component::Shell*> shells;

	////load all electrons from the json data.
	//for (size_t i = 0; i < atoms[atomIndex].electrons.size(); i++)
	//{
	//	component::Shell* shell = new component::Shell();
	//	shell->amount = atoms[atomIndex].electrons[i];
	//	shell->distance = 10 + (2 * i);
	//	shell->speed = glm::vec3(30.0f + (i * 3), 30.0f + (i * 3), 30.0f + (i * 3));
	//	shells.push_back(shell);
	//}
	//
	//component::ElectronComponent* electronComponent = new component::ElectronComponent(shells);
	//testCore->addComponent(electronComponent);

	//gameObjects.push_back(testCore);
	//component::AtomComponent* comp = testCore->getComponent<component::AtomComponent>();
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