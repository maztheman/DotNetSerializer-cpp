#include "ArrayOfObject.h"

#include "Common.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetClass.h"

void ReadArrayOfObject(CBinaryStream& stream)
{
    auto pClass = std::make_shared<CDotNetClass>();
    pClass->SetSchemaType(eSchemaType_ArrayOfObject);
    GDataIsland<CDotNetClass>().Data.push_back(pClass);
    pClass->SetID(stream.ReadUInt32());

    uint32_t count = stream.ReadUInt32();

    pClass->FieldValues().reserve(count);
    
    for(uint32_t i = 0; i < count;)
    {
        ReadCommon(stream, i, pClass);
    }
}
