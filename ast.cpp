#include "ast.h"

using namespace AST;

/* Print methods */
void Integer::printTree(){
    std::cout << value;
    return;
}

void BinOp::printTree(){
    left->printTree();
    switch(op){
        case plus: std::cout << " + "; break;
        case mult: std::cout << " * "; break;
        case assign: std::cout << " = "; break;
    }
    right->printTree();
    return;
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

void Variable::printTree() {
    if(next != NULL){
        next->printTree();
        std::cout << ", ";
    }
	std::cout << name;
	return;
}

/* Compute methods */
int Integer::computeTree(){
    return value;
}

int Variable::computeTree(){
	return Symtable::getInstance()->getVar(this->name);
}

int BinOp::computeTree(){
    int value, lvalue, rvalue;
    lvalue = left->computeTree();
    rvalue = right->computeTree();
    switch(op){
         case plus: value = lvalue + rvalue; break;
		 case mult: value = lvalue * rvalue; break;
         case assign: Symtable::getInstance()->setVar(dynamic_cast<Variable*>(left)->name, rvalue);
    }
    return value;
}

int Block::computeTree(){
    int value;
    for (Node* line: lines) {
        value = line->computeTree();
         std::cout << "Computed " << value << std::endl;
    }
    return 0;
}
