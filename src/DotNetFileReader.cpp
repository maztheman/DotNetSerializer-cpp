#include "StdAfx.h"
#include "DotNetFileReader.h"
#include "SchemaType.h"
#include "ArrayType.h"
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

CDotNetField* CDotNetFileReader::ReadPrimative(EDataType eDataType)
{
	CDotNetField* pField = nullptr;
	switch(eDataType) {
	case eDataType_String:
		{
			CStringField* pValue = new CStringField;
			m_Stream.ReadString(pValue->Value());
			pField = pValue;
		}
	break;
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
			pIntField->SetValue(m_Stream.ReadInt32());
			pField = pIntField;
		}
	break;
	case eDataType_Int64:
		{
			CInt64Field* pIntField = new CInt64Field;
			pIntField->SetValue(m_Stream.ReadInt64());
			pField = pIntField;
		}
	break;
	case eDataType_DateTime:
		{
			CDateTimeField* pDateField = new CDateTimeField;
			pDateField->SetValue(m_Stream.ReadDouble());
			pField = pDateField;
		}
	break;
	case eDataType_Double:
		{
			CDoubleField* pDoubleField = new CDoubleField;
			pDoubleField->SetValue(m_Stream.ReadDouble());
			pField = pDoubleField;
		}
		break;
	case eDataType_Boolean:
		{
			CBooleanField* p = new CBooleanField;
			p->SetValue(m_Stream.ReadBool());
			pField = p;
		}
		break;
	default:
		{
			int nStop = 0;
		}
		break;
	};

	return pField;
}

CDotNetClass* CDotNetFileReader::ReadClass(bool bSystemClass = false)
{
	UINT32 id = m_Stream.ReadInt32();
	CDotNetClass* pNewClass = new CDotNetClass;
	CDotNetClass& oClass = *pNewClass;
	oClass.SetID(id);
	m_Stream.ReadString(oClass.Name());
	m_arCurrentClass.push_back(pNewClass);
	oClass.SetFieldCount(m_Stream.ReadInt32());
	INT32 nFieldCount = oClass.GetFieldCount();
	if (nFieldCount <= 0) {
		return pNewClass;
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
		if (eSchemaDataType == eSchemaDataType_Primitive) {
			pDataTypes[n] = static_cast<EDataType>(m_Stream.ReadByte());
		} else if (eSchemaDataType == eSchemaDataType_UserDefinedObject) {
			pDataTypes[n] = eDataType_Invalid;
			m_Stream.ReadString(pDataTypeCodes[n]);
			m_Stream.ReadInt32();//assembly id
		} else if (eSchemaDataType == eSchemaDataType_SystemObject) {
			pDataTypes[n] = eDataType_Invalid;
			m_Stream.ReadString(pDataTypeCodes[n]);//looks like its fully qualified.
		} else if (eSchemaDataType == eSchemaDataType_String) {
		} else if (eSchemaDataType == eSchemaDataType_StringArray) {
			pDataTypes[n] = eDataType_Invalid;
			pArrayDataTypes[n] = eArrayDataType_String;
		} else if (eSchemaDataType == eSchemaDataType_Array) {
			pDataTypes[n] = static_cast<EDataType>(m_Stream.ReadByte());
		} else if (eSchemaDataType == eSchemaDataType_Object) {
			pDataTypes[n] = eDataType_Invalid;
		}
	}
	if (!bSystemClass) {
		oClass.SetAssemblyID(m_Stream.ReadInt32());
		//m_mpAssemblies[oClass.GetAssemblyID()]->Classes().push_back(pNewClass);
	}
	m_Log << "class " << oClass.GetName() << " { " << endl << "public:" << endl;
	for(INT32 n = 0; n < nFieldCount; n++) {
		CDotNetField* pField = nullptr;
		if (pSchemaTypes[n] == eSchemaDataType_Primitive) {
			pField= ReadPrimative(pDataTypes[n]);
		} else {
			pField = ReadField();
		}
		if (pField == nullptr)
		{
			throw std::exception("Field type is not supported!");
		}
		
		{
			if (pSchemaTypes[n] == eSchemaDataType_Array) {
				m_Log << EnumToString(pDataTypes[n]) << "[] ";
			} else if (pSchemaTypes[n] == eSchemaDataType_Object ||
					   pSchemaTypes[n] == eSchemaDataType_SystemObject) {
				m_Log << pDataTypeCodes[n] << " ";
			} else if (pSchemaTypes[n] == eSchemaDataType_Primitive) {
				m_Log << EnumToString(pDataTypes[n]) << " ";
			} else if (pSchemaTypes[n] == eSchemaDataType_UserDefinedObject) {
				m_Log << pDataTypeCodes[n] << " ";
			} else {
				m_Log << EnumToString(pSchemaTypes[n]) << " " << EnumToString(pDataTypes[n]) << " ";
			}
			m_Log << pFieldNames[n] << " = " << pField->ToString() << ";" << endl; 
		}
		pField->SetName(pFieldNames[n]);
		pField->SetParent(pNewClass);
		ppFieldValues[n] = pField;
	}
	m_Log << "};" << endl;
	m_mpClasses.insert(std::make_pair(id, pNewClass));

	return pNewClass;
}

