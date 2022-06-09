#ifndef MERGE_COMP_H
#define MERGE_COMP_H

#include "Component.h"

#include "../Data/Matter/Atom.h"
#include "../Data/Matter/Molecule.h"

#include <vector>

namespace camvis {
	namespace component
	{
		class MergeComponent : public Component
		{
		private:
			std::vector<camvis::data::Atom> existingAtoms;

			void realiseCombination(std::map<int, int> atomMap);
		public:
			void Combine(std::vector<camvis::data::Atom> atoms, std::vector<camvis::data::Molecule> molecules);
			void Combine(std::vector<camvis::data::Atom> atoms);
			void Combine(std::vector<camvis::data::Molecule> molecules);

			MergeComponent(std::vector<data::Atom> _existingAtoms) :
				existingAtoms(_existingAtoms) {};

			void update(float deltaTime);
		};
	}
}

#endif
