#ifndef MYTAB_H
#define MYTAB_H
#include<map>
#include<iostream>
#include<vector>
#include<stack>
typedef struct SInfo{
        std::string name;
        std::string type;   //local | global | arg
        int p = -1;
        int layer = 0;
    } SInfo;
class MyTab{
private:
    int layer = 0;
    std::stack<int> localcounts;
    std::map<std::string, int> mp;
    std::vector<SInfo> arr;
public:
    int getLayer();
    void in();
    int out();
    void reg(std::string name, std::string type, int argindex);
    SInfo acc(std::string name);
};
#endif