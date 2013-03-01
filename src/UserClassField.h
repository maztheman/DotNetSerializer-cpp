#ifndef __USERCLASSFIELD_H__
#define __USERCLASSFIELD_H__

#include "DotNetField.h"

using std::string;
class CDotNetClass;

class CUserClassField : public CDotNetField
{
	PROPERTY_PTR(ClassObject, CDotNetClass*, m_pClass)
public:
	CUserClassField()
		: m_pClass(nullptr)
	{
	}

	CUserClassField(CDotNetClass* pClass)
		: m_pClass(pClass)
	{
	}

private:
	EDataType Type() const { return eDataType_Invalid; }
	string do_ToString() const;
};

#endif