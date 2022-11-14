#pragma once

#include "types/DataType.h"


class CDotNetField;
class CBinaryStream;

std::shared_ptr<CDotNetField> ReadPrimative(CBinaryStream& stream, EDataType eDataType);