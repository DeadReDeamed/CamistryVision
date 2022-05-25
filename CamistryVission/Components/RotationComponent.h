#ifndef ROTATION_COMP_H
#define ROTATION_COMP_H

#include "Component.h"

namespace camvis {
	namespace component
	{
		class RotationComponent : public Component
		{
			void update(float deltaTime);
		};
	}
}

#endif
