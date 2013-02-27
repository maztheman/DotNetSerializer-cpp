#include "StdAfx.h"
#include "DotNetField.h"
#include "DotNetClass.h"

CDotNetField::CDotNetField(void)
{
}

CDotNetField::~CDotNetField(void)
{
}

void CDotNetField::SetParent(CDotNetClass* pParent)
{
	m_pParentClass = pParent;
}

CDotNetClass* CDotNetField::GetParent()
{
	return m_pParentClass;
}

size_t CDotNetFieldPtrVector::GetIndexByID(INT32 id) const
{
	CDotNetField* const* pData = &front();
	for(size_t i = 0; i < size(); i++) {
		if (pData[i]->Type() != eDataType_Invalid) continue;
		CUserClassField* pValue = dynamic_cast<CUserClassField*>(pData[i]);
		if (pValue && pValue->Value()->ID() == id) {
			return i;
		}
	}

	return -1;
}