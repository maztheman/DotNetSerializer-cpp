#pragma once

//Primitive types??
enum EDataType
{
	eDataType_Invalid		= -1,
	eDataType_Boolean		= 0x01,
	eDataType_Byte			= 0x02,
	eDataType_Char			= 0x03,
	eDataType_Decimal		= 0x05,
	eDataType_Double		= 0x06,
	eDataType_Int16			= 0x07,
	eDataType_Int32			= 0x08,
	eDataType_Int64			= 0x09,
	eDataType_SByte			= 0x0A,
	eDataType_Single		= 0x0B,
	eDataType_DateTime		= 0x0D,
	eDataType_UInt16		= 0x0E,
	eDataType_UInt32		= 0x0F,
	eDataType_UInt64		= 0x10,
	eDataType_String		= 0x12,
	eDataType_Null			= 0xFF
};

using CDataTypeVector = std::vector<EDataType>;
