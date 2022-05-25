#include "JSONParser.h"

namespace camvis
{
	template<class T>
	T JsonParser::deserializeObject(const std::string& jsonString)
	{
		throw "Not implemented!";
	}

	template<class T>
	std::vector<T> JsonParser::deserializeList(const std::string& jsonString)
	{
		throw "Not implemented!";
	}

	template<class T>
	std::string JsonParser::serializeObject(const T& object)
	{
		throw "Not implemented!";
	}
}
