#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>

namespace camvis
{
	class FileIO
	{
		static std::string loadFile(const std::string& filePath);

		static void saveFile(const std::string& filePath, const std::string& data);

		static unsigned char* loadTexture(const std::string& filePath);
	};
}

#endif
