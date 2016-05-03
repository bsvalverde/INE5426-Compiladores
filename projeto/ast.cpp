#include "ast.h"

using namespace AST;

extern ST::SymTable* symtable;

void* Block::computeTree() {
	for(Node* node : nodes) {
		node->computeTree();
	}
	return 0;
}

void* UnOp::computeTree() {
	
}

void* BinOp::computeTree() {
	void *value, *lvalue, *rvalue;
    lvalue = left->computeTree();
    rvalue = right->computeTree();
    switch(op){
    	case assign:
    	// Symbol s = symtable->getSymbol(((Variable*)left)->name);
    	// s.value = rvalue;
    	symtable->setSymbol(((Variable*)left)->name, ST::Symbol(right->type, rvalue));
	 	value = rvalue;
	 	this->type = right->type;
		printf("var '%s' with value: %d\n", ((Variable*)left)->name.c_str(), (intptr_t) rvalue);
		break;	
    }
    return value;
}

void* Variable::computeTree() {
	ST::Symbol s = symtable->getSymbol(this->name);
	this->type = s.type;
	return s.value;
}

void* Const::computeTree() {
	return this->value;
}