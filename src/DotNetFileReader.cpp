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

bool IsSystemClass(string& sClassName)
{
	return sClassName.find("System.") == 0;//starts with System.
}

void CDotNetFileReader::ReadClass(bool bSystemClass = false)
{
	BYTE id = m_Stream.ReadInt32();
	CDotNetClass* pNewClass = new CDotNetClass;
	CDotNetClass& oClass = *pNewClass;
	oClass.SetID(id);
	m_Stream.ReadString(oClass.Name());
	m_arCurrentClass.push_back(pNewClass);
	oClass.SetFieldCount(m_Stream.ReadInt32());
	INT32 nFieldCount = oClass.GetFieldCount();
	if (nFieldCount <= 0) {
		return;
	}
	//Resize All Arrays
	oClass.FieldArrayTypes().resize(nFieldCount, eArrayDataType_Invalid);
	oClass.FieldNames().resize(nFieldCount);
	oClass.FieldTypes().resize(nFieldCount, eDataType_Invalid);
	oClass.FieldTypeCodes().resize(nFieldCount);
	oClass.FieldValues().resize(nFieldCount, nullptr);
	oClass.SchemaTypes().resize(nFieldCount, eSchemaDataType_Invalid);
	oClass.ArraySizes().resize(nFieldCount, -1);
	EArrayDataType* pArrayDataTypes = GET_DATA(&oClass.FieldArrayTypes());
	string* pFieldNames = GET_DATA(&oClass.FieldNames());
	EDataType* pDataTypes = GET_DATA(&oClass.FieldTypes());
	string* pDataTypeCodes = GET_DATA(&oClass.FieldTypeCodes());
	CDotNetField** ppFieldValues = GET_DATA(&oClass.FieldValues());
	ESchemaDataType* pSchemaTypes = GET_DATA(&oClass.SchemaTypes());
	INT32* pArraySizes = GET_DATA(&oClass.ArraySizes());
	for(INT32 n = 0; n < nFieldCount; n++) {
		m_Stream.ReadString(pFieldNames[n]);
	}
	for(INT32 n = 0; n < nFieldCount; n++) {//Try to figure out the basic schema type
		//im guessing this could be a normal type or a user defined type..
		pSchemaTypes[n] = static_cast<ESchemaDataType>(m_Stream.ReadByte());
	}
	for(INT32 n = 0; n < nFieldCount; n++) {
		ESchemaDataType eSchemaDataType = pSchemaTypes[n];
		if (eSchemaDataType == eSchemaDataType_Normal) {
			pDataTypes[n] = static_cast<EDataType>(m_Stream.ReadByte());
		} else if (eSchemaDataType == eSchemaDataType_UserDefinedObject) {
			pDataTypes[n] = eDataType_Invalid;
			string sClassName;
			m_Stream.ReadString(pDataTypeCodes[n]);
			m_Stream.ReadInt32(); //some id, maybe a way to figure out it later... its 2
		} else if (eSchemaDataType == eSchemaDataType_SystemObject) {
			pDataTypes[n] = eDataType_Invalid;
			m_Stream.ReadString(pDataTypeCodes[n]);//looks like its fully qualified.
		} else if (eSchemaDataType == eSchemaDataType_String) {
			m_Stream.ReadInt32(); //no clue!!
			pDataTypes[n] = static_cast<EDataType>(m_Stream.ReadByte());
		} else if (eSchemaDataType == eSchemaDataType_StringArray) {
			pDataTypes[n] = eDataType_Invalid;
			pArrayDataTypes[n] = eArrayDataType_String;
		} else if (eSchemaDataType == eSchemaDataType_Array) {
			pDataTypes[n] = static_cast<EDataType>(m_Stream.ReadByte());
		}
	}
	if (!bSystemClass) {
		oClass.SetUnknownID(m_Stream.ReadInt32());//TODO: figure out why this 2 is here?, looks like on system classes this id is NOT present.
		//could be user object id
	}
	for(INT32 n = 0; n < nFieldCount; n++) {
		CDotNetField* pField = ReadField(pSchemaTypes[n], pDataTypes[n]);
		if (pField == nullptr) throw std::exception("Field type is not supported!");
		pField->SetName(pFieldNames[n]);
		pField->SetParent(pNewClass);
		ppFieldValues[n] = pField;
	}
	m_mpClasses.insert(std::make_pair(id, pNewClass));
}

