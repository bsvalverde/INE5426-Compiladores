/* Function Table */
#pragma once

#include <map>
#include <list>
#include "symtable.h"
#include "enums.h"
#include "ast.h"

extern void yyerror(const char* s, ...);

namespace FT{

class Function;

class FunTable {
public:
	FunTable();
	void addFunction(std::string name, std::list<ST::Symbol>);
	void defFunction(std::string name, AST::Node* exec);
	Function* getFunction(std::string name);

private:
	bool hasFunction(std::string name);
	std::map<std::string, Function*> table;
};

class Function {
public:
	Function();
	std::list<ST::Symbol> parameters;
	AST::Node* execution;
};

}