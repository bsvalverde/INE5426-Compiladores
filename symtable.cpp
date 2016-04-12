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

void Symtable::addVar(char* name, int value) {
	this->table.insert(std::pair<char*, int>(name, value));
}

int Symtable::getVar(char* name) {
	return this->table[name];
}

void Symtable::setVar(char* name, int newValue) {
	this->table[name] = newValue;
}

bool Symtable::hasVar(char* name) {
	return !(this->table.find(name) == this->table.end());
}