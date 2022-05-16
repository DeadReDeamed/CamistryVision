#ifndef ATOM_COMP_H
#define ATOM_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class AtomComponent : public DrawComponent
		{
			void update(float deltaTime);
			void draw();
		};
	}
}

#endif
