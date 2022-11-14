#pragma once

#include "types/SchemaType.h"

class CBinaryStream;
class CDotNetField;

std::shared_ptr<CDotNetField> ReadField(CBinaryStream& stream);
std::shared_ptr<CDotNetField> ReadFieldType(CBinaryStream& stream, ESchemaType eFieldType);
