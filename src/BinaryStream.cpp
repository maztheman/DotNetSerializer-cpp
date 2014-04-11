#include "StdAfx.h"
#include "BinaryStream.h"

const int ciMaxShift = 35;

CBinaryStream::CBinaryStream(void) : m_nIndex(-1), m_nSize(-1)
{
}

CBinaryStream::~CBinaryStream(void)
{
	ResetContent();
}

void CBinaryStream::ResetContent()
{
	m_arData.clear();
	CByteVector().swap(m_arData);
	m_nIndex = -1;
	m_nSize = -1;
}

void CBinaryStream::InitializeFromStream(istream& input)
{
	ResetContent();
	m_nSize = static_cast<size_t>(input.seekg(0, std::ios_base::end).tellg());
	m_arData.resize(m_nSize, 0);
	input.seekg(0, std::ios_base::beg).read(reinterpret_cast<char*>(&m_arData[0]), m_nSize);
	m_nIndex = 0;
}

bool CBinaryStream::ReadBool()
{
	return m_arData[m_nIndex++] != 0;
}

BYTE CBinaryStream::ReadByte()
{
	return m_arData[m_nIndex++];
}

void CBinaryStream::ReadBytes(BYTE* pBuffer, size_t nSize)
{
	memcpy(&pBuffer[0], &m_arData[m_nIndex], nSize);
	m_nIndex += nSize;
}


char CBinaryStream::ReadChar()
{
	return static_cast<char>(m_arData[m_nIndex++]);
}

void CBinaryStream::ReadChars(string& sBuffer, size_t nCount)
{
	size_t nBufferIndex = 0;
	CSByteVector arChars;
	arChars.reserve(nCount * 3);
	if (m_arData[m_nIndex + 0] == 0xEF &&
		m_arData[m_nIndex + 1] == 0xBB &&
		m_arData[m_nIndex + 2] == 0xBF) {
		m_nIndex += 3, nCount -= 3;
	}
	//Assume UTF-8 Encoding
	while(m_nIndex < m_nSize)
	{//Break out when finished
		BYTE b1 = m_arData[m_nIndex++];
		arChars.push_back(static_cast<char>(b1));
		if (b1 & 0x80) {
			int nCharCount = (b1 & 0xF0) >> 4;
			if (nCharCount & 0xC) {
				arChars.push_back((char)m_arData[m_nIndex++]);
			} else if (nCharCount & 0xE) {
				arChars.push_back((char)m_arData[m_nIndex++]);
				arChars.push_back((char)m_arData[m_nIndex++]);
			} else {
				throw new std::exception("Error, Unhandled UTF-8 Byte Count");
			}
		}
		if (--nCount == 0) break;
	}
	sBuffer.assign(arChars.begin(), arChars.end());
}

union long_double_u
{
	BYTE buffer[16];
	long double value;
};

union double_u
{
	BYTE buffer[8];
	double value;
};

union float_u
{
	BYTE buffer[4];
	float value;
};

union int16_u
{
	BYTE buffer[2];
	int16_t value;
};

union int32_u
{
	BYTE buffer[4];
	int32_t value;
};

union int64_u
{
	BYTE buffer[8];
	int64_t value;
};

union uint16_u
{
	BYTE buffer[2];
	uint16_t value;
};

union uint32_u
{
	BYTE buffer[4];
	uint32_t value;
};

union uint64_u
{
	BYTE buffer[8];
	uint64_t value;
};

long double CBinaryStream::ReadDecimal()
{
	int nSize = sizeof(long double);
	long_double_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

double CBinaryStream::ReadDouble()
{
	double_u value;
	ReadIntoBuffer(value.buffer);
	return value.value;
}

float CBinaryStream::ReadFloat()
{
	float_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

INT16 CBinaryStream::ReadInt16()
{
	int16_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

INT32 CBinaryStream::ReadInt32()
{
	int32_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

INT64 CBinaryStream::ReadInt64()
{
	int64_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

SBYTE CBinaryStream::ReadSByte()
{
	return static_cast<SBYTE>(m_arData[m_nIndex++]);
}

void CBinaryStream::ReadString(string& sValue)
{
	int nCount = Read7BitInt();
	ReadChars(sValue, nCount);
}

//http://msdn.microsoft.com/en-us/library/system.io.binarywriter.write7bitencodedint.aspx
int CBinaryStream::Read7BitInt()
{
	int nShift = 0;
	int nValue = 0;
	while(nShift != ciMaxShift)//7 loops max
	{
		int nTemp = ReadByte();
		nValue |= (nTemp & 0x7F) << nShift;
		if ((nTemp & 0x80) == 0)
		{
			return nValue;
		}
		nShift += 7;
	}

	throw new std::exception("7bit Encoded int Encoded incorrectly");
}

UINT16 CBinaryStream::ReadUInt16()
{
	uint16_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

UINT32 CBinaryStream::ReadUInt32()
{
	uint32_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}

UINT64 CBinaryStream::ReadUInt64()
{
	uint64_u val;
	ReadIntoBuffer(val.buffer);
	return val.value;
}


