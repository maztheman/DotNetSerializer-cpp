#pragma once

enum ESchemaDataType
{
	eSchemaDataType_Invalid = -1,
	eSchemaDataType_Normal = 0,
	eSchemaDataType_String = 1,
	eSchemaDataType_SystemObject = 3,
	eSchemaDataType_UserDefinedObject = 4,
	eSchemaDataType_StringArray  = 6,
	eSchemaDataType_Array  = 7
};

typedef std::vector<ESchemaDataType> CSchemaDataTypeVector;