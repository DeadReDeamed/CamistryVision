#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <iostream>
#include <iterator>
#include "glm/vec3.hpp"
#include "Components/Component.h"
#include "Components/DrawComponent.h"

namespace camvis
{
	class GameObject
	{
	public:
		glm::vec3 position, rotation, scale;

	private:
	    std::list<component::DrawComponent*> drawComponents;
		std::list<component::Component*> components;
		std::list<GameObject*> gameObjects;

	public:
		void addComponent(component::Component* component);
		void update(float deltaTime);
		void draw();

		template<class T>
		T* getComponent()
		{
			for (auto component : components)
			{
				if (T* v = dynamic_cast<T*>(component))
					return v;
			}
			return NULL;
		}

		void removeComponent(component::Component* component);
	};
}

#endif
