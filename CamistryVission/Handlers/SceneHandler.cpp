#include "SceneHandler.h"

#include <vector>

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
			throw "not implemented!";
		}

		void SceneHandler::handleArucoUpdate()
		{
			std::vector<Aruco::MarkerData> markers = arucoHandler->getMarkers();
			std::vector<int> tempIDList;

			for (Aruco::MarkerData marker : markers) {
				if (activeScene.linkedGameObjects.find(marker.id) == activeScene.linkedGameObjects.end()) {
					GameObject object = GameObject();

					activeScene.gameObjects.push_back(object);
					activeScene.linkedGameObjects.insert(std::make_pair(marker.id, &object));
				}
			}

			for (Aruco::MarkerData marker : markers) {
				tempIDList.push_back(marker.id);
			}

			std::unordered_map<int, GameObject*> newMap;

			for (std::pair<int, GameObject*> object : activeScene.linkedGameObjects) {
				if (std::count(tempIDList.begin(), tempIDList.end(), object.first)) {
					newMap.insert(std::make_pair(object.first, object.second));
				}
			}

			activeScene.linkedGameObjects = newMap;
			//throw "not implemented!";
		}

		void SceneHandler::parseScene(int index)
		{
			throw "not implemented!";
		}

	} 
}
