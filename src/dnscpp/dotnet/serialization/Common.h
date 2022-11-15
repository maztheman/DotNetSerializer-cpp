#pragma once

class CBinaryStream;
class CDotNetClass;

void ReadCommon(CBinaryStream& stream, uint32_t& counter, std::shared_ptr<CDotNetClass>& pParent);