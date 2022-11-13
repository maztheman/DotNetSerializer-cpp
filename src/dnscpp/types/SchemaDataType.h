#pragma once

//BinaryTypeEnumA looks like this i think!!
enum ESchemaDataType
{
	eSchemaDataType_Invalid					= -1,
	eSchemaDataType_Primitive				= 0x00,
	eSchemaDataType_String					= 0x01,
	eSchemaDataType_Object					= 0x02,
	eSchemaDataType_SystemObject			= 0x03,
	eSchemaDataType_UserDefinedObject		= 0x04,
	eSchemaDataType_ObjectArray				= 0x05,
	eSchemaDataType_StringArray				= 0x06,
	eSchemaDataType_Array					= 0x07,
};

typedef std::vector<ESchemaDataType> CSchemaDataTypeVector;