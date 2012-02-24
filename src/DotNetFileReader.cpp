#include "StdAfx.h"
#include "DotNetFileReader.h"
#include "SchemaType.h"
#include "BinaryStream.h"
#include "DotNetField.h"
#include "tools.h"
#include "ArrayReader.h"

#include <iostream>

CDotNetFileReader::CDotNetFileReader(CBinaryStream& stream) : m_Stream(stream)
{
}

CDotNetFileReader::~CDotNetFileReader(void)
{
}

void CDotNetFileReader::ReadClass()
{
	BYTE id = m_Stream.ReadInt32();
	CDotNetClass* pNewClass = new CDotNetClass;
	CDotNetClass& oClass = *pNewClass;

	oClass.ID() = id;
	m_Stream.ReadString( oClass.Name() );
	m_arCurrentClass.push_back(pNewClass);
	INT32 nFieldCount = oClass.FieldCount() = m_Stream.ReadInt32();

	//Resize All Arrays

	oClass.FieldArrayTypes().resize( nFieldCount, eArrayDataType_Invalid );
	oClass.FieldNames().resize( nFieldCount );
	oClass.FieldTypes().resize( nFieldCount, eDataType_Invalid );
	oClass.FieldValues().resize( nFieldCount, (CDotNetField*)0);
	oClass.SchemaTypes().resize( nFieldCount, eSchemaDataType_Invalid );
	oClass.ArraySizes().resize( nFieldCount, -1 );

	if ( nFieldCount <= 0 )
	{
		return;
	}

	EArrayDataType* pArrayDataTypes = GET_DATA(&oClass.FieldArrayTypes());
	string* pFieldNames = GET_DATA(&oClass.FieldNames());
	EDataType* pDataTypes = GET_DATA(&oClass.FieldTypes());
	CDotNetField** ppFieldValues = GET_DATA(&oClass.FieldValues());
	ESchemaDataType* pSchemaTypes = GET_DATA(&oClass.SchemaTypes());
	INT32* pArraySizes = GET_DATA(&oClass.ArraySizes());

	for(INT32 n = 0; n < nFieldCount; n++ )
	{
		m_Stream.ReadString(pFieldNames[n]);
	}

	for(INT32 n = 0; n < nFieldCount; n++ )
	{//Try to figure out the basic schema type
		//im guessing this could be a normal type or a user defined type..
		pSchemaTypes[n] = (ESchemaDataType)m_Stream.ReadByte();
	}

	for(INT32 n = 0; n < nFieldCount; n++ )
	{
		ESchemaDataType eSchemaDataType = pSchemaTypes[n];
		if ( eSchemaDataType == eSchemaDataType_Normal )
		{
			pDataTypes[n] = (EDataType)m_Stream.ReadByte();
		}
		else if ( eSchemaDataType == eSchemaDataType_UserDefinedObject )
		{
			pDataTypes[n] = eDataType_Invalid;
			string sClassName;
			m_Stream.ReadString( sClassName );
			m_Stream.ReadInt32(); //some id, maybe a way to figure out it later... its 2
		}
		else if ( eSchemaDataType == eSchemaDataType_String )
		{
			m_Stream.ReadInt32(); //no clue!!
			pDataTypes[n] = (EDataType)m_Stream.ReadByte();
		}
		else if ( eSchemaDataType == eSchemaDataType_StringArray )
		{
			pDataTypes[n] = eDataType_Invalid;
			pArrayDataTypes[n] = eArrayDataType_String;
		}
		else if ( eSchemaDataType == eSchemaDataType_Array )
		{
			pDataTypes[n] = (EDataType)m_Stream.ReadByte();
		}
	}

	oClass.UnknownID() = m_Stream.ReadInt32();//TODO: figure out why this 2 is here?
				
	for(INT32 n = 0; n < nFieldCount; n++ )
	{
		CDotNetField* pField = ReadField(pSchemaTypes[n], pDataTypes[n]);
		if ( pField == (CDotNetField*)0) throw new std::exception("Field type is not supported!");

		pField->Name() = pFieldNames[n];
		pField->SetParent( pNewClass );
		ppFieldValues[n] = pField ;
	}

	m_mpClasses.insert( pair<INT32,CDotNetClass*>( id, pNewClass) );

}

