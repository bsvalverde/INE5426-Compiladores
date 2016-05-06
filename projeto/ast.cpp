#include "ast.h"

using namespace AST;

extern ST::SymTable* symtable;

std::string Block::printTree() {
	for(Node* node : nodes) {
		printf("%s\n", node->printTree().c_str());
	}
	return "";
}

std::string UnOp::printTree() {
	std::string retorno = "";
	switch(op){
		case decl:
		{
			retorno = "Declaracao de variavel " + Stringfier::typeStringF(next->type) + ": ";
			Variable* next = (Variable *)this->next;
			std::string vars = next->name;
			next = (Variable *)next->next;
			while(next != NULL){
				vars = next->name + ", " + vars;
				next = (Variable *)next->next;
			}
			retorno += vars;
			break;
		}
		case _not:break;
		case neg:break;
	}
	return retorno;
}

std::string BinOp::printTree() {
	std::string retorno, lvalue, rvalue;
	retorno = "";
	lvalue = left->printTree();
	rvalue = right->printTree();
	if (left->type != right->type){
		if(left->type == Type::booleano || right->type == Type::booleano){
			yyerror("ERRO SEMANTICO\n");//TODO
		} else {
			std::string *mudar = &lvalue;
				if(right->type == Type::inteiro){
					mudar = &rvalue;
					right->type = left->type;
				}
			*mudar += " para real";
		}
	}
	this->type = right->type;
	std::string opString = Stringfier::binOpString(op);
    switch(op){
	case plus:
	case sub:
	case mult:
	case _div:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO\n");//TODO
		}
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case gt:
	case lt:
	case gte:
	case lte:
	case eq:
	case neq:
		// if(left->type != booleano) {
		// 	yyerror(("Erro semantico: operacao " + opString + " espera booleano do lado esquerdo da atribuicao.\n").c_str());
		// }
		if(left->type == Type::booleano || right->type == Type::booleano){
			yyerror(("Erro semantico: operacao " + opString + " espera inteiro ou real mas recebeu booleano\n").c_str());//TODO
		}
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _and:
	case _or:
		if(this->type != Type::booleano){
			yyerror("ERRO SEMANTICO\n");//TODO
		}
		retorno = "(" + lvalue + " (" + opString + " " + Stringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	
	case assign:
		retorno = "Atribuicao de valor para " + lvalue + ":" + " " + rvalue;
		break;
    } 	
    
    return retorno;
}

std::string Variable::printTree() {
	ST::Symbol* s = symtable->getSymbol(this->name);
	this->type = s->type;
	std::string retorno = "variavel " +	Stringfier::typeStringF(this->type) + " " + this->name;
	return retorno;
}

std::string Const::printTree() {
	std::string retorno = "valor " + Stringfier::typeStringM(this->type) + " " + this->value;
	return retorno;
}

std::string Par::printTree() {
	std::string retorno = "((abre parenteses) " + this->content->printTree() + " (fecha parenteses))";
	return retorno;
}