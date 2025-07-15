%option c++
%{
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utility>
#include <string>

#define T_INT          1
#define T_RETURN       2
#define T_MAIN         3
#define T_IDENTIFIER   4
#define T_CONST        5
#define T_PLUS         6
#define T_MINUS        7
#define T_MULTIPLY     8
#define T_DIVIDE       9
#define T_LT           10
#define T_GT           11
#define T_LE           12
#define T_GE           13
#define T_EQ           14
#define T_NE           15
#define T_ASSIGN       16
#define T_END          17
#define T_BLOCKL       18
#define T_BLOCKR       19
#define T_LPAREN       20
#define T_RPAREN       21
#define TRet std::pair<int,std::string>
int yyFlexLexer::yywrap() { 
    return 1; 
}
%}

identifier  [A-Za-z_][0-9A-Za-z_]*
integer     0|[1-9][0-9]*

%%

"int"       { printf(" %s ",yytext); return T_INT; }
"return"    { printf(" %s ",yytext); return T_RETURN;}
"main"      { printf(" %s ",yytext); return T_MAIN;}
{identifier}  { printf(" %s ",yytext); return T_IDENTIFIER; }
{integer}     { printf(" %s ",yytext); return T_CONST; }
"+"     { printf(" %s ",yytext); return T_PLUS; }
"-"     { printf(" %s ",yytext); return T_MINUS; }
"*"     { printf(" %s ",yytext); return T_MULTIPLY; }
"/"     { printf(" %s ",yytext); return T_DIVIDE; }
"<"     { printf(" %s ",yytext); return T_LT; }
">"     { printf(" %s ",yytext); return T_GT; }
"<="    { printf(" %s ",yytext); return T_LE; }
">="    { printf(" %s ",yytext); return T_GE; }
"=="    { printf(" %s ",yytext); return T_EQ; }
"!="    { printf(" %s ",yytext); return T_NE; }
"="     { printf(" %s ",yytext); return T_ASSIGN; }
";"     { printf(" %s ",yytext); return T_END; }
"{"     { printf(" %s ",yytext); return T_BLOCKL; }
"}"     { printf(" %s ",yytext); return T_BLOCKR; }
"("     { printf(" %s ",yytext); return T_LPAREN; }
")"     { printf(" %s ",yytext); return T_RPAREN; }
[ \t\n]     ;

%%