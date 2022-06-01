#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <fstream>
#include "../json.hpp"

namespace camvis
{
	class FileIO
	{
	public:
		/// <summary>
		/// Saves a std::string of data to a specified file.
		/// If the file does not exist it will create a new file.
		/// If the file does exist it will delete the old content en write
		/// the new data into it.
		/// </summary>
		/// <param name="filePath">The path to the file, containing filename and extension.</param>
		/// <param name="data">The data that will be put into the file.</param>
		/// <returns>True if the write was succesfull, false if the write failed or is compromised.</returns>
		static bool saveFile(const std::string& filePath, const std::string& data);

		/// <summary>
		/// Loads the given file and generates a nlohmann::json object.
		/// </summary>
		/// <param name="filePath">The path to the file, containing filename and extension.</param>
		/// <returns>The nlohmann::json object containing the information from the file.</returns>
		static nlohmann::json loadJsonFile(const std::string& filePath);

		static unsigned char* loadTexture(const std::string& filePath);

	private:
		/// <summary>
		/// Checks if the failbit or badbit error flag is set.
		/// </summary>
		/// <param name="state">The state of the fstream that is being used.</param>
		/// <returns>True if one or more error flags are set.</returns>
		static bool streamErrorFlagsAreSet(const std::ios_base::iostate& state);
	};
}

#endif
