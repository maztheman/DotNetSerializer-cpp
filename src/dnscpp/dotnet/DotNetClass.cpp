#include "DotNetClass.h"
#include "UserClassField.h"

#include <algorithm>

static DotNetPrimitiveType nullValue(nulltype{});

CDotNetClass::CDotNetClass(void)
	: m_nId(-1)
	, m_nFieldCount(-1)
	, m_nAssemblyID(-1)
	, m_eArrayType(eArrayType_Invalid)
	, m_eSchemaType(eSchemaType_Invalid)
	, m_pSchemaClass(nullptr)
{
}

CDotNetClass::~CDotNetClass(void)
{
	
}

size_t CDotNetClass::FindName(const std::string& name) const
{
	auto it = std::find_if(m_arFields.begin(), m_arFields.end(), [&name] (const auto& left) -> bool
	{
		std::string sTemp = left.GetName();
		size_t nIndex = sTemp.find("k__BackingField");
		if (nIndex != -1) {//auto properties need special work.
			sTemp.erase(nIndex);
			sTemp.erase(0 , 1);
			sTemp.erase(sTemp.size() - 1);
		}
		return sTemp == name;
	});
	if (it != m_arFields.end()) 
	{
		return std::distance(m_arFields.begin(), it);
	}
	return npos;
}

const DotNetPrimitiveType& CDotNetClass::GetValue(const std::string& name) const
{
	size_t nIndex = FindName(name);
	if (nIndex == npos)
	{
		return nullValue;
	}
	auto pField = m_arFieldValues[nIndex];
	auto pValue = dynamic_cast<DotNetPrimitiveTypeField*>(pField.get());
	if (pValue == nullptr)
	{
		return nullValue;
	}
	return pValue->GetValue();
}

CDotNetClass* CDotNetClass::GetObject(const std::string& name)
{
	size_t nIndex = FindName(name);
	if (nIndex == npos)
	{
		return nullptr;
	}
	auto pField = m_arFieldValues[nIndex];
	auto pValue = dynamic_cast<CUserClassField*>(pField.get());
	if (pValue == nullptr)
	{
		return nullptr;
	}
	return pValue->GetClassObject();
}
