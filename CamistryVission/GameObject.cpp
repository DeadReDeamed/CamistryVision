#include "GameObject.h"

namespace camvis
{
	void GameObject::addComponent(component::Component& component)
	{
		throw "Function not implemented!";
	}

	void GameObject::update(float deltaTime)
	{
		if (shouldShow <= 0) return;

		shouldShow -= deltaTime;

		// Update all components
		for (auto* component : components)
			component->update(deltaTime);

		// Updating all the linked gameobjects
		for (auto* gameObject : gameObjects)
			gameObject->update(deltaTime);
		
	}
	
	void GameObject::draw()
	{
		if (shouldShow <= 0) return;

	template<class T>
	T GameObject::getComponent()
	{
		throw "Function not implemented!";
	}
	
	template<class T>
	void GameObject::removeComponent()
	{
		throw "Function not implemented!";
	}
}
