#ifndef COMPONENT_H
#define COMPONENT_H

#include "..//GameObject.h"

namespace camvis
{
	class Component
	{
	protected:
		GameObject* gameObject;
		
	public:
		virtual void update(float deltaTime) = 0;
		inline void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
	};
}

#endif
