/* Symbol Table */
#pragma once

#include <map>
#include "types.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

class SymTable {
public:
	SymTable();
	void addSymbol(std::string name);
	Symbol getSymbol(std::string name);
	void setSymbol(std::string name, Symbol newValue);

private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol> table;
};

class Symbol {
public:
	Symbol();
	Symbol(Type type) : type(type) {}
	void setType(Type type);

	Type type;
};

}

