#pragma once

#include "enums.h"
#include <iostream>

class Stringfier{
public:
	static std::string typeString(Type type, bool isArr);
	static std::string typeStringM(Type type);
	static std::string typeStringF(Type type);
	static std::string binOpString(BinOperation op);
	static std::string unOpString(UnOperation op);
};