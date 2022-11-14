#include "DotNetFileReader.h"
#include "DotNetField.h"
#include "UserClassField.h"
#include "DotNetPrimitiveTypeField.h"
#include "FindDotNetAssembly.h"
#include "FindDotNetClass.h"

#include "types/SchemaType.h"
#include "types/ArrayType.h"

#include "utils/tools.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"

#include "serialization/Array.h"
#include "serialization/ArrayItem.h"
#include "serialization/ArrayOfString.h"
#include "serialization/ArrayFiller8b.h"
#include "serialization/Assembly.h"
#include "serialization/Field.h"
#include "serialization/Class.h"
#include "serialization/End.h"
#include "serialization/ArrayOfPrimitive.h"

#include <iostream>

CDotNetFileReader::CDotNetFileReader(CBinaryStream& stream)
: m_Stream(stream)
{
}

CDotNetFileReader::~CDotNetFileReader()
{
}

//may have to refactor to look like :
//https://tracemynet.svn.codeplex.com/svn/TraceMyNet/BinaryFormatReader.cs
//http://msdn.microsoft.com/en-us/library/cc237454.aspx info on values n what reading is done...
//http://primates.ximian.com/~lluis/dist/binary_serialization_format.htm#elem7
void CDotNetFileReader::Deserialize()
{
	m_Log << "Deserialize Started..." << std::endl;
	bool bContinue = true;
	{
		uint8_t junk[17] = {0};
		m_Stream.ReadBytes(junk);
		//signature here i will add check in later
	}
	try 
	{
		std::string sName;
		while (bContinue) 
		{
			ESchemaType eSchemaType = static_cast<ESchemaType>(m_Stream.ReadByte());
			m_Log << "Schema Type: " << EnumToString(eSchemaType) << std::endl;
			switch(eSchemaType)
			{
			default: m_Log << "Not Implemented Schema Type '" << EnumToString(eSchemaType) << "' (" << (int)eSchemaType << ")" << std::endl; break;
			case eSchemaType_ArrayItem:
				ReadArrayItem(m_Stream);
				break;
			case eSchemaType_BinaryArray:
				ReadArray(m_Stream);
				break;
			case eSchemaType_SystemClass:
				ReadClass(m_Stream, true);
				break;
			case eSchemaType_Class:
				ReadClass(m_Stream, false);
				break;
			case eSchemaType_End:
				ReadEnd();
				bContinue = false;
				break;
			case eSchemaType_ArrayOfString:
				ReadArrayOfString(m_Stream);
			break;
			case eSchemaType_ArrayFiller8b:
				ReadArrayFiller8b(m_Stream);
			break;
			case eSchemaType_ArrayOfPrimitiveType:
				ReadArrayOfPrimitive(m_Stream);
			break;
				//must mean assembly
			case eSchemaType_Assembly:
				ReadAssembly(m_Stream);
				break;
			}
		}
	} catch(std::exception& e) {
		std::cout << "Error caught! -->" << e.what() << "<--" << std::endl;
	}
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
