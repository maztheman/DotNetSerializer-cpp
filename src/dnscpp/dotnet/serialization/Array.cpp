#include "Array.h"

#include "Common.h"
#include "Primitive.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/DotNetField.h"
#include "dotnet/UserClassField.h"

#include "types/SchemaType.h"
#include "types/SchemaDataType.h"
#include "types/ArrayType.h"

#include "utils/tools.h"

void ReadArray(CBinaryStream& stream)
{
	auto pArray = std::make_shared<CDotNetClass>();
	GDataIsland<CDotNetClass>().Data.push_back(pArray);
	
	pArray->SetSchemaType(eSchemaType_BinaryArray);
	pArray->SetID(stream.ReadUInt32());

	const auto eType = static_cast<EArrayType>(stream.ReadByte());
	auto nDims = stream.ReadUInt32();

	pArray->SetArrayType(eType);
	pArray->ArraySizes().resize(nDims, 0);
	size_t nNumOfFields = 1;

	for(auto& item : pArray->ArraySizes())
	{
		item = stream.ReadInt32();
		nNumOfFields *= item;
	}
	
	//special case for some reason we have a 0 dimension (empty) array.
	if (nDims == 0)
	{
		nNumOfFields = 0;
	}

	pArray->FieldValues().reserve(nNumOfFields);

	const auto eSchemaDataType = static_cast<ESchemaDataType>(stream.ReadByte());
	auto& element = pArray->Element();

	element.SetSchemaDataType(eSchemaDataType);
	if (eSchemaDataType == eSchemaDataType_UserDefinedObject)
	{
		stream.ReadString(element.TypeCode());
		element.SetAssemblyID(stream.ReadUInt32());
	}
	else if (eSchemaDataType == eSchemaDataType_SystemObject)
	{
		stream.ReadString(element.TypeCode());
	}
	else if (eSchemaDataType == eSchemaDataType_Primitive)
	{
		auto eDataType = static_cast<EDataType>(stream.ReadByte());
		element.SetDataType(eDataType);
	}
	
	for(uint32_t nIndex = 0; nIndex < nNumOfFields; )
	{
		if (eSchemaDataType == eSchemaDataType_Primitive)
		{
			auto pField = ReadPrimative(stream, element.GetDataType());
			pField->SetParent(pArray);
        	pArray->FieldValues().push_back(pField);			
			nIndex++;
		}
		else
		{
			ReadCommon(stream, nIndex, pArray);
		}

	}
}

//not sure how the below logic is supposed to work multi-Dimension its laid out the in binary [y, x] -> y = 0, x = 0 to n, y = 1, x = 0 to n, etc...
/*if (eType == eArrayType_SingleDimension)
{
	if (nIndex >= static_cast<uint32_t>(pArray->ArraySizes()[0])) 
	{
		bContinue = false;
	}
}
else if (eType == eArrayType_Jagged || eType == eArrayType_MultiDimension)
{
	if (nIndex > static_cast<uint32_t>(pArray->ArraySizes()[nCurrentDim]))
	{
		nCurrentDim++;
		nIndex = 0;
	}
	if (nCurrentDim > nDims) 
	{
		bContinue = false;
	}
}*/