#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/vec3.hpp>
#include "Group.h"

namespace camvis
{
	namespace data
	{
		class Model
		{
		public:
			std::vector<glm::vec3> vertexPositions, normals, texCoords;
			std::vector<Group*> groups;

		public:
			Model(std::vector<glm::vec3> vertexPositions, std::vector <glm::vec3> normals, std::vector <glm::vec3> texCoords, std::vector<Group*> groups)
				: vertexPositions(vertexPositions), normals(normals), texCoords(texCoords), groups(groups) {}
		};
	}
}

#endif
