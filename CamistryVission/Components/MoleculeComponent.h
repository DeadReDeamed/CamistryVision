#ifndef MOLECULE_COMP_H
#define MOLECULE_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class MoleculeComponent : public DrawComponent
		{
			void update(float deltaTime);
			void draw();
		};
	}
}

#endif
