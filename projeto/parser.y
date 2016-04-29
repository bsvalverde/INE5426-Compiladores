%{
#include <string>
#include "ast.h"

AST::Block* root;

extern int yylex();
extern void yyerror(const char* s, ...);	
%}

%union {
	int inteiro;
	double real;
	const char* booleano;
	const char* id;

	AST::Node* node;
	AST::Block* block;
}

//Definição dos tokens
%token <inteiro> T_INT
%token <real> T_REAL
%token <booleano> T_BOOL
%token <id> T_ID
%token T_PLUS T_MULT T_SUB T_DIV T_ATTR
%token T_AND T_OR T_NOT
%token T_APAR T_FPAR 
%token T_DINT T_DREAL T_DBOOL
%token T_NEQ T_EQ T_GEQ T_GT T_LEQ T_LT 
%token T_COLON T_ENDL T_COMMA

//Deinição de tipos não-terminais
%type <block> program cmds
%type <node> cmd decl listvar attr expr

//Precedencia de operadores
//TODO

%start program

%%

program	: cmds { root = $1; }
;

cmds	: cmd { 
			$$ = new AST::Block(); 
			$$->nodes.push_back($1);
		}
		| cmds cmd {
			if($2 != NULL) $1->nodes.push_back($2);
		}
		;

cmd 	: decl T_ENDL
		| attr T_ENDL
		;

decl	: T_DINT T_COLON listvar { $$ = $3; }
		| T_DREAL T_COLON listvar { $$ = $3; }
		| T_DBOOL T_COLON listvar { $$ = $3; }
		;

listvar	: T_ID {
			$$ = new AST::Variable($1, NULL);
			printf("declaração da variável %s\n", $1);
		}
		| listvar T_COMMA T_ID {
			$$ = new AST::Variable($3, $1);
			printf("declaração da variável %s\n", $3);
		}
		;

attr 	: T_ID T_ATTR expr { $$ = new AST::Block(); }
		;

expr	: T_ID { printf("encontrado token %s\n", $1); }
		| T_INT { printf("encontrado inteiro %d\n", $1); }
		| T_REAL { printf("encontrado real %lf\n", $1); }
		| T_BOOL { printf("encontrado bool %s\n", $1); }
		| expr T_PLUS expr { printf("identificado soma"); }
		| expr T_SUB expr { printf("identificado subtração"); }
		| expr T_MULT expr { printf("identificado multiplicação"); }
		| expr T_DIV expr { printf("identificado divisão"); }
		| T_APAR expr T_FPAR { printf("identificado parenteses"); }
		| T_SUB expr { printf("identificado negativo"); }
		;

%%