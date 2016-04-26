%{
#include "ast.h"


AST::Block *programRoot; /* the root node of our program AST:: */
extern int yylex();
extern void yyerror(const char* s, ...);
%}

/* yylval == %union
 * union informs the different ways we can store data
 */
%union {
    int integer;
	const char* variable;
    AST::Node *node;
    AST::Block *block;
}

/* token defines our terminal symbols (tokens).
 */
%token <integer> T_INT
%token <variable> T_VAR
%token T_PLUS T_MULT T_DEF T_NL COMMA T_EQ

/* type defines the type of our nonterminal symbols.
 * Types should match the names used in the union.
 * Example: %type<node> expr
 */
%type <node> expr line declvar
%type <block> lines program

/* Operator precedence for mathematical operators
 * The latest it is listed, the highest the precedence
 */
%left T_PLUS
%left T_MULT
%nonassoc error

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
		| T_DEF declvar T_NL {$$ = $2;}
		| T_VAR T_EQ expr {
			if(!Symtable::getInstance()->hasVar($1)) {
				printf("ERRO: Variavel %s não definida.\n", $1);
				exit(1);
			}
			$$ = new AST::BinOp(new AST::Variable($1, NULL), AST::assign, $3);
		} 
        ;

expr    : T_INT { $$ = new AST::Integer($1); printf("Inteiro %d identificado.\n", $1); }
		
		| T_VAR { 
			if(!Symtable::getInstance()->hasVar($1)) {
				printf("ERRO: Variavel %s não definida.\n", $1);
				exit(1);
			}
			$$ = new AST::Variable($1, NULL); 
			printf("Variavel %s identificada.\n", $1); 
		}

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

declvar : T_VAR {
			if(Symtable::getInstance()->hasVar($1)) {
				printf("ERRO: Variavel %s já definida.\n", $1);
				exit(1);
			}
			Symtable::getInstance()->addVar((const char*)$1, 0);
			$$ = new AST::Variable($1, NULL);					
			printf("Definição da variável %s.\n", $1);
		}

		| declvar COMMA T_VAR {
			if(Symtable::getInstance()->hasVar($3)) {
				printf("ERRO: Variavel %s já definida.\n", $3);
				exit(1);
			}
			Symtable::getInstance()->addVar((const char*)$3, 0);
			$$ = new AST::Variable($3, $1);
			printf("Definição da variável %s.\n", $3);
		}
		;

%%