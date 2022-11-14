// Binary.Serializer.cpp : Defines the entry point for the console application.
//

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetFileReader.h"
#include "dotnet/DotNetAssembly.h"
#include "dotnet/DotNetField.h"
#include "dotnet/UserClassField.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;


void JsonPrint(auto&& arg)
{
	using ValueT = std::decay_t<decltype(arg)>;
	if constexpr (std::is_same_v<ValueT, CInt32Vector>)
	{
		fmt::print(stderr, "[");
		for(int n = 0; auto& v : arg)
		{
			if (n > 0)
			{
				fmt::print(stderr, ",");
			}
			fmt::print(stderr, "{}", v);
			n++;
		}
		fmt::print(stderr, "]");
	}
	else if constexpr (std::is_same_v<ValueT, CStringVector>)
	{
		fmt::print(stderr, "[");
		for(int n = 0; auto& v : arg)
		{
			if (n > 0)
			{
				fmt::print(stderr, ",");
			}
			fmt::print(stderr, "\"{}\"", v);
			n++;
		}
		fmt::print(stderr, "]");
	}
	else if constexpr (std::is_same_v<ValueT, nulltype>)
	{
		fmt::print(stderr, "null");
	}
	else if constexpr (std::is_same_v<ValueT, std::string>)
	{
		fmt::print(stderr, "\"{}\"", arg.c_str());
	}
	else
	{
		fmt::print(stderr, "{}", arg);
	}
}

static void printNTabs(int n)
{
	for(int i = 0;i < n; i++)
	{
		fmt::print(stderr, "\t");
	}
}

static void PrintClassValues(const CDotNetFieldPtrVector& fieldValues, int level)
{
	for(int n = 0; auto& field : fieldValues)
	{
		if (n > 0)
		{
			fmt::print(stderr, ",\n");
		}
		auto ft = field->GetFieldType();
		printNTabs(level);
		fmt::print(stderr, "\"{}\":", field->Name());
		switch(ft)
		{
			using enum DotNetFieldType;
			case Primitive:
			{
				auto value = static_cast<DotNetPrimitiveTypeField*>(field.get());
				std::visit([](auto&& arg) { JsonPrint(arg); }, value->GetValue());
			}
			break;
			case MemberReference:
			//these should have been resolved.
			break;
			case UserClass:
			{
				auto value = static_cast<CUserClassField*>(field.get());
				auto clo = value->GetClassObject();
				std::string startString = "{";
				std::string endString = "}";
				if (clo->GetSchemaType() == eSchemaType_ArrayOfString)
				{
					startString = "[";
					endString = "]";
				}
				fmt::print(stderr, " {}\n", startString);
				PrintClassValues(clo->GetFieldValues(), level + 1);
				fmt::print(stderr, "\n");
				printNTabs(level);
				fmt::print(stderr, "{}\n", endString);
			}
		}
		n++;
	}
}


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
	reader.Deserialize();
	std::string sLog;
	reader.GetLog(sLog);
	fmt::print(stderr, "{}\n", sLog);

	auto& assemblies = GDataIsland<CDotNetAssembly>().Data;

	fmt::print(stderr, "Found {} assemblies\n", assemblies.size());

	for(auto& assembly : assemblies)
	{
		fmt::print(stderr, "Found {} classes in assembly {}\n", assembly->Classes().size(), assembly->Name());
		for(auto& cls : assembly->Classes())
		{
			fmt::print(stderr, "Found class {}\n", cls->Name());
			fmt::print(stderr, "{{\n");
			PrintClassValues(cls->FieldValues(), 1);
			fmt::print(stderr, "\n}}\n");
		}
	}
	

	return 0;
}

