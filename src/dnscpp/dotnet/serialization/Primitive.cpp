#include "Primitive.h"

#include "core/BinaryStream.h"

#include "dotnet/DotNetField.h"
#include "dotnet/DotNetPrimitiveTypeField.h"


std::shared_ptr<CDotNetField> ReadPrimative(CBinaryStream& stream, EDataType eDataType)
{
    std::shared_ptr<CDotNetField> pField = nullptr;
	switch(eDataType) 
	{
    case eDataType_Char:
    {
        
        auto pValue = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadChar());
        pField = pValue;
    }
    break;
	case eDataType_String:
		{
			std::string sValue;
			stream.ReadString(sValue);
			auto pValue = std::make_shared<DotNetPrimitiveTypeField>(eDataType, sValue);
			pField = pValue;
		}
	break;
	case eDataType_Int16:
		{
			auto pIntField = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadInt16());
			pField = pIntField;
		}
	break;
	case eDataType_Int32:
		{
			auto pIntField = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadInt32());
			pField = pIntField;
		}
	break;
	case eDataType_Int64:
		{
			auto pIntField = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadInt64());
			pField = pIntField;
		}
	break;
	case eDataType_DateTime:
		{
			auto pDateField = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadDouble());
			pField = pDateField;
		}
	break;
	case eDataType_Double:
		{
			auto pDoubleField = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadDouble());
			pField = pDoubleField;
		}
		break;
	case eDataType_Boolean:
		{
			auto p = std::make_shared<DotNetPrimitiveTypeField>(eDataType, stream.ReadBool());
			pField = p;
		}
		break;
	default:
		break;
	};

	return pField;
}