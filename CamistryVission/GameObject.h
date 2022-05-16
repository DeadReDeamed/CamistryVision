#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include "glm/vec3.hpp"
#include "Components/component.h"

namespace camvis
{
	class GameObject
	{
	private:
		glm::vec3 position, rotation, scale;
		// std::list<DrawComponent*> drawComponents;
		std::list<Component*> components;
		std::list<GameObject*> gameObjects;

	public:
		void addComponent(Component& component);
		void update(float deltaTime);
		void draw();

		template<class T>
		T getComponent();

		template<class T>
		void removeComponent();
	};
}

#endif
