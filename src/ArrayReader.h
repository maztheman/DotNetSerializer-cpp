#pragma once
#include "DotNetField.h"

class CBinaryStream;

class CArrayReader
{
public:
	CArrayReader(CBinaryStream& stream);
	virtual ~CArrayReader(void);

	static CArrayReader* GetArrayReaderByType( EDataType eDataType, CBinaryStream& stream );

	virtual void SetField( CDotNetField* pField ) = 0;
	virtual void Read() = 0;

protected:

	CBinaryStream& m_Stream;
};

class CInt32ArrayReader : public CArrayReader
{
public:
	CInt32ArrayReader(CBinaryStream& stream);
	void SetField( CDotNetField* pField );
	void Read();

private:

	CInt32ArrayField* m_pField;
};

class CStringArrayReader : public CArrayReader
{
public:
	CStringArrayReader(CBinaryStream& stream);
	void SetField( CDotNetField* pField );
	void Read();

private:

	CStringArrayField* m_pField;
};