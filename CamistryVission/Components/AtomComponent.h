#ifndef ATOM_COMP_H
#define ATOM_COMP_H

#include "DrawComponent.h"

namespace camvis {
	namespace component
	{
		class AtomComponent : public DrawComponent
		{
		private:
			// Atom structure
			std::vector<std::pair<glm::vec3, glm::vec4>> core;

			// Number of parts in the atom
			int bolAmount;

		public:

			/// <summary>
			/// Constructs the atom component with the given amound of size
			/// </summary>
			/// <param name="bolAmount">The number of parts in the core</param>
			AtomComponent(int bolAmount) : DrawComponent(new camvis::data::Model("Resources\\models\\ball.obj")), bolAmount(bolAmount) {};

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
			void generateCore(int size, glm::mat4 model);
			glm::vec4 selectCoreColor();

		};
	}
}

#endif
