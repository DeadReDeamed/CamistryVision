#ifndef ATOM_COMP_H
#define ATOM_COMP_H

#include "DrawComponent.h"
#include <glm/gtc/matrix_transform.hpp>

namespace camvis {
	namespace component
	{
		class AtomComponent : public DrawComponent
		{
		private:

			std::vector<std::tuple<glm::mat4, glm::vec4>> core;

			// Atom structure
			int bolAmount;

		public:

			AtomComponent(int bolAmount) : bolAmount(bolAmount) {};

			void update(float deltaTime);
			void draw();

		private:
			void drawCube(glm::mat4 model, glm::vec4 color);
			void generateCore(int size, glm::mat4 model);
			glm::vec4 selectCoreColor();

		};
	}
}

#endif
