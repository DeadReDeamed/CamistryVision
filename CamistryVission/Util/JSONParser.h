#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>

namespace camvis
{
	class JsonParser
	{
		template<class T>
		static T deserializeObject(const std::string& jsonString);

		template<class T>
		static std::vector<T> deserializeList(const std::string& jsonString);

		template<class T>
		static std::string serializeObject(const T& object);
	};
}

#endif
