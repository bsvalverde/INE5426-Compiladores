#include "funtable.h"

using namespace FT;

/* FunTable */
FunTable::FunTable(){
}

void FunTable::addFunction(std::string name, Function* newFunction){
	if(hasFunction(name)) {
		yyerror("semantico: funcao %s sofrendo redeclaracao.", name.c_str());
	} else {
		this->table[name] = newFunction;
	}
}

void FunTable::defFunction(std::string name, Function* newFunction){
	if(!hasFunction(name)){
		addFunction(name, newFunction);
	}
	if(this->table[name]->defined == true){
		yyerror("semantico: funcao %s sofrendo redefinicao.", name.c_str());
	}
	this->table[name]->defined = true;
}

Function* FunTable::getFunction(std::string name){
	//unused
}

bool FunTable::hasFunction(std::string name){
	return this->table.find(name) != this->table.end();
}