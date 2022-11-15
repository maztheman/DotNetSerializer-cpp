#include "ArrayOfPrimitive.h"

#include "Primitive.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "types/SchemaType.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/DotNetField.h"

void ReadArrayOfPrimitive(CBinaryStream& stream)
{
    auto pClass = std::make_shared<CDotNetClass>();
    GDataIsland<CDotNetClass>().Data.push_back(pClass);

    pClass->SetSchemaType(eSchemaType_ArrayOfPrimitiveType);
    pClass->SetID(stream.ReadUInt32());

    uint32_t count = stream.ReadUInt32();
    auto eDataType = static_cast<EDataType>(stream.ReadByte());

    pClass->FieldValues().reserve(count);

    for(uint32_t i = 0; i < count; i++)
    {
        auto pField = ReadPrimative(stream, eDataType);
        pField->SetParent(pClass);
        pClass->FieldValues().push_back(pField);
    }

    return;
}
