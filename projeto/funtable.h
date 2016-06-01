/* Function Table */
//declara as classes tabela de função e função
//a tabela de funções tem um mapa que armazena as funções de acordo com seu nome
//as funções sabem seu tipo de retorno e os atributos com os quais devem ser chamadas
//separada da symtable pois não é suscetível a escopo
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