#pragma once

enum EDataType
{
	eDataType_Invalid = -1,
	eDataType_Double = 6,
	eDataType_Int16 = 7,
	eDataType_Int32 = 8,
	eDataType_Int64 = 9,
	eDataType_String = 100 //dunno what value is stored, seems as tho it knows implicitly the type by the schema type
};

enum EArrayDataType
{
	eArrayDataType_Invalid = -1,
	eArrayDataType_Double = 6,
	eArrayDataType_Int32 = 8,
	eArrayDataType_String = 9
};