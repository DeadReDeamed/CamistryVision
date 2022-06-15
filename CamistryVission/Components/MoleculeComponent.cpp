#include "MoleculeComponent.h"

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

		MoleculeComponent::MoleculeComponent(std::map<int, int> _atomMap, std::vector<data::Atom>& _atoms) : atomMap(_atomMap), existingAtomsMolecule(_atoms), DrawComponent(data::BallModelSingleton::getInstance().ballModel) {
			std::vector<data::Atom> atomArray;
			//Create molecule model.
			for (auto& pair : atomMap) {
				
				data::Atom atom = existingAtomsMolecule[pair.first];
				atomArray.push_back(atom);
				for (int i = 0; i < pair.second; i++) {
					AtomComponent* atomComponent = new AtomComponent(atom.neutrons + atom.atomNumber, &atom);

					cores.push_back(atomComponent->core);
					delete atomComponent;
				}
			}
		
			MoleculeGrid grid = createMoleculeGrid(4, cores);
			atoms = atomArray;
			drawList = createDrawList(grid);
		}

		void MoleculeComponent::update(float deltaTime)
		{
			throw "Function not implemented!";
		}

		void MoleculeComponent::draw()
		{
			throw "Function not implemented!";
		}
	}
}