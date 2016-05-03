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
	case plus:
		value = (void*)((intptr_t) lvalue + (intptr_t) rvalue);
		printf("%d + %d = %d\n", (intptr_t) lvalue, (intptr_t) rvalue, (intptr_t) value);

		//type

		break;
	case sub:
		value = (void*)((intptr_t) lvalue - (intptr_t) rvalue);
		printf("%d - %d = %d\n", (intptr_t) lvalue, (intptr_t) rvalue, (intptr_t) value);
		break;
	case mult:
		value = (void*)((intptr_t) lvalue * (intptr_t) rvalue);
		printf("%d * %d = %d\n", (intptr_t) lvalue, (intptr_t) rvalue, (intptr_t) value);
		break;
	case div:
		value = (void*)((intptr_t) lvalue / (intptr_t) rvalue);
		printf("%d / %d = %d\n", (intptr_t) lvalue, (intptr_t) rvalue, (intptr_t) value);
		break;
	case gt:
	case lt:
	case gte:
	case lte:
	case eq:
	case neq:
	case _and:
	case _or:
		break;
	case assign:
		std::string id = ((Variable*)left)->name;

    	symtable->setSymbol(id, ST::Symbol(right->type, rvalue));
		printf("var '%s' assigned with value: %d\n", ((Variable*)left)->name.c_str(), (intptr_t) rvalue);
	 	
 		value = rvalue;
 		this->type = right->type;
		
		break;
    } 	
    
    return value;
}

void* Variable::computeTree() {
	ST::Symbol s = symtable->getSymbol(this->name);
	// switch(s.type) {
	// case inteiro:
	// 	return s.value;
	// case real:
	// 	return 
	// }
	this->type = s.type;
	return s.value;
}

void* Const::computeTree() {
	return this->value;
}