/* Abstract Syntax Tree */
#pragma once

#include <vector>

namespace AST {

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

class Variable : public Node {
public:
	std::string name;
	Node* next;
	Variable(std::string name, Node* next) : name(name), next(next) {}
};

}