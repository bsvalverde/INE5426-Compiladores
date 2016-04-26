%{
#include <string>
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

id [a-z]+
digito [0-9]

inteiro digito+
real digito+\.digito* | digito*\.digito+
bool "TRUE" | "FALSE"

%%

 //Ignorar espaços
[ \t\n];

 //Tipos
{inteiro} {
	yylval.inteiro = std::atoi(yytext);
	return T_INT;
}
{real} {
	yylval.real = std::atof(yytext);
	return T_REAL;
}
{bool} {
	yylval.booleano = strdup(yytext);
	return T_BOOL;
}

 //Operadores
"+" 	{return T_PLUS;}
"*" 	{return T_MULT;}
"-" 	{return T_SUB;}
"/" 	{return T_DIV;}
":=" 	{return T_ATTR;}
"AND"	{return T_AND;}
"OR"	{return T_OR;}
"~"		{return T_NOT;}
"("		{return T_APAR;}
")"		{return T_FPAR;}

 //Atribuições
"int" 	{return T_DINT;}
"real" 	{return T_DREAL;}
"bool" 	{return T_DBOOL;}

 //Operadores relacionais
"~=" 	{return T_NEQ;}
"=" 	{return T_EQ;}
">=" 	{return T_GEQ;}
">" 	{return T_GT;}
"<=" 	{return T_LEQ;}
"<" 	{return T_LT;}


 //Misc
{id}	{
	yylval.id = strdup(yytext);
	return T_ID;
}
":" 	{return T_COLON;}
";" 	{return T_ENDL;}
","		{return T_COMMA;}

 //Complains about every unknown character
.		{yyerror("Unknown token! %c\n", *yytext);}

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "Line %d: error: ", yylineno);
    std::vfprintf(stderr, s, ap);
}