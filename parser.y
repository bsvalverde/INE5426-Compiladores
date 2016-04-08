%{
#include "ast.h"
AST::Block *programRoot; /* the root node of our program AST:: */
std::map<string, int> table;
extern int yylex();
extern void yyerror(const char* s, ...);
%}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int integer;
    string var;
    AST::Node *node;
    AST::Block *block;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <var> T_VAR
%token T_PLUS T_MULT T_ATTR T_VAR T_NL

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line
%type <block> lines program

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 */
%left T_PLUS
%left T_MULT
%nonassoc error T_ATTR

/* Starting rule 
 */
%start program

%%

program : lines { programRoot = $1; }
        ;
        

lines   : line { $$ = new AST::Block(); $$->lines.push_back($1); }
        | lines line { if($2 != NULL) $1->lines.push_back($2); }
        ;

line    : T_NL { $$ = NULL; } /*nothing here to be used */
        | expr T_NL /*$$ = $1 when nothing is said*/
		| varattr T_NL
        ;

expr    : T_INT { $$ = new AST::Integer($1); printf("Inteiro %d identificado.\n", $1); }

    	| expr T_PLUS expr { 
			$$ = new AST::BinOp($1, AST::plus, $3);
			printf("Equação PLUS identificada.\n"); 
		}
	
		| expr T_MULT expr { 
			$$ = new AST::BinOp($1, AST::mult, $3);  
			printf("Equação MULT identificada\n"); 
		}

        | expr error { yyerrok; $$ = $1; } /*just a point for error recovery*/
		;

varattr : var T_ATTR expr {
			table.insert(std::pair<string, int>($1, $3));
			printf("Valor atribuido a variavel %s\n", $1);
		}

%%


