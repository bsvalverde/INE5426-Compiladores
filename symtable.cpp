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
	return this->table[name];
}

void Symtable::setVar(std::string name, int newValue) {
//	this->table.find(name)->second = newValue;
	this->table[name] = newValue;
}

bool Symtable::hasVar(std::string name) {
	return this->table.find(name) != this->table.end();
}
