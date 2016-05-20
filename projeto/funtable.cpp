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
	if(!hasFunction(name)){
		//TODO fix error message
		yyerror("semantico: funcao %s nao declarada.", name.c_str());
	}
	return this->table[name];
}

bool FunTable::hasFunction(std::string name){
	return this->table.find(name) != this->table.end();
}
void FunTable::checkDefinitions(){
	std::map<std::string, Function*>::iterator it = table.begin();
	while(it != table.end()){
		if(!table[it->first]->defined){
			yyerror("semantico: funcao %s declarada mas nunca definida.", it->first.c_str());
		}
		it++;
	}
}