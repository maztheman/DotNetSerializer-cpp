#pragma once

#include "DotNetField.h"

using std::string;
class CDotNetClass;

class CUserClassField : public CDotNetField
{
	PROPERTY_PTR(ClassObject, CDotNetClass, m_pClass)
public:
	CUserClassField()
		: m_pClass(nullptr)
	{
	}

	CUserClassField(const std::shared_ptr<CDotNetClass>& pClass)
		: m_pClass(pClass)
	{
	}

private:
	EDataType Type() const { return eDataType_Invalid; }
	string do_ToString() const;
};
