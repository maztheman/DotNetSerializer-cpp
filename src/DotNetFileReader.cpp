#include "StdAfx.h"
#include "DotNetFileReader.h"
#include "SchemaType.h"
#include "ArrayType.h"
#include "BinaryStream.h"
#include "DotNetField.h"
#include "tools.h"
#include "UserClassField.h"

#include <iostream>

CDotNetFileReader::CDotNetFileReader(CBinaryStream& stream) : m_Stream(stream)
{
}

CDotNetFileReader::~CDotNetFileReader(void)
{
}

inline bool IsSystemClass(string& sClassName)
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
	UINT32 id = m_Stream.ReadUInt32();
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
	oClass.Fields().resize(nFieldCount);
	//Resize All Arrays
	oClass.FieldValues().resize(nFieldCount, nullptr);
	oClass.ArraySizes().resize(nFieldCount, -1);
	auto fields = GET_DATA(&oClass.Fields());
	CDotNetField** ppFieldValues = GET_DATA(&oClass.FieldValues());
	INT32* pArraySizes = GET_DATA(&oClass.ArraySizes());

	for(INT32 n = 0; n < nFieldCount; n++) {
		m_Stream.ReadString(fields[n].Name());
	}
	for(INT32 n = 0; n < nFieldCount; n++) {//Try to figure out the basic schema type
		fields[n].SetSchemaDataType(static_cast<ESchemaDataType>(m_Stream.ReadByte()));
	}
	for(INT32 n = 0; n < nFieldCount; n++) {
		CField& field = fields[n];
		ESchemaDataType eSchemaDataType = field.GetSchemaDataType();
		if (eSchemaDataType == eSchemaDataType_Primitive) {
			field.SetDataType(static_cast<EDataType>(m_Stream.ReadByte()));
		} else if (eSchemaDataType == eSchemaDataType_UserDefinedObject) {
			m_Stream.ReadString(field.TypeCode());
			field.SetAssemblyID(m_Stream.ReadUInt32());
		} else if (eSchemaDataType == eSchemaDataType_SystemObject) {
			m_Stream.ReadString(field.TypeCode());//looks like its fully qualified.
		} else if (eSchemaDataType == eSchemaDataType_Array) {
			field.SetDataType(static_cast<EDataType>(m_Stream.ReadByte()));
		}
	}
	if (!bSystemClass) {
		oClass.SetAssemblyID(m_Stream.ReadUInt32());
	}
	for(INT32 n = 0; n < nFieldCount; n++) {
		CDotNetField* pField = nullptr;
		CField& field = fields[n];
		if (field.GetSchemaDataType() == eSchemaDataType_Primitive) {
			pField = ReadPrimative(field.GetDataType());
		} else {
			pField = ReadField();
		}
		if (pField == nullptr) {
			throw std::exception("Field type is not supported!");
		}
		pField->SetName(field.GetName());
		pField->SetParent(pNewClass);
		ppFieldValues[n] = pField;
	}
	m_Log << "};" << endl;
	m_mpClasses.insert(std::make_pair(id, pNewClass));

	return pNewClass;
}

