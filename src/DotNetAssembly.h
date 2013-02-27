#pragma once

using std::string;
using std::map;

class CDotNetAssembly
{
	PROPERTY(ID, INT32, m_nID)
	PROPERTY(Name, string, m_sName)

public:
	CDotNetAssembly(void);
	~CDotNetAssembly(void);
};

class CDotNetAssemblyPtrMap : public map<INT32, CDotNetAssembly*>
{
public:
	CDotNetAssemblyPtrMap() {}
	~CDotNetAssemblyPtrMap()
	{
		ResetContent();
	}
	void ResetContent()
	{
		while(!empty()) {
			iterator it = begin();
			CDotNetAssembly* pObject = it->second;
			delete pObject;
			erase(it);
		}
	}
};