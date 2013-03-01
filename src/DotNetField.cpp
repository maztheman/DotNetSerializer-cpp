#include "StdAfx.h"
#include "DotNetField.h"
#include "DotNetClass.h"
#include "UserClassField.h"
#include "MemberReferenceField.h"

CDotNetField::CDotNetField(void)
	: m_uiObjectID(-1)
	, m_eSchemaType(eSchemaType_Invalid)
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

string CDotNetField::ToString() const
{
	return do_ToString();
}

size_t CDotNetFieldPtrVector::GetIndexByID(UINT32 id) const
{
	CDotNetField* const* pData = &front();
	for(size_t i = 0; i < size(); i++) {
		if (pData[i] == nullptr || pData[i]->GetSchemaType() != eSchemaType_MemberReference) continue;
		CMemberReferenceField* pValue = static_cast<CMemberReferenceField*>(pData[i]);
		if (pValue->GetReferenceID() == id) {
			return i;
		}
	}
	return -1;
}