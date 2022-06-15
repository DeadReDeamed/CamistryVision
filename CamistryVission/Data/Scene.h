#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <unordered_map>
#include "../GameObject.h"

namespace camvis
{
	namespace data
	{
		class Scene
		{
		public:
			std::vector<GameObject*> gameObjects;
			std::unordered_map<int, GameObject*> linkedGameObjects;

		public:
			inline Scene(std::vector<GameObject*> gameObjects, std::unordered_map<int, GameObject*> linkedGameObjects)
				: gameObjects(gameObjects), linkedGameObjects(linkedGameObjects) {}
			inline Scene() {};
		};
	}
}

#endif
