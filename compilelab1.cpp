#include<iostream>
#include"compilelab1.h"
#include"mylexer.h"
#include"ttype.h"
std::vector<TokenInfo> tokens = std::vector<TokenInfo>();
int cur = 0;

int main(){
    //词法分析
    int i = 0;
    MyLexer lexer;
    do{
        i = lexer.yylex();
        if(i != 0){
            tokens.push_back({
                lexer.getYYText(),
                i
            });
            std::cout << lexer.getYYText() << " " << i << std::endl;
        }
    }while(i != 0);
    //语法分析
    
    return 0;
}