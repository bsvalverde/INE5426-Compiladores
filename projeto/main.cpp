#include <iostream>
#include "ast.h"
#include "symtable.h"

extern AST::Block* root; //set on Bison file
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
    //yydebug = 1;
    yyparse();                  //parses whole data
    root->printTree();   //prints the ASTs
    std::cout << std::endl;
    // root->computeTree(); //computes the ASTs
    return 0;
}
