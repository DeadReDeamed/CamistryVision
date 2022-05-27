#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <string>
#include "../Data/Model/Texture.h"

namespace camvis {
	namespace handlers {

		class ModelHandler
		{
		public:
			static camvis::data::Texture* loadMaterialFile(const std::string& filename, const std::string& directory);
		};

	}
}

#endif // !MODEL_HANDLER_H
