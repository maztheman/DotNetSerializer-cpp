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
	uint32_t schmeaObjectID = stream.ReadUInt32();

    fmt::print(stderr, "find class id {}\n", schmeaObjectID);

	if (auto pRefClass = FindDotNetClassByID(schmeaObjectID);
	    pRefClass != nullptr)
	{
        auto& fieldValues = pRefClass->FieldValues();

        if (auto it = std::find_if(fieldValues.begin(), fieldValues.end(), FindSizeFieldValue);
            it != fieldValues.end()
        )
        {
            if (auto pSizeField = dynamic_cast<DotNetPrimitiveTypeField*>((*it).get()); pSizeField != nullptr)
            {
                uint64_t sz = std::visit([](auto&& arg) -> uint64_t {
                    return GetSize(arg);
                },  pSizeField->GetValue());

                pClass->FieldValues().resize(sz);

                for(uint64_t i = 0; i < sz; i++)
                {
                    pClass->FieldValues()[i] = ReadField(stream);
                }
            }
            else
            {
                fmt::print(stderr, "_size found, but its not a primitive??\n");
            }
        }
        else
        {
            fmt::print(stderr, "Could not find _size propert which we need to know the size of array\n");
        }
        //we always search for _size

    }

    return pClass;
}