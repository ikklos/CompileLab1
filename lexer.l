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
#include "ttype.h"

#define TRet std::pair<int,std::string>
int yyFlexLexer::yywrap() { 
    return 1; 
}
%}

identifier  [A-Za-z_][0-9A-Za-z_]*
integer     0|[1-9][0-9]*

%%

"int"       { return T_INT; }
"return"    { return T_RETURN;}
"main"      { return T_MAIN;}
"void"      {return T_VOID;}
"if"
"else"
"while"
"continue"
"break"
{identifier}  { return T_IDENTIFIER; }
{integer}     { return T_CONST; }
"+"     { return T_PLUS; }
"-"     { return T_SUB; }
"*"     { return T_MULTIPLY; }
"/"     { return T_DIVIDE; }
"<"     { return T_LT; }
">"     { return T_GT; }
"<="    { return T_LE; }
">="    { return T_GE; }
"=="    { return T_EQ; }
"!="    { return T_NE; }
"%"     { return T_MOD; }
"&&"    {return T_LAND;}
"||"    {return T_LOR;}
"&"     { return T_AND; }
"|"     { return T_OR; }
"^"     { return T_XOR; }
"="     { return T_ASSIGN; }
";"     { return T_END; }
"{"     { return T_BLOCKL; }
"}"     { return T_BLOCKR; }
"("     { return T_LPAREN; }
")"     { return T_RPAREN; }
"!"     { return T_LNOT;}
"~"     {return T_BNOT;}
[ \t\n]     ;

%%