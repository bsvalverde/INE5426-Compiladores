/* Symbol Table */
#pragma once

#include <map>
#include "enums.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

class SymTable {
public:
	SymTable(SymTable* superScope);
	void addSymbol(std::string name);
	void addSymbol(std::string name, Type type);
	Symbol* getSymbol(std::string name);
	Symbol* useSymbol(std::string name);
	void setSymbol(std::string name);

	SymTable* superScope;
private:
	bool hasSymbol(std::string name);

	std::map<std::string, Symbol*> table;
};

class Symbol {
public:
	Symbol();
	Symbol(Type type) : type(type), initialized(false), arrSize(0) {}
	void setType(Type type);

	Type type;
	int arrSize;
	bool initialized;
};

}

