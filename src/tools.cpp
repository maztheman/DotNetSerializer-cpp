#include "stdafx.h"
#include "tools.h"

INT32 ToInt32( BYTE* pBuffer )
{
	return ((INT32)pBuffer[0] | ((INT32)pBuffer[1] << 8) | ((INT32)pBuffer[2] << 16) | ((INT32)pBuffer[3] << 24));
}