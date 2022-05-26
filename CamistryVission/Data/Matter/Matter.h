#ifndef MATTER_H
#define MATTER_H

#include <string>

#include "../../json.hpp"

namespace camvis
{
	namespace data
	{
		class Matter
		{
		private:
			std::string name;
			std::string symbol;
			std::string description;

		public:
			inline Matter(std::string name, std::string symbol, std::string description)
				: name(name), symbol(symbol), description(description) {};

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(Matter, name, symbol, description);
		};
	}
}

#endif
