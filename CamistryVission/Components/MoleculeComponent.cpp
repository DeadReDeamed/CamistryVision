#include "MoleculeComponent.h"

#include "AtomComponent.h"
#include "../GameObject.h"
#include "../lib/tigl/tigl.h"

namespace camvis {
	namespace component
	{
		MoleculeComponent::MoleculeGrid createMoleculeGrid(int size, std::vector<std::vector<std::pair<glm::vec3, glm::vec4>>> cores) {
			if (cores.size() > (size * size * size)) {
				int newSize = (int)cores.size() / 3;
				if (newSize % 2) newSize++;
			}
			MoleculeComponent::MoleculeGrid grid = MoleculeComponent::MoleculeGrid(size);

			for (int i = 0; i < cores.size(); i++) {
				int x = (rand() % size) - size / 2;
				int y = (rand() % size) - size / 2;
				int z = (rand() % size) - size / 2;

				if (grid[glm::vec3(x, y, z)].second.size() <= 0) {
					//we have a open slot
					grid.add(std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>(glm::vec3(x, y, z), cores[i]));
				}
				else {
					//slot already taken try again
					i--;
				}
			}

			return grid;
		}

		std::vector<std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>> createDrawList(MoleculeComponent::MoleculeGrid moleculeGrid) {
			std::vector<std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>> list;
			for (int i = 0; i < moleculeGrid.grid.size(); i++) {
				auto& gridItem = moleculeGrid.grid[i];
				if (gridItem.second.size() <= 0) continue;
				list.push_back(std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>(gridItem.first, gridItem.second));
			}
			return list;
		}

		MoleculeComponent::MoleculeComponent(std::map<int, int> _atomMap, const std::vector<data::Atom>& atoms) : atomMap(_atomMap), DrawComponent(new camvis::data::Model("Resources\\models\\ball.obj")) {

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
			MoleculeGrid grid = createMoleculeGrid(4, cores);

			drawList = createDrawList(grid);
		}

		void MoleculeComponent::update(float deltaTime)
		{
			//throw "Function not implemented!";
		}

		void MoleculeComponent::draw()
		{
			// first we draw the C then the O then the H

			// save the positions of the molecules in an 3d array that will be used to draw
			for (auto& corePair : drawList) {
				glm::vec3 pos = corePair.first;
				pos *= 3;
				gameObject->translate(pos);

				for (auto& coreAtom : corePair.second) {
					gameObject->translate(coreAtom.first);
					DrawComponent::draw(coreAtom.second);
					gameObject->translate(-coreAtom.first);
				}

				gameObject->translate(-pos);
			}

			//draw a line between the molecules.
			tigl::begin(GL_LINE_STRIP);
			for (int i = 0; i < drawList.size(); i++) {
				glm::vec3 pos = drawList[i].first;
				pos *= 3;
				tigl::addVertex(tigl::Vertex::PC(pos, glm::vec4(1, 1, 1, 1)));
			}


			tigl::end();
		/*	for (auto& core : cores) {
				gameObject->translate(glm::vec3(4, 0, 0));
				for (auto& coreAtom : core)
				{
					
					gameObject->translate(coreAtom.first);
					DrawComponent::draw(coreAtom.second);
					gameObject->translate(-coreAtom.first);
				}
			}
			int i = -4 * cores.size();
			gameObject->translate(glm::vec3(i, 0, 0));*/
		}
	}
}