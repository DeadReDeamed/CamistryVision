#include "GameObject.h"

using namespace camvis;

namespace camvis
{
	void GameObject::addComponent(component::Component* component)
	{
		component->setGameObject(this);

		components.push_back(component);

		// Checking if component is a drawComponent
		if (component::DrawComponent* drawComponent = dynamic_cast<component::DrawComponent*>(component))
			drawComponents.push_back(drawComponent);
	}

	void GameObject::update(float deltaTime)
	{
		// Update all components
		for (auto* component : components)
			component->update(deltaTime);

		// Updating all the linked gameobjects
		for (auto* gameObject : gameObjects)
			gameObject->update(deltaTime);
		
	}
	
	void GameObject::draw()
	{
		// Drawing all drawcomponents
		for (auto* drawComponent : drawComponents)
			drawComponent->draw();

		// Drawing all the nested gameobjects
		for (auto* gameObject : gameObjects)
			gameObject->draw();
	}
	
	void GameObject::removeComponent(component::Component* component)
	{
		components.remove(component);

		// Remove component if it is a drawcomponent
		if (component::DrawComponent* v = dynamic_cast<component::DrawComponent*>(component))
			drawComponents.remove(v);
	}

}
