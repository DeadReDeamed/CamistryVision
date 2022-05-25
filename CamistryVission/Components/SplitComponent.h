#ifndef SPLIT_COMP_H
#define SPLIT_COMP_H

#include "Component.h"

namespace camvis {
	namespace component
	{
		class SplitComponent : public Component
		{
		public:
			void update(float deltaTime);
		};
	}
}

#endif
