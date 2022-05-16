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
		class Molecule : Matter
		{
		public:
			std::vector<std::tuple<Atom, int>> atoms;

		public:
			inline Molecule(std::string name, std::string symbol, std::string description, std::vector<std::tuple<Atom, int>> atoms)
				: Matter(name, symbol, description), atoms(atoms) {}
		};
	}
}

#endif
