#include "StdAfx.h"
#include "DotNetFileReader.h"
#include "SchemaType.h"
#include "BinaryStream.h"
#include "DotNetField.h"
#include "tools.h"

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

	for(INT32 n = 0; n < nFieldCount; n++ )
	{
		string sName;
		m_Stream.ReadString(sName);
		oClass.FieldNames().push_back( sName );
	}

	for(INT32 n = 0; n < nFieldCount; n++ )
	{//Try to figure out the basic schema type
		//im guessing this could be a normal type or a user defined type..
		ESchemaDataType type = (ESchemaDataType)m_Stream.ReadByte();
		oClass.SchemaTypes().push_back( type );
	}

	for(INT32 n = 0; n < nFieldCount; n++ )
	{
		ESchemaDataType eSchemaDataType = oClass.SchemaTypes()[n];
		if ( eSchemaDataType == eSchemaDataType_Normal )
		{
			EDataType dataType = (EDataType)m_Stream.ReadByte();
			oClass.FieldTypes().push_back( dataType );
		}
		else if ( eSchemaDataType == eSchemaDataType_UserDefinedObject )
		{
			oClass.FieldTypes().push_back( eDataType_Invalid );
			string sClassName;
			m_Stream.ReadString( sClassName );
			m_Stream.ReadInt32(); //some id, maybe a way to figure out it later... its 2
		}
	}

	oClass.UnknownID() = m_Stream.ReadInt32();//TODO: figure out why this 2 is here?
				
	for(INT32 n = 0; n < oClass.FieldCount(); n++ )
	{
		CDotNetField* pField = ReadField(oClass.FieldTypes()[n]);
		if ( pField == (CDotNetField*)0) throw new std::exception("Field type is not supported!");

		pField->Name() = oClass.FieldNames()[n];
		pField->SetParent( pNewClass );
		oClass.FieldValues().push_back( pField );
	}

	m_mpClasses.insert( pair<INT32,CDotNetClass*>( id, pNewClass) );

}

CDotNetField* CDotNetFileReader::ReadField(EDataType eDataType)
{
	CDotNetField* pField = (CDotNetField*)0;

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