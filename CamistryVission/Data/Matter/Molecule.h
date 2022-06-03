#ifndef MOLECULE_H
#define MOLECULE_H

#include <vector>
#include <tuple>
#include "Matter.h"
#include "Atom.h"

namespace camvis
{
	namespace data
	{
		class Molecule : public Matter
		{
		public:
			std::vector<data::Atom> atoms;
			std::map<int, int> atomMap;
		public:
			inline Molecule(std::string name, std::string symbol, std::string description, std::vector<data::Atom> atoms)
				: Matter(name, symbol, description), atoms(atoms) {}

		};
	}
}

#endif
