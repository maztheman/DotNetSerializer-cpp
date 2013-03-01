#include "stdafx.h"
#include "tools.h"

using std::map;
using std::make_pair;

#define ADD_TO_MAP(map, type, val, str) map.insert(make_pair(type, str));

INT32 ToInt32(BYTE* pBuffer)
{
	return ((INT32)pBuffer[0] | ((INT32)pBuffer[1] << 8) | ((INT32)pBuffer[2] << 16) | ((INT32)pBuffer[3] << 24));
}

template<class ENUM_TYPE>
inline const string& FindAndSendBack(map<ENUM_TYPE, string>& aEnumMap, const char* pszEnumName, ENUM_TYPE eValue) {
	auto it = aEnumMap.find(eValue);
	if (it == aEnumMap.end()) {
		std::stringstream ss;
		ss << "Unknown " << pszEnumName << " (" << (int)eValue << ")";
		string sVal = ss.str();
		auto retval = aEnumMap.insert(make_pair(eValue, sVal));
		return retval.first->second;
	}
	return it->second;
}

const string& EnumToString(EArrayType eValue)
{
	static map<EArrayType, string> aEnumMap;
	if (aEnumMap.empty()) {
		ADD_TO_MAP(aEnumMap, eArrayType_Invalid				,   -1, "Invalid")
		ADD_TO_MAP(aEnumMap, eArrayType_SingleDimension		, 0x00, "Single Dimension")
		ADD_TO_MAP(aEnumMap, eArrayType_Jagged				, 0x01, "Jagged")
		ADD_TO_MAP(aEnumMap, eArrayType_MultiDimension		, 0x02, "Multi Dimension")
	}
	return FindAndSendBack(aEnumMap, "Array Type", eValue); 
}

const string& EnumToString(ESchemaDataType eValue)
{
	static map<ESchemaDataType, string> aEnumMap;
	if (aEnumMap.empty()) {
		ADD_TO_MAP(aEnumMap, eSchemaDataType_Invalid				,   -1, "Invalid")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_Primitive				, 0x00, "Primitive")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_String					, 0x01, "String")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_Object					, 0x02, "Object")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_SystemObject			, 0x03, "System Object")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_UserDefinedObject		, 0x04, "User Defined Object")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_ObjectArray			, 0x05, "Object Array")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_StringArray			, 0x06, "String Array")
		ADD_TO_MAP(aEnumMap, eSchemaDataType_Array					, 0x07, "Array")
	}
	return FindAndSendBack(aEnumMap, "Schema Data Type", eValue); 
}

const string& EnumToString(EDataType eValue)
{
	static map<EDataType, string> aEnumMap;
	if (aEnumMap.empty()) {
		ADD_TO_MAP(aEnumMap, eDataType_Invalid		,   -1, "Invalid")
		ADD_TO_MAP(aEnumMap, eDataType_Boolean		, 0x01, "Boolean")
		ADD_TO_MAP(aEnumMap, eDataType_Byte			, 0x02, "Byte")
		ADD_TO_MAP(aEnumMap, eDataType_Char			, 0x03, "Char")
		ADD_TO_MAP(aEnumMap, eDataType_Decimal		, 0x05, "Decimal")
		ADD_TO_MAP(aEnumMap, eDataType_Double		, 0x06, "Double")
		ADD_TO_MAP(aEnumMap, eDataType_Int16		, 0x07, "Int16")
		ADD_TO_MAP(aEnumMap, eDataType_Int32		, 0x08, "Int32")
		ADD_TO_MAP(aEnumMap, eDataType_Int64		, 0x09, "Int64")
		ADD_TO_MAP(aEnumMap, eDataType_SByte		, 0x0A, "Signed Byte")
		ADD_TO_MAP(aEnumMap, eDataType_Single		, 0x0B, "Single (float)")
		ADD_TO_MAP(aEnumMap, eDataType_DateTime		, 0x0D, "DateTime")
		ADD_TO_MAP(aEnumMap, eDataType_UInt16		, 0x0E, "Unsigned Int16")
		ADD_TO_MAP(aEnumMap, eDataType_UInt32		, 0x0F, "Unsigned Int32")
		ADD_TO_MAP(aEnumMap, eDataType_UInt64		, 0x10, "Unsigned Int64")
		ADD_TO_MAP(aEnumMap, eDataType_String		, 0x12, "String")
	}
	return FindAndSendBack(aEnumMap, "Data Type", eValue); 
}

const string& EnumToString(ESchemaType eValue)
{
	static map<ESchemaType, string> aEnumMap;
	if (aEnumMap.empty()) {
		ADD_TO_MAP(aEnumMap, eSchemaType_Invalid					,   -1, "Invalid")
		ADD_TO_MAP(aEnumMap, eSchemaType_SerializedStreamHeader		, 0x00, "Serialized Stream Header")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayItem					, 0x01, "Array Item")
		ADD_TO_MAP(aEnumMap, eSchemaType_SystemClass				, 0x04, "System Class")
		ADD_TO_MAP(aEnumMap, eSchemaType_Class						, 0x05, "Class")
		ADD_TO_MAP(aEnumMap, eSchemaType_BinaryObjectString			, 0x06, "Binary Object String")
		ADD_TO_MAP(aEnumMap, eSchemaType_BinaryArray				, 0x07, "Binary Array")
		ADD_TO_MAP(aEnumMap, eSchemaType_BoxedPrimitiveType			, 0x08, "Boxed Primitive Type")
		ADD_TO_MAP(aEnumMap, eSchemaType_MemberReference			, 0x09, "Member Reference")
		ADD_TO_MAP(aEnumMap, eSchemaType_ObjectNull					, 0x0A, "Object NULL")
		ADD_TO_MAP(aEnumMap, eSchemaType_End						, 0x0B, "End")
		ADD_TO_MAP(aEnumMap, eSchemaType_Assembly					, 0x0C, "Assembly")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayFiller8b				, 0x0D, "Array Filler 8bit")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayFiller32b				, 0x0E, "Array Filler 32bit")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayOfPrimitiveType		, 0x0F, "Array of Primitive Type")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayOfObject				, 0x10, "Array of Object")
		ADD_TO_MAP(aEnumMap, eSchemaType_ArrayOfString				, 0x11, "Array of String")
		ADD_TO_MAP(aEnumMap, eSchemaType_BinaryMethodCall			, 0x15, "Binary Method Call")
		ADD_TO_MAP(aEnumMap, eSchemaType_BinaryMethodReturn			, 0x16, "Binary Method Return")
	}
	return FindAndSendBack(aEnumMap, "Schema Type", eValue); 
}