CUserClassField* CDotNetFileReader::ReadMemberReference()
{
	CUserClassField* pUserField = new CUserClassField;
	CDotNetClass* pClass = new CDotNetClass;
	pClass->SetID(m_Stream.ReadInt32());
	pUserField->SetValue(pClass);
	m_arNumber9.push_back(pUserField);
	return pUserField;
}

CDotNetClass* CDotNetFileReader::ReadArray()
{
	//this function will not really do anything until i can fiture out how to remember this information...
	CDotNetClass* pArray = new CDotNetClass;
	pArray->SetSchemaType(eSchemaType_BinaryArray);
	m_arCurrentClass.push_back(pArray);
	pArray->SetID(m_Stream.ReadUInt32());
	m_mpClasses.insert(std::make_pair(pArray->GetID(), pArray));
	EArrayType eType = static_cast<EArrayType>(m_Stream.ReadByte());
	pArray->SetArrayType(eType);
	UINT32 nDims = m_Stream.ReadUInt32();
	pArray->ArraySizes().resize(nDims, 0);
	auto ppArraySizes = GET_DATA(&pArray->ArraySizes());
	size_t nNumOfFields = 0;
	for(UINT32 i = 0; i < nDims; i++) {
		ppArraySizes[i] = m_Stream.ReadInt32();//i think i loop..
		nNumOfFields += ppArraySizes[i];
	}
	pArray->FieldValues().resize(nNumOfFields, nullptr);
	ESchemaDataType eSchemaDataType = static_cast<ESchemaDataType>(m_Stream.ReadByte());
	pArray->SetElementSchemaDataType(eSchemaDataType);
	if (eSchemaDataType == eSchemaDataType_UserDefinedObject) {
		m_Stream.ReadString(pArray->ElementTypeCode());
		pArray->SetElementAssemblyID(m_Stream.ReadInt32());
	} else if (eSchemaDataType == eSchemaDataType_SystemObject) {
		m_Stream.ReadString(pArray->ElementTypeCode());
	} else if (eSchemaDataType == eSchemaDataType_Primitive) {
		EDataType eType = static_cast<EDataType>(m_Stream.ReadByte());
		//so array is a comprised of primitive types of nNumberOfDims demensions each dim is nNumberOfElemsPerDim size
		pArray->SetElementDataType(eType);
	}
	bool bContinue = nNumOfFields > 0;
	UINT32 nCurrentDim = 0;
	int nIndex = 0;
	size_t nField = 0;
	auto ppFields = GET_DATA(&pArray->FieldValues());
	while(bContinue) {
		ESchemaType eSchemaType = static_cast<ESchemaType>(m_Stream.ReadByte());
		switch(eSchemaType) {
		case eSchemaType_MemberReference:
			ppFields[nField] = ReadMemberReference();
			ppFields[nField]->SetParent(pArray);
			nField++;
			nIndex++;
			break;
		case eSchemaType_ObjectNull:
			nIndex++;
			break;
		case eSchemaType_ArrayFiller8b: 
			{
				int nFiller = m_Stream.ReadByte();
				nIndex += nFiller;
				nField += nFiller;
			}
			break;
		case eSchemaType_ArrayFiller32b:
			{
				UINT32 nFiller = m_Stream.ReadUInt32();
				nIndex += nFiller;
				nField += nFiller;
			}
			break;
		case eSchemaDataType_Array:
			{
				CUserClassField* p = new CUserClassField;
				p->SetValue(ReadArray());
				ppFields[nField] = p;
				ppFields[nField]->SetParent(pArray);
				nField++;
				nIndex++;
			}
			break;
		default:
			m_Log << "ReadArray, Unhandled schema type " << EnumToString(eSchemaType) << " (" << (int)eSchemaType << ")" << endl;
			break;
		};
		if (eType == eArrayType_SingleDimension) {
			if (nIndex >= ppArraySizes[0]) {
				bContinue = false;
			}
		} else if (eType == eArrayType_Jagged || eType == eArrayType_MultiDimension) {
			if (nIndex > ppArraySizes[nCurrentDim]) {
				nCurrentDim++;
			}
			if (nCurrentDim > nDims) {
				bContinue = false;
			}
		}
	}

	return pArray;
}


