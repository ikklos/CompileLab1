#ifndef COMPILELAB1_H
#define COMPILELAB1_H
#include<FlexLexer.h>
#include<iostream>
#include<vector>
typedef struct TokenInfo{
    std::string word = "";
    int id = 0;
} TokenInfo;
extern std::vector<TokenInfo> tokens;
#endif