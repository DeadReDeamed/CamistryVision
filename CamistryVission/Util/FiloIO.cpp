#include "FiloIO.h"
#include <fstream>
#include <sstream>

namespace camvis
{
    bool FileIO::saveFile(const std::string& filePath, const std::string& data)
    {
        // Open file and delete old contents on new write.
        std::ofstream file(filePath, std::ios::trunc);

        if (!file.is_open())
        {
            return false; // File failed to open.
        }

        std::ostream& result = file.write(data.c_str(), data.size());

        const std::ios_base::iostate& state = result.rdstate();

        file.close();

        if (streamErrorFlagsAreSet(state))
        {
            return false;
        }

        return true;
    }

    nlohmann::json FileIO::loadJsonFile(const std::string& filePath)
    {
        // Open file.
        std::ifstream file(filePath);

        if (!file.is_open())
        {
            return std::string(); // File failed to open.
        }

        nlohmann::json jsonObject;
        file >> jsonObject;

        const std::ios_base::iostate& state = file.rdstate();

        file.close();

        if (streamErrorFlagsAreSet(state))
        {
            return std::string();
        }

        return jsonObject;
    }

	unsigned char* FileIO::loadTexture(const std::string& filePath)
	{
		
		throw "Not implemented!";
	}

	bool FileIO::streamErrorFlagsAreSet(const std::ios_base::iostate& state)
	{
		if ((state & std::ostream::failbit) == 1 || (state & std::ostream::badbit) == 1)
		{
			return true; // One ore more error flags were set.
		}

		return false;
	}
}
