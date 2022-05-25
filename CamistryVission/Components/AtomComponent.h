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
			// Atom structure
			std::vector<std::tuple<glm::mat4, glm::vec4>> core;

			// Number of parts in the atom
			int bolAmount;

		public:

			/// <summary>
			/// Constructs the atom component with the given amound of size
			/// </summary>
			/// <param name="bolAmount">The number of parts in the core</param>
			AtomComponent(int bolAmount) : bolAmount(bolAmount) {};

			/// <summary>
			/// Updates the component
			/// </summary>
			/// <param name="deltaTime">delta time since last frame</param>
			void update(float deltaTime);

			/// <summary>
			/// Draws the compenent parts to the screen
			/// </summary>
			void draw();

		private:
			void drawCube(glm::mat4 model, glm::vec4 color);
			void generateCore(int size, glm::mat4 model);
			glm::vec4 selectCoreColor();

			glm::mat4 objectMatrix = glm::mat4(1.0f);

		};
	}
}

#endif
