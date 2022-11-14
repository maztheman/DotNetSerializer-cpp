#include "MemberReference.h"

#include "core/BinaryStream.h"
#include "core/DataIsland.h"
#include "dotnet/MemberReferenceField.h"


std::shared_ptr<CMemberReferenceField> ReadMemberReference(CBinaryStream& stream)
{
	auto pMemberRefField = std::make_shared<CMemberReferenceField>(stream.ReadUInt32());
	
    GDataIsland<CMemberReferenceField>().Data.push_back(pMemberRefField);

	return pMemberRefField;
}