CDotNetField* CDotNetFileReader::ReadField(ESchemaDataType eSchemaDataType, EDataType eDataType)
{
	CDotNetField* pField = (CDotNetField*)0;

	if ( eSchemaDataType == eSchemaDataType_StringArray )
	{
		m_Stream.ReadByte();//9
		m_Stream.ReadInt32();//3
		m_Stream.ReadByte();//0x11
		m_Stream.ReadInt32();//3

		CStringArrayField* pValue = new CStringArrayField;
		INT32 nCount = m_Stream.ReadInt32();
		CArrayReader* pReader = CArrayReader::GetArrayReaderByType( eDataType_String, m_Stream );
		pReader->SetField( pValue );

		for(INT32 i = 0; i < nCount; i++ )
		{
			pReader->Read();
		}

		delete pReader;

		pField = pValue;
	}
	else if ( eSchemaDataType == eSchemaDataType_Array )
	{
		m_Stream.ReadByte();//9
		m_Stream.ReadInt32();//3
		m_Stream.ReadByte();//0xF
		m_Stream.ReadInt32();//3

		INT32 nCount = m_Stream.ReadInt32();
		EDataType type = (EDataType)m_Stream.ReadByte();

		switch( type )
		{
		case eDataType_Int32:
			{
				CInt32ArrayField* pValue = new CInt32ArrayField;
				pField = pValue;
			}
			break;
		}

		CArrayReader* pReader = CArrayReader::GetArrayReaderByType( type, m_Stream );
		pReader->SetField( pField );

		for(INT32 n = 0; n < nCount; n++ )
		{
			pReader->Read();
		}

		delete pReader;
	}
	else
	{
		switch( eDataType )
		{
			case eDataType_Invalid:
				{
					BYTE byte = m_Stream.ReadByte();
					if ( byte == 9 )
					{
						CUserClassField* pUserField = new CUserClassField;
						pUserField->Value() = new CDotNetClass;
						pUserField->Value()->ID() = m_Stream.ReadInt32();
						pField = pUserField;
						m_arNumber9.push_back( pUserField );
					}
					else
					{
						throw std::exception("I dont know what any other type (!9) for the eDataType_Invalid could be");
					}
				}
				break;
			/*case eDataType_String:
				{
					CStringField* pValue = new CStringField;
					m_Stream.ReadString( pValue->Value() );
					pField = pValue;
				}
				break;*/
			case eDataType_Int16:
				{
					CInt16Field* pIntField = new CInt16Field;
					pIntField->Value() = m_Stream.ReadInt16() ;
					pField = pIntField;
				}
				break;
			case eDataType_Int32:
				{
					CInt32Field* pIntField = new CInt32Field;
					pIntField->Value() = m_Stream.ReadInt32() ;
					pField = pIntField;
				}
				break;
			case eDataType_Int64:
				{
					CInt64Field* pIntField = new CInt64Field;
					pIntField->Value() = m_Stream.ReadInt64();
					pField = pIntField;
				}
				break;
		}
	}

	return pField;
}

void CDotNetFileReader::ReadEnd()
{
	size_t nSize = m_arNumber9.size();
	if ( nSize > 0 )
	{
		CDotNetField** pFields = &m_arNumber9.front();

		for( size_t i = 0; i < nSize; i++ )
		{
			CUserClassField* pField = dynamic_cast<CUserClassField*>(pFields[i]);
			if ( pField == (CUserClassField*)0 ) continue;

			CDotNetClass* pParent = pField->GetParent();
			CDotNetClassPtrMap::iterator itFound = m_mpClasses.find( pField->GetValue()->ID() );
			if ( itFound != m_mpClasses.end() )
			{
				size_t index = pParent->FieldValues().GetIndexByID( pField->GetValue()->ID() );

				CUserClassField* pRemoveField = dynamic_cast<CUserClassField*>(pParent->FieldValues()[index]);
				if ( pRemoveField == (CUserClassField*)0 ) continue;
				delete pRemoveField->Value();

				pRemoveField->Value() = itFound->second;
			}

		}

		m_arNumber9.clear();
	}
}

void CDotNetFileReader::ReadAssembly()
{
	INT32 id = m_Stream.ReadInt32();
	CDotNetAssembly* pAssembly = new CDotNetAssembly;
	pAssembly->ID() = id;
	m_Stream.ReadString( pAssembly->Name() );
	m_mpAssemblies.insert( pair< INT32, CDotNetAssembly*>( id, pAssembly ) );
}

CDotNetClass* CDotNetFileReader::Deserialize()
{
	bool bContinue = true;

	{
		BYTE junk[17] = {0};
		m_Stream.ReadBytes( junk );
	}

	try
	{
		while ( bContinue )
		{
			ESchemaType eSchemaType = (ESchemaType)m_Stream.ReadByte();

			switch( eSchemaType )
			{
			case eSchemaType_SystemClass:
				{

				}
			case eSchemaType_Class:
				{//class
					ReadClass();
				}
				break;
			case eSchemaType_End:
				{//assembly end or object end
					ReadEnd();
					bContinue = false;
				}
				break;
				//must mean assembly
			case eSchemaType_Assembly:
				{
					ReadAssembly();
				}
				break;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "Error caught! -->" << e.what() << "<--" << endl;
	}

	if ( m_arCurrentClass.size() > 0 )
	{
		m_mpClasses.clear();
		m_arNumber9.SetDeleteMemory( false );
		return m_arCurrentClass[0];
	}

	return (CDotNetClass*)0;
}