#include "DotNetClass.h"
#include "UserClassField.h"

#include <algorithm>

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

size_t CDotNetClass::FindName(const std::string& name)
{
	auto it = std::find_if(m_arFields.begin(), m_arFields.end(), [&name] (CField& left) -> bool
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
	return ~0UL;
}

int16_t CDotNetClass::GetInt16(const std::string& name)
{
	return GetData<int16_t, CInt16Field>(name);
}

int32_t CDotNetClass::GetInt32(const std::string& name)
{
	return GetData<int32_t, CInt32Field>(name);
}

CStringVector& CDotNetClass::GetStringArray(const std::string& name)
{
	return GetArrayData<CStringVector , CStringArrayField>(name);
}

CInt32Vector& CDotNetClass::GetInt32Array(const std::string& name)
{
	return GetArrayData<CInt32Vector , CInt32ArrayField>(name);
}

int64_t CDotNetClass::GetInt64(const std::string& name)
{
	return GetData<int64_t, CInt64Field>(name);
}

std::string CDotNetClass::GetString(const std::string& name)
{
	std::string sValue;
	size_t nIndex = FindName(name);
	if (nIndex == -1)
		return sValue;
	auto pField = m_arFieldValues[nIndex];
	auto pValue = dynamic_cast<CStringField*>(pField.get());
	if (pValue == nullptr)
		return sValue;
	return pValue->Value();
}

CDotNetClass* CDotNetClass::GetObject(const std::string& name)
{
	size_t nIndex = FindName(name);
	if (nIndex == -1)
		return nullptr;
	auto pField = m_arFieldValues[nIndex];
	auto pValue = dynamic_cast<CUserClassField*>(pField.get());
	if (pValue == nullptr)
		return nullptr;
	return pValue->GetClassObject();
}
