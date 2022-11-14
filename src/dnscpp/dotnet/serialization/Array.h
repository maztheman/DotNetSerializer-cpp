#pragma once

class CDotNetClass;
class CBinaryStream;

std::shared_ptr<CDotNetClass> ReadArray(CBinaryStream& stream);