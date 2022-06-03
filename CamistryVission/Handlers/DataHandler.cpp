#include "DataHandler.h"

#include "../Util/FiloIO.h"
#include "../Util/JSONParser.h"
#include "../Data/Matter/Matter.h"
#include "../Data/Matter/Atom.h"

namespace camvis { namespace handlers {

    static DataHandler* instance;

    DataHandler* DataHandler::getInstance()
    {
        if (instance == nullptr) instance = new DataHandler();

        return instance;
    }

    void DataHandler::loadData(std::string filename)
    {
        //loads atom and molecule data
        nlohmann::json jsonObject = FileIO::loadJsonFile(filename);

        atoms = camvis::JsonParser::deserializeAtoms(jsonObject);
        molecules = camvis::JsonParser::deserializeMolecules(jsonObject, atoms);
        
        // TODO LOAD FROM JSON
        std::unordered_map<int, data::Atom*> testSceneA;
        std::unordered_map<int, data::Molecule*> testSceneB;

        testSceneA.insert(std::make_pair<int, data::Atom*>(0, &atoms[1]));
        testSceneB.insert(std::make_pair<int, data::Molecule*>(0, &molecules[1]));
        scenesA.push_back(testSceneA);
        scenesM.push_back(testSceneB);

    }

} }
