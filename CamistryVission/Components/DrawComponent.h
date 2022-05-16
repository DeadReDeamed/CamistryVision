#ifndef DRAW_COMP_H
#define DRAW_COMP_H

#include "Component.h"

namespace camvis {
	namespace component
	{
		class DrawComponent : public Component
		{
		private:
			//Model* model;
		public:
			void update(float deltaTime);
			virtual void draw();
		};
	}
}

#endif
