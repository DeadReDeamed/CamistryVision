#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "Components/AtomComponent.h"
#include "Components/ElectronComponent.h"


double lastUpdateTime;
void update();
void draw();
void init();
#include "CardScanning/ArucoHandler.h"

using namespace camvis;

GLFWwindow* window;
Aruco::ArucoHandler a;

std::vector<GameObject*> gameObjects;

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
	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
	component::AtomComponent* atomComponent = new component::AtomComponent(45);
	testCore->addComponent(atomComponent);

	std::vector<component::Shell*> shells;

	component::Shell* shell = new component::Shell();
	shell->amount = 6;
	shell->distance = 10;
	shell->speed = glm::vec3(20.0f, 10.0f, 30.0f);
	shells.push_back(shell);

	component::Shell* shell2 = new component::Shell();
	shell2->amount = 4;
	shell2->distance = 13;
	shell2->speed = glm::vec3(10.0f, 30.0f, 50.0f);
	shells.push_back(shell2);

	component::Shell* shell3 = new component::Shell();
	shell3->amount = 12;
	shell3->distance = 16;
	shell3->speed = glm::vec3(40.0f, 60.0f, 40.0f);
	shells.push_back(shell3);


	component::ElectronComponent* electronComponent = new component::ElectronComponent(shells);
	testCore->addComponent(electronComponent);

	gameObjects.push_back(testCore);
	component::AtomComponent* comp = testCore->getComponent<component::AtomComponent>();


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
