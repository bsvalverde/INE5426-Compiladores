#include "funtable.h"
#include "stringfier.h"

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
		yyerror("semantico: funcao %s sem declaracao.", name.c_str());
		return new Function();
	}
	return this->table[name];
}

Function* FunTable::useFunction(std::string name, std::vector<Type> arguments){
	Function* fun = this->getFunction(name);
	std::vector<ST::Symbol*> parameters = fun->parameters;
	int argsSize = arguments.size();
	int paramsSize = parameters.size();
	if(argsSize != paramsSize){
		yyerror("semantico: funcao %s espera %d parametros mas recebeu %d.", name.c_str(), paramsSize, argsSize);
	} else{
		for(int i = 0; i < paramsSize; i++){
			if(arguments[i] != parameters[(argsSize-1-i)]->type){
				yyerror("semantico: parametro espera %s mas recebeu %s.", Stringfier::typeStringM(parameters[(argsSize-1-i)]->type).c_str(), Stringfier::typeStringM(arguments[i]).c_str());
			}
		}
	}
	return fun;
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