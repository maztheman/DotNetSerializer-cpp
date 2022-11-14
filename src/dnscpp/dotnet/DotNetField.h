#pragma once

#include "types/DataType.h"
#include "types/SchemaType.h"
#include <string>
#include <sstream>
#include <string_view>

using namespace std::string_view_literals;

using std::string;
using std::stringstream;

class CDotNetClass;

//maybe we use a variant to hold the different base types? this way all fields could be the "same" thing

//used to figure out what dynamic_cast we can do on the base class.
enum class DotNetFieldType
{
	Primitive,
	MemberReference,
	UserClass
};

inline std::string_view ToString(DotNetFieldType value)
{
	switch(value)
	{
		using enum DotNetFieldType;
		case Primitive:
			return "Primitive"sv;
		case MemberReference:
			return "Member Reference"sv;
		case UserClass:
			return "User Class"sv;
		default:
			return "Unknown"sv;
	}
}

class CDotNetField
{
	PROPERTY(ObjectID, uint32_t, m_uiObjectID)
	PROPERTY(SchemaType, ESchemaType, m_eSchemaType)
public:
	CDotNetField(DotNetFieldType eFieldType);
	//do not slice
	CDotNetField(const CDotNetField&) = delete;
	CDotNetField& operator=(const CDotNetField&) = delete;
	CDotNetField(CDotNetField&&) = delete;
	virtual ~CDotNetField();

	virtual EDataType Type() const = 0;

	std::string& Name() { return m_sName; }
	const string& GetName() const { return m_sName; }
	void SetName(const string& sName) { m_sName = sName; }
	

	DotNetFieldType GetFieldType() const
	{
		return m_FieldType;
	}

	void SetParent(const std::shared_ptr<CDotNetClass>& pParent);
	std::shared_ptr<CDotNetClass> GetParent();

	std::string ToString() const;

private:
	std::string m_sName;
	std::shared_ptr<CDotNetClass> m_pParentClass;
	DotNetFieldType m_FieldType;

	virtual std::string do_ToString() const = 0;

private:
};




using CDotNetFieldPtrVector = std::vector<std::shared_ptr<CDotNetField>>;

size_t GetIndexByID(const CDotNetFieldPtrVector& vec, uint32_t id);
