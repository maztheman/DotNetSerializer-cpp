#pragma once
#include "DataType.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class CDotNetClass;

class CDotNetField
{
	PROPERTY(ObjectID, UINT32, m_uiObjectID)
public:
	CDotNetField(void);
	virtual ~CDotNetField(void);

	string& Name() { return m_sName; }
	const string& GetName() const { return m_sName; }
	void SetName(const string& sName) { m_sName = sName; }
	
	virtual EDataType Type() const = 0;

	void SetParent(CDotNetClass* pParent);
	CDotNetClass* GetParent();

	string ToString() const;

private:
	string m_sName;
	CDotNetClass* m_pParentClass;
	virtual string do_ToString() const = 0;

private:
	CDotNetField(const CDotNetField&);
	CDotNetField& operator=(const CDotNetField&);
};

template<EDataType DATA_TYPE, typename VALUE_TYPE>
class CDotNetGenericField : public CDotNetField
{
public:
	CDotNetGenericField() {}
	~CDotNetGenericField() {}

	EDataType Type() const { return DATA_TYPE; }

	void SetValue(const VALUE_TYPE& value)
	{
		m_Value = value;
	}

	VALUE_TYPE& Value() { return m_Value; }

	VALUE_TYPE const GetValue() const
	{
		return m_Value;
	}

private:
	virtual string do_ToString() const {
		std::stringstream ss;
		std::string s;
		ss << GetValue();
		s = ss.str();
		return s;
	}

	VALUE_TYPE m_Value;
};

template<>
string CDotNetGenericField<eDataType_Invalid, CStringVector>::do_ToString() const {
	std::stringstream ss;
	for(auto& s : m_Value) {
		ss << s << ",";
	}
	string sVal = ss.str();
	return sVal;
}

template<>
string CDotNetGenericField<eDataType_Invalid, CInt32Vector>::do_ToString() const {
	std::stringstream ss;
	for(auto& s : m_Value) {
		ss << s << ",";
	}
	string sVal = ss.str();
	return sVal;
}


typedef CDotNetGenericField<eDataType_Invalid, CDotNetClass*> CUserClassField;
typedef CDotNetGenericField<eDataType_Int16, INT16> CInt16Field;
typedef CDotNetGenericField<eDataType_Int32, INT32> CInt32Field;
typedef CDotNetGenericField<eDataType_Int64, INT64> CInt64Field;
typedef CDotNetGenericField<eDataType_DateTime, double> CDateTimeField;
typedef CDotNetGenericField<eDataType_Double, double> CDoubleField;
typedef CDotNetGenericField<eDataType_Boolean, bool> CBooleanField;

typedef CDotNetGenericField<eDataType_Invalid, CInt32Vector> CInt32ArrayField;
typedef CDotNetGenericField<eDataType_Invalid, CStringVector> CStringArrayField;
typedef CDotNetGenericField<eDataType_String, string> CStringField;

class CDotNetFieldPtrVector : public std::vector<CDotNetField*>
{
	bool m_bDeleteMemory;
public:
	CDotNetFieldPtrVector() : m_bDeleteMemory(true) {}
	~CDotNetFieldPtrVector()
	{
		if (m_bDeleteMemory)
			ResetContent();
	}
	void SetDeleteMemory(bool bDeleteMemory) { m_bDeleteMemory = bDeleteMemory; }
	size_t GetIndexByID(INT32 id) const;
	void ResetContent()
	{
		if (size() == 0) return;
		CDotNetField** pData = &front();
		for(size_t i = 0; i < size(); i++) {
			delete pData[i];
		}
		clear();
	}
};