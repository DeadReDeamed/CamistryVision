#include <iostream>
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
#include "CardScanning/MarkerData.h"
#include "Util/JSONParser.h"

using namespace camvis;

GLFWwindow* window;
Aruco::ArucoHandler a;

unsigned int cameraTexture;

std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing

int main()
{

	std::cout << "start camistry vision" << std::endl;

	a = Aruco::ArucoHandler();
	a.start();

	if (!glfwInit())
		throw "Could not initialize glwf";

	window = glfwCreateWindow(800, 800, "CamistryVision", NULL, NULL);

	

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

	
	int atomIndex = 1;

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
}

bool showStatsWindow = true;
void update()
{
	double timeNow = glfwGetTime();
	float deltaTime = timeNow - lastUpdateTime;
	lastUpdateTime = timeNow;


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
		cv::Mat rodrigues;
		cv::Rodrigues(detectedMarkers[i].rvec, rodrigues);

		// Building the viewMatrix for this model
		glm::mat4 viewMatrix = {
			{(float)rodrigues.at<double>(0,0), (float)rodrigues.at<double>(0,1), (float)rodrigues.at<double>(0,2),(float)(detectedMarkers[i].tvec[0] * 0.1f)},
			{(float)rodrigues.at<double>(1,0), (float)rodrigues.at<double>(1,1), (float)rodrigues.at<double>(1,2),(float)(detectedMarkers[i].tvec[1] * 0.1f)},
			{(float)rodrigues.at<double>(2,0), (float)rodrigues.at<double>(2,1), (float)rodrigues.at<double>(2,2),(float)(detectedMarkers[i].tvec[2] * 0.1f)},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

		glm::mat4 cvToOpenGL = glm::mat4(0.0f);
		cvToOpenGL[0][0] = 1.0f;
		cvToOpenGL[1][1] = -1.0f;
		cvToOpenGL[2][2] = -1.0f;
		cvToOpenGL[3][3] = 1.0f;

		viewMatrix = cvToOpenGL * viewMatrix;
		viewMatrix = glm::transpose(viewMatrix);

		gameObjects[0]->cameraTransform = viewMatrix;


	}

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

	// Show Frame statistics
	ImGui::Begin("Stats", &showStatsWindow);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();
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
	/*glm::mat4 projection = glm::perspective(glm::radians(53.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setModelMatrix(glm::mat4(1.0f));*/

	tigl::shader->setProjectionMatrix(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 200.0f));
	tigl::shader->setViewMatrix(glm::lookAt(
		glm::vec3(0.0f, 0, 5),
		glm::vec3(0.0f, 0, 0),
		glm::vec3(0.0f, 1, 0)
	));

	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	// Drawing the background image
	float rectangleSize = 1;

	// Update the texture from the camera
	tigl::shader->enableTexture(true);

	glBindTexture(GL_TEXTURE_2D, cameraTexture);

	tigl::begin(GL_QUADS);

	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(-rectangleSize, -rectangleSize, 0), glm::vec2(0, 1), glm::vec4(1,0,1,1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(rectangleSize, -rectangleSize, 0), glm::vec2(1, 1), glm::vec4(1, 0, 1, 1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(rectangleSize, rectangleSize, 0), glm::vec2(1, 0), glm::vec4(1, 0, 1, 1)));
	tigl::addVertex(tigl::Vertex::PTC(glm::vec3(-rectangleSize, rectangleSize, 0), glm::vec2(0, 0), glm::vec4(1, 0, 1, 1)));

	tigl::end();

	for (auto gameobject : gameObjects)
	{
		tigl::shader->setViewMatrix(gameobject->cameraTransform);

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		gameobject->transform = modelMatrix;

		//gameobject->draw();
	}
}
