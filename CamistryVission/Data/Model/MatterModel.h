#ifndef MATTER_MODEL_H
#define MATTER_MODEL_H

#include <glm/vec4.hpp>
#include "Model.h"

namespace camvis
{
	namespace data
	{
		class MatterModel : Model
		{
		public:
			glm::vec4 color;

		public:
			MatterModel(glm::vec4 color, std::vector<glm::vec3> vertexPositions, std::vector <glm::vec3> normals, std::vector <glm::vec3> texCoords, std::vector<Group*> groups)
				: Model(vertexPositions, normals, texCoords, groups), color(color) {}
		};
	}
}

#endif
