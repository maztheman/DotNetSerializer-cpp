#include "StdAfx.h"
#include "DotNetClass.h"

#include <algorithm>

CDotNetClass::CDotNetClass(void)
	: m_nId(-1)
	, m_nFieldCount(-1)
	, m_nAssemblyID(-1)
	, m_eArrayType(eArrayType_Invalid)
	, m_eSchemaType(eSchemaType_Invalid)
	, m_eElementSchemaDataType(eSchemaDataType_Invalid)
	, m_eDataType(eDataType_Invalid)
	, m_nElementAssemblyID(-1)
	, m_pSchemaClass(nullptr)
{
}

CDotNetClass::~CDotNetClass(void)
{
	m_arFieldValues.ResetContent();
}

size_t CDotNetClass::FindName(string& name)
{
	CStringVector::iterator it = find_if(m_arFieldNames.begin(), m_arFieldNames.end(), [&name] (string& left) -> bool
	  {
		string sTemp = left;
		size_t nIndex = sTemp.find("k__BackingField");
		if (nIndex != -1) {//auto properties need special work.
			sTemp.erase(nIndex);
			sTemp.erase(0 , 1);
			sTemp.erase(sTemp.size() - 1);
		}
		return sTemp == name;
	});

	if (it != m_arFieldNames.end()) {
		return std::distance(m_arFieldNames.begin(), it);
	}

	return -1;
}

INT16 CDotNetClass::GetInt16(string name)
{
	return GetData<INT16, CInt16Field>(name);
}

INT32 CDotNetClass::GetInt32(string name)
{
	return GetData<INT32, CInt32Field>(name);
}

CStringVector& CDotNetClass::GetStringArray(string name)
{
	return GetArrayData<CStringVector , CStringArrayField>(name);
}

CInt32Vector& CDotNetClass::GetInt32Array(string name)
{
	return GetArrayData<CInt32Vector , CInt32ArrayField>(name);
}

INT64 CDotNetClass::GetInt64(string name)
{
	return GetData<INT64, CInt64Field>(name);
}

string CDotNetClass::GetString(string name)
{
	string sValue;
	size_t nIndex = FindName(name);
	if (nIndex == -1)
		return sValue;
	CDotNetField* pField = m_arFieldValues[nIndex];
	CStringField* pValue = dynamic_cast<CStringField*>(pField);
	if (pValue == nullptr)
		return sValue;
	return pValue->Value();
}

CDotNetClass* CDotNetClass::GetObject(string name)
{
	size_t nIndex = FindName(name);
	if (nIndex == -1)
		return nullptr;

	CDotNetField* pField = m_arFieldValues[nIndex];
	CUserClassField* pValue = dynamic_cast<CUserClassField*>(pField);
	if (pValue == nullptr)
		return nullptr;
	return pValue->Value();
}
