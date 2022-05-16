#include "GameObject.h"

namespace camvis
{
	void GameObject::addComponent(Component& component)
	{
		throw "Function not implemented!";
	}

	void GameObject::update(float deltaTime)
	{
		throw "Function not implemented!";
	}
	
	void GameObject::draw()
	{
		throw "Function not implemented!";
	}

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
