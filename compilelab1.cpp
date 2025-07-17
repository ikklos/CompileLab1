#include<iostream>
#include<stack>
#include"compilelab1.h"
#include"mylexer.h"
#include"ttype.h"
int tagcount = 0;
std::string getNewTag(){
    char s[20] = "";
    sprintf(s,"%s",tagcount++);
    std::string res = "_T";
    return res + s;
}
std::vector<TokenInfo> tokens = std::vector<TokenInfo>();
int cur = 0;
bool start(){
    if(cur >= tokens.size())return false;
    int id = tokens[cur].id;
    if(id != T_INT && id != T_VOID){
        return false;
    }
    if(!func()){
        return false;
    }
    if(!start1())return false;
    return true;
}
bool func(){
    int id = tokens[cur++].id;
    if(id == T_INT){
        return IF();
    }else if(id == T_VOID){
        return VF();
    }
    return false;
}
bool start1(){//s'
    if(cur >= tokens.size()){
        return true;
    }
    if(tokens[cur].id == T_INT || tokens[cur].id == T_VOID){
        return start();
    }
}
bool IF(){
    TokenInfo info = tokens[cur++];
    switch(info.id){
    case T_MAIN:
        info = tokens[cur++];
        if(info.id != T_LPAREN) return false;
        info = tokens[cur++];
        if(info.id == T_INT){
            info = tokens[cur++];
            if(info.id != T_IDENTIFIER)return false;
            info = tokens[cur++];
            if(info.id != T_COMMA)return false;
            info = tokens[cur++];
            if(info.id != T_INT)return false;
            info = tokens[cur++];
            if(info.id != T_IDENTIFIER)return false;
            info = tokens[cur++];
            if(info.id != T_RPAREN)return false;
        }else if(info.id != T_RPAREN) return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        break;
    case T_IDENTIFIER:
        return IF1();
    }
    return true;
}
bool VF(){
    TokenInfo info = tokens[cur++];
    if(info.id != T_IDENTIFIER){
        return false;
    }
    info = tokens[cur++];
    if(info.id != T_LPAREN)return false;
    if(!Args())return false;
    info = tokens[cur++];
    if(info.id != T_RPAREN)return false;
    if(info.id != T_BLOCKL)return false;
    if(!B1())return false;
    info = tokens[cur++];
    if(info.id != T_BLOCKR)return false;
    return true;
}
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
    try{
        if(!start()){
            printf("ERR: failed!\n");
        }
    }catch(std::runtime_error){
        printf("ERR: failed!\n");
    }
    return 0;
}