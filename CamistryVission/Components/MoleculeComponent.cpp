#include "MoleculeComponent.h"

#include "AtomComponent.h"
#include "../GameObject.h"

namespace camvis {
	namespace component
	{
		
		MoleculeComponent::MoleculeComponent(std::map<int, int> _atomMap, const std::vector<data::Atom>& atoms) : atomMap(_atomMap), DrawComponent(new camvis::data::Model("Resources\\models\\ball.obj")) {
			
			MoleculeGrid d = MoleculeGrid(6);
			d[glm::vec3(-3, -2, -3)];

			//Create molecule model.
			for (auto& pair : atomMap) {
				data::Atom atom = atoms[0];

				for (data::Atom a : atoms) {
					if (pair.first == a.atomNumber) {
						atom = a;
						break;
					}
				}

				for (int i = 0; i < pair.second; i++) {
					AtomComponent* atomComponent = new AtomComponent(atom.neutrons + atom.atomNumber);
					printf("hello");

					cores.push_back(atomComponent->core);
					delete atomComponent;
				}
			}
			printf("test");
			d.add(std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>(glm::vec3(-3, -2, -3), cores[0]));
		}

		void MoleculeComponent::update(float deltaTime)
		{
			//throw "Function not implemented!";
		}

		void MoleculeComponent::draw()
		{
			// first we draw the C then the O then the H

			// save the positions of the molecules in an 3d array that will be used to draw
			for (auto& core : cores) {
				gameObject->translate(glm::vec3(4, 0, 0));
				for (auto& coreAtom : core)
				{
					
					gameObject->translate(coreAtom.first);
					DrawComponent::draw(coreAtom.second);
					gameObject->translate(-coreAtom.first);
				}
			}
			int i = -4 * cores.size();
			gameObject->translate(glm::vec3(i, 0, 0));
		}
	}
}