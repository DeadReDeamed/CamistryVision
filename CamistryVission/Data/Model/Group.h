#ifndef GROUP_H
#define GROEP_H

#include <string>
#include <list>
#include "Face.h"
#include "Texture.h"

namespace camvis
{
	namespace data
	{
		class Group
		{
		public:
			std::string name;
			std::list<Face> faces;
			Texture* texture;

		public:
			Group(std::string name, std::list<Face> faces, Texture* texture)
				: name(name), faces(faces), texture(texture) {}
		};
	}
}

#endif
