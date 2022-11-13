#include "UserClassField.h"

#include "DotNetClass.h"

string CUserClassField::do_ToString() const
{
	string sVal;
	std::stringstream ss;
	if (m_pClass) {
		ss << m_pClass->GetName();
	}
	sVal = ss.str();
	return std::move(sVal);
}