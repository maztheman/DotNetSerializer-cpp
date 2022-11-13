#pragma once

#include "types/DataType.h"
#include "types/SchemaDataType.h"
#include "types/SchemaType.h"
#include "types/ArrayType.h"

int32_t ToInt32(uint8_t* pBuffer);

const std::string& EnumToString(EArrayType eValue);
const std::string& EnumToString(ESchemaDataType eValue);
const std::string& EnumToString(EDataType eValue);
const std::string& EnumToString(ESchemaType eValue);
