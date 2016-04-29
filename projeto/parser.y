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
%token T_NEQ T_EQ T_GTE T_GT T_LTE T_LT 
%token T_COLON T_ENDL T_COMMA

//Deinição de tipos não-terminais
%type <block> program cmds
%type <node> cmd decl listvar attr expr

//Precedencia de operadores
%left T_EQ T_NEQ
%left T_LT T_GT T_LTE T_GTE
%left T_PLUS T_SUB
%left T_MULT T_DIV
%right U_NEG U_NOT
%left T_APAR

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

attr 	: T_ID T_ATTR expr { 
			printf("encontrada atribuicao\n");
			$$ = new AST::BinOp(new AST::Variable($1, NULL), AST::assign, $3); 
		}
		;

expr	: T_ID { 
			printf("encontrado token %s\n", $1); 
			$$ = new AST::Variable($1, NULL);
		}
		| T_INT { printf("encontrado inteiro %d\n", $1); }
		| T_REAL { printf("encontrado real %lf\n", $1); }
		| T_BOOL { printf("encontrado bool %s\n", $1); }
		| expr T_PLUS expr { 
			printf("identificado soma\n"); 
			$$ = new AST::BinOp($1, AST::plus, $3);
		}
		| expr T_SUB expr { 
			printf("identificado subtração\n"); 
			$$ = new AST::BinOp($1, AST::sub, $3);
		}
		| expr T_MULT expr { 
			printf("identificado multiplicação\n"); 
			$$ = new AST::BinOp($1, AST::mult, $3);
		}
		| expr T_DIV expr { 
			printf("identificado divisão\n"); 
			$$ = new AST::BinOp($1, AST::div, $3);
		}
		| expr T_EQ expr {
			printf("identificado equals\n"); 
			$$ = new AST::BinOp($1, AST::eq, $3);
		}
		| expr T_NEQ expr {
			printf("identificado not equals\n"); 
			$$ = new AST::BinOp($1, AST::neq, $3);
		}
		| expr T_LT expr {
			printf("identificado less than\n"); 
			$$ = new AST::BinOp($1, AST::lt, $3);
		}
		| expr T_GT expr {
			printf("identificado greater than\n"); 
			$$ = new AST::BinOp($1, AST::gt, $3);
		}
		| expr T_LTE expr {
			printf("identificado greater than or equal\n"); 
			$$ = new AST::BinOp($1, AST::lte, $3);
		}
		| expr T_GTE expr {
			printf("identificado greater than or equal\n"); 
			$$ = new AST::BinOp($1, AST::gte, $3);
		}
		| expr T_AND expr {
			printf("identificado AND\n"); 
			$$ = new AST::BinOp($1, AST::_and, $3);
		}
		| expr T_OR expr {
			printf("identificado OR\n"); 
			$$ = new AST::BinOp($1, AST::_or, $3);
		}
		| T_SUB expr %prec U_NEG { 
			printf("identificado negativo\n"); 
			$$ = new AST::UnOp(AST::neg, $2);
		}
		| T_NOT expr %prec U_NOT {
			printf("identificado negação\n"); 
			$$ = new AST::UnOp(AST::_not, $2);
		}
		| T_APAR expr T_FPAR %prec T_APAR { 
			printf("identificado parenteses\n"); 
		}
		;

%%