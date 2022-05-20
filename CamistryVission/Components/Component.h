#ifndef COMPONENT_H
#define COMPONENT_H

namespace camvis 
{
	class GameObject;

	namespace component
	{
		class Component
		{
		protected:
			camvis::GameObject* gameObject;

		public:
			virtual void update(float deltaTime) = 0;
			inline void setGameObject(camvis::GameObject* gameObject) { this->gameObject = gameObject; }
		};
	}
}

#endif
