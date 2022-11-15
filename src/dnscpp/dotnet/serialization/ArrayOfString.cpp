#include "ArrayOfString.h"

#include "ArrayItem.h"
#include "Primitive.h"
#include "Field.h"

#include "core/DataIsland.h"

#include "core/BinaryStream.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/DotNetField.h"
#include "dotnet/DotNetPrimitiveTypeField.h"
#include "dotnet/FindDotNetClass.h"
#include "dotnet/UserClassField.h"
#include "types/SchemaType.h"

#include <algorithm>
#include <string>


static bool FindSizeFieldValue(const std::shared_ptr<CDotNetField>& value)
{
    return value->GetName() == "_size";
}

static bool FindItemsFieldValue(const std::shared_ptr<CDotNetField>& value)
{
    return value->GetName() == "_items";
}


static uint64_t GetSize(auto& arg)
{
    using ValueT = std::decay_t<decltype(arg)>;

    if constexpr (std::is_same_v<ValueT, bool>)
    {
        return uint64_t{0};
    }
    if constexpr (std::is_integral_v<ValueT>)
    {
        return static_cast<uint64_t>(arg);
    }
    else
    {
        return uint64_t{0};
    }
}


std::shared_ptr<CDotNetClass> ReadArrayOfString(CBinaryStream& stream)
{
    auto pClass = std::make_shared<CDotNetClass>();
    pClass->SetSchemaType(eSchemaType_ArrayOfString);
    GDataIsland<CDotNetClass>().Data.push_back(pClass);
    pClass->SetID(stream.ReadUInt32());
	uint32_t count = stream.ReadUInt32();

    pClass->FieldValues().reserve(count);

	for(uint32_t i = 0; i < count; i++)
	{
	    auto eSchemaType = static_cast<ESchemaType>(stream.ReadByte());
	    if (eSchemaType == eSchemaType_ArrayFiller8b)
	    {
            i += stream.ReadByte();
	    }
	    else
	    {
            auto pField = ReadFieldType(stream, eSchemaType);
            pClass->FieldValues().push_back(pField);
	    }

	}

    return pClass;
}
