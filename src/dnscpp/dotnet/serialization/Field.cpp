#include "Field.h"

#include "ArrayItem.h"
#include "Class.h"
#include "MemberReference.h"

#include "core/BinaryStream.h"

#include "types/SchemaType.h"

#include "dotnet/DotNetField.h"
#include "dotnet/UserClassField.h"
#include "dotnet/DotNetPrimitiveTypeField.h"
#include "dotnet/MemberReferenceField.h"

std::shared_ptr<CDotNetField> ReadField(CBinaryStream& stream)
{
	ESchemaType eFieldType = static_cast<ESchemaType>(stream.ReadByte());
	return ReadFieldType(stream, eFieldType);
}

std::shared_ptr<CDotNetField> ReadFieldType(CBinaryStream& stream, ESchemaType eFieldType)
{
    std::shared_ptr<CDotNetField> pField;
    switch(eFieldType) 
	{
	default: 
		throw std::runtime_error("Unhandled ReadField Type"); 
	break;	
	//1,4,5,6,8,9,10,13,14
	case eSchemaType_ArrayItem:
		{
			auto pClass = std::make_shared<CUserClassField>();
			auto pSub = ReadArrayItem(stream);
			pClass->SetClassObject(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_SystemClass:
		{
			auto pClass = std::make_shared<CUserClassField>();
			auto pSub = ReadClass(stream, true);
			pClass->SetClassObject(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_Class:
		//value type only?
		{
			auto pClass = std::make_shared<CUserClassField>();
			auto pSub = ReadClass(stream, false);
			pClass->SetClassObject(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_BinaryObjectString:
        {
            std::string sValue;
            uint32_t objectID = stream.ReadUInt32();
            stream.ReadString(sValue);
            auto pSField = std::make_shared<DotNetPrimitiveTypeField>(eDataType_String, sValue);
            pSField->SetObjectID(objectID);
            pField = pSField;
        }
		break;
	case eSchemaType_BoxedPrimitiveType:
		//type spec, raw value
		break;
	case eSchemaType_MemberReference:
	    pField = ReadMemberReference(stream);
		break;
	case eSchemaType_ObjectNull://nop?
		pField = std::make_shared<DotNetPrimitiveTypeField>(eDataType_Null, nulltype{});
		break;
	case eSchemaType_ArrayFiller8b:
		//ideally these should be handled in the thing calling this function
		fmt::print(stderr, "Ideally eSchemaType_ArrayFiller8b should be handled in the calling function.\n");
		stream.ReadByte();
		break;
	case eSchemaType_ArrayFiller32b:
		fmt::print(stderr, "Ideally eSchemaType_ArrayFiller32b should be handled in the calling function.\n");
		stream.ReadUInt32();
		break;
	};
	pField->SetSchemaType(eFieldType);
	return pField;
}