#pragma once

class CBinaryStream;
class CMemberReferenceField;

std::shared_ptr<CMemberReferenceField> ReadMemberReference(CBinaryStream& stream);