#pragma once

#include "DataType.h"

#include <string>
#include <vector>
using namespace std;

class CDotNetClass;

class CDotNetField
{
public:
	CDotNetField(void);
	~CDotNetField(void);

	string& Name() { return m_sName; }
	
	virtual EDataType Type() = 0;

	void SetParent( CDotNetClass* pParent );
	CDotNetClass* GetParent();

private:
	string m_sName;
	CDotNetClass* m_pParentClass; 
};

template<EDataType DATA_TYPE, typename VALUE_TYPE>
class CDotNetGenericField : public CDotNetField
{
public:
	CDotNetGenericField() {}
	~CDotNetGenericField() {}

	EDataType Type() { return DATA_TYPE; }

	void SetValue(VALUE_TYPE& value )
	{
		m_Value = value;
	}

	VALUE_TYPE& Value() { return m_Value; }

	VALUE_TYPE const GetValue() const
	{
		return m_Value;
	}

private:

	VALUE_TYPE m_Value;
};

typedef CDotNetGenericField< eDataType_Invalid, CDotNetClass* > CUserClassField;
typedef CDotNetGenericField< eDataType_Int16, INT16 > CInt16Field;
typedef CDotNetGenericField< eDataType_Int32, INT32 > CInt32Field;
typedef CDotNetGenericField< eDataType_Int64, INT64 > CInt64Field;

class CDotNetFieldPtrVector : public vector<CDotNetField*>
{
	bool m_bDeleteMemory;
public:
	CDotNetFieldPtrVector() : m_bDeleteMemory(true) {}
	~CDotNetFieldPtrVector()
	{
		if ( m_bDeleteMemory )
			ResetContent();
	}

	void SetDeleteMemory( bool bDeleteMemory ) { m_bDeleteMemory = bDeleteMemory; }

	size_t GetIndexByID( INT32 id );

	void ResetContent()
	{
		if ( size() == 0 ) return;

		CDotNetField** pData = &front();
		for(size_t i = 0; i < size(); i++ )
		{
			switch( pData[i]->Type() )
			{
			case eDataType_Invalid:
				{
					CUserClassField* pValue = dynamic_cast<CUserClassField*>(pData[i]);
					if ( pValue )
					{
						delete pValue;
					}
				}
				break;
			case eDataType_Int16:
				{
					CInt16Field* pValue = (CInt16Field*)pData[i];
					delete pValue;
				}
				break;
			case eDataType_Int32:
				{
					CInt32Field* pValue = (CInt32Field*)pData[i];
					delete pValue;
				}
				break;
			case eDataType_Int64:
				{
					CInt64Field* pValue = (CInt64Field*)pData[i];
					delete pValue;
				}
				break;
			}
		}

		clear();
	}
};