#ifndef COMPILELAB1_H
#define COMPILELAB1_H
#include<FlexLexer.h>
#include<iostream>
#include<vector>
#include"mytab.h"
typedef struct TokenInfo{
    std::string word = "";
    int id = 0;
} TokenInfo;
extern std::vector<TokenInfo> tokens;
extern MyTab mytab;
bool start();   //S->FuncS'
bool func();    //Func->int IF|void VF
bool start1();  //S'->epsilon|S
bool IF();      //IF->main(mainArg){B}|id IF1
bool IF1();     //IF1->(Args){B}|;|D1';
bool VF();      //VF->id(Args){B'}
bool Dp1();     //D1'-> =TD2'|D2'
bool Dp2();     //D2'-> epsilon|,idD1'
bool B1();      //B'->epsilon|B
bool B();       //B->AB'
bool A();       //A->D'|R|E|continue;|break;|if(T){B'}E'|while(T){B'}
bool Ep();      //E'->else{B'}B'|B'
bool Dp();      //D'->int id D1';
bool R();       //R->return T;
bool E();       //E->id P;
bool P();       //P->(T')|=T
bool T();       //表达式
bool Args();    //Args->D|epsilon
bool Pp();      //P'->id(T')
bool Tp();      //T'->TT1'
bool Tp1();     //T1'->epsilon|,T'
bool D();       //D->int id D1
bool D1();      //D1->epsilon|,D
#endif