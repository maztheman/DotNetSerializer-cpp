#pragma once

using std::string;
using std::map;

class CDotNetClass;

class CDotNetAssembly
{
	PROPERTY(ID, int32_t, m_nID)
	PROPERTY(Name, std::string, m_sName)
	PROPERTY(Classes, std::vector<std::shared_ptr<CDotNetClass>>, m_arClasses)
public:
	CDotNetAssembly(void);
	~CDotNetAssembly(void);
};

using CDotNetAssemblyPtrMap = std::unordered_map<int32_t, std::shared_ptr<CDotNetAssembly>>;
