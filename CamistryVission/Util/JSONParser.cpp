#include <iostream>

#include "JSONParser.h"
#include "../json.hpp"

using namespace nlohmann;

namespace camvis
{
	template<typename T>
	T JsonParser::deserializeObject(const std::string& jsonString)
	{
		throw "Not implemented!";
	}

	template<typename T>
	std::vector<T> JsonParser::deserializeList(const std::string& fileName)
	{
		
		std::ifstream reader(fileName);

		json jsonObject;
		reader >> jsonObject;
		reader.close;

		for (auto& el : jsonObject["atoms"].items()) {
			std::cout << el.value()["name"] << std::endl;
		}

		//throw "Not implemented!";
	}

	template<typename T>
	std::string JsonParser::serializeObject(const T& object)
	{
		throw "Not implemented!";
	}
}
