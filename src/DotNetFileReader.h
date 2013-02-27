#pragma once

#include "DataType.h"
#include "DotNetClass.h"
#include "DotNetField.h"
#include "DotNetAssembly.h"

class CBinaryStream;

using std::vector;
using std::map;

class CDotNetFileReader
{
public:
	CDotNetFileReader(CBinaryStream& stream);
	~CDotNetFileReader(void);
	//TODO: eventually return a special object that contains all the objects read in.
	CDotNetClass* Deserialize();

protected:

	void ReadClass(bool bSystemClass);
	CDotNetField* ReadField(ESchemaDataType eSchemaDataType,EDataType eDataType);
	void ReadEnd();
	void ReadAssembly();

private:

	CBinaryStream& m_Stream;
	CDotNetClassPtrMap m_mpClasses;
	CDotNetAssemblyPtrMap m_mpAssemblies;
	CDotNetFieldPtrVector m_arNumber9;
	vector<CDotNetClass*> m_arCurrentClass;
};
