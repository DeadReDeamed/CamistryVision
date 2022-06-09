#include "MoleculeComponent.h"

#include "AtomComponent.h"
#include "../GameObject.h"
#include "../lib/tigl/tigl.h"
#include <time.h>

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
				srand(time(NULL));
				int x = (rand() % size) - size / 2;
				srand(time(NULL));
				int y = (rand() % size) - size / 2;
				srand(time(NULL));
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

		MoleculeComponent::MoleculeComponent(std::map<int, int> _atomMap, std::vector<data::Atom>& _atoms) : atomMap(_atomMap), atoms(_atoms), DrawComponent(new camvis::data::Model("Resources\\models\\ball.obj")) {

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
					AtomComponent* atomComponent = new AtomComponent(atom.neutrons + atom.atomNumber, &atom);
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
				pos *= grid_offset;

				gameObject->translate(pos);

				for (auto& coreAtom : corePair.second) {
					gameObject->translate(coreAtom.first);
					DrawComponent::draw(coreAtom.second);
					gameObject->translate(-coreAtom.first);
				}

				gameObject->translate(-pos);
			}

			//draw a line between the molecules.
			if (drawList.size() <= 1) return;

			if (drawList.size() > 2) {
				glm::vec3 firstPos;
				glm::vec3 prevPos = drawList[0].first;
				prevPos *= grid_offset;
				firstPos = prevPos;
				gameObject->translate(firstPos);
				tigl::shader->setModelMatrix(gameObject->transform);
				tigl::begin(GL_LINE_STRIP);
				tigl::addVertex(tigl::Vertex::P(glm::vec3(0, 0, 0)));

				for (int i = 1; i < drawList.size(); i++) {
					gameObject->translate(prevPos);
					
					glm::vec3 nextPos = drawList[i].first;
					nextPos *= grid_offset;
					nextPos -= firstPos;
					tigl::addVertex(tigl::Vertex::P(nextPos));
					gameObject->translate(-prevPos);
					prevPos = nextPos;
				}

				tigl::end();
				gameObject->translate(-firstPos);
			}
		
		}
	}
}