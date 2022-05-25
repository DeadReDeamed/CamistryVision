#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <vector>
#include <unordered_map>

namespace camvis { namespace handlers {

	class DataHandler
	{
	private:
		static DataHandler instance;

		//std::vector<Matter> matter;
		//std::vector<std::unordered_map<int, Matter>> scenes;

	public:
		static DataHandler getInstance();

		void loadData();

	};

} }


#endif // !DATA_HANDLER_H





