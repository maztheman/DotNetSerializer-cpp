#pragma once

#include "DataType.h"
#include "SchemaDataType.h"
#include "DotNetField.h"

using std::vector;
using std::map;

class CDotNetClass
{
	PROPERTY(ID, INT32, m_nId)
	PROPERTY(Name, string, m_sName)
	PROPERTY(FieldCount, INT32, m_nFieldCount)
	PROPERTY_OBJ(FieldNames, CStringVector, m_arFieldNames)
	PROPERTY_OBJ(FieldTypes, CDataTypeVector, m_arFieldTypes)
	PROPERTY_OBJ(FieldTypeCodes, CStringVector, m_arFieldTypeCodes)
	PROPERTY_OBJ(FieldArrayTypes, CArrayDataTypeVector, m_arArrayTypes)
	PROPERTY_OBJ(ArraySizes, CInt32Vector, m_arArraySizes)
	PROPERTY_OBJ(SchemaTypes, CSchemaDataTypeVector, m_arSchemaTypes)
	PROPERTY_OBJ(FieldValues, CDotNetFieldPtrVector, m_arFieldValues)
	PROPERTY(UnknownID, INT32, m_nUnknownID)

public:
	CDotNetClass(void);
	~CDotNetClass(void);

	INT16 GetInt16(string name);
	INT32 GetInt32(string name);
	INT64 GetInt64(string name);
	string GetString(string name);
	CDotNetClass* GetObject(string name);
	CInt32Vector& GetInt32Array(string name);
	CStringVector& GetStringArray(string name);

protected:

	size_t FindName(string& name);

	template<typename DATA_TYPE, typename FIELD_TYPE>
	DATA_TYPE GetData(string& name)
	{
		size_t nIndex = FindName(name);
		if (nIndex == -1)
			return static_cast<DATA_TYPE>(-1);
		CDotNetField* pField = m_arFieldValues[nIndex];
		FIELD_TYPE* pValue = dynamic_cast<FIELD_TYPE*>(pField);
		if (pValue == 0)
			return static_cast<DATA_TYPE>(-1);
		return static_cast<DATA_TYPE>(pValue->Value());
	}

	template<typename DATA_TYPE, typename FIELD_TYPE>
	DATA_TYPE& GetArrayData(string& name)
	{
		size_t nIndex = FindName(name);
		if (nIndex == -1)
			throw std::exception("Field doesnt exist");

		CDotNetField* pField = m_arFieldValues[nIndex];
		FIELD_TYPE* pValue = dynamic_cast<FIELD_TYPE*>(pField);
		if (pValue == 0)
			throw std::exception("Field is not correct type");

		return pValue->Value();
	}
};

class CDotNetClassPtrMap : public std::map<INT32, CDotNetClass*>
{
public:
	CDotNetClassPtrMap() {}
	~CDotNetClassPtrMap()
	{
		ResetContent();
	}
	void ResetContent()
	{
		while(!empty()) {
			iterator it = begin();
			CDotNetClass* pObject = it->second;
			delete pObject;
			erase(it);
		}
	}
};