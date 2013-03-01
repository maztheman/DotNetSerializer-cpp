#pragma once

#include "DataType.h"
#include "SchemaDataType.h"
#include "DotNetField.h"
#include "ArrayType.h"
#include "SchemaType.h"

using std::vector;
using std::map;

class CField
{
	PROPERTY(Name, string, m_sName)
	PROPERTY(DataType, EDataType, m_eDataType)
	PROPERTY(TypeCode, string, m_sTypeCode)
	PROPERTY(SchemaDataType, ESchemaDataType, m_eSchemaDataType)
	PROPERTY(AssemblyID, UINT32, m_uiAssemblyID)
public:
	CField()
		: m_eDataType(eDataType_Invalid)
		, m_eSchemaDataType(eSchemaDataType_Invalid)
		, m_uiAssemblyID(-1)
	{}
};

typedef std::vector<CField> CFieldVector;

class CDotNetClass
{
	PROPERTY(ID, UINT32, m_nId)
	PROPERTY(Name, string, m_sName)
	PROPERTY(FieldCount, INT32, m_nFieldCount)
	PROPERTY_OBJ(Fields, CFieldVector, m_arFields)
	PROPERTY_OBJ(ArraySizes, CInt32Vector, m_arArraySizes)
	PROPERTY_OBJ(FieldValues, CDotNetFieldPtrVector, m_arFieldValues)
	PROPERTY(AssemblyID, INT32, m_nAssemblyID)
	PROPERTY(ArrayType, EArrayType, m_eArrayType)
	PROPERTY(SchemaType, ESchemaType, m_eSchemaType)
	PROPERTY(Element, CField, m_Element)
	PROPERTY_PTR(SchemaReferenceClass, CDotNetClass*, m_pSchemaClass)

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

class CDotNetClassPtrMap : public std::map<UINT32, CDotNetClass*>
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