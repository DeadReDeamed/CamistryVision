#include <iostream>
#include <opencv2/aruco.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/tigl/tigl.h"
#include <GLFW/glfw3.h>

//Delete for test
#include "handlers/SceneHandler.h"
#include "handlers/DataHandler.h"

double lastUpdateTime;
void update();
void draw();
void init();
#include "CardScanning/ArucoHandler.h"
#include "CardScanning/MarkerData.h"
#include "Util/JSONParser.h"
#include "debuging/DebugWindow.h"
#include "debuging/imgui/imgui.h"

using namespace camvis;

GLFWwindow* window;
Aruco::ArucoHandler a;
bool showStatsWindow = true;

unsigned int cameraTexture;

std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing

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
	
	handlers::DataHandler::getInstance()->scenesA.push_back({0, 0});
	sceneHandler = new handlers::SceneHandler(&a);
	sceneHandler->changeScene(0);
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

#include "Components/DrawComponent.h"
void init()
{
	lastUpdateTime = glfwGetTime();	

	// Starting the debug gui
#ifdef DEBUG_ENABLED
	debugging::DebugWindow::init(window);
#endif // DEBUG_ENABLED


	// Create first test gameobject
	GameObject* testCore = new GameObject();
	testCore->addComponent(new component::DrawComponent(new camvis::data::Model("Resources\\models\\spider.obj")));
	gameObjects.push_back(testCore);
	testCore->translate(glm::vec3(0, 0, -1));
	testCore->scale(glm::vec3(0.005f, 0.005f, 0.005f));
	/*
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
	component::AtomComponent* comp = testCore->getComponent<component::AtomComponent>();*/
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

	// PLEASE FOR THE LOVE OF GOD, REMOVE
	bool showCardsDebug = true;

	std::vector<Aruco::MarkerData> detectedMarkers = a.getMarkers();

	for (int i = 0; i < detectedMarkers.size(); i++)
	{
		// Calculate rodrigues transform 
		cv::Mat viewMatrix = cv::Mat::zeros(4, 4, 5);
		cv::Mat rodrigues;
		cv::Rodrigues(detectedMarkers[i].rvec, rodrigues);

		// Building the viewMatrix for this model
		/*glm::mat4 viewMatrix = {
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

		//viewMatrix = cvToOpenGL * viewMatrix;
		viewMatrix = glm::matrixCompMult(cvToOpenGL, viewMatrix);
		viewMatrix = glm::transpose(viewMatrix);*/

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


		gameObjects[0]->cameraTransform = glmMatrix;


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
	ImGui::Begin("Stats", &showCardsDebug);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();
}

int rot = 0;

// testcube
std::vector<tigl::Vertex> cube{

	// Front face   
	tigl::Vertex::PC(glm::vec3(0, 0, 0), glm::vec4(1, 0, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0), glm::vec4(1, 0, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0), glm::vec4(1, 0, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0), glm::vec4(1, 0, 0, 1)),

	// Back face
	tigl::Vertex::PC(glm::vec3(0, 0, 0.1), glm::vec4(0, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0.1), glm::vec4(0, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0.1), glm::vec4(0, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0.1), glm::vec4(0, 1, 0, 1)),

	// Right face
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0), glm::vec4(0, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0.1), glm::vec4(0, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0.1), glm::vec4(0, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0), glm::vec4(0, 0, 1, 1)),

	// Left Face
	tigl::Vertex::PC(glm::vec3(0, 0, 0), glm::vec4(0, 1, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0, 0.1), glm::vec4(0, 1, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0.1), glm::vec4(0, 1, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0), glm::vec4(0, 1, 1, 1)),

	// Top Face
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0), glm::vec4(1, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0), glm::vec4(1, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0.1, 0.1), glm::vec4(1, 1, 0, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0.1), glm::vec4(1, 1, 0, 1)),

	// Down Face
	tigl::Vertex::PC(glm::vec3(0, 0.1, 0), glm::vec4(1, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0), glm::vec4(1, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0.1, 0, 0.1), glm::vec4(1, 0, 1, 1)),
	tigl::Vertex::PC(glm::vec3(0, 0, 0.1), glm::vec4(1, 0, 1, 1)),
};

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

	for (auto& gameobject : gameObjects)
	{
		tigl::shader->setViewMatrix(gameobject->cameraTransform);

		glm::mat4 modelMatrix = glm::mat4(1.0f);

		//modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));

		//gameobject->transform = modelMatrix;

		tigl::shader->setModelMatrix(modelMatrix);
		tigl::drawVertices(GL_QUADS ,cube);

		//gameobject->draw();
	}
}