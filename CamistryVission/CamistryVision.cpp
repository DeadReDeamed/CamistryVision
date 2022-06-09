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

	// Create first test gameobject
	GameObject* testCore = new GameObject();
	
	
	int atomIndex = 1;

	std::map<int, int> atomMap;
	for (auto& a : molecules[8].atoms) {
		if (atomMap.count(a.atomNumber)) {
			atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
		}
		else {
			atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
		}
	}
	
	/*
	component::MoleculeComponent* molecule = new component::MoleculeComponent(atomMap, atoms);
	testCore->addComponent(molecule);
	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	component::RotationComponent* rotate = new component::RotationComponent();
	testCore->addComponent(rotate);
	testCore->scale(glm::vec3(1, 1, 1));
	gameObjects.push_back(testCore);
	*/

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
	}
	
	component::ElectronComponent* electronComponent = new component::ElectronComponent(shells);
	testCore->addComponent(electronComponent);
	
	gameObjects.push_back(testCore);
	
	component::AtomComponent* comp = testCore->getComponent<component::AtomComponent>();
	
	GameObject* test = new GameObject();
	test->translate({ 100,10,10 });
	gameObjects.push_back(test);
	/*GameObject* test2 = new GameObject();
	gameObjects.push_back(test2);
	GameObject* test3 = new GameObject();
	gameObjects.push_back(test3);*/
	// Give the gameobject to which the merged result will be put upon.
	// MergeComponent does not delete the atoms/molecules on the cards previous to the combine. This should be done outside of the mergecomponent.
	// To go from atomcomponent to data we will need a list a data::Atom. Find a way to do this since we cannot retrieve which atom is in an AtomComponent.

	component::MergeComponent* mergeComponent = new component::MergeComponent(test,atoms);
	mergeComponent->Combine({ atoms[0], atoms[0] });

	delete mergeComponent;
}

bool showGeneralDebug = true;
void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

    sceneHandler->update(deltaTime);

	for (auto gameObject : gameObjects)
	{
		gameObject->update(deltaTime);
	}

	// PLEASE FOR THE LOVE OF GOD, REMOVE
	bool showCardsDebug = true;

	std::vector<Aruco::MarkerData> detectedMarkers = a.getMarkers();

	for (int i = 0; i < detectedMarkers.size(); i++)
	{
		// Calculate rodrigues transform 
		cv::Mat viewMatrix = cv::Mat::zeros(4, 4, 5);
		cv::Mat rodrigues;
		cv::Rodrigues(detectedMarkers[i].rvec, rodrigues);

		for (unsigned int row = 0; row < 3; ++row)
		{
			for (unsigned int col = 0; col < 3; ++col)
			{
				viewMatrix.at<float>(row, col) = (float)rodrigues.at<double>(row, col);
			}
			viewMatrix.at<float>(row, 3) = (float)detectedMarkers[i].tvec[row] * 0.1f;
		}
		viewMatrix.at<float>(3, 3) = 1.0f;

		cv::Mat cvToGl = cv::Mat::zeros(4, 4, 5);
		cvToGl.at<float>(0, 0) = 1.0f;
		cvToGl.at<float>(1, 1) = -1.0f; // Invert the y axis 
		cvToGl.at<float>(2, 2) = -1.0f; // invert the z axis 
		cvToGl.at<float>(3, 3) = 1.0f;
		viewMatrix = cvToGl * viewMatrix;
		cv::transpose(viewMatrix, viewMatrix);

		glm::mat4 glmMatrix = {
			{(float)viewMatrix.at<float>(0,0), (float)viewMatrix.at<float>(0,1), (float)viewMatrix.at<float>(0,2), (float)viewMatrix.at<float>(0,3)},
			{(float)viewMatrix.at<float>(1,0), (float)viewMatrix.at<float>(1,1), (float)viewMatrix.at<float>(1,2), (float)viewMatrix.at<float>(1,3)},
			{(float)viewMatrix.at<float>(2,0), (float)viewMatrix.at<float>(2,1), (float)viewMatrix.at<float>(2,2), (float)viewMatrix.at<float>(2,3)},
			{(float)viewMatrix.at<float>(3,0), (float)viewMatrix.at<float>(3,1), (float)viewMatrix.at<float>(3,2), (float)viewMatrix.at<float>(3,3)},
		};


		gameObjects[i]->cameraTransform = glmMatrix;


	}
	int it = 1;
	for (int i = 0; i < gameObjects.size() - 1; i++) {
		GameObject* object  = gameObjects[i];
		for (int j = it; j < gameObjects.size(); j++) {
			GameObject* object2 = gameObjects[j];
			float length = glm::length(object->cameraTransform[3] - object2->cameraTransform[3]);
			if (length < 1 && length != 0) {
				std::cout << "Collision" << std::endl;
				//component::MergeComponent merge(object, atoms);
			}
		}

		it++;
	}
	//float length = glm::length(testCore->transform[3] - test->transform[3]);

	ImGui::Begin("Cards", &showCardsDebug);
	sort(detectedMarkers.begin(), detectedMarkers.end(), [&](Aruco::MarkerData x, Aruco::MarkerData y) { return x.id < y.id; });
	for (int i = 0; i < detectedMarkers.size(); i++)
	{
		ImGui::BeginChild("Marker");
		ImGui::Text("ID: %d", detectedMarkers[i].id);
		ImGui::Text("Pos: %.2f, %.2f, %.2f", detectedMarkers[i].transform[0], detectedMarkers[i].transform[1], detectedMarkers[i].transform[2]);
		ImGui::Text("rot: %.2f, %.2f, %.2f", detectedMarkers[i].rotation[0], detectedMarkers[i].rotation[1], detectedMarkers[i].rotation[2]);
		ImGui::EndChild();
	}
	ImGui::End();

	// END

#ifdef DEBUG_ENABLED
    // Show Frame statistics
	ImGui::Begin("Stats", &showGeneralDebug);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();
#endif
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