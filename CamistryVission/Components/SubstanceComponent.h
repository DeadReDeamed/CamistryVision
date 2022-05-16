#ifndef SUBSTANCE_COMP_H
#define SUBSTANCE_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class SubstanceComponent : public DrawComponent
		{
			void update(float deltaTime);
			void draw();
		};
	}
}

#endif
