#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <vector>
#include <unordered_map>

#include <string>

#include "../Data/Matter/Matter.h"
#include "../Data/Matter/Molecule.h"
#include "../Data/Matter/Atom.h"

using namespace camvis;

namespace camvis { namespace handlers {

	class DataHandler
	{
	public:
		std::vector<data::Atom> atoms;
		std::vector<data::Molecule> molecules;

		std::vector<std::unordered_map<int, data::Molecule*>> scenesMolecules;
		std::vector<std::unordered_map<int, data::Atom*>> scenesAtoms;
	public:
		static DataHandler* getInstance();

		void loadData(std::string Datafilename, std::string scenesFilename);

	};

} }


#endif // !DATA_HANDLER_H





