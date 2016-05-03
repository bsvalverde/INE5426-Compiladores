#include "symtable.h"

using namespace ST;

/* SymTable */
SymTable::SymTable() {
	
}

void SymTable::addSymbol(std::string name) {
	if(hasSymbol(name)) {
		yyerror("ERRO: Variável %s já definida.\n", name.c_str());
	} else {
		this->table[name] = Symbol();
	}
}

Symbol SymTable::getSymbol(std::string name) {
	if(!hasSymbol(name)) {
		yyerror("ERRO: Variável %s não definida.\n", name.c_str());
		return Symbol();
	} else {
		return this->table[name];
	}
}

void SymTable::setSymbol(std::string name, Symbol newValue) {
	if(!hasSymbol(name)) {
		yyerror("ERRO: Variável %s não definida.\n", name.c_str());
	} else {
		this->table[name] = newValue;
	}
}

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

/* Symbol */
Symbol::Symbol() {
	this->type = inteiro; 
	this->value = 0;
}

void Symbol::setType(Type type) {
	this->type = type;
}