#ifndef MODEL_H
#define MODEL_H

#include "Group.h"
#include <vector>
#include <glm/fwd.hpp>

namespace camvis
{
	namespace data
	{	
		class Model
		{

		public:
			std::vector<glm::vec3> vertexPositions, normals;
			std::vector<glm::vec2> texCoords;
			std::vector<Group*> groups;

			Model(std::string);
		};
	}

}

#endif
