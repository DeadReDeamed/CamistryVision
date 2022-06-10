#ifndef MATTER_H
#define MATTER_H

#include <string>

namespace camvis
{
	namespace data
	{
		class Matter
		{
		public:
			std::string name;
			std::string symbol;
			std::string description;

		public:
			inline Matter(std::string name, std::string symbol, std::string description)
				: name(name), symbol(symbol), description(description) {};
		};
	}
}

#endif
