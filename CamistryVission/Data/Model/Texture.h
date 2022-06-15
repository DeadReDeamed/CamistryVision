#ifndef TEXTURE_H
#define TEXTURE_H

namespace camvis
{
	namespace data
	{
		class Texture
		{
		protected:
			GLuint id;

		public:
			Texture() {};
			Texture(const std::string& fileName);
			void bind();
		};
	}
}

#endif
