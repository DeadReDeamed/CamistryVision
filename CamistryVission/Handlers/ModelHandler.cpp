#include "ModelHandler.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../Data/Model/Texture.h"

/**
		* Replaces a substring in a string
		*/
static std::string replace(std::string str, const std::string& toReplace, const std::string& replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

/**
* Splits a string into substrings, based on a seperator
*/
static std::vector<std::string> split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

/**
* Turns a string to lowercase
*/
static inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

/**
* Cleans up a line for processing
*/
static inline std::string cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}

namespace camvis {
	namespace handlers {

		camvis::data::Texture* ModelHandler::loadMaterialFile(const std::string& fileName, const std::string& directory)
		{
			std::cout << "Loading " << fileName << std::endl;

			camvis::data::Texture* texture = nullptr;

			std::ifstream pFile(fileName.c_str());
			if (!pFile.is_open())
			{
				std::cout << "Could not open file " << fileName << std::endl;
				return nullptr;
			}

			while (!pFile.eof())
			{
				std::string line;
				std::getline(pFile, line);
				line = cleanLine(line);
				if (line == "" || line[0] == '#')
					continue;

				std::vector<std::string> params = split(line, " ");
				params[0] = toLower(params[0]);

				if (params[0] == "newmtl")
				{
				}
				else if (params[0] == "map_kd")
				{
					std::string tex = params[1];
					if (tex.find("/"))
						tex = tex.substr(tex.rfind("/") + 1);
					if (tex.find("\\"))
						tex = tex.substr(tex.rfind("\\") + 1);
					//TODO
					texture = new camvis::data::Texture(directory + "/" + tex);
				}
				else if (params[0] == "kd")
				{//TODO, diffuse color
				}
				else if (params[0] == "ka")
				{//TODO, ambient color
				}
				else if (params[0] == "ks")
				{//TODO, specular color
				}
				else if (
					params[0] == "illum" ||
					params[0] == "map_bump" ||
					params[0] == "map_ke" ||
					params[0] == "map_ka" ||
					params[0] == "map_d" ||
					params[0] == "d" ||
					params[0] == "ke" ||
					params[0] == "ns" ||
					params[0] == "ni" ||
					params[0] == "td" ||
					params[0] == "tf" ||
					params[0] == "tr" ||
					false)
				{
					//these values are usually not used for rendering at this time, so ignore them
				}
				else
					std::cout << "Didn't parse " << params[0] << " in material file" << std::endl;
			}
			return texture;
		}
	}
}