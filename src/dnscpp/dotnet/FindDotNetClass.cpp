#include "FindDotNetClass.h"

#include "core/DataIsland.h"

#include "DotNetClass.h"

#include <algorithm>

std::shared_ptr<CDotNetClass> FindDotNetClassByID(uint32_t id)
{
    auto& di = GDataIsland<CDotNetClass>().Data;

    if (auto it = std::find_if(di.begin(), di.end(), [&id](const auto& arg) -> auto {
        return arg->GetID() == id;
    }); it != di.end())
    {
        return *it;
    }

    return nullptr;
}