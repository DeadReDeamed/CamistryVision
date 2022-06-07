#include "CameraTexture.h"

namespace camvis
{
	namespace data
	{
		CameraTexture::CameraTexture()
		{
			glGenTextures(1, &id);
		}

		void CameraTexture::UpdateTexture(unsigned char* imgData, int height, int width, GLint channels)
		{
			glBindTexture(GL_TEXTURE_2D, id);

			glTexImage2D(GL_TEXTURE_2D,
				0, //level
				GL_RGBA, //internal format
				width, //width
				height, //height
				0, //border
				channels, //data format
				GL_UNSIGNED_BYTE, //data type
				imgData);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}
