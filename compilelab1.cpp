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
bool IF1(){
    TokenInfo info = tokens[cur];
    if(info.id == T_LPAREN){
        cur++;
        if(!Args())return false;
        info = tokens[cur++];
        if(info.id != T_RPAREN)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
    }else if(info.id == T_END || info.id == T_ASSIGN || info.id == T_COMMA){
        if(!B())return false;
        cur++;
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
bool Dp1(){
    TokenInfo info = tokens[cur];
    if(info.id == T_END){
        if(!Dp2())return false;
    }else if(info.id == T_ASSIGN){
        cur++;
        if(!T())return false;
        if(!Dp2())return false;
    }
    return true;
}
bool Dp2(){
    if(tokens[cur].id == T_END){//epsilon
        return true;
    }
    if(tokens[cur++].id == T_COMMA){  //,idD1'
        TokenInfo info = tokens[cur++];
        if(info.id != T_IDENTIFIER)return false;
        if(tokens[cur].id == T_ASSIGN || tokens[cur].id == T_END){
            return Dp1();
        }else return false;
    }
    return false;
}
bool B1(){
    if(tokens[cur].id == T_BLOCKR){ //epsilon
        return true;
    }
    if(tokens[cur].id == T_INT
    ||tokens[cur].id == T_RETURN
    ||tokens[cur].id == T_IDENTIFIER
    ||tokens[cur].id == T_CONTINUE
    ||tokens[cur].id == T_BREAK
    ||tokens[cur].id == T_IF
    ||tokens[cur].id == T_WHILE){ //B
        return B();
    }
    return false;
}
bool B(){
    if(tokens[cur].id == T_INT
    ||tokens[cur].id == T_RETURN
    ||tokens[cur].id == T_IDENTIFIER
    ||tokens[cur].id == T_CONTINUE
    ||tokens[cur].id == T_BREAK
    ||tokens[cur].id == T_IF
    ||tokens[cur].id == T_WHILE){ //AB'
        if(!A())return false;
        if(!B1())return false;
        return true;
    }
    return false;
}
bool A(){
    if(tokens[cur].id == T_INT && Dp()){
        return true;
    }
    if(tokens[cur].id == T_RETURN && R()){
        return true;
    }
    if(tokens[cur].id == T_IDENTIFIER && E()){
        return true;
    }
    TokenInfo info = tokens[cur++];
    if(info.id == T_CONTINUE){
        info = tokens[cur++];
        if(info.id != T_END)return false;
        return true;
    }else if(info.id == T_BREAK){
        info = tokens[cur++];
        if(info.id != T_END)return false;
        return true;
    }else if(info.id == T_IF || info.id == T_WHILE){
        TokenInfo temp = info;
        info = tokens[cur++];
        if(info.id != T_LPAREN)return false;
        if(!T())return false;
        info = tokens[cur++];
        if(info.id != T_RPAREN)return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B1())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        return (temp.id==T_IF)?Ep():true;
    }
    return false;
}
bool Ep(){
    if(tokens[cur].id == T_ELSE){
        cur++;
        TokenInfo info = tokens[cur++];
        if(info.id != T_BLOCKL)return false;
        if(!B1())return false;
        info = tokens[cur++];
        if(info.id != T_BLOCKR)return false;
        if(!B1())return false;
        return true;
    }else{
        if(tokens[cur].id == T_INT
        ||tokens[cur].id == T_RETURN
        ||tokens[cur].id == T_IDENTIFIER
        ||tokens[cur].id == T_CONTINUE
        ||tokens[cur].id == T_BREAK
        ||tokens[cur].id == T_IF
        ||tokens[cur].id == T_WHILE
        ||tokens[cur].id == T_BLOCKR){
            return B1();
        }
    }
    return false;
}
bool Dp(){

}
bool R(){

}
bool E(){

}
bool P(){

}
bool T(){

}
bool Args(){

}
bool D(){

}
bool D1(){

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