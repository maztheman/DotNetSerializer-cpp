#pragma once
#include "DataType.h"

INT32 ToInt32(BYTE* pBuffer);

#if _MSC_VER >= 1400
#define GET_DATA(x) ((*(x)).data());
#else
#define GET_DATA(x) &((*(x)).front());
#endif