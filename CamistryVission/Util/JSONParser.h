#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>

#include "../json.hpp"

#include "../Data/Matter/Matter.h"

namespace camvis
{
	class JsonParser
	{
	public:
		JsonParser() {};

		template<typename T>
		static T deserializeObject(const nlohmann::json& json);

		template<typename T>
		static std::vector <T> deserializeList(const nlohmann::json& json);

		template<typename T>
		static std::string serializeObject(const T& object);
	};
}

#endif
