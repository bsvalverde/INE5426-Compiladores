%{
#include <string>
#include "ast.h"
#include "symtable.h"
#include "funtable.h"
#include "enums.h"

AST::Block* root;
ST::SymTable* symtable = new ST::SymTable(NULL);

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
	AST::Arguments* argList;

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
%token T_IF T_THEN T_ELSE
%token T_WHILE T_DO
%token T_TYPE T_DOT

//Definição de tipos não-terminais
%type <block> program code cmds funcmds
%type <node> global cmd funcmd decl listvar attr expr const arr arrexpr fun params cond loop composite multdecl dot
%type <typeEnum> type
%type <argList> arglist
%type <node> newscope endscope

//Precedencia de operadores
%left T_OR T_AND
%left T_NOT
%left T_EQ T_NEQ
%left T_LT T_GT T_LTE T_GTE
%left T_PLUS T_SUB
%left T_MULT T_DIV
%right U_NEG
%left U_PAR
%nonassoc error

%start program

%%

program	: code {
			root = $1;
			funtable->checkDefinitions();
		}
		;

code	: global {
			$$ = new AST::Block(); 
			$$->nodes.push_back($1);
		}
		| code global {
			if($2 != NULL) $1->nodes.push_back($2);
		}
		;

global  : cmd
		| fun
		| composite
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
		| cond
		| loop
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

funcmd  : cmd
		| T_RET expr T_ENDL {
			AST::Variable* var = dynamic_cast<AST::Variable*> ($2);
			if(var != NULL){
				if(var->arrExpr == NULL && symtable->getSymbol(var->name)->arrSize > 0){
					yyerror("semantico: funcao retornando arranjo.");
				}
			}
			$$ = new AST::Return($2);
		}

decl	: type arr T_COLON listvar { 
			AST::Variable* var = (AST::Variable*) $4;
			while(var != NULL) {
				ST::Symbol* s = symtable->getSymbol(var->name);
				AST::Const* array = (AST::Const*) $2;
				if($2 != NULL){
					if(atoi(array->value.c_str()) < 1){
						array = new AST::Const("1", Type::inteiro);
						yyerror("semantico: arranjo %s com tamanho menor do que um.", var->name.c_str());
					}
					s->arrSize = atoi(array->value.c_str());
					s->initialized = true;
				}
				var->arrExpr = array;
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
			if($2->type != Type::inteiro){
				yyerror("semantico: indice de tipo %s.", Stringfier::typeStringM($2->type).c_str());
			}
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

attr 	: T_ID arrexpr dot T_ATTR expr {
			AST::Variable* var = new AST::Variable($1, NULL);
			ST::Symbol* s = symtable->getSymbol($1);
			var->type = s->type;
			if(s->type == Type::desconhecido){
				if(funtable->getFunction($1)->returnType != Type::desconhecido){
					if($2 == NULL){
						yyerror("semantico: funcao %s com uso como variavel.", $1);
					} else{
						yyerror("semantico: funcao %s com uso como arranjo.", $1);
					}
				} else if($2 == NULL){
					yyerror("semantico: variavel %s sem declaracao.", $1);
				} else{
					yyerror("semantico: arranjo %s sem declaracao.", $1);
				}
			} else if(s->arrSize == 0 && $2 != NULL){
				yyerror("semantico: variavel %s com uso como arranjo;", $1);
				var->type = Type::desconhecido;
			} else if (s->arrSize > 0 && $2 == NULL){
				yyerror("semantico: arranjo %s com uso como variavel;", $1);
				var->type = Type::desconhecido;
			}
			var->arrExpr = $2;
			symtable->setSymbol($1);
			$$ = new AST::AssignVar(var, $5, $2);
		}
		;

dot 	: T_DOT T_ID {
			$$ = new AST::Variable($2, NULL);
		}
		| {
			$$ = NULL;
		}

expr	: const 
		| T_ID arrexpr dot {
			AST::Variable* var = new AST::Variable($1, NULL);
			ST::Symbol* s = symtable->useSymbol($1);
			var->type = s->type;
			if(s->type != Type::desconhecido){
				if(s->arrSize == 0 && $2 != NULL){
					yyerror("semantico: variavel %s com uso como arranjo;", $1);
					var->type = Type::desconhecido;
				} else if (s->arrSize > 0 && $2 == NULL){
					yyerror("semantico: arranjo %s com uso como variavel;", $1);
					var->type = Type::desconhecido;
				}
			} else{
				if(funtable->getFunction($1)->returnType != Type::desconhecido){
					if($2 == NULL){
						yyerror("semantico: funcao %s com uso como variavel.", $1);
					} else{
						yyerror("semantico: funcao %s com uso como arranjo.", $1);
					}
				} else if($2 == NULL){
					yyerror("semantico: variavel %s sem declaracao.", $1);
				} else{
					yyerror("semantico: arranjo %s sem declaracao.", $1);
				}
			}
			var->arrExpr = $2;
			$$ = var;
		}
		| T_ID T_APAR arglist T_FPAR {
			AST::FunCall* fun = new AST::FunCall($1, $3);
			std::vector<ST::Symbol*> args;
			for(AST::Node* arg : $3->arguments){
				ST::Symbol *s = new ST::Symbol();
				s->setType(arg->type);
				AST::Variable* var = dynamic_cast<AST::Variable*>(arg);
				if(var != NULL){
					s = symtable->getSymbol(var->name);
				}
				args.push_back(s);
			}
			FT::Function* f = funtable->getFunction($1);
			if(funtable->getFunction($1)->returnType == Type::desconhecido){
				ST::Symbol* s = symtable->getSymbol($1);
				if(s->type == Type::desconhecido){
					yyerror("semantico: funcao %s sem declaracao.", $1);
				} else{
					if(s->arrSize == 0){
						yyerror("semantico: variavel %s com uso como funcao.", $1);
					} else{
						yyerror("semantico: arranjo %s com uso como funcao.", $1);
					}
				}
			}
			fun->type = funtable->useFunction($1, args)->returnType;
			$$ = fun;
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

arglist : expr {
			$$ = new AST::Arguments();
			$$->arguments.push_back($1);
		}
		| arglist T_COMMA expr {
			if($3 != NULL) $1->arguments.push_back($3);
		}
		| {
			$$ = new AST::Arguments();
		}
		;

fun 	: T_DECL T_FUN type T_COLON T_ID newscope T_APAR params T_FPAR endscope T_ENDL {
			std::vector<ST::Symbol*> symbols;
			AST::Variable* aux = (AST::Variable*)$8;
			while(aux != NULL){
				ST::Symbol* s = new ST::Symbol(aux->type);
				if(aux->arrExpr != NULL){
					AST::Const* c = (AST::Const*)aux->arrExpr;
					s->arrSize = atoi(c->value.c_str());	
				}
				symbols.push_back(s);
				aux = (AST::Variable*) aux->next;
			}
			FT::Function* fun = new FT::Function($3, symbols);
			funtable->addFunction($5, fun);
			$$ = new AST::DeclFunc($5, new AST::Parameters($8));
			$$->type = $3;
		}
		| T_DEF T_FUN type T_COLON T_ID newscope T_APAR params T_FPAR funcmds endscope T_END T_DEF {
			std::vector<ST::Symbol*> symbols;
			AST::Variable* aux = (AST::Variable*)$8;
			while(aux != NULL){
				ST::Symbol* s = new ST::Symbol(aux->type);
				if(aux->arrExpr != NULL){
					AST::Const* c = (AST::Const*)aux->arrExpr;
					s->arrSize = atoi(c->value.c_str());	
				}
				symbols.push_back(s);
				aux = (AST::Variable*) aux->next;
			}
			FT::Function* fun = new FT::Function($3, symbols);
			funtable->defFunction($5, fun);
			$$ = new AST::DefFunc($5, new AST::Parameters($8), $10);
			$$->type = $3;
		}
		;

const   : T_INT { $$ = new AST::Const($1, Type::inteiro); }
		| T_REAL { $$ = new AST::Const($1, Type::real); }
		| T_BOOL { $$ = new AST::Const($1, Type::booleano); }
		;

type 	: T_DINT { $$ = Type::inteiro; }
		| T_DREAL { $$ = Type::real; }
		| T_DBOOL { $$ = Type::booleano; }
		| T_ID { $$ = Type::composto; }
		;

params	: type arr T_COLON T_ID {
			symtable->addSymbol($4, $1);
			AST::Variable* var = new AST::Variable($4, NULL);
			var->type = $1;
			if($2 != NULL){
				ST::Symbol* s = symtable->getSymbol($4);
				s->arrSize = atoi(((AST::Const*)$2)->value.c_str());
				var->arrExpr = $2;
			}
			$$ = var;
		}
		| params T_COMMA type arr T_COLON T_ID {
			symtable->addSymbol($6, $3);
			AST::Variable* var = new AST::Variable($6, $1);
			var->type = $3;
			if($4 != NULL){
				ST::Symbol* s = symtable->getSymbol($6);
				s->arrSize = atoi(((AST::Const*)$4)->value.c_str());
				var->arrExpr = $4;
			}
			$$ = var;
		}
		| {
			$$ = NULL;
		}
		;

cond	: T_IF expr T_THEN newscope cmds endscope T_END T_IF {
			if($2->type != Type::booleano){
				yyerror("semantico: operacao teste espera booleano mas recebeu %s.", Stringfier::typeStringM($2->type).c_str());
			}
			$$ = new AST::Conditional($2, $5, NULL);
		}
		| T_IF expr T_THEN newscope cmds endscope T_ELSE newscope cmds endscope T_END T_IF {
			if($2->type != Type::booleano){
				yyerror("semantico: operacao teste espera booleano mas recebeu %s.", Stringfier::typeStringM($2->type).c_str());
			}
			$$ = new AST::Conditional($2, $5, $9);
		}
		;

loop	: T_WHILE expr T_DO newscope cmds endscope T_END T_WHILE {
			if($2->type != Type::booleano){
				yyerror("semantico: operacao teste espera booleano mas recebeu %s.", Stringfier::typeStringM($2->type).c_str());
			}
			$$ = new AST::Loop($2, $5);
		}
		;

composite	: T_DEF T_TYPE T_COLON T_ID multdecl T_END T_DEF {
			$$ = new AST::Composite($4, $5);
		}
		;

multdecl: type arr T_COLON T_ID T_ENDL {
			// symtable->addSymbol($1);
			$$ = new AST::Variable($4, NULL);
			$$->type = $1;
		}
		| multdecl type arr T_COLON T_ID T_ENDL {
			// symtable->addSymbol($1);
			$$ = new AST::Variable($5, $1);
			$$->type = $2;
		}
		;

newscope: {
			symtable = new ST::SymTable(symtable);
			$$ = NULL;
		}
		;

endscope: {
			ST::SymTable* oldSymtable = symtable;
			symtable = symtable->superScope;
			delete oldSymtable;
			$$ = NULL;
		}
		;

%%
