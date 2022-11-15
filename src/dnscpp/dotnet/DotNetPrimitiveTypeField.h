#pragma once

#include "DotNetField.h"

#include <variant>

struct nulltype
{};

using DotNetPrimitiveType = std::variant<bool, nulltype, char, int8_t, int16_t, int32_t, int64_t,
        double, std::string, uint8_t, uint16_t, uint32_t, uint64_t, CStringVector, CInt32Vector>;

class DotNetPrimitiveTypeField : public CDotNetField
{
public:
	DotNetPrimitiveTypeField(EDataType dataType, DotNetPrimitiveType value)
	: m_Type(dataType)
    , m_Value(value)
    , CDotNetField(DotNetFieldType::Primitive)
	{

	}

	~DotNetPrimitiveTypeField()
    {

    }

	EDataType Type() const override
	{
	    return m_Type;
	}

	const DotNetPrimitiveType& GetValue() const
	{
		return m_Value;
	}

private:
	std::string do_ToString() const override;

	EDataType m_Type;
	DotNetPrimitiveType m_Value;
};