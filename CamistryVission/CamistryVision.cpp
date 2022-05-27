#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "Components/AtomComponent.h"

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
	
	a = Aruco::ArucoHandler();
	a.start();

	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();
	tigl::shader->enableTexture(true);
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
	GameObject* testObject = new GameObject();
	testObject->scale = glm::vec3(0.1, 0.1, 0.1);

	component::AtomComponent* atomComponent = new component::AtomComponent(7);
	testObject->addComponent(atomComponent);

	gameObjects.push_back(testObject);
	component::AtomComponent* comp = testObject->getComponent<component::AtomComponent>();
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

	glEnable(GL_DEPTH_TEST);

	//tigl::shader->enableTexture(true);
	tigl::shader->enableColor(true);

	for (auto gameobject : gameObjects)
	{
		gameobject->draw();
	}
}
