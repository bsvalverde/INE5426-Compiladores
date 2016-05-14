/* Function Table */
#pragma once

#include <map>
#include <vector>
#include "symtable.h"
#include "enums.h"
#include "ast.h"

extern void yyerror(const char* s, ...);

namespace FT{

class Function;

class FunTable {
public:
	FunTable();
	void addFunction(std::string name, Function* newFunction);
	void defFunction(std::string name, Function* newFunction);
	Function* getFunction(std::string name);
	//metodo para verificar funcao (numero certo de parametros)

private:
	bool hasFunction(std::string name);
	std::map<std::string, Function*> table;
};

class Function {
public:
	Function(Type returnType, std::vector<ST::Symbol*> parameters) : returnType(returnType), parameters(parameters), defined(false) {};
	Type returnType;
	std::vector<ST::Symbol*> parameters;
	bool defined;
};

}