#pragma once

#include "DataType.h"
#include "SchemaDataType.h"
#include "DotNetField.h"

#include <vector>
#include <string>
#include <map>
using namespace std;

class CDotNetClass
{
public:
	CDotNetClass(void);
	~CDotNetClass(void);

	INT32& ID() { return m_nId; }
	string& Name() { return m_sName; }
	INT32& FieldCount() { return m_nFieldCount; }
	vector<string>& FieldNames() { return m_arFieldNames; }
	vector<EDataType>& FieldTypes() { return m_arFieldTypes; }
	vector< ESchemaDataType>& SchemaTypes() { return m_arSchemaTypes; }
	CDotNetFieldPtrVector& FieldValues() { return m_arFieldValues; }
	INT32& UnknownID() { return m_nUnknownID; }

	INT16 GetInt16( string name );
	INT32 GetInt32( string name );
	INT64 GetInt64( string name );
	CDotNetClass* GetObject( string name );

protected:

	size_t FindName( string& name );

	template<typename DATA_TYPE, typename FIELD_TYPE>
	DATA_TYPE GetData( string& name )
	{
		size_t nIndex = FindName( name );
		if ( nIndex == -1 )
			return (DATA_TYPE)-1;

		CDotNetField* pField = m_arFieldValues[nIndex];
		FIELD_TYPE* pValue = dynamic_cast<FIELD_TYPE*>(pField);
		if ( pValue == 0)
			return (DATA_TYPE)-1;

		return (DATA_TYPE)pValue->Value();
	}

private:

	INT32 m_nId;
	string m_sName;
	INT32 m_nFieldCount;
	vector<string> m_arFieldNames;
	vector<EDataType> m_arFieldTypes;
	vector<ESchemaDataType> m_arSchemaTypes;
	CDotNetFieldPtrVector m_arFieldValues;
	INT32 m_nUnknownID;
};


class CDotNetClassPtrMap : public map< INT32, CDotNetClass*>
{
public:
	CDotNetClassPtrMap() {}
	~CDotNetClassPtrMap()
	{
		ResetContent();
	}

	void ResetContent()
	{
		while(!empty())
		{
			iterator it = begin();
			CDotNetClass* pObject = it->second;
			delete pObject;
			erase( it );
		}
	}
};