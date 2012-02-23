#pragma once

#include <istream>
#include <string>
using namespace std;

class CBinaryStream
{
public:
	CBinaryStream(void);
	~CBinaryStream(void);

	void InitializeFromStream(istream& input);
	void ResetContent();
	
	//Reader stuff
	bool ReadBool();
	BYTE ReadByte();

	template<size_t SIZE>
	void ReadBytes( BYTE (&buffer)[SIZE] )
	{
		ReadBytes( &buffer[0], SIZE );
	}

	void ReadBytes( BYTE* ppBuffer, size_t nSize );
	char ReadChar();
	void ReadChars(string& ppBuffer, size_t nCount);
	//Supposed to be 16 byte QUAD-precision double.
	long double ReadDecimal();
	double ReadDouble();
	float ReadFloat();
	INT16 ReadInt16();
	INT32 ReadInt32();
	INT64 ReadInt64();
	SBYTE ReadSByte();
	void ReadString(string& sValue);
	UINT16 ReadUInt16();
	UINT32 ReadUInt32();
	UINT64 ReadUInt64();

	inline bool IsEOF() 
	{ 
		if ( m_nSize == -1 || m_nIndex == -1) return true;
		return m_nIndex >= m_nSize; 
	}

protected:

	template<size_t SIZE>
	void ReadIntoBuffer( BYTE (&buffer)[SIZE] )
	{
		memcpy( &buffer[0], &m_pData[m_nIndex], SIZE );
		m_nIndex += SIZE;
	}

	int Read7BitInt();

private:
	LPBYTE m_pData;
	size_t m_nIndex;
	size_t m_nSize;
};
