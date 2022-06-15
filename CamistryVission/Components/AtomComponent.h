#ifndef ATOM_COMP_H
#define ATOM_COMP_H

#include "DrawComponent.h"
#include "../Data/Matter/Atom.h"
#include "../Data/Model/BallModelSingleton.h"

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

			// Atom structure
			std::vector<std::pair<glm::vec3, glm::vec4>> core;
			/// <summary>
			/// Constructs the atom component with the given amound of size
			/// </summary>
			/// <param name="bolAmount">The number of parts in the core</param>
			AtomComponent(int bolAmount, data::Atom* _atomData = nullptr) : DrawComponent(data::BallModelSingleton::getInstance().ballModel), bolAmount(bolAmount), atomData(_atomData) { generateCore(bolAmount, glm::mat4(1.0f)); };

			/// <summary>
			/// Updates the component
			/// </summary>
			/// <param name="deltaTime">delta time since last frame</param>
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
