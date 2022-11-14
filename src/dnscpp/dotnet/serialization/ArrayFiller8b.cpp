#include "ArrayFiller8b.h"

#include "core/BinaryStream.h"

void ReadArrayFiller8b(CBinaryStream& stream)
{
    auto peeks = stream.Peek(30);
    auto fills = stream.ReadByte();
    return;
}