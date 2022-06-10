#ifndef CAMERATEXTURE_H
#define CAMERATEXTURE_H

#include "Texture.h"

namespace camvis
{
	namespace data
	{
		class CameraTexture : public Texture
		{
		public:
			CameraTexture();
			void UpdateTexture(unsigned char* imgData, int height, int width, GLint channels);
		};
	}
}

#endif
