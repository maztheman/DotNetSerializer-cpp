#include "tools.h"

#define ADD_TO_MAP(map, type, val, str) map.insert(make_pair(type, str));

int32_t ToInt32(uint8_t* pBuffer)
{
	int32_t also = {0};
	memcpy(&also, pBuffer, sizeof(int32_t));
	return ((int32_t)pBuffer[0] | ((int32_t)pBuffer[1] << 8) | ((int32_t)pBuffer[2] << 16) | ((int32_t)pBuffer[3] << 24));
}

template<class ENUM_TYPE>
inline const std::string& FindAndSendBack(std::unordered_map<ENUM_TYPE, std::string>& aEnumMap, const char* pszEnumName, ENUM_TYPE eValue) {
	
	if (auto it = aEnumMap.find(eValue); it == aEnumMap.end()) {
		std::stringstream ss;
		ss << "Unknown " << pszEnumName << " (" << (int)eValue << ")";
		std::string sVal = ss.str();
		auto retval = aEnumMap.insert(std::make_pair(eValue, sVal));
		return retval.first->second;
	}
	else
	{
		return it->second;
	}
}

const std::string& EnumToString(EArrayType eValue)
{
	static std::unordered_map<EArrayType, std::string> aEnumMap = {
		{ eArrayType_Invalid, "Invalid" },
		{ eArrayType_SingleDimension, "Single Dimension" },
		{ eArrayType_Jagged, "Jagged" },
		{ eArrayType_MultiDimension, "Multi Dimension" }
	};
	return FindAndSendBack(aEnumMap, "Array Type", eValue); 
}

const std::string& EnumToString(ESchemaDataType eValue)
{
	static std::unordered_map<ESchemaDataType, std::string> aEnumMap = {
		{ eSchemaDataType_Invalid, "Invalid" },
		{ eSchemaDataType_Primitive, "Primitive"},
		{ eSchemaDataType_String, "String" },
		{ eSchemaDataType_Object, "Object" },
		{ eSchemaDataType_SystemObject, "System Object" },
		{ eSchemaDataType_UserDefinedObject, "User Defined Object" },
		{ eSchemaDataType_ObjectArray, "Object Array" },
		{ eSchemaDataType_StringArray, "String Array" },
		{ eSchemaDataType_Array, "Array" }
	};
	return FindAndSendBack(aEnumMap, "Schema Data Type", eValue); 
}

const std::string& EnumToString(EDataType eValue)
{
	static std::unordered_map<EDataType, std::string> aEnumMap = {
		std::pair<EDataType, std::string>
		{ eDataType_Invalid, "Invalid" },
		{ eDataType_Boolean, "Boolean" },
		{ eDataType_Byte, "Byte" },
		{ eDataType_Char, "Char" },
		{ eDataType_Decimal, "Decimal" },
		{ eDataType_Double, "Double" },
		{ eDataType_Int16, "Int16" },
		{ eDataType_Int32, "Int32" },
		{ eDataType_Int64, "Int64" },
		{ eDataType_SByte, "Signed Byte" },
		{ eDataType_Single, "Single (float)" },
		{ eDataType_DateTime, "Datetime" },
		{ eDataType_UInt16, "Unsigned Int16" },
		{ eDataType_UInt32, "Unsigned Int32" },
		{ eDataType_UInt64, "Unsigned Int64" },
		{ eDataType_String, "String" }
	};
	return FindAndSendBack(aEnumMap, "Data Type", eValue); 
}

const std::string& EnumToString(ESchemaType eValue)
{
	static std::unordered_map<ESchemaType, std::string> aEnumMap = {
		std::pair<ESchemaType, std::string>
		{ eSchemaType_Invalid					, "Invalid" }, 
		{ eSchemaType_SerializedStreamHeader	, "Serialized Stream Header" },
		{ eSchemaType_ArrayItem					, "Array Item" },
		{ eSchemaType_SystemClass				, "System Class" },
		{ eSchemaType_Class						, "Class" },
		{ eSchemaType_BinaryObjectString		, "Binary Object String" },
		{ eSchemaType_BinaryArray				, "Binary Array" },
		{ eSchemaType_BoxedPrimitiveType		, "Boxed Primitive Type" },
		{ eSchemaType_MemberReference			, "Member Reference" },
		{ eSchemaType_ObjectNull				, "Object NULL" },
		{ eSchemaType_End						, "End" },
		{ eSchemaType_Assembly					, "Assembly" },
		{ eSchemaType_ArrayFiller8b				, "Array Filler 8bit" },
		{ eSchemaType_ArrayFiller32b			, "Array Filler 32bit" },
		{ eSchemaType_ArrayOfPrimitiveType		, "Array of Primitive Type" },
		{ eSchemaType_ArrayOfObject				, "Array of Object" },
		{ eSchemaType_ArrayOfString				, "Array of String" },
		{ eSchemaType_BinaryMethodCall			, "Binary Method Call" },
		{ eSchemaType_BinaryMethodReturn		, "Binary Method Return" },
	};
	return FindAndSendBack(aEnumMap, "Schema Type", eValue); 
}