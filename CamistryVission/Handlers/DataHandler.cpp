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

    void DataHandler::loadData(std::string Datafilename, std::string scenesFilename)
    {
        //loads atom and molecule data
        nlohmann::json jsonObject = FileIO::loadJsonFile(Datafilename);

        atoms = camvis::JsonParser::deserializeAtoms(jsonObject);
        molecules = camvis::JsonParser::deserializeMolecules(jsonObject, atoms);
        
        // Load the scenes
        nlohmann::json scenesJson = FileIO::loadJsonFile(scenesFilename);

        for (auto& sceneJson : scenesJson["Scenes"].items())
        {
            nlohmann::json scene = sceneJson.value();

            // Create the list with atoms
            std::unordered_map<int, data::Atom*> sceneAtoms;
            for (auto& atomData : scene["atoms"].items())
               sceneAtoms.insert(std::make_pair<int, data::Atom*>(atomData.value()["code"], &atoms[atomData.value()["index"]]));

            std::unordered_map<int, data::Molecule*> scenesMolecule;
            for (auto& atomData : scene["molecules"].items())
                scenesMolecule.insert(std::make_pair<int, data::Molecule*>(atomData.value()["code"], &molecules[atomData.value()["index"]]));

            // Push the scene to the scenes
            scenesAtoms.push_back(sceneAtoms);
            scenesMolecules.push_back(scenesMolecule);
        }

    }

} }
