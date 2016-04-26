%{
#include <string>

extern int yylex();
extern void yyerror(const char* s, ...);	
}%

%union {
	int inteiro;
	double real;
	string booleano;
}

//Definição dos tokens
%token <inteiro> T_INT
%token <real> T_REAL
%token <booleano> T_BOOL
%token T_ID
%token T_PLUS T_MULT T_SUB T_DIV T_ATTR
%token T_AND T_OR T_NOT
%token T_APAR T_FPAR 
%token T_DINT T_DREAL T_DBOOL
%token T_NEQ T_EQ T_GEQ T_GT T_LEQ T_LT 
%token T_COLON T_ENDL T_COMMA

//Deinição de tipos não-terminais
%type /* tipo */ program cmds
%type /* tipo */ cmd decl listvar attr expr

//Precedencia de operadores
//TODO

%start program

%%

program: cmds {}
;

cmds: cmd {}
	| cmds cmd {}
;

cmd: decl T_ENDL {}
	| attr T_ENDL {}
;

decl: T_DINT T_COLON listvar {}
	| T_DREAL T_COLON listvar {}
	| T_DBOOL T_COLON listvar {}
;

listvar: T_ID {}
	| listvar T_COMMA T_ID {}
;

attr: T_ID T_ATTR expr {}
;

expr: T_ID {}
	| T_INT {}
	| T_REAL {}
	| T_BOOL {}
	| expr T_PLUS expr {}
	| expr T_SUB expr {}
	| expr T_MULT expr {}
	| expr T_DIV expr {}
	| T_APAR expr T_FPAR {}
;

%%