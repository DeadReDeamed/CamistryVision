#ifndef FACE_H
#define FACE_H

#include <list>
#include "Vertex.h"

namespace camvis
{
	namespace data
	{
		class Face
		{
		public:
			std::list<Vertex> vertices;

		public:
			Face(std::list<Vertex> vertices)
				: vertices(vertices) {}
		};
	}
}

#endif
