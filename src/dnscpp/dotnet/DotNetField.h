#pragma once

#include "types/DataType.h"
#include "types/SchemaType.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class CDotNetClass;

//maybe we use a variant to hold the different base types? this way all fields could be the "same" thing

class CDotNetField
{
	PROPERTY(ObjectID, uint32_t, m_uiObjectID)
	PROPERTY(SchemaType, ESchemaType, m_eSchemaType)
public:
	CDotNetField(void);
	virtual ~CDotNetField(void);

	string& Name() { return m_sName; }
	const string& GetName() const { return m_sName; }
	void SetName(const string& sName) { m_sName = sName; }
	
	virtual EDataType Type() const = 0;

	void SetParent(const std::shared_ptr<CDotNetClass>& pParent);
	std::shared_ptr<CDotNetClass> GetParent();

	std::string ToString() const;

private:
	string m_sName;
	std::shared_ptr<CDotNetClass> m_pParentClass;
	virtual std::string do_ToString() const = 0;

private:
	CDotNetField(const CDotNetField&) = delete;
	CDotNetField& operator=(const CDotNetField&) = delete;
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
	virtual string do_ToString() const 
	{
		std::stringstream ss;
		std::string s;

		if constexpr (std::is_same_v<VALUE_TYPE, CStringVector>)
		{
			for(auto& s : m_Value) {
				ss << s << ",";
			}
		}
		else if constexpr (std::is_same_v<VALUE_TYPE, CInt32Vector>)
		{
			for(auto& s : m_Value) {
				ss << s << ",";
			}
		}

		s = ss.str();
		return s;
	}

	VALUE_TYPE m_Value;
};

/* template<>
inline string CDotNetGenericField<eDataType_Invalid, CStringVector>::do_ToString() const 
{
	std::stringstream ss;
	for(auto& s : m_Value) {
		ss << s << ",";
	}
	string sVal = ss.str();
	return sVal;
}

template<>
inline  string CDotNetGenericField<eDataType_Invalid, CInt32Vector>::do_ToString() const 
{
	std::stringstream ss;
	for(auto& s : m_Value) {
		ss << s << ",";
	}
	string sVal = ss.str();
	return sVal;
} */


//typedef CDotNetGenericField<eDataType_Invalid, CDotNetClass*> CUserClassField;
using CInt16Field = CDotNetGenericField<eDataType_Int16, int16_t>;
using CInt32Field = CDotNetGenericField<eDataType_Int32, int32_t>;
using CInt64Field = CDotNetGenericField<eDataType_Int64, int64_t>;
using CDateTimeField = CDotNetGenericField<eDataType_DateTime, double>;
using CDoubleField = CDotNetGenericField<eDataType_Double, double>;
using CBooleanField = CDotNetGenericField<eDataType_Boolean, bool>;

using CInt32ArrayField = CDotNetGenericField<eDataType_Invalid, CInt32Vector>;
using CStringArrayField = CDotNetGenericField<eDataType_Invalid, CStringVector>;
using CStringField = CDotNetGenericField<eDataType_String, string>;

using CDotNetFieldPtrVector = std::vector<std::shared_ptr<CDotNetField>>;

size_t GetIndexByID(const CDotNetFieldPtrVector& vec, uint32_t id);
