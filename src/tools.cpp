#include "stdafx.h"
#include "tools.h"

using std::map;
using std::make_pair;

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

const string& EnumToString(ESchemaDataType eValue)
{
	static map<ESchemaDataType, string> aEnumMap;
	if (aEnumMap.empty()) {
 		aEnumMap.insert(make_pair(eSchemaDataType_Invalid, "Invalid"));
		aEnumMap.insert(make_pair(eSchemaDataType_Normal, "Normal"));
		aEnumMap.insert(make_pair(eSchemaDataType_String, "String"));
		aEnumMap.insert(make_pair(eSchemaDataType_SystemObject, "System Object"));
		aEnumMap.insert(make_pair(eSchemaDataType_UserDefinedObject, "User Defined Object"));
		aEnumMap.insert(make_pair(eSchemaDataType_StringArray, "String Array"));
		aEnumMap.insert(make_pair(eSchemaDataType_Array, "Array"));
		aEnumMap.insert(make_pair(eSchemaDataType_GenericObject, "???Generic Object???"));
	}
	return FindAndSendBack(aEnumMap, "Schema Data Type", eValue); 
}

const string& EnumToString(EDataType eValue)
{
	static map<EDataType, string> aEnumMap;
	if (aEnumMap.empty()) {
		aEnumMap.insert(make_pair(eDataType_Invalid, "Invalid"));
		aEnumMap.insert(make_pair(eDataType_Double, "Double"));
		aEnumMap.insert(make_pair(eDataType_Int16, "Int16"));
		aEnumMap.insert(make_pair(eDataType_Int32, "Int32"));
		aEnumMap.insert(make_pair(eDataType_Int64, "Int64"));
		aEnumMap.insert(make_pair(eDataType_DateTime, "DateTime"));
		aEnumMap.insert(make_pair(eDataType_String, "String"));
	}
	return FindAndSendBack(aEnumMap, "Data Type", eValue); 
}
const string& EnumToString(EArrayDataType eValue)
{
	static map<EArrayDataType, string> aEnumMap;
	if (aEnumMap.empty()) {
		aEnumMap.insert(make_pair(eArrayDataType_Invalid, "Invalid"));
		aEnumMap.insert(make_pair(eArrayDataType_Double, "Double"));
		aEnumMap.insert(make_pair(eArrayDataType_Int32, "Int32"));
		aEnumMap.insert(make_pair(eArrayDataType_String, "String"));
	}
	return FindAndSendBack(aEnumMap, "Array Data Type", eValue); 
}

const string& EnumToString(ESchemaType eValue)
{
	static map<ESchemaType, string> aEnumMap;
	if (aEnumMap.empty()) {
		aEnumMap.insert(make_pair(eSchemaType_SystemClass, "System Class"));
		aEnumMap.insert(make_pair(eSchemaType_Class, "Class"));
		aEnumMap.insert(make_pair(eSchemaType_Seven, "??Seven??"));
		aEnumMap.insert(make_pair(eSchemaType_End, "End"));
		aEnumMap.insert(make_pair(eSchemaType_Assembly, "Assembly"));
		aEnumMap.insert(make_pair(eSchemaType_FieldRef, "FieldRef"));
	}
	return FindAndSendBack(aEnumMap, "Schema Type", eValue); 
}