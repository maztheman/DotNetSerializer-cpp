#include "End.h"

#include "core/DataIsland.h"

#include "dotnet/MemberReferenceField.h"
#include "dotnet/UserClassField.h"
#include "dotnet/FindDotNetClass.h"
#include "dotnet/DotNetClass.h"




void ReadEnd()
{
    auto& number9 = GDataIsland<CMemberReferenceField>().Data;

	if (number9.empty())
	{
		return;
	}

	std::string sName;
	sName.reserve(256);
	for(auto& pField : number9)
	{
		auto pParent = pField->GetParent();
		
		if (auto pRefClass = FindDotNetClassByID(pField->GetReferenceID()); pRefClass)
		{
			auto ppParentFieldValues = pParent->FieldValues().data();
			auto index = GetIndexByID(pParent->GetFieldValues(), pField->GetReferenceID());
			sName = ppParentFieldValues[index]->GetName();
			//not exactly sure why we copy and remove the old one..?
			auto pNewClass = std::make_shared<CUserClassField>(pRefClass);
			ppParentFieldValues[index] = pNewClass;
			pNewClass->SetSchemaType(pNewClass->GetClassObject()->GetSchemaType());
			pNewClass->SetParent(pParent);
			pNewClass->SetName(sName);
		}
	}
	number9.clear();
	
}