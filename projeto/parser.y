%{
#include <string>
#include "ast.h"
#include "symtable.h"
#include "funtable.h"
#include "enums.h"

AST::Block* root;
ST::SymTable* symtable = new ST::SymTable();
FT::FunTable* funtable = new FT::FunTable();

extern int yylex();
extern void yyerror(const char* s, ...);	
%}

%define parse.trace

%union {
	const char* inteiro;
	const char* real;
	const char* booleano;
	const char* id;

	Type typeEnum;
	BinOperation opEnum;
	//std::vector<ST::Symbol*> parameters;
	int parameters;

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
%token T_DEF T_DECL T_END T_FUN T_RET

//Definição de tipos não-terminais
%type <block> program cmds funcmds
%type <node> cmd funcmd decl listvar attr expr const arr arrexpr fun funsig 
%type <typeEnum> type
%type <opEnum> op
%type <parameters> params

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
		| fun
        | error T_ENDL {yyerrok; $$=NULL;}
		;

funcmds : funcmd { 
			$$ = new AST::Block(); 
			$$->nodes.push_back($1);
		}
		| funcmds funcmd {
			if($2 != NULL) $1->nodes.push_back($2);
		}
		;

funcmd 	: decl T_ENDL
		| attr T_ENDL
		| T_RET expr T_ENDL {
			$$ = new AST::Return($2);
		}
        | error T_ENDL {yyerrok; $$=NULL;}
		;

decl	: type arr T_COLON listvar { 
			AST::Variable* var = (AST::Variable*) $4;
			while(var != NULL) {
				var->arrExpr = $2;
				ST::Symbol* s = symtable->getSymbol(var->name);
				s->setType($1);
				var->type = s->type;
				var = (AST::Variable*) var->next;
			}
			$$ = new AST::DeclVar($4);
		}
		;

arr 	: T_AARR T_INT T_FARR {
			$$ = new AST::Const($2, Type::inteiro);
		}
		| { 
			$$ = NULL;
		}
		;

arrexpr : T_AARR expr T_FARR {
			$$ = $2;
		}
		| { 
			$$ = NULL;
		}

listvar	: T_ID {
			symtable->addSymbol($1);
			$$ = new AST::Variable($1, NULL);
		}
		| listvar T_COMMA T_ID {
			symtable->addSymbol($3);
			$$ = new AST::Variable($3, $1);
		}
		;

attr 	: T_ID arrexpr T_ATTR expr {
			AST::Variable* var = new AST::Variable($1, NULL);
			var->type = symtable->getSymbol($1)->type;
			var->arrExpr = $2;
			symtable->setSymbol($1);
			$$ = new AST::AssignVar(var, $4, $2);
		}
		;

expr	: const 
		| T_ID arrexpr { 
			AST::Variable* var = new AST::Variable($1, NULL);
			var->type = symtable->useSymbol($1)->type;
			var->arrExpr = $2;
			$$ = var;
		}
		| expr op expr {
			$$ = new AST::BinOp($1, $2, $3);
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

op 		: T_PLUS { $$ = BinOperation::plus; }
		| T_SUB { $$ = BinOperation::sub; }
		| T_MULT { $$ = BinOperation::mult; }
		| T_DIV { $$ = BinOperation::_div; }
		| T_EQ { $$ = BinOperation::eq; }
		| T_NEQ { $$ = BinOperation::neq; }
		| T_LT { $$ = BinOperation::lt; }
		| T_GT { $$ = BinOperation::gt; }
		| T_LTE { $$ = BinOperation::lte; }
		| T_GTE { $$ = BinOperation::gte; }
		| T_AND { $$ = BinOperation::_and; }
		| T_OR { $$ = BinOperation::_or; }
		;


fun 	: T_DECL funsig T_ENDL { //aqui nodo declvar
	$$ = NULL; }
		| T_DEF funsig funcmds T_END T_DEF { //aqui nodo defvar que analisa semanticamente funcmds
			$$ = NULL; }
		;

funsig 	: T_FUN type T_COLON T_ID T_APAR params T_FPAR {
			//FT::Function* fun = new FT::Function($2, $6);
			//funtable->addFunction($4, fun);
			//retorna nodo function
			$$ = NULL; 
		}
		;

const   : T_INT { $$ = new AST::Const($1, Type::inteiro); }
		| T_REAL { $$ = new AST::Const($1, Type::real); }
		| T_BOOL { $$ = new AST::Const($1, Type::booleano); }
		;

type 	: T_DINT { $$ = Type::inteiro; }
		| T_DREAL { $$ = Type::real; }
		| T_DBOOL { $$ = Type::booleano; }
		;

params	: type T_COLON T_ID { //cria um vector e para cada tid adiciona new Symbol nele
	$$ = NULL; }
		| params T_COMMA type T_COLON T_ID { //ja criu entao so adiciona new Symbel (se basear em cmds)
			$$ = NULL; }
		;

%%
