#include "SceneHandler.h"

#include "DataHandler.h"
#include "../Components/AtomComponent.h"
#include "../Components/MoleculeComponent.h"
#include "../Components/ElectronComponent.h"
#include "../Components/RotationComponent.h"
#include <vector>

using namespace camvis;

namespace camvis { 
	namespace handlers {
		
		void SceneHandler::update(float deltaTime)
		{
			for (auto gameObject : activeScene->gameObjects)
			{
				gameObject->update(deltaTime);
			}
		}

		void SceneHandler::draw()
		{
			for (auto& gameobject : activeScene->gameObjects)
			{
				tigl::shader->setViewMatrix(gameobject->cameraTransform);

				glm::mat4 modelMatrix = glm::mat4(1.0f);

				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));

				gameobject->transform = modelMatrix;
				gameobject->scale(glm::vec3(0.05f, 0.05f, 0.05f));

				gameobject->draw();
			}
		}

		void SceneHandler::changeScene(int index)
		{
			parseScene(index);
		}

		void SceneHandler::handleArucoUpdate()
		{
			throw "not implemented!";
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

			if (scenesA->size() < index) throw "Index out of bounds exception";

			std::unordered_map<int, data::Atom*> sceneDataA = scenesA->at(index);
			std::unordered_map<int, data::Molecule*> sceneDataM = scenesM->at(index);

			// Going over the items in the scene
			for (std::pair<int, data::Atom*> matterPair : sceneDataA)
			{
				data::Atom* atom = matterPair.second;
				//Initializing game object from atoms
				GameObject* object = new GameObject();
				object->addComponent(new component::AtomComponent(atom->atomNumber));
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



	} 
}
