#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include "glm/vec3.hpp"
#include "Components/Component.h"
#include "Components/DrawComponent.h"

namespace camvis
{
	class GameObject
	{
	public:
		glm::mat4 cameraTransform = glm::mat4(1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		float shouldShow = false;
		bool firstPos = true;
		glm::mat4 currentPos;

	private:
	    std::list<component::DrawComponent*> drawComponents;
		std::list<component::Component*> components;
		std::list<GameObject*> gameObjects;

	public:
		void addComponent(component::Component& component);
		void update(float deltaTime);
		void draw();

		template<class T>
		T getComponent();

		template<class T>
		void removeComponent();
	};
}

#endif
