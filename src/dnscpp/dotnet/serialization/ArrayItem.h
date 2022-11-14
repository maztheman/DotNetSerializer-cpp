#pragma once

class CBinaryStream;
class CDotNetClass;

std::shared_ptr<CDotNetClass> ReadArrayItem(CBinaryStream& stream);