/* Symbol Table */
#pragma once

#include <map>
#include "enums.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

class SymTable {
public:
	SymTable();
	void addSymbol(std::string name);
	Symbol* getSymbol(std::string name);
	Symbol* useSymbol(std::string name);
	void setSymbol(std::string name);

private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol*> table;
};

class Symbol {
public:
	Symbol();
	Symbol(Type type) : type(type), initialized(false) {}
	void setType(Type type);

	Type type;
	bool initialized;
};

}

