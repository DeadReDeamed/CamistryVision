#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

namespace camvis
{
	namespace data
	{
		class Texture
		{
		private:
			GLuint id;

		public:
			Texture(const std::string& fileName);
			void bind();
		};
	}
}

#endif
