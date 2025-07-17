#ifndef MYLEXER_H
#define MYLEXER_H
#include<iostream>
#include<FlexLexer.h>
class MyLexer: public yyFlexLexer{
public:
    std::string getYYText(){
        return std::string(this->yytext);
    }
    int yywrap() override { 
        return 1; 
    }
};
#endif