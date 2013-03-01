#ifndef __MEMBERREFERENCEFIELD_H__
#define __MEMBERREFERENCEFIELD_H__

#include "DotNetField.h"

class CMemberReferenceField : public CDotNetField
{
	PROPERTY(ReferenceID, UINT32, m_uiReferenceID)
public:
	CMemberReferenceField()
		: m_uiReferenceID(-1)
	{
	}
	CMemberReferenceField(UINT32 id)
		: m_uiReferenceID(id)
	{
	}

private:
	EDataType Type() const { return eDataType_UInt32; }
	string do_ToString() const { 
		std::stringstream ss;
		ss << m_uiReferenceID;
		string sVal = ss.str();
		return std::move(sVal);
	}
};

class CMemberReferenceFieldPtrVector : public std::vector<CMemberReferenceField*>
{
	PROPERTY(DeleteMemory, bool, m_bDeleteMemory)
public:
	CMemberReferenceFieldPtrVector(bool bDeleteMemory = true) 
		: m_bDeleteMemory(bDeleteMemory)
	{

	}

	~CMemberReferenceFieldPtrVector()
	{
		ResetContent();
	}

	void ResetContent()
	{
		if (m_bDeleteMemory) {
			if (!empty()) {
				auto ppData = data();
				for(auto i = 0UL; i < size(); i++) {
					delete ppData[i];
				}
			}
		}
		clear();
	}
};

#endif