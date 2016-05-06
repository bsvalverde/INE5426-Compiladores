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
			retorno = "Declaracao de variavel " + TypeStringfier::typeStringF(next->type) + ": ";
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
			yyerror("ERRO SEMANTICO:");//TODO
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
    switch(op){
	case plus:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (soma " + TypeStringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case sub:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (subtracao " + TypeStringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case mult:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (multiplicacao " + TypeStringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case div:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (divisao " + TypeStringfier::typeStringF(this->type) + ") " + rvalue + ")";
		break;
	case gt:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (maior " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case lt:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (menor " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case gte:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (maior ou igual " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case lte:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (menor ou igual " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case eq:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (igual " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case neq:
		if(this->type == Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (diferente " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _and:
		if(this->type != Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (e " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
		break;
	case _or:
		if(this->type != Type::booleano){
			yyerror("ERRO SEMANTICO");//TODO
		}
		retorno = "(" + lvalue + " (ou " + TypeStringfier::typeStringM(this->type) + ") " + rvalue + ")";
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
	std::string retorno = "variavel " +	TypeStringfier::typeStringF(this->type) + " " + this->name;
	return retorno;
}

std::string Const::printTree() {
	std::string retorno = "valor " + TypeStringfier::typeStringM(this->type) + " " + this->value;
	return retorno;
}

std::string Par::printTree() {
	std::string retorno = "((abre parenteses) " + this->content->printTree() + " (fecha parenteses))";
	return retorno;
}