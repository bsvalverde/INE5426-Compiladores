/* Function Table */
#pragma once

#include <map>
#include <vector>
#include "symtable.h"
#include "enums.h"

extern void yyerror(const char* s, ...);

namespace FT{

class Function;

class FunTable {
public:
	FunTable();
	void addFunction(std::string name, Function* newFunction);
	void defFunction(std::string name, Function* newFunction);
	Function* getFunction(std::string name);
	Function* useFunction(std::string name, std::vector<ST::Symbol*> arguments);
	//metodo para verificar funcao (numero certo de parametros)
	void checkDefinitions();

private:
	bool hasFunction(std::string name);
	std::map<std::string, Function*> table;
};

class Function {
public:
	Function(Type returnType, std::vector<ST::Symbol*> parameters) : returnType(returnType), parameters(parameters), defined(false) {};
	Function() : returnType(desconhecido), defined(true) {};
	Type returnType;
	std::vector<ST::Symbol*> parameters;
	bool defined;
};

}