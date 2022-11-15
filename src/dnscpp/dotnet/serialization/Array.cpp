#include "Array.h"

#include "MemberReference.h"
#include "ArrayItem.h"
#include "Class.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "dotnet/DotNetClass.h"
#include "dotnet/UserClassField.h"
#include "dotnet/MemberReferenceField.h"

#include "types/SchemaType.h"
#include "types/SchemaDataType.h"
#include "types/ArrayType.h"

#include "utils/tools.h"

std::shared_ptr<CDotNetClass> ReadArray(CBinaryStream& stream)
{
	//this function will not really do anything until i can fiture out how to remember this information...
	auto pArray = std::make_shared<CDotNetClass>();
	GDataIsland<CDotNetClass>().Data.push_back(pArray); //could be sorted by id or something
	
	pArray->SetSchemaType(eSchemaType_BinaryArray);
	pArray->SetID(stream.ReadUInt32());

	EArrayType eType = static_cast<EArrayType>(stream.ReadByte());
	pArray->SetArrayType(eType);
	uint32_t nDims = stream.ReadUInt32();
	pArray->ArraySizes().resize(nDims, 0);
	size_t nNumOfFields = 0;
	for(auto& item : pArray->ArraySizes())
	{
		item = stream.ReadInt32();//i think i loop..
		nNumOfFields += item;
	}
	pArray->FieldValues().resize(nNumOfFields, nullptr);
	ESchemaDataType eSchemaDataType = static_cast<ESchemaDataType>(stream.ReadByte());
	CField& element = pArray->Element();

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
		EDataType eType = static_cast<EDataType>(stream.ReadByte());
		element.SetDataType(eType);
	}

    auto peek = stream.Peek(50);
    for(auto& p : peek)
    {
        fmt::print(stderr, "{:X} ", p);
    }

	
	bool bContinue = nNumOfFields > 0;
	uint32_t nCurrentDim = 0;
	int nIndex = 0;
	size_t nField = 0;
	auto ppFields = pArray->FieldValues().data();
	while(bContinue)
	{
		ESchemaType eSchemaType = static_cast<ESchemaType>(stream.ReadByte());
		switch(eSchemaType) {
		case eSchemaType_MemberReference:
			ppFields[nField] = ReadMemberReference(stream);
			ppFields[nField]->SetParent(pArray);
			ppFields[nField]->SetSchemaType(eSchemaType_MemberReference);
			nField++;
			nIndex++;
			break;
		case eSchemaType_ObjectNull:
			nIndex++;
			break;
		case eSchemaType_ArrayFiller8b:
			{
				int nFiller = stream.ReadByte();
				nIndex += nFiller;
				nField += nFiller;
			}
			break;
		case eSchemaType_ArrayFiller32b:
			{
				uint32_t nFiller = stream.ReadUInt32();
				nIndex += nFiller;
				nField += nFiller;
			}
			break;
			case eSchemaType_Class:
            {
				auto p = std::make_shared<CUserClassField>();
                auto pNewClass = ReadClass(stream, false);
                p->SetClassObject(pNewClass);
                p->SetSchemaType(pNewClass->GetSchemaType());
                p->SetParent(pArray);
                ppFields[nField] = p;
                nField++;
                nIndex++;
            }
			break;
			case eSchemaType_SystemClass:
            {
				auto p = std::make_shared<CUserClassField>();
                auto pNewClass = ReadClass(stream, true);
                p->SetClassObject(pNewClass);
                p->SetSchemaType(pNewClass->GetSchemaType());
                p->SetParent(pArray);
                ppFields[nField] = p;
                nField++;
                nIndex++;
            }
			break;
			case eSchemaType_ArrayItem:
			{
				auto p = std::make_shared<CUserClassField>();
                auto pNewClass = ReadArrayItem(stream);
                p->SetClassObject(pNewClass);
                p->SetSchemaType(pNewClass->GetSchemaType());
                p->SetParent(pArray);
                ppFields[nField] = p;
                nField++;
                nIndex++;
			}
			break;
		case eSchemaDataType_Array:
			{
				auto p = std::make_shared<CUserClassField>();
				auto pArray = ReadArray(stream);
				p->SetClassObject(pArray);
				p->SetSchemaType(pArray->GetSchemaType());
				ppFields[nField] = p;
				ppFields[nField]->SetParent(pArray);
				nField++;
				nIndex++;
			}
			break;
		default:
			fmt::print(stderr, "ReadArray, Unhandled schema type {} ({})\n", EnumToString(eSchemaType), (int)eSchemaType);
			    exit(0);

			break;
		};
		if (eType == eArrayType_SingleDimension)
		{
			if (nIndex >= pArray->ArraySizes()[0]) 
			{
				bContinue = false;
			}
		} 
		else if (eType == eArrayType_Jagged || eType == eArrayType_MultiDimension)
		{
			//this still needs to be tested...
			if (nIndex > pArray->ArraySizes()[nCurrentDim])
			{
				nCurrentDim++;
				nIndex = 0;
			}
			if (nCurrentDim > nDims) {
				bContinue = false;
			}
		}
	}
	return pArray;
}
