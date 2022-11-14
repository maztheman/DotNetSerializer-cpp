#pragma once

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <exception>

#include "types/TypeAliases.h"

#include <fmt/format.h>

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
std::shared_ptr<type> var;\
public:\
void Set ## name (const std::shared_ptr<type> & value) { var = value; }\
type* Get ## name () const { return var.get(); }
