#include "ast.h"

using namespace AST;

extern ST::SymTable* symtable;

std::string Block::printTree() {
	for(Node* node : nodes) {
		if(node != NULL)
			printf("%s\n", node->printTree().c_str());
	}
	return "";
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
			left->type = Type::real;
		} else if (left->type == Type::real && right->type == Type::inteiro){
			rvalue += " para real";
			right->type = Type::real;
		}
	}
	this->type = left->type;
    switch(op){
	case plus: case sub: case mult: case _div:
		if(left->type == Type::booleano || right->type == Type::booleano){
			yyerror(("semantico: operacao " + opString + " espera inteiro ou real mas recebeu booleano.").c_str());
			this->type = Type::inteiro;
		} else if(left->type == Type::desconhecido || right->type == Type::desconhecido){
			yyerror(("semantico: operacao " + opString + " espera inteiro ou real mas recebeu desconhecido.").c_str());
			this->type = Type::inteiro;
		}
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case gt: case lt: case gte: case lte: case eq: case neq:
		if(left->type == Type::booleano || right->type == Type::booleano){
			yyerror(("semantico: operacao " + opString + " espera inteiro ou real mas recebeu booleano.").c_str());
		} else if(left->type == Type::desconhecido || right->type == Type::desconhecido){
			yyerror(("semantico: operacao " + opString + " espera inteiro ou real mas recebeu desconhecido.").c_str());
		}
		this->type = Type::booleano;
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _and: case _or:
		if(!(left->type == Type::booleano)){
			yyerror(("semantico: operacao " + opString + " espera booleano mas recebeu " + Stringfier::typeStringF(left->type) + ".").c_str());
		}
		if(!(right->type == Type::booleano)){
			yyerror(("semantico: operacao " + opString + " espera booleano mas recebeu " + Stringfier::typeStringF(right->type) + ".").c_str());
		}
		this->type = Type::booleano;
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
    }
    return retorno;
}

std::string Variable::printTree() {
	ST::Symbol* s = symtable->useSymbol(this->name);
	this->type = s->type;
	// std::string retorno = "variavel " +	Stringfier::typeStringF(this->type) + " " + this->name;
	std::string retorno = Stringfier::typeString(this->type, this->arrExpr != NULL) + " " + this->name;
	if(this->arrExpr != NULL) {
		retorno += " {+indice: " + arrExpr->printTree() + "}";
	}
	return retorno;
}

// std::string VariableArr::printTree() {
// 	ST::Symbol* s = symtable->useSymbol(this->name);
// 	this->type = s->type;
// 	std::string retorno = "variavel arr " +	Stringfier::typeStringF(this->type) + " " + this->name + " de tamanho " + std::to_string(this->size);
// 	return retorno;	
// }

std::string Const::printTree() {
	std::string retorno = "valor " + Stringfier::typeStringM(this->type) + " " + this->value;
	return retorno;
}

std::string AssignVar::printTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	rvalue = right->printTree();
	Variable *var = (Variable *)left;
	symtable->setSymbol(var->name);
	lvalue = left->printTree();
	//TODO tirar duvida com prof sobre esse erro
	if (left->type != right->type){
		if(left->type == Type::real && right->type == Type::inteiro)
			rvalue += " para real";
		else
			yyerror(("semantico: operacao atribuicao espera " + Stringfier::typeStringM(left->type) + " mas recebeu " + Stringfier::typeStringM(right->type) + ".").c_str());
	}
	this->type = left->type;

	retorno = "Atribuicao de valor para " + lvalue + ":";
	// bool isArr = this->arrExpr != NULL;
	bool isArr = var->arrExpr != NULL;
	if(isArr) {
		// this->arrExpr->size = 0;
		retorno += "\n+indice: " + var->arrExpr->printTree() + "\n+valor: " + rvalue;
	} else {
		retorno += " " + rvalue;
	}
	return retorno;
}

std::string DeclVar::printTree() {
	//TODO e se adicionarmos ao symtable aqui? otherwise, input: int:a,a;
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
	this->type = this->content->type;
	return retorno;
}