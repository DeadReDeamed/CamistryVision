#include "Model.h"
#include "../../Handlers/ModelHandler.h" //ew
#include "../tigl/tigl.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using tigl::Vertex;

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
	namespace data {

		Model::Model(std::string fileName)
		{
			if (fileName._Equal("Resources\\models\\ball.obj")) {
				
			}
			std::cout << "Loading " << fileName << std::endl;
			std::string dirName = fileName;
			if (dirName.rfind("/") != std::string::npos)
				dirName = dirName.substr(0, dirName.rfind("/"));
			if (dirName.rfind("\\") != std::string::npos)
				dirName = dirName.substr(0, dirName.rfind("\\"));
			if (fileName == dirName)
				dirName = "";

			std::ifstream pFile(fileName.c_str());

			if (!pFile.is_open())
			{
				std::cout << "Could not open file " << fileName << std::endl;
				return;
			}

			Group* currentGroup = new Group();
			currentGroup->texture = nullptr;

			while (!pFile.eof())
			{
				std::string line;
				std::getline(pFile, line);
				line = cleanLine(line);
				if (line == "" || line[0] == '#') //skip empty or commented line
					continue;

				std::vector<std::string> params = split(line, " ");
				params[0] = toLower(params[0]);

				if (params[0] == "v")
					vertexPositions.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
				else if (params[0] == "vn")
					normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
				else if (params[0] == "vt")
					texCoords.push_back(glm::vec2((float)atof(params[1].c_str()), 1 - (float)atof(params[2].c_str())));
				else if (params[0] == "f")
				{
					for (size_t ii = 4; ii <= params.size(); ii++)
					{
						Face face;

						for (size_t i = ii - 3; i < ii; i++)	//magische forlus om van quads triangles te maken ;)
						{
							Vertex vertex;
							std::vector<std::string> indices = split(params[i == (ii - 3) ? 1 : i], "/");
							if (indices.size() >= 1)	//er is een positie
								vertex.positionIndex = atoi(indices[0].c_str()) - 1;
							if (indices.size() == 2)		//alleen texture
								vertex.texCoordIndex = atoi(indices[1].c_str()) - 1;
							if (indices.size() == 3)		//v/t/n of v//n
							{
								if (indices[1] != "")
									vertex.texCoordIndex = atoi(indices[1].c_str()) - 1;
								vertex.normalIndex = atoi(indices[2].c_str()) - 1;
							}
							face.vertices.push_back(vertex);
						}
						currentGroup->faces.push_back(face);
					}
				}
				else if (params[0] == "s")
				{//smoothing groups
				}
				else if (params[0] == "mtllib")
				{
					currentGroup->texture = camvis::handlers::ModelHandler::loadMaterialFile(dirName + "/" + params[1], dirName);
				}
			}
			groups.push_back(currentGroup);
		}
	}
}
