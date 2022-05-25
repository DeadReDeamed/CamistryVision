#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <string>

namespace camvis {
	namespace handlers {

		class ModelHandler
		{
		public:
			static void loadMaterialFile(const std::string& filename, const std::string& directory);

		};

	}
}

#endif // !MODEL_HANDLER_H
