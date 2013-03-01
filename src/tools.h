#pragma once
#include "DataType.h"
#include "SchemaDataType.h"
#include "SchemaType.h"
#include "ArrayType.h"

using std::string;

INT32 ToInt32(BYTE* pBuffer);

#if _MSC_VER >= 1400
#define GET_DATA(x) ((*(x)).data());
#else
#define GET_DATA(x) &((*(x)).front());
#endif

const string& EnumToString(EArrayType eValue);
const string& EnumToString(ESchemaDataType eValue);
const string& EnumToString(EDataType eValue);
const string& EnumToString(ESchemaType eValue);
