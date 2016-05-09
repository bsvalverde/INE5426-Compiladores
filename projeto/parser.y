%{
#include <string>
#include "ast.h"
#include "symtable.h"
#include "enums.h"

AST::Block* root;
ST::SymTable* symtable = new ST::SymTable();

extern int yylex();
extern void yyerror(const char* s, ...);	
%}

%define parse.trace

%union {
	const char* inteiro;
	const char* real;
	const char* booleano;
	const char* id;

	const char* arrLen;
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
%token T_AARR T_FARR
%token T_DINT T_DREAL T_DBOOL
%token T_NEQ T_EQ T_GTE T_GT T_LTE T_LT 
%token T_COLON T_ENDL T_COMMA

//Deinição de tipos não-terminais
%type <block> program cmds
%type <node> cmd decl listvar attr expr
%type <arrLen> arr

//Precedencia de operadores
%left T_EQ T_NEQ
%left T_LT T_GT T_LTE T_GTE
%left T_PLUS T_SUB
%left T_MULT T_DIV
%right U_NEG T_NOT
%left U_PAR
%nonassoc error

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
        | error T_ENDL {yyerrok; $$=NULL;}
		;

decl	: T_DINT arr T_COLON listvar { 
			AST::Variable* var = (AST::Variable*) $4;
			bool isArr = $2 == NULL;
			while(var != NULL) {
				ST::Symbol* s = symtable->getSymbol(var->name);
				s->setType(Type::inteiro);
				var->type = s->type;
				var = (AST::Variable*) var->next;
			}
			$$ = new AST::UnOp(decl, $4);
		}
		| T_DREAL arr T_COLON listvar { 
			AST::Variable* var = (AST::Variable*) $4;
			bool isArr = $2 == NULL;
			while(var != NULL) {
				ST::Symbol* s = symtable->getSymbol(var->name);
				s->setType(Type::real);
				var->type = s->type;
				var = (AST::Variable*) var->next;
			}
			$$ = new AST::UnOp(decl, $4);
		}
		| T_DBOOL arr T_COLON listvar { 
			AST::Variable* var = (AST::Variable*) $4;
			bool isArr = $2 == NULL;
			while(var != NULL) {
				ST::Symbol* s = symtable->getSymbol(var->name);
				s->setType(Type::booleano);
				var->type = s->type;
				var = (AST::Variable*) var->next;
			}
			$$ = new AST::UnOp(decl, $4);
		}
		;

arr 	: T_AARR T_INT T_FARR {
			$$ = $2;
		}
		| {}
		;

listvar	: T_ID {
			symtable->addSymbol($1);
			$$ = new AST::Variable($1, NULL);
		}
		| listvar T_COMMA T_ID {
			symtable->addSymbol($3);
			$$ = new AST::Variable($3, $1);
		}
		;

attr 	: T_ID T_ATTR expr {
			$$ = new AST::BinOp(new AST::Variable($1, NULL), assign, $3); 
		}
		;

expr	: T_ID { 
			$$ = new AST::Variable($1, NULL);
		}
		| T_INT {
			$$ = new AST::Const($1, Type::inteiro);
		}
		| T_REAL {
			$$ = new AST::Const($1, Type::real);
		}
		| T_BOOL {
			$$ = new AST::Const($1, Type::booleano);
		}
		| expr T_PLUS expr {
			$$ = new AST::BinOp($1, plus, $3);
		}
		| expr T_SUB expr {
			$$ = new AST::BinOp($1, sub, $3);
		}
		| expr T_MULT expr {
			$$ = new AST::BinOp($1, mult, $3);
		}
		| expr T_DIV expr {
			$$ = new AST::BinOp($1, _div, $3);
		}
		| expr T_EQ expr {
			$$ = new AST::BinOp($1, eq, $3);
		}
		| expr T_NEQ expr {
			$$ = new AST::BinOp($1, neq, $3);
		}
		| expr T_LT expr {
			$$ = new AST::BinOp($1, lt, $3);
		}
		| expr T_GT expr {
			$$ = new AST::BinOp($1, gt, $3);
		}
		| expr T_LTE expr {
			$$ = new AST::BinOp($1, lte, $3);
		}
		| expr T_GTE expr {
			$$ = new AST::BinOp($1, gte, $3);
		}
		| expr T_AND expr {
			$$ = new AST::BinOp($1, _and, $3);
		}
		| expr T_OR expr { 
			$$ = new AST::BinOp($1, _or, $3);
		}
		| T_SUB expr %prec U_NEG { 
			$$ = new AST::UnOp(neg, $2);
		}
		| T_NOT expr { 
			$$ = new AST::UnOp(_not, $2);
		}
		| T_APAR expr T_FPAR %prec U_PAR { 
			$$ = new AST::Par($2);
		}
		;

%%
