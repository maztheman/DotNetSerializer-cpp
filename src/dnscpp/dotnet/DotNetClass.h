#pragma once

#include "DotNetField.h"

#include "types/DataType.h"
#include "types/SchemaDataType.h"
#include "types/ArrayType.h"
#include "types/SchemaType.h"

class CField
{
	PROPERTY(Name, std::string, m_sName)
	PROPERTY(DataType, EDataType, m_eDataType)
	PROPERTY(TypeCode, std::string, m_sTypeCode)
	PROPERTY(SchemaDataType, ESchemaDataType, m_eSchemaDataType)
	PROPERTY(AssemblyID, uint32_t, m_uiAssemblyID)
public:
	CField()
		: m_eDataType(eDataType_Invalid)
		, m_eSchemaDataType(eSchemaDataType_Invalid)
		, m_uiAssemblyID(-1)
	{}
};

using CFieldVector = std::vector<CField>;

class CDotNetClass
{
	PROPERTY(ID, uint32_t, m_nId)
	PROPERTY(Name, std::string, m_sName)
	PROPERTY(FieldCount, int32_t, m_nFieldCount)
	PROPERTY_OBJ(Fields, CFieldVector, m_arFields)
	PROPERTY_OBJ(ArraySizes, CInt32Vector, m_arArraySizes)
	PROPERTY_OBJ(FieldValues, CDotNetFieldPtrVector, m_arFieldValues)
	PROPERTY(AssemblyID, uint32_t, m_nAssemblyID)
	PROPERTY(ArrayType, EArrayType, m_eArrayType)
	PROPERTY(SchemaType, ESchemaType, m_eSchemaType)
	PROPERTY(Element, CField, m_Element)
	PROPERTY_PTR(SchemaReferenceClass, CDotNetClass, m_pSchemaClass)

public:
	CDotNetClass(void);
	~CDotNetClass(void);

	int16_t GetInt16(const std::string& name);
	int32_t GetInt32(const std::string& name);
	int64_t GetInt64(const std::string& name);
	std::string GetString(const std::string& name);
	CDotNetClass* GetObject(const std::string& name);
	CInt32Vector& GetInt32Array(const std::string& name);
	CStringVector& GetStringArray(const std::string& name);

protected:

	size_t FindName(const std::string& name);

	template<typename DATA_TYPE, typename FIELD_TYPE>
	DATA_TYPE GetData(const std::string& name)
	{
		size_t nIndex = FindName(name);
		if (nIndex == -1)
			return static_cast<DATA_TYPE>(-1);
		auto pField = m_arFieldValues[nIndex];
		FIELD_TYPE* pValue = dynamic_cast<FIELD_TYPE*>(pField.get());
		if (pValue == nullptr)
			return static_cast<DATA_TYPE>(-1);
		return static_cast<DATA_TYPE>(pValue->Value());
	}

	template<typename DATA_TYPE, typename FIELD_TYPE>
	DATA_TYPE& GetArrayData(const std::string& name)
	{
		size_t nIndex = FindName(name);
		if (nIndex == -1)
			throw std::runtime_error("Field doesnt exist");

		auto pField = m_arFieldValues[nIndex];
		FIELD_TYPE* pValue = dynamic_cast<FIELD_TYPE*>(pField.get());
		if (pValue == nullptr)
			throw std::runtime_error("Field is not correct type");

		return pValue->Value();
	}
};

using CDotNetClassPtrVector = std::vector<std::shared_ptr<CDotNetClass>>;
using CDotNetClassPtrMap = std::unordered_map<uint32_t, std::shared_ptr<CDotNetClass>>;
