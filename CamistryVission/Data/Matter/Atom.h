#ifndef ATOM_H
#define ATOM_H

#include <vector>
#include <string>
#include "Matter.h"

namespace camvis
{
	namespace data
	{
		class Atom : Matter
		{
		public:
			int atomNumber;
			int neutrons;
			float mass;
			std::vector<int> electrons;

		public:
			inline Atom(std::string name, std::string symbol, std::string description, int atomNumber, int neutrons, float mass, std::vector<int> electrons)
				: Matter(name, symbol, description), atomNumber(atomNumber), neutrons(neutrons), mass(mass), electrons(electrons) {}
		};
	}
}

#endif
