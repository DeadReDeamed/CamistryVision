#include "RotationComponent.h"

#include "../GameObject.h"
namespace camvis {
	namespace component
	{
		void RotationComponent::update(float deltaTime)
		{
			gameObject->rotate(glm::vec3(1, 1, 1), false);
			//throw "Function not implemented!";
		}
	}
}