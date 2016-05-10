#include "funtable.h"

using namespace FT;

/* FunTable */
Funtable::FunTable(){
}

void Funtable::addFunction(std::string name, std::list<Symbol>){

}

void Funtable::defFunction(std::string name, Node* exec){
	if(!hasFunction){
		addFunction(name, NULL);
	}
}

Function* Funtable::getFunction(std::string name){

}

bool Funtable::hasFunction(std::string name){
	return this->table.find(name) != this->table.end();
}