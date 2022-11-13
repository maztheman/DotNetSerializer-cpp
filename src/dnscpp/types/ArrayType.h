#ifndef __ARRAY_TYPE_H__
#define __ARRAY_TYPE_H__

enum EArrayType {
	eArrayType_Invalid				=   -1,
	eArrayType_SingleDimension		= 0x00,
	eArrayType_Jagged				= 0x01,
	eArrayType_MultiDimension		= 0x02,
};

#endif