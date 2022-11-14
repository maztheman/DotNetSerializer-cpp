#pragma once

#include "DotNetClass.h"
#include "DotNetField.h"
#include "DotNetAssembly.h"
#include "MemberReferenceField.h"

#include "types/DataType.h"

#include <span>

class CBinaryStream;

class CDotNetFileReader
{
public:
	CDotNetFileReader(CBinaryStream& stream);
	~CDotNetFileReader();
	void Deserialize();
	void GetLog(string& sLog) 
	{
		sLog = m_Log.str();
	}
private:

	CBinaryStream& m_Stream;
	std::stringstream m_Log;
};
