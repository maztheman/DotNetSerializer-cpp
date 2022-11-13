#include "DotNetField.h"
#include "DotNetClass.h"
#include "UserClassField.h"
#include "MemberReferenceField.h"

CDotNetField::CDotNetField(void)
	: m_uiObjectID(~0U)
	, m_eSchemaType(eSchemaType_Invalid)
{
}

CDotNetField::~CDotNetField(void)
{
}

void CDotNetField::SetParent(const std::shared_ptr<CDotNetClass>& pParent)
{
	m_pParentClass = pParent;
}

std::shared_ptr<CDotNetClass> CDotNetField::GetParent()
{
	return m_pParentClass;
}

string CDotNetField::ToString() const
{
	return do_ToString();
}

size_t GetIndexByID(const CDotNetFieldPtrVector& vec, uint32_t id)
{
	for(size_t i = 0; auto& item : vec)
	{
		if (item == nullptr || item->GetSchemaType() != eSchemaType_MemberReference)
		{
			//skip!
		}
		else
		{
			auto pValue = static_cast<CMemberReferenceField*>(item.get());
			if (pValue->GetReferenceID() == id) {
				return i;
			}
		}
		i++;
	}
	return ~0UL;
}