#pragma once

#include "DotNetField.h"

class CMemberReferenceField : public CDotNetField
{
	PROPERTY(ReferenceID, uint32_t, m_uiReferenceID)
public:
	CMemberReferenceField()
		: m_uiReferenceID(-1)
	{
	}
	CMemberReferenceField(uint32_t id)
		: m_uiReferenceID(id)
	{
	}

private:
	EDataType Type() const { return eDataType_UInt32; }
	std::string do_ToString() const { 
		std::stringstream ss;
		ss << m_uiReferenceID;
		std::string sVal = ss.str();
		return std::move(sVal);
	}
};

using CMemberReferenceFieldPtrVector = std::vector<std::shared_ptr<CMemberReferenceField>>;