CDotNetField* CDotNetFileReader::ReadField(ESchemaDataType eSchemaDataType, EDataType eDataType)
{
	CDotNetField* pField = nullptr;

	if (eSchemaDataType == eSchemaDataType_StringArray) {
		m_Stream.ReadByte();//9
		m_Stream.ReadInt32();//3
		m_Stream.ReadByte();//0x11
		m_Stream.ReadInt32();//3
		CStringArrayField* pValue = new CStringArrayField;
		INT32 nCount = m_Stream.ReadInt32();
		CArrayReader* pReader = CArrayReader::GetArrayReaderByType(eDataType_String, m_Stream);
		pReader->SetField(pValue);
		for(INT32 i = 0; i < nCount; i++) {
			pReader->Read();
		}
		delete pReader;
		pField = pValue;
	} else if (eSchemaDataType == eSchemaDataType_Array) {
		BYTE byte = m_Stream.ReadByte();
		if (byte == 9) {//should this be an enum or something?
			CUserClassField* pUserField = new CUserClassField;
			pUserField->SetValue(new CDotNetClass);
			pUserField->Value()->SetID(m_Stream.ReadInt32());
			pField = pUserField;
			m_arNumber9.push_back(pUserField);
		} else {
			throw std::exception("I dont know what any other type (!9) for the eSchemaDataType_Array could be");
		}
		/*m_Stream.ReadByte();//9
		INT32 nArrayType = m_Stream.ReadInt32();//3
		if (nArrayType == 4)
		{
			m_Stream.ReadInt32();//Real Size
			m_Stream.ReadInt32();//3
		}
		m_Stream.ReadByte();//0xF
		m_Stream.ReadInt32();//3

		INT32 nCount = m_Stream.ReadInt32();//on system arrays like lists this is the padding count
		EDataType type = (EDataType)m_Stream.ReadByte();

		switch(type)
		{
		case eDataType_Int32:
			{
				CInt32ArrayField* pValue = new CInt32ArrayField;
				pField = pValue;
			}
			break;
		}

		if (nArrayType == 4)
		{
			pField->Name() = "_items";
		}


		CArrayReader* pReader = CArrayReader::GetArrayReaderByType(type, m_Stream);
		pReader->SetField(pField);

		for(INT32 n = 0; n < nCount; n++)
		{
			pReader->Read();
		}

		delete pReader;*/
	} else {
		switch(eDataType)
		{
			case eDataType_Invalid:
				{
					BYTE byte = m_Stream.ReadByte();
					if (byte == 9) {
						CUserClassField* pUserField = new CUserClassField;
						pUserField->SetValue(new CDotNetClass);
						pUserField->Value()->SetID(m_Stream.ReadInt32());
						pField = pUserField;
						m_arNumber9.push_back(pUserField);
					} else {
						throw std::exception("I dont know what any other type (!9) for the eDataType_Invalid could be");
					}
				}
				break;
			/*case eDataType_String:
				{
					CStringField* pValue = new CStringField;
					m_Stream.ReadString(pValue->Value());
					pField = pValue;
				}
				break;*/
			case eDataType_Int16:
				{
					CInt16Field* pIntField = new CInt16Field;
					pIntField->SetValue(m_Stream.ReadInt16());
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
	if (nSize > 0) {
		CDotNetField** pFields = GET_DATA(&m_arNumber9);
		for(size_t i = 0; i < nSize; i++) {
			CUserClassField* pField = dynamic_cast<CUserClassField*>(pFields[i]);
			if (pField == nullptr) continue;
			CDotNetClass* pParent = pField->GetParent();
			CDotNetClassPtrMap::iterator itFound = m_mpClasses.find(pField->GetValue()->GetID());
			if (itFound != m_mpClasses.end()) {
				size_t index = pParent->GetFieldValues().GetIndexByID(pField->GetValue()->GetID());
				CUserClassField* pRemoveField = dynamic_cast<CUserClassField*>(pParent->FieldValues()[index]);//TODO: see if you can static cast this....
				if (pRemoveField == nullptr) continue;
				delete pRemoveField->Value();
				pRemoveField->SetValue(itFound->second);
			}
		}
		m_arNumber9.clear();
	}
}

void CDotNetFileReader::ReadAssembly()
{
	INT32 id = m_Stream.ReadInt32();
	CDotNetAssembly* pAssembly = new CDotNetAssembly;
	pAssembly->SetID(id);
	m_Stream.ReadString(pAssembly->Name());
	m_mpAssemblies.insert(std::make_pair(id, pAssembly));
}

CDotNetClass* CDotNetFileReader::Deserialize()
{
	bool bContinue = true;
	{
		BYTE junk[17] = {0};
		m_Stream.ReadBytes(junk);
	}
	try
	{
		while (bContinue)
		{
			ESchemaType eSchemaType = static_cast<ESchemaType>(m_Stream.ReadByte());

			switch(eSchemaType)
			{
			case eSchemaType_SystemClass:
				{
					ReadClass(true);
				}
				break;
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
			case eSchemaType_FieldRef:
				{
					INT32 refId = m_Stream.ReadInt32();
					size_t nIndex = m_arNumber9.GetIndexByID(refId);
					if (nIndex == -1) throw std::exception("Bad Field Reference ID");
					CDotNetField* pField = m_arNumber9[nIndex];
					CDotNetClass* pParent = pField->GetParent();
					if (pParent->FieldCount() <= 0) throw std::exception("bad field count");
					size_t nFieldIndex = pParent->GetFieldValues().GetIndexByID(refId);
					if (nFieldIndex == -1) throw std::exception("Could not find field...");
					CDotNetField** pFields = GET_DATA(&pParent->FieldValues());
					if (pParent->SchemaTypes()[nFieldIndex] == eSchemaDataType_Array) {
						INT32 nCount = m_Stream.ReadInt32();//on system arrays like lists this is the padding count
						EDataType type = static_cast<EDataType>(m_Stream.ReadByte());
						CDotNetField* pNewField = nullptr;
						switch(type)
						{
						case eDataType_Int32:
							{
								CInt32ArrayField* pValue = new CInt32ArrayField;
								pNewField = pValue;
							}
							break;
						}
						CArrayReader* pReader = CArrayReader::GetArrayReaderByType(type, m_Stream);
						pReader->SetField(pNewField);
						for(INT32 n = 0; n < nCount; n++) {
							pReader->Read();
						}
						delete pReader;
						pNewField->SetName(pFields[nFieldIndex]->GetName());
						delete pFields[nFieldIndex];
						pNewField->SetParent(pParent);
						pFields[nFieldIndex] = pNewField;
					}
					m_arNumber9.erase(m_arNumber9.begin() + nIndex);
				}
				break;
			}
		}
	} catch(std::exception& e) {
		std::cout << "Error caught! -->" << e.what() << "<--" << std::endl;
	}
	if (m_arCurrentClass.size() > 0) {
		m_mpClasses.clear();
		m_arNumber9.SetDeleteMemory(false);
		return m_arCurrentClass[0];
	}
	return nullptr;
}