%{
#include <string>
#include "ast.h"
#include "parser.h"
extern int yylineno;
void yyerror(const char* s, ...);
%}

%option noyywrap nodefault yylineno

digs [0-9]+
var [a-zA-Z]+

%%

[ \t]               ; //ignores spaces
{digs}              {yylval.integer = std::atoi(yytext); return T_INT;}
{var}		    {yylval.var = yytext; return T_VAR;}
"+"                 {return T_PLUS;}
"*"		    {return T_MULT;}
"="		    {return T_ATTR;}
\n                  {return T_NL;}
.                   {yyerror("Unknown token! %c\n", *yytext);} /*complains about every unknown character*/

%%

void yyerror(const char *s, ...){
    va_list ap;
    va_start(ap, s);
    std::fprintf(stderr, "Line %d: error: ", yylineno);
    std::vfprintf(stderr, s, ap);
}
