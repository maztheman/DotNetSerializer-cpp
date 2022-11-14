#pragma once

#include "DotNetField.h"
#include "DotNetPrimitiveTypeField.h"

#include "types/DataType.h"
#include "types/SchemaDataType.h"
#include "types/ArrayType.h"
#include "types/SchemaType.h"

#include <string>

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
	inline static constexpr size_t npos = ~0UL;

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

	const DotNetPrimitiveType& GetValue(const std::string& name) const;
	CDotNetClass* GetObject(const std::string& name);
protected:
	size_t FindName(const std::string& name) const;
};

using CDotNetClassPtrVector = std::vector<std::shared_ptr<CDotNetClass>>;
using CDotNetClassPtrMap = std::unordered_map<uint32_t, std::shared_ptr<CDotNetClass>>;
