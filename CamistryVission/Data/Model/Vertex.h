#ifndef VERTEX_H
#define VERTEX_H

namespace camvis
{
	namespace data
	{
		class Vertex
		{
		public:
			int positionIndex, normalIndex, texCoordIndex;

		public:
			Vertex(int positionIndex, int normalIndex, int texCoordIndex)
				: positionIndex(positionIndex), normalIndex(normalIndex), texCoordIndex(texCoordIndex) {}
		};
	}
}

#endif
