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
			retorno = "Declaracao de variavel ";
			Variable* next = (Variable *)this->next;
			switch(next->type){
				case inteiro:
					retorno += "inteira: ";
					break;
				case real:
					retorno += "real: ";
					break;
				case booleano:
					retorno += "booleana: ";
					break;
			}
			retorno += next->name;
			next = (Variable *)next->next;
			while(next != NULL){
				retorno += ", " + next->name;
				next = (Variable *)next->next;
			}
			break;
		case _not:break;
		case neg:break;
	}
	return retorno;
}

std::string BinOp::printTree() {
	std::string retorno, lvalue, rvalue, tipo;
	retorno = "";
	lvalue = left->printTree();
	rvalue = right->printTree();
	if (left->type != right->type){
		if(left->type == Type::booleano || right->type == Type::booleano){
			yyerror("ERRO SINTATICO:");//TODO
		} else {
			std::string *mudar = &lvalue;
				if(right->type == Type::inteiro){
					mudar = &lvalue;
					right->type = left->type;
				}
			*mudar += " para real";
		}
	}
	this->type = right->type;
	tipo = "" + this->type;
    switch(op){
	case plus:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (soma " + tipo + ") " + rvalue + ")";
		break;
	case sub:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (subtracao " + tipo + ") " + rvalue + ")";
		break;
	case mult:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (multiplicacao " + tipo + ") " + rvalue + ")";
		break;
	case div:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (divisao " + tipo + ") " + rvalue + ")";
		break;
	case gt:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (maior " + tipo + ") " + rvalue + ")";
		break;
	case lt:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (menor " + tipo + ") " + rvalue + ")";
		break;
	case gte:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (maior ou igual " + tipo + ") " + rvalue + ")";
		break;
	case lte:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (menor ou igual " + tipo + ") " + rvalue + ")";
		break;
	case eq:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (igualdade " + tipo + ") " + rvalue + ")";
		break;
	case neq:
		if(this->type == Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (desigualdade " + tipo + ") " + rvalue + ")";
		break;
	case _and:
		if(this->type != Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (e logico " + tipo + ") " + rvalue + ")";
		break;
	case _or:
		if(this->type != Type::booleano){
			yyerror("ERRO SINTATICO");//TODO
		}
		retorno = "(" + lvalue + " (ou logico " + tipo + ") " + rvalue + ")";
		break;
	case assign:
		retorno = "Atribuicao de valor para " + lvalue + ":" + " " + rvalue;
		break;
    } 	
    
    return retorno;
}

std::string Variable::printTree() {
	ST::Symbol s = symtable->getSymbol(this->name);
	std::string retorno = "variavel ";
	switch(s.type) {
	case inteiro:
		retorno += "inteira ";
		break;
	case real:
		retorno += "real ";
		break;
	case booleano:
		retorno += "booleana ";
		break;
	}
	retorno += this->name;
	this->type = s.type;
	return retorno;
}

std::string Const::printTree() {
	std::string retorno = "valor " + /*this->type*/this->value + " " + this->value;
	return retorno;
}
