#include <iostream>
#include "ast.h"
#include "symtable.h"

extern AST::Block* root; //set on Bison file
extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();                  //parses whole data
    // programRoot->printTree();   //prints the ASTs
    std::cout << std::endl;
    root->computeTree(); //computes the ASTs
    return 0;
}
