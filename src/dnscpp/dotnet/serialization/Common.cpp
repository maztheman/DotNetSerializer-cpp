#include "Common.h"

#include "Primitive.h"
#include "Field.h"

#include "core/BinaryStream.h"

#include "dotnet/DotNetClass.h"

#include "types/SchemaType.h"

void ReadCommon(CBinaryStream& stream, uint32_t& counter, std::shared_ptr<CDotNetClass>& pParent)
{
    auto schemaType = static_cast<ESchemaType>(stream.ReadByte());

    if (schemaType == eSchemaType_ArrayFiller8b)
    {
        counter += stream.ReadByte();
    }
    else if (schemaType == eSchemaType_ArrayFiller32b)
    {
        counter += stream.ReadUInt32();
    }
    else if (schemaType == eSchemaType_ObjectNull)
    {
        //blank intentionally
        counter++;
    }
    else if (schemaType == eSchemaType_BoxedPrimitiveType)
    {
        auto eDataType = static_cast<EDataType>(stream.ReadByte());
        auto pField = ReadPrimative(stream, eDataType);
        pField->SetSchemaType(schemaType);
        pField->SetParent(pParent);
        pParent->FieldValues().push_back(pField);
        counter++;
    }
    else
    {
        auto pField = ReadFieldType(stream, schemaType);
        pField->SetSchemaType(schemaType);
        pField->SetParent(pParent);
        pParent->FieldValues().push_back(pField);
        counter++;
    }
}