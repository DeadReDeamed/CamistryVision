#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <iostream>
#include <iterator>
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Components/Component.h"
#include "Components/DrawComponent.h"

namespace camvis
{
	class GameObject
	{
	public:
		glm::mat4 transform = glm::mat4(1.0f);

	private:
	    std::list<component::DrawComponent*> drawComponents;
		std::list<component::Component*> components;
		std::list<GameObject*> gameObjects;

	public:

		/// <summary>
		/// Adds the given component to the gameobject
		/// </summary>
		/// <param name="component">The component to add to the gameobject</param>
		void addComponent(component::Component* component);

		/// <summary>
		/// Updates the gameobject
		/// </summary>
		/// <param name="deltaTime">delta time since last frame</param>
		void update(float deltaTime);
		
		/// <summary>
		/// Draws the gameobject to the screen
		/// </summary>
		void draw();

		/// <summary>
		/// Removes the given component from the components
		/// </summary>
		/// <param name="component">The component to delete</param>
		void removeComponent(component::Component* component);

		/// <summary>
		/// Gets the component of the giben time
		/// </summary>
		/// <typeparam name="T">The type of component to search for</typeparam>
		/// <returns>The component with the requested type, NULL if not found</returns>
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
	};
}

#endif
