#pragma once

#include <istream>
#include <string>
#include <vector>
#include <span>

using std::istream;
using std::string;
using std::vector;

class CBinaryStream
{
public:
	CBinaryStream(void);
	~CBinaryStream(void);

	void InitializeFromStream(istream& input);
	void ResetContent();
	
	//Reader stuff
	bool ReadBool();
	uint8_t ReadByte();

	template<size_t SIZE>
	void ReadBytes(uint8_t (&buffer)[SIZE])
	{
		ReadBytes(&buffer[0], SIZE);
	}

	void ReadBytes(uint8_t* ppBuffer, size_t nSize);
	char ReadChar();
	void ReadChars(std::string& ppBuffer, size_t nCount);
	//Supposed to be 16 byte QUAD-precision double.
	long double ReadDecimal();
	double ReadDouble();
	float ReadFloat();
	int16_t ReadInt16();
	int32_t ReadInt32();
	int64_t ReadInt64();
	int8_t ReadSByte();
	void ReadString(std::string& sValue);
	uint16_t ReadUInt16();
	uint32_t ReadUInt32();
	uint64_t ReadUInt64();

	std::span<uint8_t> Peek(size_t sz);

	inline bool IsEOF() 
	{ 
		if (m_nSize == -1 || m_nIndex == -1) return true;
		return m_nIndex >= m_nSize; 
	}

protected:

	template<typename T>
	void SafeReadIntoType(T* ptr)
	{
		static constexpr size_t sz = sizeof(T);
		memcpy(ptr, &m_arData[m_nIndex], sz);
		m_nIndex += sz;
	}

	int Read7BitInt();

private:
	CByteVector m_arData;
	size_t m_nIndex;
	size_t m_nSize;
};