CMemberReferenceField* CDotNetFileReader::ReadMemberReference()
{
	CMemberReferenceField* pMemberRefField = new CMemberReferenceField(m_Stream.ReadUInt32());
	m_arNumber9.push_back(pMemberRefField);
	return pMemberRefField;
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
	CField& element = pArray->Element();
	element.SetSchemaDataType(eSchemaDataType);
	if (eSchemaDataType == eSchemaDataType_UserDefinedObject) {
		m_Stream.ReadString(element.TypeCode());
		element.SetAssemblyID(m_Stream.ReadUInt32());
	} else if (eSchemaDataType == eSchemaDataType_SystemObject) {
		m_Stream.ReadString(element.TypeCode());
	} else if (eSchemaDataType == eSchemaDataType_Primitive) {
		EDataType eType = static_cast<EDataType>(m_Stream.ReadByte());
		element.SetDataType(eType);
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
			ppFields[nField]->SetSchemaType(eSchemaType_MemberReference);
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
				auto pArray = ReadArray();
				p->SetClassObject(pArray);
				p->SetSchemaType(pArray->GetSchemaType());
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
			//this still needs to be tested...
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
	if (nArrayFiller > 0) {
		nArrayFiller--;
		return nullptr;
	}
	CDotNetField* pField = nullptr;
	ESchemaType eFieldType = static_cast<ESchemaType>(m_Stream.ReadByte());
	switch(eFieldType) {
	default: throw std::exception("Unhandled ReadField Type"); break;	
	//1,4,5,6,8,9,10,13,14
	case eSchemaType_ArrayItem:
		{
			CUserClassField* pClass = new CUserClassField;
			CDotNetClass* pSub = ReadArrayItem();
			pClass->SetClassObject(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_SystemClass:
		{
			CUserClassField* pClass = new CUserClassField;
			CDotNetClass* pSub = ReadClass(true);
			pClass->SetClassObject(pSub);
			pField = pClass;
		}
		break;
	case eSchemaType_Class:
		//value type only?
		{
			CUserClassField* pClass = new CUserClassField;
			CDotNetClass* pSub = ReadClass();
			pClass->SetClassObject(pSub);
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
	pField->SetSchemaType(eFieldType);
	return pField;
}

void CDotNetFileReader::ReadEnd()
{
	size_t nSize = m_arNumber9.size();
	if (nSize > 0) {
		auto pFields = GET_DATA(&m_arNumber9);
		string sName;
		sName.reserve(256);
		for(size_t i = 0; i < nSize; i++) {
			auto pField = pFields[i];
			CDotNetClass* pParent = pField->GetParent();
			CDotNetClassPtrMap::iterator itFound = m_mpClasses.find(pField->GetReferenceID());
			if (itFound != m_mpClasses.end()) {
				auto ppParentFieldValues = GET_DATA(&pParent->FieldValues());
				auto index = pParent->GetFieldValues().GetIndexByID(pField->GetReferenceID());
				sName = ppParentFieldValues[index]->GetName();
				delete ppParentFieldValues[index];
				CUserClassField* pNewClass = new CUserClassField(itFound->second);
				ppParentFieldValues[index] = pNewClass;
				pNewClass->SetSchemaType(pNewClass->GetClassObject()->GetSchemaType());
				pNewClass->SetParent(pParent);
				pNewClass->SetName(sName);
			}
		}
		m_arNumber9.clear();
	}
}

void CDotNetFileReader::ReadAssembly()
{
	UINT32 id = m_Stream.ReadUInt32();
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
	auto fields = GET_DATA(&pRefClass->Fields());
	pClass->FieldValues().resize(nFieldCount, nullptr);
	auto ppFieldValues = GET_DATA(&pClass->FieldValues());
	for(int i = 0; i < nFieldCount; i++) {
		CField& field = fields[i];
		CDotNetField* pField = nullptr;
		if (field.GetSchemaDataType() == eSchemaDataType_Primitive) {
			pField = ReadPrimative(field.GetDataType());
		} else {
			pField = ReadField();
		}
		if (pField == nullptr) throw std::exception("Field type is not supported!");
		pField->SetName(field.GetName());
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
		//signature here i will add check in later
	}
	try {
		string sName;
		while (bContinue) {
			ESchemaType eSchemaType = static_cast<ESchemaType>(m_Stream.ReadByte());
			m_Log << "Schema Type: " << EnumToString(eSchemaType) << endl;
			switch(eSchemaType)
			{
			default: m_Log << "Not Implemented Schema Type '" << EnumToString(eSchemaType) << "' (" << (int)eSchemaType << ")" << endl; break;
			case eSchemaType_ArrayItem:
				ReadArrayItem();
				break;
			case eSchemaType_BinaryArray:
				ReadArray();
				break;
			case eSchemaType_SystemClass:
				ReadClass(true);
				break;
			case eSchemaType_Class:
				ReadClass();
				break;
			case eSchemaType_End:
				ReadEnd();
				bContinue = false;
				break;
				//must mean assembly
			case eSchemaType_Assembly:
				ReadAssembly();
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

/*
unused code, prolly garbage:
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
*/