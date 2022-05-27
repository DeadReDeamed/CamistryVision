#include <iostream>

#include "JSONParser.h"
#include "../json.hpp"
#include "../Data/Matter/Atom.h"
#include "../Data/Matter/Molecule.h"

using namespace nlohmann;

namespace camvis
{
	template<typename T>
	T JsonParser::deserializeObject(const json& jsonString)
	{

		throw "not implemted";
	}

	template<typename T>
	std::vector<T> JsonParser::deserializeList(const json& jsonObject)
	{
		std::vector<data::Matter> matter;
		std::vector<data::Atom> tempAtomList;

		for (auto& el : jsonObject["atoms"].items()) {
			json j = el.value();
			
			data::Atom a = data::Atom(j["name"], j["symbol"], j["description"], j["atomnumber"], j["neutrons"], j["mass"], j["elektrons"]);

			tempAtomList.push_back(a);
			matter.push_back(a);
		}

		for (auto& el : jsonObject["substances"].items()) {
			json substance = el.value();

			std::vector<data::Atom> atomsInSubstance;

			for (auto i : substance["atoms"].items()) {
				for (data::Atom atom : tempAtomList) {
					if (atom.atomNumber == i.value()) {
						atomsInSubstance.push_back(atom);
					}
				}
			}

			data::Molecule m = data::Molecule(substance["name"], substance["symbol"], substance["description"], atomsInSubstance);

			matter.push_back(m);
		}

		return matter;
	}

	template<typename T>
	std::string JsonParser::serializeObject(const T& object)
	{
		throw "Not implemented!";
	}

	template data::Matter JsonParser::deserializeObject<data::Matter>(const json& object);
	template std::vector<data::Matter> JsonParser::deserializeList<data::Matter>(const json& object);
	template std::string JsonParser::serializeObject<data::Matter>(const data::Matter& object);
}
