#include "SceneHandler.h"

#include "DataHandler.h"
#include "../Components/AtomComponent.h"
#include "../Components/MoleculeComponent.h"
#include "../Components/ElectronComponent.h"
#include "../Components/RotationComponent.h"
#include "../Components/MergeComponent.h"
#include <vector>
#include <random>

namespace camvis { 
	namespace handlers {
		

		bool showCardsDebug = true;

		SceneHandler::SceneHandler(Aruco::ArucoHandler* cardHandler) : cardHandler(cardHandler), activeScene(nullptr)
		{
			//emptyGameObject = new GameObject();
		}

		void SceneHandler::update(float deltaTime)
		{
			// Update the gameobjects
			for (auto gameObject : activeScene->gameObjects)
			{
				gameObject->update(deltaTime);
			}

			// Update Aruco data
			updateAruco();
			checkCollision();
		}

		void SceneHandler::draw()
		{
			// Draw all gameobjects
			for (auto& gameobject : activeScene->gameObjects)
			{
				// Setting the "camera" perspective to the gameobject
				tigl::shader->setViewMatrix(gameobject->cameraTransform);

				glm::mat4 modelMatrix = glm::mat4(1.0f);

				// TODO fix
				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));

				gameobject->transform = modelMatrix;
				gameobject->scale(glm::vec3(0.05f, 0.05f, 0.05f));

