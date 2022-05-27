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


	std::vector<data::Atom> JsonParser::deserializeAtoms(const json& jsonObject)
	{
		std::vector<data::Atom> matter;

		for (auto& el : jsonObject["atoms"].items()) {
			json j = el.value();
			
			data::Atom a = data::Atom(j["name"], j["symbol"], j["description"], j["atomnumber"], j["neutrons"], j["mass"], j["elektrons"]);

			matter.push_back(a);
		}

		return matter;
	}

	std::vector<data::Molecule> JsonParser::deserializeMolecules(const json& jsonObject, std::vector<data::Atom> atoms)
	{
		std::vector<data::Molecule> matter;

		for (auto& el : jsonObject["substances"].items()) {
			json substance = el.value();

			std::vector<data::Atom> atomsInSubstance;

			for (auto i : substance["atoms"].items()) {
				for (data::Atom atom : atoms) {
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
	template std::string JsonParser::serializeObject<data::Matter>(const data::Matter& object);
}
