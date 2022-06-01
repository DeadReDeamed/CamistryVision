#include "SceneHandler.h"

#include "DataHandler.h"

using namespace camvis;

namespace camvis { 
	namespace handlers {
		
		void SceneHandler::update(float deltaTime)
		{
			throw "not implemented!";
		}

		void SceneHandler::draw()
		{
			throw "not implemented!";
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
			// Clearing the current Scene
			activeScene->gameObjects.clear();
			activeScene->linkedGameObjects.clear();

			// Loading the scene from data handler
			std::vector<std::unordered_map<int, data::Matter*>>* scenes = &(DataHandler::getInstance()->scenes);

			if (scenes->size() >= index) throw "Index out of bounds exception";

			std::unordered_map<int, data::Matter*> sceneData = scenes->at(index);

			// Going over the items in the scene
			for (std::pair<int, data::Matter*> matterPair : sceneData)
			{
				data::Matter* test = matterPair.second;
				if ((data::Atom*) test)
				{

				}
			}

			/*
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
			*/
		}



	} 
}
