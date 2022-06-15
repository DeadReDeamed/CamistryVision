#ifndef ELECTRON_COMP_H
#define ELECTRON_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class ElectronComponent : public DrawComponent
		{
			void update(float deltaTime);
			void draw();
		};
	}
}

#endif
