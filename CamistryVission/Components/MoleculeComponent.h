#ifndef MOLECULE_COMP_H
#define MOLECULE_COMP_H

#include "DrawComponent.h"

#include <map>

#include "../Data/Matter/Atom.h"

#include <iostream>

namespace camvis {
	namespace component
	{
		class MoleculeComponent : public DrawComponent
		{
		private:
			std::map<int,int> atomMap;
			std::vector<std::vector<std::pair<glm::vec3, glm::vec4>>> cores;
		public:
			MoleculeComponent(std::map<int,int> _atomMap, const std::vector<data::Atom>& atoms);
			void update(float deltaTime);
			void draw();

			class MoleculeGrid {
			private:
				int size;
				std::vector<std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>> grid;


			public:
				MoleculeGrid(int _size) : size(_size) {
					if ((size % 2)) { 
						std::cerr << "Error| MoleculeComponent.h: Grid size must be an even number" << std::endl;
						throw "";
					}
					grid = std::vector<std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>>>
						(size * size * size);
				};

				inline std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>> operator[](glm::vec3 position) const noexcept
				{
					int x = (int)position[0];
					int y = (int)position[1];
					int z = (int)position[2];
					x = (size / 2) + x;
					y = (size / 2) + y;
					z = (size / 2) + z;

					int16_t index = ((x * size + y) * size) + z;
					auto& gridPair = grid[index];
					return gridPair;
				}

				inline void add(std::pair<glm::vec3, std::vector<std::pair<glm::vec3, glm::vec4>>> gridPair) {
					int x = (int)gridPair.first[0];
					int y = (int)gridPair.first[1];
					int z = (int)gridPair.first[2];
					x = (size / 2) + x;
					y = (size / 2) + y;
					z = (size / 2) + z;

					int16_t index = ((x * size + y) * size) + z;
					grid.insert(grid.begin() + index, gridPair);
				}

			};
		};
	}
}

#endif
