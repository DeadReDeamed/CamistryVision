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
#include "Data/Model/CameraTexture.h"


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

data::CameraTexture camTex;

std::vector<GameObject*> gameObjects;

std::vector<data::Atom> atoms; //temporary varialbe for testing

int main()
{

	std::cout << "start camistry vision" << std::endl;

	a = Aruco::ArucoHandler();
	a.start();

	if (!glfwInit())
		throw "Could not initialize glwf";

	//480 height
	//640 width
	window = glfwCreateWindow(640, 480, "CamistryVision", NULL, NULL);

	

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
	ImGui::Begin("Stats", &showStatsWindow);
	ImGui::Text("Frame time: %.2f", deltaTime);
	ImGui::Text("FPS: %.2f", 1.0f / deltaTime);
	ImGui::End();
}

int rot = 0;

void drawBackground() {

	glDisable(GL_DEPTH_TEST);

	cv::Mat image = a.getLastImage();
	
	camTex.UpdateTexture(image.data, image.rows, image.cols, image.channels() == 3 ? GL_RGB : GL_RGBA);
	camTex.bind();

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);
	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::mat4(1.0f));
	tigl::shader->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)));
	tigl::shader->enableTexture(true);

	tigl::begin(GL_QUADS);
	tigl::addVertex(tigl::Vertex::PT(glm::vec3( 0.640, -0.480, -1.2),  glm::vec2(1, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-0.640, -0.480, -1.2),  glm::vec2(0, 1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(-0.640,  0.480, -1.2),  glm::vec2(0, 0)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3( 0.640,  0.480, -1.2),  glm::vec2(1, 0)));
	tigl::end();

	tigl::shader->enableTexture(false);

}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawBackground();

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

		gameobject->transform = modelMatrix;
		gameobject->scale(glm::vec3(0.05f, 0.05f, 0.05f));

		gameobject->draw();
	}
}
