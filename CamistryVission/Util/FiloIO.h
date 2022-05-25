#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <fstream>
#include "../json.hpp"

namespace camvis
{
	class FileIO
	{
	public:
		static bool saveFile(const std::string& filePath, const std::string& data);

		static nlohmann::json loadJsonFile(const std::string& filePath);

		static unsigned char* loadTexture(const std::string& filePath);

	private:
		/*
		* Checks if the failbit or badbit arror flag is set.
		* returns false if no error flags are set.
		*/
		static bool streamErrorFlagsAreSet(const std::ios_base::iostate& state);
	};
}

#endif
