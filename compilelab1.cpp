#include<iostream>
#include<FlexLexer.h>

int main(){
    int i = 0;
    yyFlexLexer lexer;
    do{
        i = lexer.yylex();
    }while(i != 0);
    return 0;
}