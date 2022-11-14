#pragma once

class CBinaryStream;
class CDotNetClass;

std::shared_ptr<CDotNetClass> ReadClass(CBinaryStream& stream, bool bSystemClass/* = false*/);