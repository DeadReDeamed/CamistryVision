#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>


#include "../json.hpp"
#include "../Data/Matter/Atom.h"
#include "../Data/Matter/Molecule.h"


namespace camvis
{
	class JsonParser
	{
	public:
		JsonParser() {};

		template<typename T>
		static T deserializeObject(const nlohmann::json& json);

		template<typename T>
		static std::string serializeObject(const T& object);

		static std::vector<data::Atom> deserializeAtoms(const nlohmann::json& jsonObject);
		static std::vector<data::Molecule> deserializeMolecules(const nlohmann::json& jsonObject, std::vector<data::Atom> atoms);
	};
}

#endif
