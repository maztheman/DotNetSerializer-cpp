#pragma once

#include "DataType.h"
#include "DotNetClass.h"
#include "DotNetField.h"
#include "DotNetAssembly.h"
#include "MemberReferenceField.h"

class CBinaryStream;


using std::vector;
using std::map;
using std::stringstream;
using std::endl;

class CDotNetFileReader
{
public:
	CDotNetFileReader(CBinaryStream& stream);
	~CDotNetFileReader(void);
	//TODO: eventually return a special object that contains all the objects read in.
	CDotNetClass* Deserialize();
	void GetLog(string& sLog) {
		sLog = m_Log.str();
	}
protected:

	CDotNetClass* ReadClass(bool bSystemClass);
	CDotNetField* ReadField();
	void ReadEnd();
	void ReadAssembly();
	CDotNetClass* ReadArray();
	CDotNetClass* ReadArrayItem();
	CMemberReferenceField* ReadMemberReference();
	CDotNetField* ReadPrimative(EDataType eDataType);
private:

	CBinaryStream& m_Stream;
	CDotNetClassPtrMap m_mpClasses;
	CDotNetAssemblyPtrMap m_mpAssemblies;
	CMemberReferenceFieldPtrVector m_arNumber9;
	vector<CDotNetClass*> m_arCurrentClass;
	stringstream m_Log;
};
