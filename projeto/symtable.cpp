#include "symtable.h"

using namespace ST;

/* SymTable */
SymTable::SymTable() {
}

void SymTable::addSymbol(std::string name) {
	if(hasSymbol(name)) {
		yyerror("Erro semantico: redefinicao da varivel %s.", name.c_str());
	} else {
		this->table[name] = new Symbol();
	}
}

Symbol* SymTable::getSymbol(std::string name) {
	if(!hasSymbol(name)) {
		yyerror("Erro semantico: variavel %s nao declarada.", name.c_str());
		return new Symbol();
	} else {
		return this->table[name];
	}
}

Symbol* SymTable::useSymbol(std::string name) {
	Symbol* retorno = getSymbol(name);
	if(!retorno->initialized)
		yyerror("Erro semantico: variavel %s nao inicializada.", name.c_str());
	return retorno;
}

void SymTable::setSymbol(std::string name) {
	this->table[name]->initialized = true;
}

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

/* Symbol */
Symbol::Symbol() {
	this->type = desconhecido;
	this->initialized = false;
}

void Symbol::setType(Type type) {
	this->type = type;
}
