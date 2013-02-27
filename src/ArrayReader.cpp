#include "stdafx.h"
#include "ArrayReader.h"
#include "BinaryStream.h"

CArrayReader::CArrayReader(CBinaryStream& stream) : m_Stream(stream)
{
}


CArrayReader::~CArrayReader(void)
{
}


CArrayReader* CArrayReader::GetArrayReaderByType(EDataType eDataType, CBinaryStream& stream)
{
	switch(eDataType)
	{
	case eDataType_Int32:
		return new CInt32ArrayReader(stream);
		break;

	case eDataType_String:
		return new CStringArrayReader(stream);
		break;
	}

	throw std::exception("Array type not handled");
}

CInt32ArrayReader::CInt32ArrayReader(CBinaryStream& stream) : CArrayReader(stream)
{

}

void CInt32ArrayReader::SetField(CDotNetField* pField)
{
	m_pField = static_cast<CInt32ArrayField*>(pField);
}

void CInt32ArrayReader::Read()
{
	m_pField->Value().push_back(m_Stream.ReadInt32());
}

CStringArrayReader::CStringArrayReader(CBinaryStream& stream) : CArrayReader(stream)
{

}

void CStringArrayReader::SetField(CDotNetField* pField)
{
	m_pField = static_cast<CStringArrayField*>(pField);
}

void CStringArrayReader::Read()
{
	m_Stream.ReadByte();//next item is another string?
	m_Stream.ReadInt32(); // some sort if global id?
	string sValue;
	m_Stream.ReadString(sValue);
	m_pField->Value().push_back(sValue);
}