#include "Assembly.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetAssembly.h"

void ReadAssembly(CBinaryStream& stream)
{
	auto pAssembly = std::make_shared<CDotNetAssembly>();
	pAssembly->SetID(stream.ReadUInt32());
	stream.ReadString(pAssembly->Name());

	GDataIsland<CDotNetAssembly>().Data.push_back(pAssembly);

	fmt::print(stderr, "Assembly({}) {}\n", pAssembly->GetID(),  pAssembly->GetName());
}
