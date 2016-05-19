#include "symtable.h"

using namespace ST;

/* SymTable */
SymTable::SymTable(SymTable* superScope) {
	this->superScope = superScope;
}

void SymTable::addSymbol(std::string name) {
	if(hasSymbol(name)) {
		if(this->table[name]->arrSize == 0){
			yyerror("semantico: variavel %s sofrendo redefinicao.", name.c_str());
		} else{
			yyerror("semantico: arranjo %s sofrendo redefinicao.", name.c_str());
		}
	} else {
		this->table[name] = new Symbol();
		this->table[name]->initialized = false;
	}
}

void SymTable::addSymbol(std::string name, Type type) {
	this->addSymbol(name);
	this->table[name]->type = type;
	this->table[name]->initialized = true;
}

Symbol* SymTable::getSymbol(std::string name) {
	if(hasSymbol(name)) {
		return this->table[name];
	}

	if(this->superScope == NULL) {
		// if(!isArr){
		// 	yyerror("semantico: arranjo %s sem declaracao.", name.c_str());
		// } else{
		// 	yyerror("semantico: variavel %s sem declaracao.", name.c_str());
		// }
		return new Symbol();
	}

	return this->superScope->getSymbol(name);
}

Symbol* SymTable::useSymbol(std::string name) {
	Symbol* retorno = getSymbol(name);
	if(!retorno->initialized)
		yyerror("semantico: variavel %s nao inicializada.", name.c_str());
	return retorno;
}

void SymTable::setSymbol(std::string name) {
	if(hasSymbol(name)){
		this->table[name]->initialized = true;
	}
}

bool SymTable::hasSymbol(std::string name) {
	return this->table.find(name) != this->table.end();
}

/* Symbol */
Symbol::Symbol() {
	this->type = desconhecido;
	this->initialized = true;
}

void Symbol::setType(Type type) {
	this->type = type;
}
