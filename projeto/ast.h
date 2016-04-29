/* Abstract Syntax Tree */
#pragma once

#include <vector>

namespace AST {

enum UnOperation {
	_not,
	neg
};

enum BinOperation {
	plus,
	sub,
	mult,
	div,
	gt,
	lt,
	gte,
	lte,
	eq,
	neq,
	_and,
	_or,
	assign
};

class Node;

typedef std::vector<Node*> NodeList;

class Node {
public:
    virtual ~Node() {}
};

class Block : public Node {
public:
	NodeList nodes;
	Block() {}
};

class UnOp : public Node {
public:
	UnOperation op;
	Node* right;

	UnOp(UnOperation op, Node* right) : op(op), right(right) {}
};

class BinOp : public Node {
public:
	BinOperation op;
	Node* left;
	Node* right;

	BinOp(Node* left, BinOperation op, Node* right) : left(left), op(op), right(right) {}
};

class Variable : public Node {
public:
	std::string name;
	Node* next;
	Variable(std::string name, Node* next) : name(name), next(next) {}
};

}