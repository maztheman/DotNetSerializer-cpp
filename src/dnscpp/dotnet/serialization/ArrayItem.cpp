#include "ArrayItem.h"

#include "Primitive.h"
#include "Field.h"

#include "core/DataIsland.h"

#include "core/BinaryStream.h"
#include "dotnet/DotNetClass.h"
#include "dotnet/FindDotNetClass.h"


std::shared_ptr<CDotNetClass> ReadArrayItem(CBinaryStream& stream)
{
    auto pClass = std::make_shared<CDotNetClass>();
	GDataIsland<CDotNetClass>().Data.push_back(pClass);
	
	pClass->SetID(stream.ReadUInt32());
	uint32_t schemaObjectID = stream.ReadUInt32();

    fmt::print(stderr, "find class id {}\n", schemaObjectID);

	if (auto pRefClass = FindDotNetClassByID(schemaObjectID);
	    pRefClass != nullptr)
	{
        fmt::print(stderr, "found class\n");

    	int32_t nFieldCount = pRefClass->GetFieldCount();
	    pClass->SetSchemaReferenceClass(pRefClass);
	    auto fields = pRefClass->Fields().data();
	    pClass->FieldValues().resize(nFieldCount, nullptr);
	    auto ppFieldValues = pClass->FieldValues().data();
	    for(int32_t i = 0; i < nFieldCount; i++) 
	    {
			auto& field = fields[i];
			std::shared_ptr<CDotNetField> pField = nullptr;
			if (field.GetSchemaDataType() == eSchemaDataType_Primitive) 
			{
				pField = ReadPrimative(stream, field.GetDataType());
			}
			else
			{
				pField = ReadField(stream);
			}
			if (pField == nullptr)
			{
				throw std::runtime_error("Field type is not supported!");
			}
		    pField->SetName(field.GetName());
		    pField->SetParent(pClass);
	    	ppFieldValues[i] = pField;
    	}
	}
	return pClass;
}