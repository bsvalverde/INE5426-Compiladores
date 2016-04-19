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

void Symtable::addVar(const char* name, int value) {
	this->table.insert(std::pair<const char*, int>(name, value));
//	this->table[name] = value;
}

int Symtable::getVar(const char* name) {
	for(std::map<const char*, int>::iterator it = table.begin(); it != table.end(); ++it) {
	  printf(" lala: %s", it->first);
	}
	printf("\n");
	printf("get %s = %d\n", name, this->table[name]);
	return this->table[name];
}

void Symtable::setVar(const char* name, int newValue) {
//	this->table.find(name)->second = newValue;
	this->table[name] = newValue;
	printf("set %s = %d\n", name, this->table[name]);
}

bool Symtable::hasVar(const char* name) {
	return this->table.find(name) != this->table.end();
}
