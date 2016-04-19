#include "symtable.h"

#include <iostream>

Symtable* Symtable::instance = 0;

Symtable::Symtable() {

}

Symtable* Symtable::getInstance() {
	if(instance == NULL) {
		instance = new Symtable();
	}
	return instance;
}

void Symtable::addVar(std::string name, int value) {
	this->table.insert(std::pair<std::string, int>(name, value));
//	this->table[name] = value;
}

int Symtable::getVar(std::string name) {
	for(std::map<std::string, int>::iterator it = table.begin(); it != table.end(); ++it) {
	  printf(" lala: %s", it->first.c_str());
	}
	printf("\n");
	printf("get %s = %d\n", name.c_str(), this->table[name]);
	return this->table[name];
}

void Symtable::setVar(std::string name, int newValue) {
//	this->table.find(name)->second = newValue;
	this->table[name] = newValue;
	printf("set %s = %d\n", name.c_str(), this->table[name]);
}

bool Symtable::hasVar(std::string name) {
	return this->table.find(name) != this->table.end();
}
