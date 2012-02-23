#include "StdAfx.h"
#include "BinaryStream.h"

#include <vector>
using namespace std;

const int ciMaxShift = 35;

CBinaryStream::CBinaryStream(void) : m_pData(0), m_nIndex(-1), m_nSize(-1)
{
}

CBinaryStream::~CBinaryStream(void)
{
	ResetContent();
}

void CBinaryStream::ResetContent()
{
	if ( m_pData != 0 )
		delete [] m_pData;
	m_pData = 0;
	m_nIndex = -1;
	m_nSize = -1;
}

void CBinaryStream::InitializeFromStream(istream& input)
{
	ResetContent();
	m_nSize = input.seekg( 0, ios_base::end ).tellg();
	m_pData = new BYTE[m_nSize];
	input.seekg( 0, ios_base::beg ).read( (char*)&m_pData[0], m_nSize );
	m_nIndex = 0;
}

bool CBinaryStream::ReadBool()
{
	return m_pData[m_nIndex++] != 0;
}

BYTE CBinaryStream::ReadByte()
{
	return m_pData[m_nIndex++];
}

void CBinaryStream::ReadBytes( BYTE* pBuffer, size_t nSize )
{
	memcpy( &pBuffer[0], &m_pData[m_nIndex], nSize );
	m_nIndex += nSize;
}


char CBinaryStream::ReadChar()
{
	return (char)m_pData[m_nIndex++];
}

void CBinaryStream::ReadChars(string& sBuffer, size_t nCount)
{
	size_t nBufferIndex = 0;
	vector<char> arChars;
	arChars.reserve( nCount * 3 );
	//Assume UTF-8 Encoding
	while(m_nIndex < m_nSize)
	{//Break out when finished
		BYTE b1 = m_pData[m_nIndex++];
	
		arChars.push_back( (char)b1 );

		if ( b1 & 0x80 )
		{
			int nCharCount = (b1 & 0xF0) >> 4;
			if ( nCharCount & 0xC )
			{
				arChars.push_back( (char)m_pData[m_nIndex++] );
			}
			else if ( nCharCount & 0xE )
			{
				arChars.push_back( (char)m_pData[m_nIndex++] );
				arChars.push_back( (char)m_pData[m_nIndex++] );
			}
			else
			{
				throw new std::exception("Error, Unhandled UTF-8 Byte Count");
			}
		}

		if ( --nCount == 0 ) break;
	}

	sBuffer.assign( arChars.begin(), arChars.end() );
}

long double CBinaryStream::ReadDecimal()
{
	int nSize = sizeof(long double);
	BYTE buffer[16] = {0};
	ReadIntoBuffer( buffer );
	return *reinterpret_cast<long double * const>(&buffer[16 - nSize]);
}

double CBinaryStream::ReadDouble()
{
	BYTE buffer[8] = {0};
	ReadIntoBuffer( buffer );
	UINT32 n1 = ((INT32)buffer[0] | ((INT32)buffer[1] << 8) | ((INT32)buffer[2] << 16) | ((INT32)buffer[3] << 24));
	UINT32 n2 = ((INT32)buffer[4] | ((INT32)buffer[5] << 8) | ((INT32)buffer[6] << 16) | ((INT32)buffer[7] << 24));
	UINT64 n3 = (INT64)n1 | ((UINT64)n1 << 32 );
	return *((double*)&n3);
}

float CBinaryStream::ReadFloat()
{
	BYTE buffer[4] = {0};
	ReadIntoBuffer( buffer );
	UINT32 n1 = ((INT32)buffer[0] | ((INT32)buffer[1] << 8) | ((INT32)buffer[2] << 16) | ((INT32)buffer[3] << 24));
	return *((float*)&n1);
}

INT16 CBinaryStream::ReadInt16()
{
	BYTE buffer[2] = {0};
	ReadIntoBuffer( buffer );
	return (INT32)buffer[0] | ((INT32)buffer[1] << 8);
}

INT32 CBinaryStream::ReadInt32()
{
	BYTE buffer[4] = {0};
	ReadIntoBuffer( buffer );
	return ((INT32)buffer[0] | ((INT32)buffer[1] << 8) | ((INT32)buffer[2] << 16) | ((INT32)buffer[3] << 24));
}

INT64 CBinaryStream::ReadInt64()
{
	BYTE buffer[8] = {0};
	ReadIntoBuffer( buffer );
	UINT32 n1 = ((INT32)buffer[0] | ((INT32)buffer[1] << 8) | ((INT32)buffer[2] << 16) | ((INT32)buffer[3] << 24));
	UINT32 n2 = ((INT32)buffer[4] | ((INT32)buffer[5] << 8) | ((INT32)buffer[6] << 16) | ((INT32)buffer[7] << 24));
	return (INT64)n1 | ((UINT64)n1 << 32 );
}

SBYTE CBinaryStream::ReadSByte()
{
	return (SBYTE)m_pData[m_nIndex++];
}

void CBinaryStream::ReadString(string& sValue)
{
	int nCount = Read7BitInt();
	ReadChars( sValue, nCount );
}

//http://msdn.microsoft.com/en-us/library/system.io.binarywriter.write7bitencodedint.aspx
int CBinaryStream::Read7BitInt()
{
	int nShift = 0;
	int nValue = 0;
	while(nShift != ciMaxShift )//7 loops max
	{
		int nTemp = ReadByte();
		nValue |= nTemp << nShift;
		if ( (nTemp & 0x80) == 0)
		{
			return nValue;
		}
		nShift += 7;
	}

	throw new std::exception("7bit Encoded int Encoded incorrectly");
}

UINT16 CBinaryStream::ReadUInt16()
{
	BYTE buffer[2] = {0};
	ReadIntoBuffer( buffer );
	return (UINT16)buffer[0] | ((UINT16)buffer[1] << 8);
}

UINT32 CBinaryStream::ReadUInt32()
{
	BYTE buffer[4] = {0};
	ReadIntoBuffer( buffer );
	return ((UINT32)buffer[0] | ((UINT32)buffer[1] << 8) | ((UINT32)buffer[2] << 16) | ((UINT32)buffer[3] << 24));
}

UINT64 CBinaryStream::ReadUInt64()
{
	BYTE buffer[8] = {0};
	ReadIntoBuffer( buffer );
	UINT32 n1 = ((INT32)buffer[0] | ((INT32)buffer[1] << 8) | ((INT32)buffer[2] << 16) | ((INT32)buffer[3] << 24));
	UINT32 n2 = ((INT32)buffer[4] | ((INT32)buffer[5] << 8) | ((INT32)buffer[6] << 16) | ((INT32)buffer[7] << 24));
	return (UINT64)n1 | ((UINT64)n1 << 32 );
}


