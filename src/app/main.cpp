// Binary.Serializer.cpp : Defines the entry point for the console application.
//

#include "core/BinaryStream.h"
#include "dotnet/DotNetFileReader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	fs::path exe = argv[0];

	/*if (argc != 2) {
		std::cout << "Usage: " << exe.filename().generic_string() << " <file>" << std::endl;
		return 0;
	}*/

	std::fstream fs("Person.bin", std::ios_base::in | std::ios_base::binary);
	if (!fs.is_open()) {
		std::cout << "File doesnt exist" << std::endl;
		return 0;
	}
	CBinaryStream stream;
	stream.InitializeFromStream(fs);
	fs.close();
	CDotNetFileReader reader(stream);
	std::shared_ptr<CDotNetClass> pObject = reader.Deserialize();
	std::string sLog;
	reader.GetLog(sLog);
	return 0;
}

