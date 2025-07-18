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
    void switch_streams(std::istream* new_in = 0, std::ostream* new_out = 0) override{
        yyFlexLexer::switch_streams(new_in, new_out);
    }
};
#endif