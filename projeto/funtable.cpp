#include "funtable.h"

using namespace FT;

/* FunTable */
FunTable::FunTable(){
}

void FunTable::addFunction(std::string name, std::list<ST::Symbol>){

}

void FunTable::defFunction(std::string name, AST::Node* exec){
	if(!hasFunction(name)){
		//addFunction(name, NULL);
	}
}

Function* FunTable::getFunction(std::string name){

}

bool FunTable::hasFunction(std::string name){
	return this->table.find(name) != this->table.end();
}