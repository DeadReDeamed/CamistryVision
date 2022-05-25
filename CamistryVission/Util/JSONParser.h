#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>

namespace camvis
{
	class JsonParser
	{
	public:
		JsonParser() {};

		template<typename T>
		T deserializeObject(const std::string& jsonString);

		template<typename T>
		std::vector<T> deserializeList(const std::string& jsonString);

		template<typename T>
		std::string serializeObject(const T& object);
	};
}

#endif
