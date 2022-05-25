#ifndef CARD_COMP_H
#define CARD_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class CardComponent : public DrawComponent
		{
			void update(float deltaTime);
			void draw();
		};
	}
}

#endif
