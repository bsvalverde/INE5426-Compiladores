%{
#include <string>
#include "ast.h"
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

id [a-z]+

inteiro [0-9]+
real [0-9]+\.[0-9]*|[0-9]*\.[0-9]+
bool "TRUE"|"FALSE"

%%

 /* Ignorar espaços */
[ \t] ;
\n ;

 /* Tipos */
{inteiro} {
	yylval.inteiro = strdup(yytext);
	return T_INT;
}
{real} {
	yylval.real = strdup(yytext);
	return T_REAL;
}
{bool} {
	yylval.booleano = strdup(yytext);
	return T_BOOL;
}

 /* Operadores */
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
"["		{return T_AARR;}
"]"		{return T_FARR;}

 /* Atribuições */
"int" 	{return T_DINT;}
"real" 	{return T_DREAL;}
"bool" 	{return T_DBOOL;}

 /* Operadores relacionais */
"~=" 	{return T_NEQ;}
"=" 	{return T_EQ;}
">=" 	{return T_GTE;}
">" 	{return T_GT;}
"<=" 	{return T_LTE;}
"<" 	{return T_LT;}

 /* Tokens de Função */
"def"	{return T_DEF;}
"decl"	{return T_DECL;}
"fun"	{return T_FUN;}
"return"	{return T_RET;}
 
 /* Tokens de Condição */
"if"	{return T_IF;}
"then"	{return T_THEN;}
"else"	{return T_ELSE;}

 /* Tokens de Laço */
"while"	{return T_WHILE;}
"do"	{return T_DO;}

 /* Misc */
"end"	{return T_END;}
{id}	{
	yylval.id = strdup(yytext);
	return T_ID;
}
":" 	{return T_COLON;}
";" 	{return T_ENDL;}
","		{return T_COMMA;}

 /* Complains about every unknown character */
.		{yyerror("lexico: simbolo desconhecido: %c.", *yytext);}

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "[Linha %d] Erro ", yylineno);
    std::vfprintf(stderr, s, ap);
    std::fprintf(stderr, "\n");
}
