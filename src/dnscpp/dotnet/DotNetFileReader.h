#pragma once

#include "DotNetClass.h"
#include "DotNetField.h"
#include "DotNetAssembly.h"
#include "MemberReferenceField.h"

#include "types/DataType.h"

class CBinaryStream;

class CDotNetFileReader
{
public:
	CDotNetFileReader(CBinaryStream& stream);
	~CDotNetFileReader(void);
	//TODO: eventually return a special object that contains all the objects read in.
	std::shared_ptr<CDotNetClass> Deserialize();
	void GetLog(string& sLog) 
	{
		sLog = m_Log.str();
	}

	const CDotNetClassPtrVector& Types() const { return m_Types; }
protected:

	std::shared_ptr<CDotNetClass> 			ReadClass(bool bSystemClass);
	std::shared_ptr<CDotNetField> 			ReadField();
	void 									ReadEnd();
	void 									ReadAssembly();
	std::shared_ptr<CDotNetClass> 			ReadArray();
	std::shared_ptr<CDotNetClass> 			ReadArrayItem();
	std::shared_ptr<CMemberReferenceField> 	ReadMemberReference();
	std::shared_ptr<CDotNetField> 			ReadPrimative(EDataType eDataType);
private:

	CBinaryStream& m_Stream;
	CDotNetClassPtrMap m_mpClasses;
	CDotNetAssemblyPtrMap m_mpAssemblies;
	CMemberReferenceFieldPtrVector m_arNumber9;
	CDotNetClassPtrVector m_arCurrentClass;
	CDotNetClassPtrVector m_Types;
	std::stringstream m_Log;
};
