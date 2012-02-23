#pragma once

#include <string>
#include <map>
using namespace std;

class CDotNetAssembly
{
public:
	CDotNetAssembly(void);
	~CDotNetAssembly(void);

	INT32& ID() { return m_nID; }
	string& Name() { return m_sName; }

private:
	string m_sName;
	INT32 m_nID;
};

class CDotNetAssemblyPtrMap : public map< INT32, CDotNetAssembly*>
{
public:
	CDotNetAssemblyPtrMap() {}
	~CDotNetAssemblyPtrMap()
	{
		ResetContent();
	}

	void ResetContent()
	{
		while(!empty() )
		{
			iterator it = begin();
			CDotNetAssembly* pObject = it->second;
			delete pObject;
			erase( it );
		}
	}
};