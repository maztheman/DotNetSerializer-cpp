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

uint8_t CBinaryStream::ReadByte()
{
	return m_arData[m_nIndex++];
}

void CBinaryStream::ReadBytes(uint8_t* pBuffer, size_t nSize)
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
		uint8_t b1 = m_arData[m_nIndex++];
		arChars.push_back(static_cast<char>(b1));
		if (b1 & 0x80) {
			int nCharCount = (b1 & 0xF0) >> 4;
			if (nCharCount & 0xC) {
				arChars.push_back((char)m_arData[m_nIndex++]);
			} else if (nCharCount & 0xE) {
				arChars.push_back((char)m_arData[m_nIndex++]);
				arChars.push_back((char)m_arData[m_nIndex++]);
			} else {
				throw std::runtime_error("Error, Unhandled UTF-8 Byte Count");
			}
		}
		if (--nCount == 0) break;
	}
	sBuffer.assign(arChars.begin(), arChars.end());
}

long double CBinaryStream::ReadDecimal()
{
	long double val{0.0};
	SafeReadIntoType(&val);
	return val;
}

double CBinaryStream::ReadDouble()
{
	double val{0.0};
	SafeReadIntoType(&val);
	return val;
}

float CBinaryStream::ReadFloat()
{
	float val{0.0f};
	SafeReadIntoType(&val);
	return val;
}

int16_t CBinaryStream::ReadInt16()
{
	int16_t val = {0};
	SafeReadIntoType(&val);
	return val;
}

int32_t CBinaryStream::ReadInt32()
{
	int32_t val = {0};
	SafeReadIntoType(&val);
	return val;
}

int64_t CBinaryStream::ReadInt64()
{
	int64_t val = {0};
	SafeReadIntoType(&val);
	return val;
}

int8_t CBinaryStream::ReadSByte()
{
	return static_cast<int8_t>(m_arData[m_nIndex++]);
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

	throw std::runtime_error("7bit Encoded int Encoded incorrectly");
}

uint16_t CBinaryStream::ReadUInt16()
{
	uint16_t val = {0};
	SafeReadIntoType(&val);
	return val;
}

uint32_t CBinaryStream::ReadUInt32()
{
	uint32_t val = {0};
	SafeReadIntoType(&val);
	return val;
}

uint64_t CBinaryStream::ReadUInt64()
{
	uint64_t val = {0};
	SafeReadIntoType(&val);
	return val;
}


std::span<uint8_t> CBinaryStream::Peek(size_t sz)
{
	if ((m_nIndex + sz) >= m_nSize)
	{
		sz = m_nSize - m_nIndex;
	}

	return std::span<uint8_t>(m_arData.data(), m_arData.size()).subspan(m_nIndex, sz);
}

