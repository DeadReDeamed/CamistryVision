#include "MergeComponent.h"
#include "MoleculeComponent.h"
#include "RotationComponent.h"

#include "../GameObject.h"

namespace camvis {
	namespace component
	{
		void MergeComponent::realiseCombination(std::map<int, int> atomMap) {
			component::MoleculeComponent* moleculeComponent = new component::MoleculeComponent(atomMap, existingAtoms);
			gameObject->addComponent(moleculeComponent);
			gameObject->transform = glm::translate(gameObject->transform, glm::vec3(0, -5, -50));
			component::RotationComponent* rotate = new component::RotationComponent();
			gameObject->addComponent(rotate);
			gameObject->scale(glm::vec3(1, 1, 1));

			gameObject->removeComponent(this);
		}

		void MergeComponent::Combine(std::vector<camvis::data::Atom> atoms, std::vector<camvis::data::Molecule> molecules) {
			std::map<int, int> atomMap;

			for (auto& a : atoms) {
				if (atomMap.count(a.atomNumber)) {
					atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
				}
				else {
					atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
				}
			}

			for (camvis::data::Molecule m : molecules) {
				for (camvis::data::Atom a : m.atoms) {
					if (atomMap.count(a.atomNumber)) {
						atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
					}
					else {
						atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
					}
				}
			}

			realiseCombination(atomMap);
		}

		void MergeComponent::Combine(std::vector<camvis::data::Atom> atoms) {
			std::map<int, int> atomMap;
			for (auto& a : atoms) {
				if (atomMap.count(a.atomNumber)) {
					atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
				}
				else {
					atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
				}
			}

			realiseCombination(atomMap);

		}

		void MergeComponent::Combine(std::vector<camvis::data::Molecule> molecules) {
			std::map<int, int> atomMap;

			for (camvis::data::Molecule m : molecules) {
				for (camvis::data::Atom a : m.atoms) {
					if (atomMap.count(a.atomNumber)) {
						atomMap[a.atomNumber] = atomMap[a.atomNumber] + 1;
					}
					else {
						atomMap.insert(std::pair<int, int>(a.atomNumber, 1));
					}
				}
			}


			realiseCombination(atomMap);
		}

		void MergeComponent::update(float deltaTime)
		{
			//throw "Function not implemented!";
		}
	}
}