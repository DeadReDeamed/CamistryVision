#ifndef MERGE_COMP_H
#define MERGE_COMP_H

#include "Component.h"

namespace camvis {
	namespace component
	{
		class MergeComponent : public Component
		{
		public:
			void update(float deltaTime);
		};
	}
}

#endif
