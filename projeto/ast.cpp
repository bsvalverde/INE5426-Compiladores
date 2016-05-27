#include "ast.h"

using namespace AST;

extern FT::FunTable* funtable;

std::string Block::printTree() {
	std::string retorno = "";
	for(Node* node : nodes) {
		if(node != NULL)
			retorno += node->printTree() + "\n";
	}
	return retorno;
}

std::string UnOp::printTree() {
	std::string retorno = "";
	retorno = next->printTree() + ")";
	retorno = "((" + Stringfier::unOpString(op) + " " + Stringfier::typeStringM(next->type) + ") " + retorno;
	this->type = next->type;
	return retorno;
}

std::string BinOp::printTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	lvalue = left->printTree();
	rvalue = right->printTree();
	std::string opString = Stringfier::binOpString(op);
	if (left->type != right->type){
		if(left->type == Type::inteiro && right->type == Type::real){
			lvalue += " para real";
		} else if (left->type == Type::real && right->type == Type::inteiro){
			rvalue += " para real";
		}
	}
    switch(op){
	case plus: case sub: case mult: case _div:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case gt: case lt: case gte: case lte: case eq: case neq:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _and: case _or:
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
    }
    return retorno;
}

std::string Variable::printTree() {
	std::string retorno = Stringfier::typeString(this->type, this->arrExpr != NULL) + " " + this->name;
	if(this->arrExpr != NULL) {
		retorno += " {+indice: " + arrExpr->printTree() + "}";
	}
	return retorno;
}

std::string Const::printTree() {
	std::string retorno = "valor " + Stringfier::typeStringM(this->type) + " " + this->value;
	return retorno;
}

std::string AssignVar::printTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	lvalue = left->printTree();
	rvalue = right->printTree();
	if (left->type != right->type && left->type == Type::real && right->type == Type::inteiro){
		rvalue += " para real";
	}
	return "Atribuicao de valor para " + lvalue + ": " + rvalue;;
}

std::string DeclVar::printTree() {
	std::string retorno = "";
	
	Variable* next = (Variable *)this->next;
	bool isArr = next->arrExpr != NULL;
	retorno = "Declaracao de " + Stringfier::typeString(next->type, isArr);
	if(isArr) {
		retorno += " de tamanho " + ((Const*)(next->arrExpr))->value;
	}
	retorno += ": ";
	
	std::string vars = next->name;
	next = (Variable *)next->next;
	while(next != NULL){
		vars = next->name + ", " + vars;
		next = (Variable *)next->next;
	}
	retorno += vars;
	return retorno;
}

std::string Par::printTree() {
	std::string retorno = "((abre parenteses) " + this->content->printTree() + " (fecha parenteses))";
	return retorno;
}

std::string FunCall::printTree() {
	std::string retorno = "chamada de funcao " + Stringfier::typeStringF(this->type) + " " + this->name;
	retorno += " " + args->printTree();
	return retorno;
}

std::string Arguments::printTree(){
	std::string retorno = "{+parametros: ";
	retorno += this->arguments[0]->printTree();
	for(int i = 1; i < this->arguments.size(); i++){
		retorno += ", " + this->arguments[i]->printTree();
	}
	retorno += "}";
	return retorno;
}

std::string Parameters::printTree(){
	std::string retorno = "";
	AST::Variable* aux = (AST::Variable*) this->param;
	while(aux != NULL) {
		std::string params = "Parametro ";
		if(aux->arrExpr != NULL){
			params += "arranjo ";
		}
		params += Stringfier::typeStringM(aux->type) + ": " + aux->name + "\n" + retorno;
		retorno = params;
		aux = (AST::Variable*) aux->next;
	}
	retorno = "+parametros:\n" + retorno;
	return retorno;
}

std::string DeclFunc::printTree() {
	FT::Function* fun = funtable->getFunction(this->funName);
	std::string retorno = "Declaracao de funcao " + Stringfier::typeStringF(fun->returnType) + ": " + this->funName + "\n";

	retorno += this->parameters->printTree();
	retorno += "Fim declaracao";
	return retorno;
}

std::string DefFunc::printTree() {
	FT::Function* fun = funtable->getFunction(this->funName);
	std::string retorno = "Definicao de funcao " + Stringfier::typeStringF(fun->returnType) + ": " + this->funName + "\n";
	
	retorno += this->parameters->printTree();

	retorno += "+corpo:\n";
	retorno += this->code->printTree();
	retorno += "Fim definicao";
	return retorno;
}

std::string Return::printTree() {
	return "Retorno de funcao: " + this->expr->printTree();
}

std::string Conditional::printTree() {
	std::string retorno = "Expressao condicional";
	retorno += "\n+se:\n" + this->condition->printTree();
	retorno += "\n+entao:\n" + this->then->printTree();
	if(this->_else != NULL){
		retorno += "+senao:\n" + this->_else->printTree();
	}
	retorno += "Fim expressao condicional";
	return retorno;
}

std::string Loop::printTree() {
	std::string retorno = "+enquanto: ";
	retorno += this->condition->printTree();
	retorno += "\n+faca:\n";
	retorno += this->loopBlock->printTree();
	retorno += "Fim laco";
	return retorno;
}

std::string Composite::printTree() {
	std::string retorno = "Definicao tipo: ";
	retorno += this->name;
	retorno += "\n+componentes:";
	AST::Variable* aux = (AST::Variable*) this->attributes;
	while(aux != NULL) {
		retorno += "\nComponente " + aux->printTree();
		aux = (AST::Variable*) aux->next;
	}
	retorno += "\nFim definicao";
	return retorno;
}