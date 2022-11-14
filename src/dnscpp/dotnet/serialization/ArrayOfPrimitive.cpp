#include "ArrayOfPrimitive.h"

#include "Primitive.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "Field.h"

#include "types/SchemaType.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/DotNetField.h"
#include "dotnet/DotNetPrimitiveTypeField.h"
#include "dotnet/FindDotNetClass.h"
#include "dotnet/UserClassField.h"


static bool FindSizeFieldValue(const std::shared_ptr<CDotNetField>& value)
{
    return value->GetName() == "_size";
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

void ReadArrayOfPrimitive(CBinaryStream& stream)
{
    auto peeked = stream.Peek(50);

    auto pClass = std::make_shared<CDotNetClass>();
    pClass->SetSchemaType(eSchemaType_ArrayOfPrimitiveType);
    GDataIsland<CDotNetClass>().Data.push_back(pClass);
    pClass->SetID(stream.ReadUInt32());

    uint32_t count = stream.ReadUInt32();
    auto eDataType = static_cast<EDataType>(stream.ReadByte());

    pClass->FieldValues().resize(count);

    std::span<uint8_t> pp;
    for(uint32_t i = 0; i < count; i++)
    {
        pClass->FieldValues()[i] = ReadPrimative(stream, eDataType);
    }

    return;
}