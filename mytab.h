#ifndef MYTAB_H
#define MYTAB_H
#include<map>
#include<iostream>
#include<vector>
typedef struct SInfo{
        std::string name;
        std::string type;
        int p = -1;
        int layer = 0;
        int args = 0;
    } SInfo;
class MyTab{
private:
    int layer = 0;
    std::map<std::string, int> mp;
    std::vector<SInfo> arr;
    int rcount = 0;
public:
    int getLayer();
    void in();
    int out();
    void reg(std::string name, std::string type, int args);
    SInfo acc(std::string name);
};
#endif