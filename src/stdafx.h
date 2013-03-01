// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"

#include <cstdio>
#include <tchar.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

typedef unsigned char BYTE, *LPBYTE;
typedef signed char SBYTE, *LPSBYTE;

typedef signed short int INT16, *LPINT16;
typedef unsigned short int UINT16, *LPUINT16;

typedef signed int INT32, *LPINT32;
typedef unsigned int UINT32, *LPUINT32;

typedef signed long long int INT64, *LPINT64;
typedef unsigned long long int UINT64, *LPUINT64;

typedef std::vector<BYTE> CByteVector;
typedef std::vector<SBYTE> CSByteVector;
typedef std::vector<INT32> CInt32Vector;
typedef std::vector<std::string> CStringVector;

#define PROPERTY(name, type, var)\
private:\
type var;\
public:\
type& name () { return var; }\
const type& Get ## name () const { return var; }\
void Set ## name (type value) { var = value; }

#define PROPERTY_OBJ(name, type, var)\
private:\
type var;\
public:\
type& name () { return var; }\
const type& Get ## name () const { return var; }

#define PROPERTY_PTR(name, type, var)\
private:\
type var;\
public:\
void Set ## name (type value) { var = value; }\
type Get ## name () const { return var; }
