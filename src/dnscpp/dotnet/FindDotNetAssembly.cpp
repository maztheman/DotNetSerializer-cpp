#include "FindDotNetAssembly.h"

#include "core/DataIsland.h"

#include "DotNetAssembly.h"


std::shared_ptr<CDotNetAssembly> FindAssemblyByID(int32_t id)
{
	auto& assemblies = GDataIsland<CDotNetAssembly>().Data;

	const auto it = std::find_if(assemblies.begin(), assemblies.end(), [&id](const auto& assembly) {
		return assembly->ID() == id;
	});

	return it != assemblies.end() ? *it : nullptr;
}