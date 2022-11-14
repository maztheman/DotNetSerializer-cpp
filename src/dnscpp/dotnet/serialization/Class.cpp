#include "Class.h"

#include "Primitive.h"
#include "Field.h"


#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/FindDotNetAssembly.h"
#include "dotnet/DotNetAssembly.h"

#include "utils/tools.h"

std::shared_ptr<CDotNetClass> ReadClass(CBinaryStream& stream, const bool bSystemClass = false)
{
	uint32_t id = stream.ReadUInt32();
	auto pNewClass = std::make_shared<CDotNetClass>();
	GDataIsland<CDotNetClass>().Data.push_back(pNewClass);

	pNewClass->SetID(id);
	stream.ReadString(pNewClass->Name());

	fmt::print(stderr, "[{}][{}]:  {}\n", bSystemClass ? "SC" : "C", id, pNewClass->Name());

	pNewClass->SetFieldCount(stream.ReadInt32());
	int32_t nFieldCount = pNewClass->GetFieldCount();
	if (nFieldCount <= 0) 
	{
		return pNewClass;
	}
	pNewClass->Fields().resize(nFieldCount);
	//Resize All Arrays
	pNewClass->FieldValues().resize(nFieldCount, nullptr);
	pNewClass->ArraySizes().resize(nFieldCount, -1);
	//int32_t* pArraySizes = GET_DATA(&oClass.ArraySizes());

	for(auto& field : pNewClass->Fields())
	{
		stream.ReadString(field.Name());
	}
	for(auto& field : pNewClass->Fields())
	{
		field.SetSchemaDataType(static_cast<ESchemaDataType>(stream.ReadByte()));
	}
	for(auto& field : pNewClass->Fields())
	{
		ESchemaDataType eSchemaDataType = field.GetSchemaDataType();
		if (eSchemaDataType == eSchemaDataType_Primitive)
		{
			field.SetDataType(static_cast<EDataType>(stream.ReadByte()));
		}
		else if (eSchemaDataType == eSchemaDataType_UserDefinedObject)
		{
			stream.ReadString(field.TypeCode());
			field.SetAssemblyID(stream.ReadUInt32());
		}
		else if (eSchemaDataType == eSchemaDataType_SystemObject)
		{
			stream.ReadString(field.TypeCode());//looks like its fully qualified.
		}
		else if (eSchemaDataType == eSchemaDataType_Array)
		{
			field.SetDataType(static_cast<EDataType>(stream.ReadByte()));
		}
		/*else if (eSchemaDataType == eSchemaDataType_StringArray)
		{
			auto peeked = stream.Peek(4);
			fmt::print(stderr, "\t{:X}{:X}{:X}{:X}\n", peeked[0], peeked[1], peeked[2], peeked[3]);
		}*/
		else
		{

		    fmt::print(stderr, "Processing Data Type {} Field: {}\n",
		        EnumToString(eSchemaDataType), field.Name());
		}
	}
	if (!bSystemClass) 
	{
		auto assemblyID = stream.ReadUInt32();
		pNewClass->SetAssemblyID(assemblyID);

		if (auto assembly = FindAssemblyByID(assemblyID); assembly)
		{
			assembly->Classes().push_back(pNewClass);
		}
	}
	for(int32_t n = 0; n < nFieldCount; n++) 
	{
		std::shared_ptr<CDotNetField> pField;
		CField& field = pNewClass->Fields()[n];
		if (field.GetSchemaDataType() == eSchemaDataType_Primitive) {
			pField = ReadPrimative(stream, field.GetDataType());
		} else {
			pField = ReadField(stream);
		}
		if (pField == nullptr) 
		{
			throw std::runtime_error("Field type is not supported!");
		}
		pField->SetName(field.GetName());
		pField->SetParent(pNewClass);
		pNewClass->FieldValues()[n] = pField;
	}

	return pNewClass;
}