CDotNetField* CDotNetFileReader::ReadField()
{
	static int nArrayFiller = 0;
	CDotNetField* pField = nullptr;
	if (nArrayFiller > 0) {
		nArrayFiller--;
		return nullptr;
	}
	ESchemaType eFieldType = static_cast<ESchemaType>(m_Stream.ReadByte());
	switch(eFieldType) {
	default: throw std::exception("Unhandled ReadField Type"); break;	
	//1,4,5,6,8,9,10,13,14
	case eSchemaType_ArrayItem:
		{
			CUserClassField* pClass = new CUserClassField;
			CDotNetClass* pSub = ReadArrayItem();
			pClass->SetValue(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_SystemClass:
		{
			//value type only?
		}
		break;
	case eSchemaType_Class:
		//value type only?
		{
			CUserClassField* pClass = new CUserClassField;
			CDotNetClass* pSub = ReadClass();
			pClass->SetValue(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_BinaryObjectString:
			{
				CStringField* pSField = new CStringField;
				pSField->SetObjectID(m_Stream.ReadUInt32());
				m_Stream.ReadString(pSField->Value());
				pField = pSField;
			}
		break;
	case eSchemaType_BoxedPrimitiveType:
		//type spec, raw value
		break;
	case eSchemaType_MemberReference:
		pField = ReadMemberReference();
		break;
	case eSchemaType_ObjectNull://nop?
		pField = new CStringField;
		break;
	case eSchemaType_ArrayFiller8b:
		nArrayFiller = m_Stream.ReadByte();
		break;
	case eSchemaType_ArrayFiller32b:
		nArrayFiller = m_Stream.ReadUInt32();
		break;
	};
/*




	if (eSchemaDataType == eSchemaDataType_Object) {//maybe it means string?? or xml document???
		m_Stream.ReadByte();//6
		m_Stream.ReadInt32();//3
		CStringField* pValue = new CStringField;
		m_Stream.ReadString(pValue->Value());
		pField = pValue;
	} else if (eSchemaDataType == eSchemaDataType_StringArray) {
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
/*	} else {
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
			/*case eDataType_Int16:
				{
					CInt16Field* pIntField = new CInt16Field;
					pIntField->SetValue(m_Stream.ReadInt16());
					pField = pIntField;
				}
				break;
			case eDataType_Int32:
				{
					CInt32Field* pIntField = new CInt32Field;
					pIntField->SetValue(m_Stream.ReadInt32());
					pField = pIntField;
				}
				break;
			case eDataType_Int64:
				{
					CInt64Field* pIntField = new CInt64Field;
					pIntField->SetValue(m_Stream.ReadInt64());
					pField = pIntField;
				}
				break;
			case eDataType_DateTime:
				{
					CDateTimeField* pDateField = new CDateTimeField;
					pDateField->SetValue(m_Stream.ReadDouble());
					pField = pDateField;
				}
				break;
		}
	}*/

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
	m_Log << "Assembly(" << pAssembly->GetID() << ") " << pAssembly->GetName() << endl;
}

CDotNetClass* CDotNetFileReader::ReadArrayItem()
{
	CDotNetClass* pClass = new CDotNetClass;
	m_arCurrentClass.push_back(pClass);
	pClass->SetID(m_Stream.ReadUInt32());
	m_mpClasses.insert(std::make_pair(pClass->GetID(), pClass));
	UINT32 schmeaObjectID = m_Stream.ReadUInt32();
	auto pMapItem = m_mpClasses.find(schmeaObjectID);
	auto pRefClass = pMapItem->second;
	INT32 nFieldCount = pRefClass->GetFieldCount();
	pClass->SetSchemaReferenceClass(pRefClass);

	auto pSchemaTypes = GET_DATA(&pRefClass->SchemaTypes());
	auto pDataTypes = GET_DATA(&pRefClass->FieldTypes());
	auto pDataTypeCodes = GET_DATA(&pRefClass->FieldTypeCodes());
	auto pFieldNames = GET_DATA(&pRefClass->FieldNames());
	pClass->FieldValues().resize(nFieldCount, nullptr);
	auto ppFieldValues = GET_DATA(&pClass->FieldValues());

	for(int i = 0; i < nFieldCount; i++) {
		CDotNetField* pField = nullptr;
		if (pSchemaTypes[i] == eSchemaDataType_Primitive) {
			pField = ReadPrimative(pDataTypes[i]);
		} else {
			pField = ReadField();
		}
		if (pField == nullptr) throw std::exception("Field type is not supported!");
		{
			if (pSchemaTypes[i] == eSchemaDataType_Array) {
				m_Log << EnumToString(pDataTypes[i]) << "[] ";
			} else if (pSchemaTypes[i] == eSchemaDataType_Object ||
					   pSchemaTypes[i] == eSchemaDataType_SystemObject) {
				m_Log << pDataTypeCodes[i] << " ";
			} else if (pSchemaTypes[i] == eSchemaDataType_Primitive) {
				m_Log << EnumToString(pDataTypes[i]) << " ";
			} else if (pSchemaTypes[i] == eSchemaDataType_UserDefinedObject) {
				m_Log << pDataTypeCodes[i] << " ";
			} else {
				m_Log << EnumToString(pSchemaTypes[i]) << " " << EnumToString(pDataTypes[i]) << " ";
			}
			m_Log << pFieldNames[i] << " = " << pField->ToString() << ";" << endl; 
		}
		pField->SetName(pFieldNames[i]);
		pField->SetParent(pClass);
		ppFieldValues[i] = pField;
	}

	return pClass;
}

//may have to refactor to look like :
//https://tracemynet.svn.codeplex.com/svn/TraceMyNet/BinaryFormatReader.cs
//http://msdn.microsoft.com/en-us/library/cc237454.aspx info on values n what reading is done...
//http://primates.ximian.com/~lluis/dist/binary_serialization_format.htm#elem7
CDotNetClass* CDotNetFileReader::Deserialize()
{
	m_Log << "Deserialize Started..." << endl;
	bool bContinue = true;
	{
		BYTE junk[17] = {0};
		m_Stream.ReadBytes(junk);
	}
	try
	{
		string sName;
		while (bContinue)
		{
			ESchemaType eSchemaType = static_cast<ESchemaType>(m_Stream.ReadByte());

			m_Log << "Schema Type: " << EnumToString(eSchemaType) << endl;

			switch(eSchemaType)
			{
			case eSchemaType_ArrayItem:
				{
					ReadArrayItem();
				}
				break;
			case eSchemaType_BinaryArray:
				{
					ReadArray();
				}
				break;
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
						default:
							m_Log << "Unsupported Array Type " << EnumToString(type) << endl;
							break;
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