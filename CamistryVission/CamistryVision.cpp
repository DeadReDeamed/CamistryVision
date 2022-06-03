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


#include "Util/FiloIO.h"
#include "Util/JSONParser.h"
#include "Data/Matter/Matter.h"
#include "Data/Matter/Atom.h"

#define DEBUG_ENABLED

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

unsigned int cameraTexture;

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

	//loads atom and molecule data
	nlohmann::json jsonObject = FileIO::loadJsonFile("Resources/VisualCamistryJSON.json");

	atoms = camvis::JsonParser::deserializeAtoms(jsonObject);
	std::vector<data::Molecule> molecules = camvis::JsonParser::deserializeMolecules(jsonObject, atoms);

	tigl::init();
	init();

	while (!glfwWindowShouldClose(window))
	{
#ifdef DEBUG_ENABLED
		debugging::DebugWindow::startFrame();
#endif // DEBUG_ENABLED

		update();
		draw();

#ifdef DEBUG_ENABLED
		debugging::DebugWindow::endFrame();
#endif // DEBUG_ENABLED

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void init()
{
	lastUpdateTime = glfwGetTime();

	// Starting the debug gui
#ifdef DEBUG_ENABLED
	debugging::DebugWindow::init(window);
#endif // DEBUG_ENABLED


	// Create first test gameobject
	GameObject* testCore = new GameObject();


	glm::mat4 Projection = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);     
	glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));     
	tigl::shader->setProjectionMatrix(Projection);     
	tigl::shader->setViewMatrix(View);

	
//	int atomIndex = 6;
//
//	//load and init atom from the json data
//	testCore->transform = glm::translate(testCore->transform, glm::vec3(0, -5, -50));
//	component::AtomComponent* atomComponent = new component::AtomComponent(atoms[atomIndex].atomNumber + atoms[atomIndex].neutrons);
//	testCore->addComponent(atomComponent);
//
//	std::vector<component::Shell*> shells;
//
//	//load all electrons from the json data.
//	for (size_t i = 0; i < atoms[atomIndex].electrons.size(); i++)
//	{
//		component::Shell* shell = new component::Shell();
//		shell->amount = atoms[atomIndex].electrons[i];
//		shell->distance = 10 + (2 * i);
//		shell->speed = glm::vec3(30.0f + (i * 3), 30.0f + (i * 3), 30.0f + (i * 3));
//		shells.push_back(shell);
//	}
//
//	component::ElectronComponent* electronComponent = new component::ElectronComponent(shells);
//	testCore->addComponent(electronComponent);
//
//	gameObjects.push_back(testCore);
//	component::AtomComponent* comp = testCore->getComponent<component::AtomComponent>();
}

bool showStatsWindow = true;
void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;

	for (auto gameObject : gameObjects)
	{
		gameObject->update(deltaTime);
	}

	// Show Frame statistics
	ImGui::Begin("Stats", &showStatsWindow);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(1, &cameraTexture);
	glBindTexture(GL_TEXTURE_2D, cameraTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	// Update the image
	cv::Mat image = cv::Mat();
	a.getLastImage().copyTo(image);

	glTexImage2D(GL_TEXTURE_2D,         // Type of texture
		0,                   // Pyramid level (for mip-mapping) - 0 is the top level
		GL_RGB,              // Internal colour format to convert to
		image.cols,          // Image width  i.e. 640 for Kinect in standard mode
		image.rows,          // Image height i.e. 480 for Kinect in standard mode
		0,                   // Border width in pixels (can either be 1 or 0)
		GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
		GL_UNSIGNED_BYTE,    // Image data type
		image.ptr());        // The actual image data itself
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

	// Drawing the background image
	float rectangleSize = 1;

	// Update the texture from the camera
	tigl::shader->enableTexture(true);

	glBindTexture(GL_TEXTURE_2D, cameraTexture);
	glm::mat4 mat(1.0f);

	tigl::begin(GL_QUADS);

	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(-rectangleSize, -rectangleSize, 0), glm::vec2(0, 1), glm::vec4(1, 0, 1, 1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(rectangleSize, -rectangleSize, 0), glm::vec2(1, 1), glm::vec4(1, 0, 1, 1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(rectangleSize, rectangleSize, 0), glm::vec2(1, 0), glm::vec4(1, 0, 1, 1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(-rectangleSize, rectangleSize, 0), glm::vec2(0, 0), glm::vec4(1, 0, 1, 1)));

	tigl::end();
}


