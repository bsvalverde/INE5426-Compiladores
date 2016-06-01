/* Abstract Syntax Tree */
//Classe que forma os nodos da árvore sintática de funcionamento do código gerado.
#pragma once

#include <vector>
#include "funtable.h"
#include "enums.h"
#include "stringfier.h"

namespace AST {

class Node;

typedef std::vector<Node*> NodeList;

class Node { //nodo generalizado
public:
    virtual ~Node() {}
    virtual std::string printTree() {return "";}
    Type type;
};

class Block : public Node { //lista de nodos
public:
	NodeList nodes;
	Block() {}
	std::string printTree();
};

class UnOp : public Node { //nodo utilizado em operações unárias, como menos unário ou não booleno
public:
	UnOperation op;
	Node* next;
	UnOp(UnOperation op, Node* next) : op(op), next(next) {
		this->type = next->type;
	}
	std::string printTree();
};

class BinOp : public Node { //nodo utilizado em operações binárias, tanto matemáticas (+,-,...) quanto lógicas (>,=,...)
public:
	BinOperation op;
	Node* left;
	Node* right;
	BinOp(Node* left, BinOperation op, Node* right) : left(left), op(op), right(right) {
		switch(op){
			case plus: case sub: case mult: case _div:
				this->type = Type::inteiro;
				if((left->type == Type::real || left->type == Type::real) && (right->type == Type::inteiro || right->type == Type::real)){
					this->type = Type::real;
				}
				if(left->type != Type::inteiro && left->type != Type::real){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera inteiro ou real mas recebeu " + Stringfier::typeStringM(left->type) + ".").c_str());
				}
				if(right->type != Type::inteiro && right->type != Type::real){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera inteiro ou real mas recebeu " + Stringfier::typeStringM(right->type) + ".").c_str());
				}
				break;
			case gt: case lt: case gte: case lte: case eq: case neq:
				if(left->type != Type::inteiro && left->type != Type::real){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera inteiro ou real mas recebeu " + Stringfier::typeStringM(left->type) + ".").c_str());
				}
				if(right->type != Type::inteiro && right->type != Type::real){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera inteiro ou real mas recebeu " + Stringfier::typeStringM(right->type) + ".").c_str());
				}
				this->type = Type::booleano;
				break;
			case _and: case _or:
				if(!(left->type == Type::booleano)){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera booleano mas recebeu " + Stringfier::typeStringM(left->type) + ".").c_str());
				}
				if(!(right->type == Type::booleano)){
					yyerror(("semantico: operacao " + Stringfier::binOpString(op) + " espera booleano mas recebeu " + Stringfier::typeStringF(right->type) + ".").c_str());
				}
				this->type = Type::booleano;
				break;
		}
	}
	std::string printTree();
};

class Variable : public Node { //nodo utilizado no uso de variáveis
public:
	std::string name;
	Node* next;
	Node* arrExpr;
	Variable(std::string name, Node* next) : name(name), next(next) {
		this->arrExpr = NULL;
	}
	std::string printTree();
};

class Const : public Node { //nodo utilizado no uso de valores constantes (1, .1 ou até TRUE)
public:
	std::string value;
	Const(std::string value, Type type) : value(value) {
		this->type = type;
	}
	std::string printTree();
};

class AssignVar : public Node { //nodo utilizado na atribuição de variáveis. separado das operações binárias para facilitar a operação printTree()
public:
	Node* left;
	Node* right;
	Node* arrExpr;
	AssignVar(Node* left, Node* right, Node* arrExpr) : left(left), right(right), arrExpr(arrExpr) {
		this->type = left->type;
		if(left->type != right->type && (left->type != Type::real || right->type != Type::inteiro)){
			yyerror(("semantico: operacao atribuicao espera " + Stringfier::typeStringM(left->type) + " mas recebeu " + Stringfier::typeStringM(right->type) + ".").c_str());
		}
	}
	std::string printTree();
};

class DeclVar : public Node { //nodo utilizado na declaração de variáveis. separado das operações unárias para facilitar o printTree()
public:
	Node* next;
	DeclVar(Node* next) : next(next) {}
	std::string printTree();
};

class Par : public Node { //nodo utilizado quando há parênteses nas fórmulas
public:
	Node* content;
	Par(Node* content) : content(content) {
		this->type = content->type;
	}
	std::string printTree();
};

class FunCall : public Node { //nodo utilizado na chamada de funções
public:
	std::string name;
	Node* args;
	FunCall(std::string name, Node* args) : name(name), args(args) {}
	std::string printTree();
};

class Arguments : public Node { //nodo utilizado na utilização de argumentos na chamada de funções
public:
	NodeList arguments;
	Arguments() {}
	std::string printTree();
};

class Parameters : public Node { //nodo utilizado na criação de parâmetros na declaração/definição de funções
public:
	Node* param;
	Parameters(Node* param) : param(param) {}
	std::string printTree();
};

class DeclFunc : public Node { //nodo utilizado na declaração de funções
public:
	std::string funName;
	Node* parameters;
	DeclFunc(std::string funName, Node* parameters) : funName(funName), parameters(parameters) {}
	std::string printTree();
};

class Return : public Node { //nodo utilizado na chamada retorno de funções
public:
	Node* expr;
	Return(Node* expr) : expr(expr) {}
	std::string printTree();
};

class DefFunc : public Node { //nodo utilizado na definição de funções
public:
	std::string funName;
	Node* parameters;
	Node* code;
	DefFunc(std::string funName, Node* parameters, Node* code) : funName(funName), parameters(parameters), code(code) {
		Block* c = (Block*) code;
		bool ret = false;
		for(Node* line : c->nodes){
			if(dynamic_cast<Return*>(line)){
				ret = true;
				break;
			}
		}
		if(!ret){
			yyerror("semantico: funcao %s sem retorno.", funName.c_str());
		}
	}
	std::string printTree();
};

class Conditional : public Node { //nodo utilizado na chamada de condicionais
public:
	Node* condition;
	Node* then;
	Node* _else;
	Conditional(Node* condition, Node* then, Node* _else) : condition(condition), then(then), _else(_else) {}
	std::string printTree();
};

class Loop : public Node { //nodo utilizado na chamada de loops
public:
	Node* condition;
	Node* loopBlock;
	Loop(Node* condition, Node* loopBlock) : condition(condition), loopBlock(loopBlock) {}
	std::string printTree();
};

class Composite : public Node { //nodo utilizado na definição de tipos compostos
public:
	std::string name;
	Node* attributes;
	Composite(std::string name, Node* attributes) : name(name), attributes(attributes) {}
	std::string printTree();
};

}
