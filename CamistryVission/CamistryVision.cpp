#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "Components/AtomComponent.h"
#include "Components/ElectronComponent.h"
#include "Components/MoleculeComponent.h"
#include "Components/RotationComponent.h"

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

GLFWwindow* window;
Aruco::ArucoHandler a;

std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing
std::vector<data::Molecule> molecules; // same here

int main()
{

	std::cout << "start camistry vision" << std::endl;

	if (!glfwInit())
		throw "Could not initialize glwf";

	window = glfwCreateWindow(800, 800, "CamistryVision", NULL, NULL);

	//a = Aruco::ArucoHandler();
	//a.start();

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	//loads atom and molecule data
	nlohmann::json jsonObject = FileIO::loadJsonFile("Resources/VisualCamistryJSON.json");

	atoms = camvis::JsonParser::deserializeAtoms(jsonObject);
	molecules = camvis::JsonParser::deserializeMolecules(jsonObject, atoms);

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

	// Create first test gameobject
	GameObject* testCore = new GameObject();

	
	int atomIndex = 1;

	std::map<int, int> atomMap;
	for (auto& a : molecules[2].atoms) {
		if (atomMap.count(a.atomNumber)) {
			atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
		}
		else {
			atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
		}
	}
	
	component::MoleculeComponent* molecule = new component::MoleculeComponent(atomMap, atoms);
	testCore->addComponent(molecule);
	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	component::RotationComponent* rotate = new component::RotationComponent();
	testCore->addComponent(rotate);
	testCore->scale(glm::vec3(0.8, 0.8, 0.8));
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

void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

	for (auto gameObject : gameObjects)
	{
		gameObject->update(deltaTime);
	}

}

int rot = 0;

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setModelMatrix(glm::mat4(1.0f));


	for (auto gameobject : gameObjects)
	{
		gameobject->draw();
	}
}
