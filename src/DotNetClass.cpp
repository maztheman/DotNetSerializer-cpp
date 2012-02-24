#include "StdAfx.h"
#include "DotNetClass.h"

#include <algorithm>

CDotNetClass::CDotNetClass(void)
{
}

CDotNetClass::~CDotNetClass(void)
{
	m_arFieldValues.ResetContent();
}

size_t CDotNetClass::FindName( string& name )
{
	vector<string>::iterator it = find_if( m_arFieldNames.begin(), m_arFieldNames.end(), [&name] ( string& left) -> bool
	  {
		string sTemp = left;
		size_t nIndex = sTemp.find("k__BackingField");
		if ( nIndex != -1 )
		{//auto properties need special work.
			sTemp.erase( nIndex );
			sTemp.erase( 0 , 1);
			sTemp.erase( sTemp.size() - 1 );
		}

		return sTemp == name;
	});

	if ( it != m_arFieldNames.end() )
	{
		return std::distance( m_arFieldNames.begin(), it );
	}

	return -1;
}

INT16 CDotNetClass::GetInt16( string name )
{
	return GetData< INT16, CInt16Field >( name );
}

INT32 CDotNetClass::GetInt32( string name )
{
	return GetData< INT32, CInt32Field>( name );
}

vector<string>& CDotNetClass::GetStringArray( string name )
{
	return GetArrayData< vector<string> , CStringArrayField>( name );
}

vector<INT32>& CDotNetClass::GetInt32Array( string name )
{
	return GetArrayData< vector<INT32> , CInt32ArrayField>( name );
}

INT64 CDotNetClass::GetInt64( string name )
{
	return GetData<INT64, CInt64Field>( name );
}

string CDotNetClass::GetString( string name )
{
	string sValue;
	size_t nIndex = FindName( name );
	if ( nIndex == -1 )
		return sValue;

	CDotNetField* pField = m_arFieldValues[nIndex];
	CStringField* pValue = dynamic_cast<CStringField*>(pField);
	if ( pValue == 0)
		return sValue;

	return pValue->Value();
}

CDotNetClass* CDotNetClass::GetObject( string name )
{
	size_t nIndex = FindName( name );
	if ( nIndex == -1 )
		return (CDotNetClass*)0;

	CDotNetField* pField = m_arFieldValues[nIndex];
	CUserClassField* pValue = dynamic_cast<CUserClassField*>(pField);
	if ( pValue == 0)
		return (CDotNetClass*)0;

	return pValue->Value();
}
