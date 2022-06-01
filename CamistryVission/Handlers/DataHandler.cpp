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
        std::vector<data::Molecule> molecules = camvis::JsonParser::deserializeMolecules(jsonObject, atoms);
    }

} }