				gameobject->draw();
			}
		}

		void SceneHandler::changeScene(int index)
		{
			parseScene(index);
			//activeScene->gameObjects.push_back(emptyGameObject);
		}

		SceneHandler::~SceneHandler()
		{
			//activeScene->gameObjects.remove(emptyGameObject);
			delete emptyGameObject;
		}

		void SceneHandler::updateAruco()
		{
			// Disable all should show of gameobject
			for (auto& gameobject : activeScene->gameObjects)
				gameobject->shouldShow = false;

			// Get the detected markers from Aruco
			std::vector<Aruco::MarkerData> detectedMarkers = cardHandler->getMarkers();

			for (int i = 0; i < detectedMarkers.size(); i++)
			{
				//Calculate rodrigues transform 
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

				// Update the cv matrix to a glm matrix 
				glm::mat4 glmMatrix = {
					{(float)viewMatrix.at<float>(0,0), (float)viewMatrix.at<float>(0,1), (float)viewMatrix.at<float>(0,2), (float)viewMatrix.at<float>(0,3)},
					{(float)viewMatrix.at<float>(1,0), (float)viewMatrix.at<float>(1,1), (float)viewMatrix.at<float>(1,2), (float)viewMatrix.at<float>(1,3)},
					{(float)viewMatrix.at<float>(2,0), (float)viewMatrix.at<float>(2,1), (float)viewMatrix.at<float>(2,2), (float)viewMatrix.at<float>(2,3)},
					{(float)viewMatrix.at<float>(3,0), (float)viewMatrix.at<float>(3,1), (float)viewMatrix.at<float>(3,2), (float)viewMatrix.at<float>(3,3)},
				};

				// Updating the camera
				auto gameObjectIt = activeScene->linkedGameObjects.find(detectedMarkers[i].id);

				if (gameObjectIt == activeScene->linkedGameObjects.end())
				{
					handleEmptyCard(detectedMarkers[i]);
					continue;
				}

				// Updating the position of the model
				gameObjectIt->second->cameraTransform = glmMatrix;

				// Enable showing the gameobject
				gameObjectIt->second->shouldShow = true;

			}

#ifdef DEBUG_ENABLED
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
#endif
		}

		void SceneHandler::checkCollision() {
			std::vector<camvis::GameObject*> gameObjects = activeScene->gameObjects;

			for (int i = 0; i < gameObjects.size() - 1; i++) {
				GameObject* object = gameObjects[i];
				if (!object->shouldShow) continue;
				for (int j = i + 1; j < gameObjects.size(); j++) {
					GameObject* object2 = gameObjects[j];
					if (!object2->shouldShow) continue;
					float length = glm::length(object->cameraTransform[3] - object2->cameraTransform[3]);
				
					if (length < 0.3f && length != 0) {
						bool leftOrRight = rand() % 2;
						GameObject* objects[2] = { object, object2 };

						GameObject* mergeTo = objects[leftOrRight];
						// if leftOrRight is 1 then result is 0. If it is 0 then it would be -1 which would be converted back to 1.
						GameObject* other = objects[-(leftOrRight - 1)];
						std::vector<data::Atom> atomsToMerge;
						
						//Check if the object has atleast an atom or a molecole in it's components. Otherwise it is an empty card.
						bool twoHaveElements = true;
						for (int k = 0; k < 2; k++) {
							component::AtomComponent* a = objects[k]->getComponent<component::AtomComponent>();
							component::MoleculeComponent* m = objects[k]->getComponent<component::MoleculeComponent>();
							twoHaveElements = twoHaveElements && (a || m);
						}
						if (!twoHaveElements) break;

						// Copy the data of the cards into a vector of data::Atom. This will be used to create a new molecule.
						for (int k = 0; k < 2; k++) {
							GameObject* object = objects[k];
							component::AtomComponent* atom = object->getComponent<component::AtomComponent>();
							if (atom) {
								if (object->shouldShow) {
									if (atom->atomData) {
										atomsToMerge.push_back(*atom->atomData);
									}
									// Remove the atom and electrons from the card after copying the data.
									object->removeComponent(atom);
									object->removeComponent(object->getComponent<component::ElectronComponent>());
								}
								else {
									break;
								}
							}
							else {
								component::MoleculeComponent* molecule = object->getComponent<component::MoleculeComponent>();

								if (object->shouldShow) {
									if (molecule) {
										atomsToMerge.insert(atomsToMerge.end(), molecule->atoms.begin(), molecule->atoms.end());
										// Remove the molecule from the card after copying the data.
										object->removeComponent(molecule);
									}
									else {
										// is a blank card and we only check collision between two cards that have an atom or a molecule on them.
										break;
									}
								}
								else {
									break;
								}
							}
						}
						if (atomsToMerge.size() <= 1) continue;
						//atoms here must be the atom database
						component::MergeComponent* mergeComponent = new component::MergeComponent(mergeTo, existingAtoms);
						mergeComponent->Combine(atomsToMerge);
					
					}
				}
			}
		}

		void SceneHandler::parseScene(int index)
		{
			if (activeScene == nullptr) activeScene = new data::Scene();
			

			// Clearing the current Scene
			activeScene->gameObjects.clear();
			activeScene->linkedGameObjects.clear();

			// Loading the scene from data handler
			std::vector<std::unordered_map<int, data::Atom*>>* scenesA = &(DataHandler::getInstance()->scenesAtoms);
			std::vector<std::unordered_map<int, data::Molecule*>>* scenesM = &(DataHandler::getInstance()->scenesMolecules);

			if (scenesA->size() < index) return;

			std::unordered_map<int, data::Atom*> sceneDataA = scenesA->at(index);
			std::unordered_map<int, data::Molecule*> sceneDataM = scenesM->at(index);

			// Going over the items in the scene
			for (std::pair<int, data::Atom*> matterPair : sceneDataA)
			{
				data::Atom* atom = matterPair.second;
				//Initializing game object from atoms
				GameObject* object = new GameObject();
				object->addComponent(new component::AtomComponent(atom->atomNumber + atom->neutrons, atom));
				std::vector<component::Shell*> shells;
				for (size_t i = 0; i < atom->electrons.size(); i++)
					{
						component::Shell* shell = new component::Shell();
						shell->amount = atom->electrons[i];
						shell->distance = 10 + (2 * i);
						shell->speed = glm::vec3(30.0f + (i * 3), 30.0f + (i * 3), 30.0f + (i * 3));
						shells.push_back(shell);
					}
				object->addComponent(new component::ElectronComponent(shells));
				activeScene->gameObjects.push_back(object);
				activeScene->linkedGameObjects.insert({ matterPair.first, object });
			}

			for (std::pair<int, data::Molecule*> matterPair : sceneDataM) {
				data::Molecule* molecule = matterPair.second;
				GameObject* object = new GameObject();
				component::MoleculeComponent* m = new component::MoleculeComponent(molecule->atomMap, molecule->atoms);
				component::RotationComponent* rotate = new component::RotationComponent();
				object->addComponent(rotate);
				object->addComponent(m);
				activeScene->gameObjects.push_back(object);
				activeScene->linkedGameObjects.insert({ matterPair.first, object });
			}
			
		}

		/// <summary>
		/// When a card is detected with a code that does not exits the following needs to happen:
		///  - create a empty gameobject and set it's should show to true
		/// a Maximum of one empty card will be detected in frame at a time
		/// </summary>
		/// <param name="detectedMarker">The empty marker</param>
		void SceneHandler::handleEmptyCard(Aruco::MarkerData detectedMarker)
		{
			//emptyGameObject->shouldShow = true;
		}



	} 
}
