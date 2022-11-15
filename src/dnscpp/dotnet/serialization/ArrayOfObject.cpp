#include "ArrayOfObject.h"

#include "Primitive.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetClass.h"

#include "types/DataType.h"
#include "types/SchemaType.h"

void ReadArrayOfObject(CBinaryStream& stream)
{
    auto pClass = std::make_shared<CDotNetClass>();
    pClass->SetSchemaType(eSchemaType_ArrayOfObject);
    GDataIsland<CDotNetClass>().Data.push_back(pClass);
    pClass->SetID(stream.ReadUInt32());

    uint32_t count = stream.ReadUInt32();

    pClass->FieldValues().reserve(count);

    for(uint32_t i = 0; i < count; i++)
    {
        auto schemaType = static_cast<ESchemaType>(stream.ReadByte());

        if (schemaType == eSchemaType_ArrayFiller8b)
        {
            i += stream.ReadByte();
        }
        else
        {
            auto eDataType = static_cast<EDataType>(stream.ReadByte());
            auto pField = ReadPrimative(stream, eDataType);
            pField->SetSchemaType(schemaType);
            pClass->FieldValues().push_back(pField);
        }
    }
}
