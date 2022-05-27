#ifndef DRAW_COMP_H
#define DRAW_COMP_H

#include "Component.h"
#include "../Data/Model/Model.h"

namespace camvis 
{
	namespace component
	{
		class DrawComponent : public Component
		{
		private:
			camvis::data::Model* model;
		public:
			DrawComponent(camvis::data::Model* model) : model(model) {};
			void update(float deltaTime);
			virtual void draw();
		};
	}
}

#endif
